//
// Created by HP on 2020-11-12.
//

int getPriority(char ch) {
    int priority = 0;
    if (ch == '+' || ch == '-')
        priority = 1;
    else if (ch == '*' || ch == '/')
        priority = 2;
    else if (ch == '(' || ch == ')')
        priority = 0;
    return priority;
}

//c1的优先级是否比c2大
bool isBetterOREqual(char c1, char c2) {
    return getPriority(c1) >= getPriority(c2);
}

bool isLeftBracket(char ch) {
    return ch == '(';
}

bool isRightBracket(char ch) {
    return ch == ')';
}

bool isNum(char ch) {
    return ch >= '0' && ch <= '9';
}

bool isMathSymbol(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int getNum(char ch) {
    return ch - '0';
}

bool isNumAndMathSymbol(char ch) {
    return isNum(ch) || isMathSymbol(ch) || isLeftBracket(ch) || isRightBracket(ch);
}

int calculate(int x, int y, char op) {
    int result = 0;
    if (op == '+')
        result = x + y;
    else if (op == '-')
        result = x - y;
    else if (op == '*')
        result = x * y;
    else if (op == '/')
        result = x / y;
    return result;
}

int getMax(const int *list, int len) {
    if (list != nullptr && len > 0) {
        int max = list[0];
        for (int i = 1; i < len; ++i) {
            if (list[i] > max) {
                max = list[i];
            }
        }
        return max;
    }
    return -1;
}