#include "RPN.hpp"
#include <iostream>
#include <string>
#include <sstream>

static void read_command(RPN&, const char*);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: wrong arguments." << std::endl;
        return 1;
    }

    try {
        RPN rpn;
        read_command(rpn, argv[1]);
        std::cout << rpn.printResult() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

static void read_command(RPN& rpn, const char* input) {
    std::stringstream ss;
    std::string element;

    ss << input;
    while (!ss.eof()) {
        ss >> element;
        if (ss.fail())
            throw std::runtime_error("Error: failed to read input.");
        if (RPN::can_apply(element))
            rpn.applyOperator(element);
        else {
            double d;
            std::stringstream trans;

            trans << element;
            trans >> d;
            if (trans.fail())
                throw std::runtime_error("Error: invalid input.");
            rpn.addNumber(d);
        }
    }
}