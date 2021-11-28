#pragma once

#include "tgcat_types.h"

#include <fasttext.h>
#include <onmt/Tokenizer.h>

class TLanguageDetector {
public:
    TLanguageDetector(const std::string& model = {});

    std::string Detect(const TelegramChannelInfo& channelInfo) const;

private:
    using TPredictions = std::vector<std::pair<fasttext::real, std::string>>;
    TPredictions RunModel(const std::string& text) const;

private:
    const onmt::Tokenizer Tokenizer;
    fasttext::FastText Model; // TODO: make const
};
