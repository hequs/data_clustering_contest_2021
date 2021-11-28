#include <text_preprocessing.h>

#include <boost/regex.hpp>
#include <unicode/uchar.h>

#include <codecvt>
#include <locale>

std::string NTP::RemoveUrls(const std::string& text) {
    static const boost::regex regex(R"(((https?://\S*|t\.me/\S+)))", boost::regex_constants::icase);
    return boost::regex_replace(text, regex, "");
}

std::string NTP::RemoveTags(const std::string& text) {
    static const boost::regex regex(R"(((@|#)\w+))", boost::regex_constants::icase);
    return boost::regex_replace(text, regex, "");
}

std::u32string NTP::ToUTF32(const std::string& text) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
    return cvt.from_bytes(text);
}

std::string NTP::ToUTF8(const std::u32string& text) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
    return cvt.to_bytes(text);
}

void NTP::RemoveNonAlpha(std::u32string& text) {
    std::replace_if(text.begin(), text.end(), [](auto c) { return !u_isalpha(c); }, U' ');
}

void NTP::ToLower(std::u32string& text) {
    std::for_each(text.begin(), text.end(), [](auto &c) { c = u_tolower(c); });
}

std::string NTP::LangPreprocess(const char* text, const onmt::Tokenizer& tokenizer, size_t lengthLimit) {
    std::string res = NTP::RemoveTags(NTP::RemoveUrls(std::string(text)));

    std::u32string u32res = NTP::ToUTF32(res);
    if (u32res.size() > lengthLimit) {
        u32res.erase(u32res.cbegin() + lengthLimit, u32res.cend());
    }
    NTP::ToLower(u32res);
    NTP::RemoveNonAlpha(u32res);
    res = NTP::ToUTF8(u32res);

    std::vector<std::string> tokens;
    tokenizer.tokenize(res, tokens);

    return tokenizer.detokenize(tokens);
}

std::string NTP::LangChannelPreprocess(
    const TelegramChannelInfo& channelInfo,
    const onmt::Tokenizer& tokenizer,
    size_t perTextMinLength,
    size_t perTextMaxLength,
    size_t sumMaxLength
) {
    std::vector<std::string> texts;
    texts.emplace_back(NTP::LangPreprocess(channelInfo.title, tokenizer, perTextMaxLength));
    texts.emplace_back(NTP::LangPreprocess(channelInfo.description, tokenizer, perTextMaxLength));
    for (auto i = 0; i < channelInfo.recent_post_count; ++i) {
        texts.emplace_back(NTP::LangPreprocess(channelInfo.recent_posts[i].text, tokenizer, perTextMaxLength));
    }
    std::vector<std::u32string> textsFixed;
    for (const auto& text : texts) {
        std::u32string u32text = NTP::ToUTF32(text);
        if (u32text.size() < perTextMinLength) {
            continue;
        }
        textsFixed.emplace_back(std::move(u32text));
    }

    std::u32string finalText;
    finalText.reserve(sumMaxLength + perTextMaxLength + channelInfo.recent_post_count + 2);
    for (const auto& text : textsFixed) {
        finalText.append(text);
        finalText.append(U" ");
        if (finalText.size() > sumMaxLength) {
            break;
        }
    }
    return NTP::ToUTF8(finalText);
}

std::string NTP::TopicPreprocess(const std::string& text, const onmt::Tokenizer& tokenizer, size_t lengthLimit) {
    std::string res = NTP::RemoveUrls(text);

    std::u32string u32res = NTP::ToUTF32(res);
    if (u32res.size() > lengthLimit) {
        u32res.erase(u32res.cbegin() + lengthLimit, u32res.cend());
    }
    NTP::ToLower(u32res);
    res = NTP::ToUTF8(u32res);

    std::vector<std::string> tokens;
    tokenizer.tokenize(res, tokens);

    return tokenizer.detokenize(tokens);
}
