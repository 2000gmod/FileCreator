#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const std::string templatePath("/usr/local/etc/filecreator/templates");

void doTextReplacement(const std::string &text);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: filecreator [template]\n";
    }
    else {
        std::string arg(argv[1]);
        arg = templatePath + "/" + arg;
        std::ifstream file(arg);
        std::stringstream buffer;

        if (!(buffer << file.rdbuf())) {
            std::cout << "File not found \n";
            return EXIT_FAILURE;
        }
        file.close();
        doTextReplacement(buffer.str());
    }

    return 0;
}

int findClosingIndex(const std::string &text, int currIndex) {
    for (unsigned long i = currIndex; i < text.size(); i++) {
        std::string lookahead;
        lookahead += text[i];
        lookahead += text[i + 1];
        lookahead += text[i + 2];
        
        if (lookahead == "}}}") return i + 2;
    }
    return -1;
}

void doTextReplacement(const std::string &text) {
    std::cout << "Generated file name: ";
    std::string outputFileName;
    std::getline(std::cin, outputFileName);
    std::string newFile;
    std::cout << "----------------\n";

    for (unsigned long i = 0; i < text.size(); i++) {
        std::string lookahead;
        lookahead += text[i];
        lookahead += text[i + 1];
        lookahead += text[i + 2];

        if (lookahead == "{{{") {
            int closingPair = findClosingIndex(text, i);
            std::string questionText = text.substr(i + 3, closingPair - i - 5);
            std::cout << questionText << ": ";
            std::string replacementText;
            std::getline(std::cin, replacementText);
            newFile += replacementText;
            i = closingPair;
            continue;
        }
        else {
            newFile += text[i];
        }
    }

    std::ofstream out(outputFileName);
    out << newFile;
    out.close();
}