#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP

# include <algorithm>
# include <map>
# include <string>
# include <iostream>
# include <exception>

class BitcoinExchange {
    private:
        static const char* DATA_NAME;
        static const std::string DATE_DELIM;
        static const std::string INPUT_DATA_DELIM;
        static const std::string EXEC_DATA_DELIM;
        static const std::string INPUT_HEADER;
        static const std::string EXEC_HEADER;

        std::map<std::string, float> database;

        static bool is_valid_date(const std::string&);
        void readFileAndDoFunc(const char*, bool);
        void addDatabase(const std::string&, float);
        void printInfo(const std::string&, float);

        BitcoinExchange(const BitcoinExchange&);
        BitcoinExchange& operator = (const BitcoinExchange&);
    public:
        BitcoinExchange(void);
        ~BitcoinExchange();

        void exchange(const char*);

        class DuplicateDateException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                DuplicateDateException(const std::string&);
        };
        class FileOpenException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                FileOpenException(void);
        };
        class OverflowException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                OverflowException(void);
        };
        class IllegalFormatException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                IllegalFormatException(const std::string&);
        };
        class NotAPositiveException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                NotAPositiveException(void);
        };
        class TooEarlyDateException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                TooEarlyDateException(const std::string&);
        };
        class EmptyFileException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                EmptyFileException(void);
        };
        class WrongHeaderException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                WrongHeaderException(void);
        };
        class IllegalDateException : public std::runtime_error {
            private:
                static const std::string ERROR_MESSAGE;
            public:
                IllegalDateException(const std::string&);
        };
};

#endif