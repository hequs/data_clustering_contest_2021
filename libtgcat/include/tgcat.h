#pragma once

/**
 * Library for determining topic and main language of Telegram channels.
 */

#include "tgcat_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#  ifdef tgcat_EXPORTS
#    define TGCAT_EXPORT __declspec(dllexport)
#  else
#    define TGCAT_EXPORT __declspec(dllimport)
#  endif
#else
#  define TGCAT_EXPORT __attribute__((visibility("default")))
#endif

/**
 * Initializes the library. Must be called once before any other request.
 * \return 0 on success and a negative value on fail.
 */
TGCAT_EXPORT int tgcat_init();
TGCAT_EXPORT int tgcat_init_w_models(const char* languageModel, const char* categoryModel);

/**
 * Detects main language of channel posts.
 * \param[in] channel_info Information about the channel.
 * \param[out] language_code Array to be filled with null-terminated ISO 639-1 language code
 *                           of the channel posts, or "other" if the language doesn’t have
 *                           a two-letter code.
 * \return 0 on success and a negative value on fail.
 */
TGCAT_EXPORT int tgcat_detect_language(const struct TelegramChannelInfo *channel_info,
                                       char language_code[6]);

/**
 * Detects main topics of a channel.
 * \param[in] channel_info Information about the channel.
 * \param[out] category_probability Array to be filled with probabilities that
 *                                  channel belongs to a corresponding category.
 * \return 0 on success and a negative value on fail.
 */
TGCAT_EXPORT int tgcat_detect_category(const struct TelegramChannelInfo *channel_info,
                                       double category_probability[TGCAT_CATEGORY_OTHER + 1]);

#ifdef __cplusplus
}
#endif
