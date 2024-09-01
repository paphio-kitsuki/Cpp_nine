#include "BitcoinExchange.hpp"
#include <string>
#include <iterator>
#include <cctype>
#include <sstream>
#include <fstream>
#include <utility>
#include <map>
#include <exception>

const char* BitcoinExchange::DATA_NAME("data.csv");
const std::string BitcoinExchange::DATE_DELIM("-");
const std::string BitcoinExchange::INPUT_DATA_DELIM(",");
const std::string BitcoinExchange::EXEC_DATA_DELIM(" | ");
const std::string BitcoinExchange::INPUT_HEADER("date,exchange_rate");
const std::string BitcoinExchange::EXEC_HEADER("date | value");

const std::string BitcoinExchange::DuplicateDateException::ERROR_MESSAGE
        = "Error: duplicate date => ";
const std::string BitcoinExchange::FileOpenException::ERROR_MESSAGE
        = "Error: could not open file.";
const std::string BitcoinExchange::OverflowException::ERROR_MESSAGE
        = "Error: too large a number.";
const std::string BitcoinExchange::IllegalFormatException::ERROR_MESSAGE
        = "Error: bad input => ";
const std::string BitcoinExchange::NotAPositiveException::ERROR_MESSAGE
        = "Error: not a positive number.";
const std::string BitcoinExchange::TooEarlyDateException::ERROR_MESSAGE
        = "Error: too early date => ";
const std::string BitcoinExchange::EmptyFileException::ERROR_MESSAGE
        = "Error: the file is empty.";
const std::string BitcoinExchange::WrongHeaderException::ERROR_MESSAGE
        = "Error: the header is wrong.";
const std::string BitcoinExchange::IllegalDateException::ERROR_MESSAGE
        = "Error: wrong date => ";

static bool is_all_digit(std::string& str) {
    for (std::string::iterator it = str.begin(); it != str.end(); it++) {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}

// グレゴリオ暦
static int get_max_day(int year, int month) {
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        if (year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0))
            return 29;
        return 28;
    }
}

bool BitcoinExchange::is_valid_date(const std::string& date) {
    std::string::size_type prev = 0;
    std::string::size_type pos = 0;
    std::string tmp_date = date + BitcoinExchange::DATE_DELIM;
    int year = 0, month = 0, day = 0;

    for (int i = 0; i < 3; i++) {
        pos = tmp_date.find(BitcoinExchange::DATE_DELIM, pos);
        if (pos == std::string::npos)
            return false;
        std::string tmp = tmp_date.substr(prev, pos - prev);
        std::stringstream ss;
        ss << tmp;
        switch (i) {
        case 0:
            if (tmp.size() != 4 || !is_all_digit(tmp) || !(ss >> year))
                return false;
            break;
        case 1:
            if (tmp.size() != 2 || !is_all_digit(tmp) || !(ss >> month && (1 <= month && month <= 12)))
                return false;
            break;
        default:
            if (tmp.size() != 2 || !is_all_digit(tmp) ||
                    !(ss >> day && (1 <= day && day <= get_max_day(year, month))))
                return false;
        }
        pos += BitcoinExchange::DATE_DELIM.size();
        prev = pos;
    }
    return (tmp_date.find(BitcoinExchange::DATE_DELIM, pos) == std::string::npos);
}

void BitcoinExchange::readFileAndDoFunc(const char* filename, bool is_database) {
    std::string header;
    std::string delim;
    if (is_database) {
        header = BitcoinExchange::INPUT_HEADER;
        delim = BitcoinExchange::INPUT_DATA_DELIM;
    } else {
        header = BitcoinExchange::EXEC_HEADER;
        delim = BitcoinExchange::EXEC_DATA_DELIM;
    }
    std::ifstream ifs;
    std::string line;
    bool is_first = true;

    ifs.open(filename);
    if (!ifs.good())
        throw BitcoinExchange::FileOpenException();
    while (std::getline(ifs, line)) {
        try {
            if (is_first) {
                is_first = false;
                if (line != header)
                    throw BitcoinExchange::WrongHeaderException();
                continue;
            }
            std::string::size_type pos = line.find(delim, 0);
            if (pos == std::string::npos)
                throw BitcoinExchange::IllegalFormatException(line);
            std::string date = line.substr(0, pos);
            std::string value = line.substr(pos + delim.size(), line.size());
            if (!BitcoinExchange::is_valid_date(date))
                throw BitcoinExchange::IllegalDateException(date);
            std::stringstream ss;
            float f_value;
            ss << value;
            if (!(ss >> f_value))
                throw BitcoinExchange::IllegalFormatException(line);
            if (f_value < 0)
                throw BitcoinExchange::NotAPositiveException();
            if (!is_database && f_value > 1000)
                throw BitcoinExchange::OverflowException();
            if (is_database)
                this->addDatabase(date, f_value);
            else
                this->printInfo(date, f_value);
        } catch (const std::exception& e) {
            // std::cerr << "Error: reading " << filename << ": ";
            std::cerr << e.what() << std::endl;
        }
    }
    ifs.close();
    if (is_first)
        throw BitcoinExchange::EmptyFileException();
}

void BitcoinExchange::addDatabase(const std::string& date, float value) {
    if (this->database.find(date) != this->database.end())
        throw BitcoinExchange::DuplicateDateException(date);
    this->database.insert(std::make_pair(date, value));
}

BitcoinExchange::BitcoinExchange() : database() {
    this->readFileAndDoFunc(BitcoinExchange::DATA_NAME, true);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& src) {
    (void) src;
}

BitcoinExchange& BitcoinExchange::operator = (const BitcoinExchange& src) {
    (void) src;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {
}

void BitcoinExchange::printInfo(const std::string& date, float value) {
    float data_value;

    if (this->database.empty())
        throw BitcoinExchange::TooEarlyDateException(date);
    for (std::map<std::string, float>::iterator it = this->database.begin(); it != this->database.end(); it++) {
        if (date < (*it).first) {
            if (it == this->database.begin())
                throw BitcoinExchange::TooEarlyDateException(date);
            break;
        }
        data_value = (*it).second;
    }
    std::cout << date << " => " << value << " = " << (value * data_value) << std::endl;
}

void BitcoinExchange::exchange(const char* filename) {
    this->readFileAndDoFunc(filename, false);
}

BitcoinExchange::DuplicateDateException::DuplicateDateException(const std::string& date) : std::runtime_error(BitcoinExchange::DuplicateDateException::ERROR_MESSAGE + date) {
}

BitcoinExchange::FileOpenException::FileOpenException() : std::runtime_error(BitcoinExchange::FileOpenException::ERROR_MESSAGE) {
}

BitcoinExchange::OverflowException::OverflowException() : std::runtime_error(BitcoinExchange::OverflowException::ERROR_MESSAGE) {
}

BitcoinExchange::IllegalFormatException::IllegalFormatException(const std::string& form) : std::runtime_error(BitcoinExchange::IllegalFormatException::ERROR_MESSAGE + form) {
}

BitcoinExchange::NotAPositiveException::NotAPositiveException() : std::runtime_error(BitcoinExchange::NotAPositiveException::ERROR_MESSAGE) {
}

BitcoinExchange::TooEarlyDateException::TooEarlyDateException(const std::string& date) : std::runtime_error(BitcoinExchange::TooEarlyDateException::ERROR_MESSAGE + date) {
}

BitcoinExchange::EmptyFileException::EmptyFileException() : std::runtime_error(BitcoinExchange::EmptyFileException::ERROR_MESSAGE) {
}

BitcoinExchange::WrongHeaderException::WrongHeaderException() : std::runtime_error(BitcoinExchange::WrongHeaderException::ERROR_MESSAGE) {
}

BitcoinExchange::IllegalDateException::IllegalDateException(const std::string& date) : std::runtime_error(BitcoinExchange::IllegalDateException::ERROR_MESSAGE + date) {
}