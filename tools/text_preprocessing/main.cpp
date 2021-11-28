#include <text_preprocessing.h>

#include <onmt/Tokenizer.h>
#include <unicode/unistr.h>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

constexpr auto TEXT_LENGTH_LIMIT = 512;

static const onmt::Tokenizer Tokenizer(onmt::Tokenizer::Mode::Conservative);

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << "<mode> <input_file> <output_file> [<omit_label>]" << std::endl;
        return 1;
    }

    std::string mode(argv[1]);
    if (mode != "language" && mode != "category") {
        std::cerr << "Bad mode: " << mode << std::endl;
        return 1;
    }

    bool omitLabel = false;
    if (argc > 4) {
        std::istringstream(argv[4]) >> std::boolalpha >> omitLabel;
    }

    std::cerr << "Mode: " << mode << std::boolalpha << " (omit label - " << omitLabel << ")" << std::endl;

    std::ifstream fin(argv[2]);
    std::ofstream fout(argv[3]);

    size_t i = 0;

    std::string line;
    if (fin.is_open()) {
        while (std::getline(fin, line)) {
            std::string label = {};
            if (!omitLabel) {
                const size_t pos = line.find(' ');
                if (pos == std::string::npos) {
                    std::cerr << "bad string: " << line << std::endl;
                }
                label = line.substr(0, pos);
                line = line.substr(pos + 1);
            }

            std::string processedLine = {};
            if (mode == "language") {
                processedLine = NTP::LangPreprocess(line.c_str(), Tokenizer, 256);
            } else {
                processedLine = NTP::TopicPreprocess(line.c_str(), Tokenizer, 256);
            }

            if (!omitLabel) {
                fout << label << " ";
            }
            fout << processedLine << std::endl;

            if (++i % 10000 == 0) {
                std::cout << i << std::endl;
            }
        }
        fin.close();
    }
    fout.close();

    return 0;
};
