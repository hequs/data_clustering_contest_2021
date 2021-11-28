#include "topic_detector.h"

#include <text_preprocessing.h>

#include <cmath>
#include <iterator>
#include <string.h>
#include <sstream>

namespace {
    constexpr auto MODEL_PATH = "resources/topicdetect_new_2.ftz";
    constexpr size_t MODEL_TOP_N = 4;
    constexpr double MODEL_BORDER = 0.1;
    constexpr double DESCRIPTION_WEIGHT = 2.0;
    constexpr size_t CLEAN_TEXT_MIN_LENGTH = 10;
    constexpr size_t TEXT_LENGTH_LIMIT = 256;
}

TTopicDetector::TTopicDetector(const std::string& model)
    : Tokenizer(onmt::Tokenizer::Mode::Conservative)
{
    Model.loadModel(model.empty() ? MODEL_PATH : model);
}

namespace {
    TgcatCategory ToEnum(const std::string& label) {
        constexpr size_t FT_PREFIX_LENGTH = std::char_traits<char>::length("__label__");
        return static_cast<TgcatCategory>(std::atoi(label.c_str() + FT_PREFIX_LENGTH));
    }

    void Softmax(double probs[TGCAT_CATEGORY_OTHER + 1]) {
        const size_t size = TGCAT_CATEGORY_OTHER + 1;
        double sum = 0.0;
        for (int i = 0; i < size; ++i) { // std::size ?
            probs[i] = (probs[i] > 0.00001) ? std::exp(probs[i]) : 0.0;
            sum += probs[i];
        }
        for (int i = 0; i < size; ++i) {
            probs[i] /= sum;
        }
    }
}

TTopicDetector::TPredictions TTopicDetector::RunModel(const std::string& text) const {
    TTopicDetector::TPredictions predictions;

    std::istringstream ifs(text); // use boost::iostreams::basic_array_source to avoid copying string_view -> string
    Model.predictLine(ifs, predictions, MODEL_TOP_N, MODEL_BORDER);

    return predictions;
}

namespace {
    std::string TypeToString(TelegramChannelPostType type) {
        if (type == TELEGRAM_CHANNEL_POST_TYPE_PHOTO) {
            return "photo";
        }
        if (type == TELEGRAM_CHANNEL_POST_TYPE_VIDEO) {
            return "video";
        }
        if (type == TELEGRAM_CHANNEL_POST_TYPE_MUSIC) {
            return "audio";
        }
        if (type == TELEGRAM_CHANNEL_POST_TYPE_FILE) {
            return "file";
        }
        return "text";
    }
}

int TTopicDetector::Detect(const TelegramChannelInfo& channelInfo, double probs[TGCAT_CATEGORY_OTHER + 1]) const {
    TTopicDetector::TPredictions predictions;
    predictions.reserve(MODEL_TOP_N * (2 + channelInfo.recent_post_count));

    std::vector<std::tuple<std::string, std::string, double>> weightedTexts;
    weightedTexts.reserve(2 + channelInfo.recent_post_count);
    weightedTexts.emplace_back(channelInfo.title, "", 1.0);
    weightedTexts.emplace_back(channelInfo.description, "", DESCRIPTION_WEIGHT);
    for (auto i = 0; i < channelInfo.recent_post_count; ++i) {
        const auto& post = channelInfo.recent_posts[i];

        std::stringstream ss;
        ss << post.text;
        if (post.music_title) {
            ss << " " << post.music_title;
        }
        if (post.music_performer) {
            ss << " " << post.music_performer;
        }
        if (const auto* link = post.link_preview) {
            ss << " " << link->title << " " << link->description;
        }
        weightedTexts.emplace_back(ss.str(), TypeToString(post.type) , 1.0);
    }

    std::unordered_map<std::string, double> weightedCleanTexts;
    for (const auto& [text, type, weight] : weightedTexts) {
        const std::string cleanText = NTP::TopicPreprocess(text, Tokenizer, TEXT_LENGTH_LIMIT);
        if (cleanText.length() < CLEAN_TEXT_MIN_LENGTH) { // TODO: consider utf-8 + move to config
            continue;
        }
        std::stringstream ss;
        ss << type << " " << cleanText;
        weightedCleanTexts.emplace(ss.str(), weight);
    }

    for (const auto& [cleanText, weight]: weightedCleanTexts) {
        auto ps = RunModel(cleanText);
        for (auto& [prob, _] : ps) {
            prob *= weight;
        }
        std::move(ps.begin(), ps.end(), std::back_inserter(predictions));
    }

    std::for_each(predictions.cbegin(), predictions.cend(), [&](const auto& p) { probs[ToEnum(p.second)] += p.first; });
    if (predictions.empty()) {
        probs[TGCAT_CATEGORY_OTHER] = 1.0;
    }

    Softmax(probs);

    return 0;
}
