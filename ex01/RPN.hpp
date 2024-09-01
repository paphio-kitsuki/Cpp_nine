#ifndef RPN_HPP
# define RPN_HPP

# include <deque>
# include <iostream>

// 例外が発生したあとの計算の保存状態は未定義なので、
// 引き続き使用したい場合はclear()の実行を推奨する
class RPN {
    private:
        static double calc_plus(double, double);
        static double calc_minus(double, double);
        static double calc_multi(double, double);
        static double calc_devide(double, double);
        static double calc_modulo(double, double);

        std::deque<double> data;
    public:
        RPN(void);
        RPN(const RPN&);
        RPN& operator = (const RPN&);
        ~RPN();

        void addNumber(double);
        void applyOperator(char);
        double printResult(void) const;
        void clear(void);

        class OverflowException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                OverflowException(void);
        };
        class DevideByZeroException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                DevideByZeroException(void);
        };
        class ModuloByZeroException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                ModuloByZeroException(void);
        };
        class RemainNumberException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                RemainNumberException(void);
        };
        class LackNumberException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                LackNumberException(void);
        };
        class OperatorNotFoundException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                OperatorNotFoundException(void);
        };
};

#endif