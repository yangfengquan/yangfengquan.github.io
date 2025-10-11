#ifndef READABLE_CONDITION_PARSER_H
#define READABLE_CONDITION_PARSER_H

#include <string>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <vector>

class ConditionParser {
private:
    std::string expression;
    size_t currentPosition = 0;
    std::string variableName;

    void skipWhitespace() {
        while (currentPosition < expression.size() && std::isspace(expression[currentPosition])) {
            currentPosition++;
        }
    }

    bool hasMoreCharacters() const {
        return currentPosition < expression.size();
    }

    char getCurrentCharacter() const {
        return hasMoreCharacters() ? expression[currentPosition] : '\0';
    }

    bool isVariableAtCurrentPosition() const {
        if (currentPosition + variableName.size() > expression.size()) {
            return false;
        }
        for (size_t i = 0; i < variableName.size(); i++) {
            if (expression[currentPosition + i] != variableName[i]) {
                return false;
            }
        }
        return true;
    }

    double parseValue(double variableValue) {
        skipWhitespace();

        if (isVariableAtCurrentPosition()) {
            currentPosition += variableName.size();
            return variableValue;
        }

        size_t startPosition = currentPosition;

        if (getCurrentCharacter() == '+' || getCurrentCharacter() == '-') {
            currentPosition++;
        }

        if (!std::isdigit(getCurrentCharacter())) {
            throw std::invalid_argument("表达式中缺少数字字符");
        }
        while (std::isdigit(getCurrentCharacter())) {
            currentPosition++;
        }

        if (getCurrentCharacter() == '.') {
            currentPosition++;
            if (!std::isdigit(getCurrentCharacter())) {
                throw std::invalid_argument("表达式中小数点后缺少数字");
            }
            while (std::isdigit(getCurrentCharacter())) {
                currentPosition++;
            }
        }

        return std::stod(expression.substr(startPosition, currentPosition - startPosition));
    }

    std::string parseOperator() {
        skipWhitespace();

        if (!hasMoreCharacters()) {
            throw std::invalid_argument("表达式中缺少运算符");
        }

        char firstChar = getCurrentCharacter();
        if (firstChar != '<' && firstChar != '>' && firstChar != '=') {
            throw std::invalid_argument("无效的运算符: " + std::string(1, firstChar));
        }

        currentPosition++;

        if (getCurrentCharacter() == '=') {
            currentPosition++;
            return std::string(1, firstChar) + "=";
        }

        return std::string(1, firstChar);
    }

    // 执行单次比较
    bool compare(double left, const std::string& op, double right) {
        if (op == "<")      return left < right;
        if (op == ">")      return left > right;
        if (op == "=" || op == "==") return left == right;
        if (op == "<=")     return left <= right;
        if (op == ">=")     return left >= right;
        throw std::invalid_argument("未知的运算符: " + op);
    }

public:
    ConditionParser(std::string expr, std::string varName)
        : expression(std::move(expr)), variableName(std::move(varName)) {
        auto end = std::remove_if(expression.begin(), expression.end(), ::isspace);
        expression.erase(end, expression.end());

        if (variableName.empty()) {
            throw std::invalid_argument("变量名不能为空");
        }
    }

    bool evaluate(double variableValue) {
        currentPosition = 0;

        // 至少需要解析一个左值
        if (!hasMoreCharacters()) {
            throw std::invalid_argument("表达式为空，缺少比较运算");
        }

        double currentValue = parseValue(variableValue);
        bool hasComparison = false;

        // 循环解析运算符和右值（支持连续比较）
        while (hasMoreCharacters()) {
            std::string op = parseOperator();
            double nextValue = parseValue(variableValue);
            hasComparison = true;

            // 执行当前比较
            if (op == "<" && !(currentValue < nextValue)) return false;
            if (op == ">" && !(currentValue > nextValue)) return false;
            if ((op == "=" || op == "==") && !(currentValue == nextValue)) return false;
            if (op == "<=" && !(currentValue <= nextValue)) return false;
            if (op == ">=" && !(currentValue >= nextValue)) return false;

            // 连续比较时，用右值作为下一轮的左值（如 a < b < c 中，b 作为下一轮左值）
            currentValue = nextValue;
        }

        // 确保至少有一次比较
        if (!hasComparison) {
            throw std::invalid_argument("表达式格式错误，缺少比较运算");
        }

        return true;
    }
};

#endif // READABLE_CONDITION_PARSER_H