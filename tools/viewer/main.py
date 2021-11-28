import os
import json
import argparse
import shutil
from collections import Counter, defaultdict

from jinja2 import Environment, FileSystemLoader


def get_cat_code(category):
    return category.lower().replace("&", "and").replace(" ", "_")

def add_t(original, translated):
    original.update({
        "title_t": translated["title"],
        "description_t": translated["description"]
    })
    for op, tp in zip(original["recent_posts"], translated["recent_posts"]):
        op.update({
            "text_t": tp["text"]
        })
    return original

def main(input_file, input_t_file, output_file, output_dir, templates_dir):
    script_dir = os.path.dirname(os.path.realpath(__file__))
    templates_dir =  os.path.join(script_dir, templates_dir)
    static_dirs = ("icons", "css", "js")
    for dir_name in static_dirs:
        static_output_dir = os.path.join(output_dir, dir_name)
        static_input_dir = os.path.join(templates_dir, dir_name)
        os.makedirs(static_output_dir, exist_ok=True)
        for file_name in os.listdir(static_input_dir):
            from_file = os.path.join(static_input_dir, file_name)
            to_file = os.path.join(static_output_dir, file_name)
            shutil.copyfile(from_file, to_file)
    records = []
    languages = Counter()
    categories = Counter()
    with open(input_file, "r") as i, open(input_t_file, "r") as it, open(output_file, "r") as o:
        for input_line, input_t_line, output_line in zip(i, it, o):
            channel_info = json.loads(input_line)
            channel_t_info = json.loads(input_t_line)
            result = json.loads(output_line)
            channel_info.update(result)
            add_t(channel_info, channel_t_info)
            languages[result["lang_code"]] += 1
            best_category = max(result["category"].items(), key=lambda x: x[1])[0]
            categories[best_category] += 1
            records.append(channel_info)
    languages = [k for k, v in languages.most_common()]
    categories = [k for k, v in categories.most_common()]
    if "Other" not in categories:
        categories.append("Other")
    directories = defaultdict(list)
    for record in records:
        language = record["lang_code"]
        category = "Other"
        if language in ("en", "ru", "fa", "uz", "ar"):
            category = max(record["category"].items(), key=lambda x: x[1])[0]
        directories[(language, category)].append(record)

    file_loader = FileSystemLoader(templates_dir)
    env = Environment(loader=file_loader)
    dir_template = env.get_template("directory.html")
    cat_codes = [get_cat_code(cat) for cat in categories]
    cat_code_names = {get_cat_code(cat): cat for cat in categories}

    items_count = dict()
    for language in languages:
        items_count[language] = dict()
        for category in categories:
            if language in ("en", "ru", "fa", "uz", "ar"):
                cnt = len(directories[(language, category)])
                items_count[language][get_cat_code(category)] = cnt

    lang_count = Counter()
    for language in languages:
        for category in categories:
            cnt = len(directories.get((language, category), []))
            lang_count[language] += cnt

    for language in languages:
        for category in categories:
            cat_code = get_cat_code(category)
            channels = directories.get((language, category), [])
            if not channels:
                continue
            channels = channels[:200]
            with open(os.path.join(output_dir, "{}_{}.html".format(language, cat_code)), "w", encoding="utf-8") as w:
                w.write(dir_template.render(
                    channels=channels,
                    current_lang_code=language,
                    current_cat_code=cat_code,
                    languages=languages,
                    categories=cat_codes,
                    cat_code_names=cat_code_names,
                    items_count=items_count,
                    lang_count=lang_count
                ))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", type=str)
    parser.add_argument("input_t_file", type=str)
    parser.add_argument("output_file", type=str)
    parser.add_argument("output_dir", type=str)
    parser.add_argument("--templates-dir", type=str, default="templates")
    args = parser.parse_args()
    main(**vars(args))
