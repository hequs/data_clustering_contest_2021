#include "language_detector.h"

#include "text_preprocessing.h"

#include <string.h>
#include <sstream>

namespace {
    constexpr auto MODEL_PATH = "resources/langdetect.ftz";
    constexpr size_t MODEL_TOP_N  = 1;
    constexpr double MODEL_BORDER  = 0.2;
    constexpr auto OTHER_LANG = "other";
    constexpr size_t PER_TEXT_MIN_LENGTH = 3;
    constexpr size_t PER_TEXT_MAX_LENGTH = 256;
    constexpr size_t SUM_MIN_LENGTH = 4;
    constexpr size_t SUM_MAX_LENGTH = 2048;
    constexpr size_t FT_PREFIX_LENGTH = std::char_traits<char>::length("__label__");
}

TLanguageDetector::TLanguageDetector(const std::string& model)
    : Tokenizer(onmt::Tokenizer::Mode::Conservative)
{
    Model.loadModel(model.empty() ? MODEL_PATH : model);
}

TLanguageDetector::TPredictions TLanguageDetector::RunModel(const std::string& text) const {
    TLanguageDetector::TPredictions predictions;
    std::istringstream ifs(text); // use boost::iostreams::basic_array_source to avoid copying string_view -> string
    Model.predictLine(ifs, predictions, MODEL_TOP_N, MODEL_BORDER);
    return predictions;
}

std::string TLanguageDetector::Detect(const TelegramChannelInfo& channelInfo) const {
   std::string sumText = NTP::LangChannelPreprocess(
        channelInfo,
        Tokenizer,
        /* perTextMinLength = */ PER_TEXT_MIN_LENGTH,
        /* perTextMaxLength = */ PER_TEXT_MAX_LENGTH,
        /* sumMaxLength = */ SUM_MAX_LENGTH
    );
    if (sumText.size() < SUM_MIN_LENGTH) {
        return OTHER_LANG;
    }
    auto ps = RunModel(sumText);
    if (ps.empty()) {
        return OTHER_LANG;
    }
    const std::string lang = ps.at(0).second.substr(FT_PREFIX_LENGTH);
    return lang.size() <= 2 ? lang : OTHER_LANG;
}
