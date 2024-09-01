#include "BitcoinExchange.hpp"
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error: wrong number of arguments." << std::endl;
        return 1;
    }

    try {
        BitcoinExchange btc;
        btc.exchange(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}