#include <iostream>
#include "BIgFloat.h"
#include "BigInteger.h"
#include "Matrix.h"
#include "Formula.h"
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <iterator>
#include <algorithm>
int M_count = 0;
std::map<std::string, Matrix> UniqMatrix;

const int ActionTable[5][5] = {
    // (  ) " " "\0"sym 
       {1, 1, 5, 7, 3}, // )
       {1, 6, 5, 6, 3}, // (
       {1, 1, 5, 7, 3}, // " "
       {1, 6, 5, 6, 6}, // begin
       {2, 2, 4, 6, 3}, // sym
};
int ColTable(char col) {
    if (col == '(') { return 0; }
    if (col == ')') { return 1; }
    if (col == ' ') { return 2; }
    if (col == '\0') { return 3; }
    return 4;
}
int RowTable(char row) {
    if (row == '(') { return 1; }
    if (row == ')') { return 0; }
    if (row == ' ') { return 2; }
    if (row == '\0') { return 3; }
    return 4;
}
std::vector<std::string> Fragmentation(char* str) {
    std::vector<std::string> res;
    std::string Formula;
    int i = 0;
    int action = 0;
    int col = 0;
    int row = 0;

    do {
        row = (i == 0) ? 3 : RowTable(str[i - 1]); //prev
        col = ColTable(str[i]);

        switch (ActionTable[row][col]) {
        case 1: {
            Formula.clear();
            Formula.push_back(str[i]);
            res.push_back(Formula);
            Formula.clear();
            break;
        }
        case 2: {
            res.push_back(Formula);
            Formula.clear();
            Formula.push_back(str[i]);
            res.push_back(Formula);
            Formula.clear();
            break;
        }
        case 3: {
            Formula.push_back(str[i]);
            break;
        }
        case 4: {
            res.push_back(Formula);
            Formula.clear();
            break;
        }
        case 6: {
            throw 2;
            break;
        }
        default: {
            break;
        }

        }
    } while (str[++i] != '\0');

    return res;
}



void to_func(std::vector<Matrix> operands, const std::string& func, Matrix& res) {


    if (func == "=") { res = operands[0]; }
    else  if (func == "+") { res = operands[0] + operands[1]; }
    else if (func == "-" and operands.size() != 1) { res = operands[0] - operands[1]; }
    else if (func == "-" and operands.size() == 1) { res = -operands[0]; }
    else if (func == "horzcat") { res = horzcat(operands); }
    else if (func == "vertcat") { res = vertcat(operands); }
    else if (func == "transpose") { res = operands[0].transpose(); }
    else if (func == "det") { res = operands[0].det(); }
    else if (func == "inv") { res = operands[0].inv(); }
    else if (func == "sum") { res = operands[0].sum(); }
    else if (func == "prod") { res = operands[0].prod(); }
    else if (func == "min") { res = operands[0].Min(operands[1]); }
    else if (func == "max") { res = operands[0].Max(operands[1]); }
    else if (func == ".*") { res = operands[0].ElMult(operands[1]); }
    else if (func == "./") { res = operands[0].ElDiv(operands[1]); }
    else if (func == "*") { res = operands[0] * (operands[1]); }
    else if (func == "/") { res = operands[0] / (operands[1]); }
    else if (func == "zeroes") { res = zeros(*(operands[0].getel(0, 0)).getnum().real_number(), *(operands[1].getel(0, 0)).getnum().real_number()); }
    else if (func == "ones") { res = ones(*(operands[0].getel(0, 0)).getnum().real_number(), *(operands[1].getel(0, 0)).getnum().real_number()); }
    else if (func == "eye") { res = eye(*(operands[0].getel(0, 0)).getnum().real_number()); }
    else if (func == "linspace") { res = linspace(operands[0].getel(0, 0), operands[1].getel(0, 0), *(operands[2].getel(0, 0)).getnum().real_number()); }
    


}

void like_out(const std::vector<std::string>& in, std::stringstream& str) {
    str.str("");
    str.clear();
    //на двух стеках
    std::stack<std::string> full;
    std::stack<std::string> inside;
    int count = 0;

    while (count < in.size()) {
        // Заполняем стек full до закрывающей скобки
        while (in[count] != ")") {
            full.push(in[count]);
            count++;
        }
        ++count;
        // Переносим элементы из стека full в стек inside до открывающей скобки
        while (full.top() != "(") {
            inside.push(full.top());
            full.pop();
        }
        if (full.top() == "(") {
            full.pop();
        }
        std::string func = inside.top(); inside.pop(); // сверху будет как раз команда
        bool havename = 0;
        std::vector<Matrix> operands;
        std::string name;
        std::string value;
        // Обрабатываем оставшиеся элементы в стеке inside
        while (!inside.empty()) {
            value = inside.top();
            inside.pop();

            if (std::all_of(value.begin(), value.end(), ::isdigit)) {
                operands.push_back(Matrix(BigFloat(std::stoi(value))));
                continue;
            }
            auto it = UniqMatrix.find(value);
            if (it == UniqMatrix.end()) { //не нашло
                name = value;
                havename = 1;
                continue;
            }
            if (func == "disp") {
                name = value;
                havename = 1;
                break;
            }
            operands.push_back(it->second);
        }

        Matrix res = UniqMatrix[value];

        if (func != "=") {
            name = "M" + std::to_string('a' + M_count);
            ++M_count;
        }

        if (func != "disp") {
            to_func(operands, func, res);
        }

        else if (func == "disp") {
            if (value[0] == 'M') {
                str << "ans = ";
            }
            else {   //если имя  "M****" - todo
                str << value << " = ";
            }

            str << res << std::endl;
        }


        full.push(name);
        UniqMatrix[name] = res;

    }
}

int main()
{
    std::stringstream str;
    char c[1024];
    while (c) {
        std::cin.getline(c, 1024);
        std::vector<std::string> q = Fragmentation(c);
        like_out(q, str);
        std::cout << str.str();
    }
    
    
}

