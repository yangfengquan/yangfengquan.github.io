#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <algorithm>

class FormulaParser {
private:
    std::string expr;       // 表达式字符串
    std::string varName;    // 变量名（如"x"、"t"等）
    size_t pos;             // 当前解析位置

    // 跳过空白字符
    void skipWhitespace() {
        while (pos < expr.size() && std::isspace(expr[pos])) {
            pos++;
        }
    }

    // 检查是否还有字符
    bool hasMoreChars() {
        return pos < expr.size();
    }

    // 获取当前字符
    char currentChar() {
        return (pos < expr.size()) ? expr[pos] : '\0';
    }

    // 解析数字
    double parseNumber() {
        size_t start = pos;

        // 处理正负号
        if (currentChar() == '+' || currentChar() == '-') {
            if (pos == 0 || expr[pos - 1] == '+' || expr[pos - 1] == '-' ||
                expr[pos - 1] == '*' || expr[pos - 1] == '/' || expr[pos - 1] == '^' ||
                expr[pos - 1] == '(') {
                pos++;
            }
            else {
                return std::stod(expr.substr(start, 1));
            }
        }

        // 整数部分
        while (hasMoreChars() && std::isdigit(currentChar())) {
            pos++;
        }

        // 小数部分
        if (hasMoreChars() && currentChar() == '.') {
            pos++;
            while (hasMoreChars() && std::isdigit(currentChar())) {
                pos++;
            }
        }

        // 指数部分 (e或E)
        if (hasMoreChars() && (currentChar() == 'e' || currentChar() == 'E')) {
            pos++;
            if (hasMoreChars() && (currentChar() == '+' || currentChar() == '-')) {
                pos++;
            }
            while (hasMoreChars() && std::isdigit(currentChar())) {
                pos++;
            }
        }

        // 转换为数字
        std::string numStr = expr.substr(start, pos - start);
        try {
            return std::stod(numStr);
        }
        catch (...) {
            throw std::invalid_argument("无效的数字格式: " + numStr);
        }
    }

    // 检查是否匹配变量名
    bool matchVariable() {
        if (pos + varName.size() > expr.size()) {
            return false;
        }
        // 检查变量名是否匹配
        for (size_t i = 0; i < varName.size(); ++i) {
            if (expr[pos + i] != varName[i]) {
                return false;
            }
        }
        // 确保变量名后面不是字母或数字（避免部分匹配）
        size_t nextPos = pos + varName.size();
        if (nextPos < expr.size() && (std::isalnum(expr[nextPos]))) {
            return false;
        }
        return true;
    }

    // 解析因子
    double parseFactor(double varValue) {
        skipWhitespace();

        if (!hasMoreChars()) {
            throw std::invalid_argument("表达式不完整");
        }

        // 处理括号
        if (currentChar() == '(') {
            pos++;  // 跳过 '('
            double result = parseExpression(varValue);  // 递归解析括号内的表达式
            skipWhitespace();

            if (!hasMoreChars() || currentChar() != ')') {
                throw std::invalid_argument("括号不匹配: 缺少 ')'");
            }
            pos++;  // 跳过 ')'
            return result;
        }
        // 处理变量
        else if (matchVariable()) {
            pos += varName.size();  // 跳过变量名
            // 检查是否需要处理隐式乘法
            if (hasMoreChars() && (currentChar() == '(' || matchVariable() || std::isdigit(currentChar()))) {
                pos -= varName.size();  // 回退变量名位置，让parseTerm处理乘法
            }
            return varValue;
        }
        // 处理数字
        else if (std::isdigit(currentChar()) || currentChar() == '+' || currentChar() == '-') {
            return parseNumber();
        }

        throw std::invalid_argument("无效字符: " + std::string(1, currentChar()));
    }

    // 解析幂运算(^)
    double parsePower(double varValue) {
        double result = parseFactor(varValue);
        skipWhitespace();

        while (hasMoreChars() && currentChar() == '^') {
            pos++;  // 跳过 '^'
            skipWhitespace();
            double exponent = parseFactor(varValue);
            result = std::pow(result, exponent);
            skipWhitespace();
        }

        return result;
    }

    // 解析乘除运算
    double parseTerm(double varValue) {
        double result = parsePower(varValue);
        skipWhitespace();

        while (hasMoreChars()) {
            // 处理显式乘除
            if (currentChar() == '*' || currentChar() == '/') {
                char op = currentChar();
                pos++;  // 跳过运算符
                skipWhitespace();

                double factor = parsePower(varValue);

                if (op == '*') {
                    result *= factor;
                }
                else {  // '/'
                    if (factor == 0) {
                        throw std::domain_error("除以零错误");
                    }
                    result /= factor;
                }
                skipWhitespace();
            }
            // 处理隐式乘法 (如 var(...) 或 number(...) 或 (...) (...) )
            else if (currentChar() == '(' || matchVariable() || std::isdigit(currentChar())) {
                double factor = parsePower(varValue);
                result *= factor;
                skipWhitespace();
            }
            else {
                break;  // 退出循环
            }
        }

        return result;
    }

    // 解析加减运算
    double parseExpression(double varValue) {
        skipWhitespace();
        if (!hasMoreChars()) {
            throw std::invalid_argument("表达式为空");
        }

        double result = parseTerm(varValue);
        skipWhitespace();

        while (hasMoreChars() && (currentChar() == '+' || currentChar() == '-')) {
            char op = currentChar();
            pos++;  // 跳过运算符
            skipWhitespace();

            double term = parseTerm(varValue);

            if (op == '+') {
                result += term;
            }
            else {
                result -= term;
            }
            skipWhitespace();
        }

        return result;
    }

public:
    // 构造函数：接收表达式和变量名（默认变量名为"x"）
    FormulaParser(const std::string& expression, const std::string& variableName = "x")
        : expr(expression), varName(variableName), pos(0) {
        // 验证变量名是否有效
        if (varName.empty()) {
            throw std::invalid_argument("变量名不能为空");
        }
        for (char c : varName) {
            if (!std::isalnum(c)) {
                throw std::invalid_argument("无效的变量名: " + varName);
            }
        }
        // 移除表达式中的所有空白
        expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());
    }

    // 计算表达式值，接收变量值作为参数
    double evaluate(double varValue = 0.0) {
        pos = 0;  // 重置解析位置
        double result = parseExpression(varValue);

        if (pos != expr.size()) {
            throw std::invalid_argument("表达式末尾有无效字符: " +
                expr.substr(pos));
        }

        return result;
    }
};

#endif // FORMULA_PARSER_H