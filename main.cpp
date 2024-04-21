#include <iostream>
#include <vector>
#include <variant>
#include <unordered_map>

struct OpeningBracket//операция открытия скобки
{
    const char value = '(';
};

struct ClosingBracket//операция закрытия скобки
{
    const char value = ')';
};

struct Number//число
{
    const int value;
};

struct UnknownToken//неизвестная операция
{
    const std::string value;
};

struct MinToken//операция меньше
{
    const std::string value = "min";
};

struct AbsToken//модуль
{
    const std::string value = "abs";  
};

struct PlusToken//плюс
{
    const char value = '+';
};

struct MinusToken//минус
{
    const char value = '-';
};

struct MultiplyToken//умножить
{
    const char value = '*';
};

struct ModuloToken//взятие остатка
{
    const char value = '%';
};

struct DivideToken//деление
{
    const char value = '/';
};

struct MaxToken//максимум
{
    const std::string value = "max";
};

struct SqrToken//квадрат
{
    const std::string value = "sqr";
};

struct CommaToken//запятая
{
    const char value = ',';
};


using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, PlusToken, MinusToken, MultiplyToken, ModuloToken, DivideToken, MaxToken, SqrToken, CommaToken>;

const std::unordered_map<unsigned char, Token> kSymbol2Token { {'+', PlusToken{}}, {'-', MinusToken{}}, {'*', MultiplyToken{}}, {'%', ModuloToken{}}, {'/', DivideToken{}}, {'(', OpeningBracket{}}, {')', ClosingBracket{}}, {',', CommaToken{}} };
const std::unordered_map<std::string, Token> kString2Token { {"max", MaxToken{}}, {"min", MinToken{}}, {"sqr", SqrToken{}}, {"abs", AbsToken{}} };

int ToDigit(unsigned char symbol) //переводим их символа в число
{
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos)//парсим целое число
{
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);

    while (std::isdigit(symbol))
    {
        value = value * 10 + ToDigit(symbol);

        if (pos == input.size() - 1)//если дошли до конца выражения
        {
            break;
        }

        symbol = static_cast<unsigned char>(input[++pos]);
    }

    return Number{value}; 
}

Token ParseName(const std::string& input, size_t& pos)//считываем слово
{
    std::string name;
    auto symbol = static_cast<unsigned char>(input[pos]);

    while (std::isalpha(symbol))
    {
        name.push_back(symbol);

        if (pos == input.size() - 1)
        {
            break;
        }

        symbol = static_cast<unsigned char>(input[++pos]);
    }

    if (auto it = kString2Token.find(name); it != kString2Token.end()) return it->second;//ищем по ключу ключевое слово

    ++pos;

    return UnknownToken{};//возвращаем, если считали неизвестное слово
}

std::vector<Token> Tokenize(const std::string& input)//разбиваем всю строку на токены
{
    std::vector<Token> tokens;//вектор для ответа
    const size_t size = input.size();
    size_t pos = 0;//текущая позиция в строке

    while (pos < size)
    {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol))
        {
            ++pos;
        } else if (std::isdigit(symbol))//проверка на число
        {
            tokens.emplace_back(ParseNumber(input, pos));
        } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end())//ищем по ключу ключевое слово
        {
            tokens.push_back(it->second);
            ++pos;
        } else if (std::isalpha(symbol))//проверка на ключевое слово
        {
            tokens.emplace_back(ParseName(input, pos));
        }
    }

    return tokens;
}

int main() 
{
    std::vector<Token> copy;

    copy = Tokenize("14 + 45 - max(1, 5) * sqr(4) + 14 * (5 + 40)");

    return 0;
}