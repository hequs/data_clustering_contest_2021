import json
import argparse
import random

LANGUAGES = ["ru", "en", "es", "it", "und", "hi"]
CATEGORIES = ["Art & Design", "Bets & Gambling", "Books", "Business & Entrepreneurship", "Cars & Other Vehicles", "Celebrities & Lifestyle", "Cryptocurrencies", "Culture & Events", "Curious Facts", "Directories of Channels & Bots", "Economy & Finance", "Education", "Erotic Content"]


def main(input_file, output_file):
    with open(input_file, "r") as r, open(output_file, "w") as w:
        for line in r:
            channel = json.loads(line)
            lang_num = random.randrange(0, len(LANGUAGES))
            cat_num = random.randrange(0, len(CATEGORIES))
            language = LANGUAGES[lang_num]
            category = CATEGORIES[cat_num]
            w.write(json.dumps({
                "lang_code": language,
                "category": {
                    category: 1.0
                }
            }).strip() + "\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str)
    parser.add_argument("output_file", type=str)
    args = parser.parse_args()
    main(**vars(args))
