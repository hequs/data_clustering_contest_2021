#pragma once

#include "tgcat_types.h"

#include <fasttext.h>
#include <onmt/Tokenizer.h>

class TTopicDetector {
public:
    TTopicDetector(const std::string& model = {});

    int Detect(const TelegramChannelInfo& channelInfo, double categoryProbability[TGCAT_CATEGORY_OTHER + 1]) const;

private:
    using TPredictions = std::vector<std::pair<fasttext::real, std::string>>;
    TPredictions RunModel(const std::string& text) const;

private:
    const onmt::Tokenizer Tokenizer;
    fasttext::FastText Model; // TODO: make const
};
