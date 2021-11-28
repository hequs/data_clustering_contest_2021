#pragma once

#include "tgcat_types.h"

#include <onmt/Tokenizer.h>

#include <string>

namespace NTP {

    std::string RemoveUrls(const std::string& text);
    std::string RemoveTags(const std::string& text);

    std::u32string ToUTF32(const std::string& text);
    std::string ToUTF8(const std::u32string& text);

    void RemoveNonAlpha(std::u32string& text);
    void ToLower(std::u32string& text);

    std::string LangPreprocess(const char* text, const onmt::Tokenizer& tokenizer, size_t lengthLimit);
    std::string LangChannelPreprocess(
        const TelegramChannelInfo& channelInfo,
        const onmt::Tokenizer& tokenizer,
        size_t perTextMinLength,
        size_t perTextMaxLength,
        size_t sumMaxLength
    );

    std::string TopicPreprocess(const std::string& text, const onmt::Tokenizer& tokenizer, size_t lengthLimit);
}
