#include "RPN.hpp"
#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: wrong arguments." << std::endl;
        return 1;
    }
    std::stringstream ss;
    double d;
    char c;
    RPN rpn;

    ss << argv[1];
    while (!ss.eof()) {
        if (!(ss >> d)) {
            if (!(ss >> c))
                throw std::runtime_error("Error: failed to read input.");
            rpn.applyOperator(c);
        }
        std::cout << (bool)(ss >> d) << ": " << d << std::endl;
        //ss.clear();
        //std::cout << (bool)(ss >> s) << ": " << s << std::endl;
        std::cout << (bool)(ss >> d) << ": " << d << std::endl;
        std::cout << (bool)(ss >> d) << ": " << d << std::endl;
        std::cout << (bool)(ss >> s) << ": " << s << std::endl;
        std::cout << (bool)(ss >> d) << ": " << d << std::endl;
        std::cout << (bool)(ss >> s) << ": " << s << std::endl;
    }
}