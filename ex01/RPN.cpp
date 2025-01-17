#include "RPN.hpp"
#include <deque>
#include <limits>
#include <cmath>

const std::string RPN::OverflowException::ERROR_MESSAGE
        = "Error: overflow occurred.";
const std::string RPN::DevideByZeroException::ERROR_MESSAGE
        = "Error: deviding by zero happened.";
const std::string RPN::ModuloByZeroException::ERROR_MESSAGE
        = "Error: moduling by zero happened.";
const std::string RPN::LackNumberException::ERROR_MESSAGE
        = "Error: too few elements to do.";
const std::string RPN::RemainNumberException::ERROR_MESSAGE
        = "Error: calculations are continuing.";
const std::string RPN::OperatorNotFoundException::ERROR_MESSAGE
        = "Error: the operator is not found.";

RPN::RPN() : data() {
}

RPN::RPN(const RPN& src) :data(src.data) {
}

RPN& RPN::operator = (const RPN& src) {
    if (this == &src)
        return *this;
    this->data = src.data;
    return *this;
}

RPN::~RPN() {
}

void RPN::addNumber(double number) {
    this->data.push_back(number);
}

double RPN::calc_plus(double first, double second) {
    if ((second > 0 && first > std::numeric_limits<double>::max() - second) ||
            (second < 0 && - first > std::numeric_limits<double>::max() + second))
        throw RPN::OverflowException();
    return first + second;
}

double RPN::calc_minus(double first, double second) {
    if ((second > 0 && first < - std::numeric_limits<double>::max() + second) ||
            (second < 0 && first > std::numeric_limits<double>::max() + second))
        throw RPN::OverflowException();
    return first - second;
}

double RPN::calc_multi(double first, double second) {
    if (std::abs(second) > 1 && std::abs(first) > std::numeric_limits<double>::max() / std::abs(second))
        throw RPN::OverflowException();
    return first * second;
}

double RPN::calc_devide(double first, double second) {
    if (second == 0)
        throw RPN::DevideByZeroException();
    if (std::abs(second) < 1 && std::abs(first) > std::numeric_limits<double>::max() * std::abs(second))
        throw RPN::OverflowException();
    return first / second;
}

double RPN::calc_modulo(double first, double second) {
    if (second == 0)
        throw RPN::ModuloByZeroException();
    return std::fmod(first, second);
}

bool RPN::can_apply(const std::string& str) {
    return (str.size() == 1 &&
            (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/' || str[0] == '%'));
}

void RPN::applyOperator(const std::string& str) {
    if (str.size() != 1)
        throw RPN::OperatorNotFoundException();
    if (this->data.size() < 2)
        throw RPN::LackNumberException();

    char op = str[0];
    double result;
    double second = this->data.back();
    this->data.pop_back();
    double first = this->data.back();
    this->data.pop_back();

    switch (op) {
    case '+':
        result = RPN::calc_plus(first, second);
        break;
    case '-':
        result = RPN::calc_minus(first, second);
        break;
    case '/':
        result = RPN::calc_devide(first, second);
        break;
    case '*':
        result = RPN::calc_multi(first, second);
        break;
    case '%':
        result = RPN::calc_modulo(first, second);
        break;
    default:
        throw RPN::OperatorNotFoundException();
    }
    this->data.push_back(result);
}

double RPN::printResult() const {
    if (this->data.size() > 1)
        throw RPN::RemainNumberException();
    if (this->data.empty())
        throw RPN::LackNumberException();
    return this->data.back();
}

void RPN::clear() {
    this->data.clear();
}

RPN::OverflowException::OverflowException() : std::runtime_error(RPN::OverflowException::ERROR_MESSAGE) {
}

RPN::DevideByZeroException::DevideByZeroException() : std::runtime_error(RPN::DevideByZeroException::ERROR_MESSAGE) {
}

RPN::ModuloByZeroException::ModuloByZeroException() : std::runtime_error(RPN::ModuloByZeroException::ERROR_MESSAGE) {
}

RPN::RemainNumberException::RemainNumberException() : std::runtime_error(RPN::RemainNumberException::ERROR_MESSAGE) {
}

RPN::LackNumberException::LackNumberException() : std::runtime_error(RPN::LackNumberException::ERROR_MESSAGE) {
}

RPN::OperatorNotFoundException::OperatorNotFoundException() : std::runtime_error(RPN::OperatorNotFoundException::ERROR_MESSAGE) {
}