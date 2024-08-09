#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

struct information {
    std::string type;
    std::string functionName;
    std::string docstring;
};

void removeWhitespace(std::string &s) {
    s.erase(std::unique(s.begin(), s.end(),
                      [] (char a, char b) {return a == '\n' && b == '\n';}),
                        s.end());
}

std::string createPattern(const std::vector<std::string> &types) {
    std::string pattern = "(";
    for (size_t i = 0; i < types.size(); ++i) {
        pattern += types[i];
        if (i < types.size() - 1) {
            pattern += "|";
        }
    }
    pattern += ")\\s+\\w+\\s*\\([^)]*\\)\\s*\\{?";
    pattern = "(?!" + pattern + ".*=).*" + pattern;

    return pattern;
}

std::vector<std::string> findAllFunctions(const std::string& input) {
    std::vector<std::string> functions;
    
    std::vector<std::string> functionPrefixes = {"int", "void", "string", "def"};

    std::regex pattern(createPattern(functionPrefixes));

    std::sregex_iterator begin(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    for (std::sregex_iterator i = begin; i != end; ++i) {
        functions.push_back(i->str());
    }

    return functions;
}


int main() {
    std::ifstream file("test.cpp");

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string fileAsString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    removeWhitespace(fileAsString);
    
    std::vector<std::string> functions = findAllFunctions(fileAsString);

    std::cout << "Found functions:" << std::endl;
    for ( auto &f : functions) {
        std::cout << f << std::endl;
    }

    return 0;
}


/**
 * "docstring with a lot of information"
 * 
 * void function()
 * 
 * =========================
 * 
 * The idea is to remove all whiteline / empty spaces, find all function names
 * and read their docstring if they have one.
 */
