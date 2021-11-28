#include "tgcat.h"

#include "language_detector.h"
#include "topic_detector.h"

#include <memory>
#include <cstring>

namespace {
    static std::unique_ptr<TLanguageDetector> LanguageDetector;
    static std::unique_ptr<TTopicDetector> TopicDetector;
}

int tgcat_init() {
    // TODO: add some config file
    LanguageDetector = std::make_unique<TLanguageDetector>();
    TopicDetector = std::make_unique<TTopicDetector>();
    return 0;
}

int tgcat_init_w_models(const char* languageModel, const char* categoryModel) {
    // TODO: add some config file
    LanguageDetector = std::make_unique<TLanguageDetector>(std::string(languageModel));
    TopicDetector = std::make_unique<TTopicDetector>(std::string(categoryModel));
    return 0;
}

int tgcat_detect_language(const struct TelegramChannelInfo *channel_info, char language_code[6]) {
    const std::string languageCode = LanguageDetector->Detect(*channel_info);
    strcpy(language_code, languageCode.data()); // TODO: do some check
    return 0;
}

int tgcat_detect_category(const struct TelegramChannelInfo *channel_info, double category_probability[TGCAT_CATEGORY_OTHER + 1]) {
    return TopicDetector->Detect(*channel_info, category_probability);
}
