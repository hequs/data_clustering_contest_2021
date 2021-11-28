import json
import argparse
from sklearn.metrics import classification_report


def calc_metrics(language, gold_file, output_file, metrics_file):
    with open(output_file, "r") as output, open(gold_file, "r") as gold:
        true_labels = []
        pred_labels = []
        for o, g in zip(output, gold):
            record = json.loads(o)
            gold_record = json.loads(g)
            l = gold_record["language"]
            if l != language:
                continue
            categories = record["category"]
            best_category = max(categories.items(), key=lambda x: x[1])[0]
            true_category = gold_record["category"]
            true_labels.append(true_category)
            pred_labels.append(best_category)
        print(language, "report")
        print(language, "report", file=metrics_file)
        report = classification_report(true_labels, pred_labels)
        print(report)
        print(report, file=metrics_file)


def main(
    gold_file,
    output_file,
    metrics_file
):
    with open(metrics_file, "w") as w:
        calc_metrics("ru", gold_file, output_file, w)
        calc_metrics("en", gold_file, output_file, w)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("gold_file", type=str)
    parser.add_argument("output_file", type=str)
    parser.add_argument("metrics_file", type=str)
    args = parser.parse_args()
    main(**vars(args))
