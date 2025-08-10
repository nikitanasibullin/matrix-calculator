#include "BigInteger.h"
#include <iostream>
#include <utility>
#include <string>
#include <cstring>



bool isDigit(char c) {
    return c >= '0' && c <= '9';
}


BigInteger::BigInteger(const std::string& str)
{
    data_ = new unsigned int[1];
    data_[0] = 0;
    size_ = 1;
    is_empty = 1;


    for (int x = 1; x < str.length(); x++) {
        if (!isDigit(str[x])) {
            delete[] data_;
            throw std::invalid_argument("1");

        }
    }
    int cnt = 0;
    for (int x = 0; x < str.length(); x++) {
        if (isDigit(str[x])) {
            cnt++;

        }
    }
    if (cnt == 0) {
        delete[] data_;
        throw std::invalid_argument("1");
    }


    std::string number(str);
    std::string resdel = "";
    if (str[0] == '-' or str[0] == '+') {

        for (int i = 1; i < number.length(); ++i) {
            number[i - 1] = number[i];
        }
        number[number.length() - 1] = 0;
        number.pop_back();

    }

    do {
        if (resdel != "") {
            number = resdel;
        }
        resdel = "";
        long long int i = 0;
        unsigned long long int x = 0;
        unsigned long long int y = 0;
        int is_started = 0;
        std::string temp;
        while (i < number.length()) {

            temp = std::to_string(y) + number[i];
            ///std::cout << temp<<"    ";
            x = std::stoull(temp) / base;
            if (is_started == 0 and x != 0) {
                is_started = 1;
            }
            //std::cout << x<<" ";
            y = static_cast<unsigned long long>(std::stoull(temp)) % base;
            //std::cout << y << std::endl;
            ++i;
            if (is_started == 1) {
                resdel = resdel + std::to_string(x);
            }

        }
        //std::cout << resdel;
        i = 0;
        x = 0;
        unsigned long long int resproc=0;

        while (i < number.length()) {
            temp = std::to_string(x) + number[i];
            //std::cout << temp << "    ";
            x = std::stoull(temp) % base;
            if (is_started == 0 and x != 0) {
                is_started = 1;
            }
            //std::cout << x << std::endl;



            ++i;
        }
        resproc = x;
        push_back(resproc);

    } while (resdel != "");

    if (str[0] == '-') {
        if (size_ == 1 and data_[0] == 0) {
            sign_ = 1;

        }
        else
        {
            sign_ = -1;
        }

    }
    else { sign_ = 1; }






    is_empty = 0;

    while (size() > 1 and longnumber()[size() - 1] == 0) {

        pop_back();

    }

}





BigInteger::BigInteger(long long int x)
{
    if (x == 0) {
        data_ = new unsigned int[1];
        size_ = 1;
        data_[0] = 0;
        sign_ = 1;
        is_empty = 1;
        return;
    }
    size_ = 0;
    data_ = nullptr;
    is_empty = 0;
    if (x < 0) {
        sign_ = -1;
        x *= -1;

    }
    else {
        sign_ = 1;
    }

    while (x > 0) {

        push_back(x % base);
        x /= base;


    }

}



BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        BigInteger tmp(other);
        swap_mv(tmp);
    }
    is_empty = 0;
    return *this;
}
BigInteger& BigInteger::operator=(BigInteger&& other) {
    if (this != &other) {
        swap_mv(other);
    }
    //std::cout << "hey";
    is_empty = 0;
    return *this;
}

BigInteger::BigInteger(const BigInteger& other) {//êîïèðóþùèé êîíñòðóêòîð
    size_ = other.size_;
    sign_ = other.sign_;
    data_ = new unsigned int[size_];
    for (int i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
    is_empty = 0;

}

BigInteger::BigInteger(BigInteger&& other) {
    size_ = 0;
    data_ = nullptr;
    sign_ = 1;
    swap_mv(other);
}
BigInteger::~BigInteger() {
    delete[] data_;
}



bool BigInteger::operator==(const BigInteger& right) const {

    if (size_ != right.size()) {

        return false;
    }
    if ((*this).size() == 1 and data_[0] == 0 and right.size() == 1 and right.longnumber()[0] == 0) { //åñëè íóëè, ÷òîáû íå ñðàâíèâàòü çíàê
        return true;
    }
    int i = 0;
    while (i < size_) {
        if (data_[i] != right.longnumber()[i]) {
            return false;
        }
        ++i;
    }
    if ((*this).is_pos() != right.is_pos()) {
        return false;
    }
    return true;

}
bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

bool abs_less(const BigInteger& left, const BigInteger& right) {
    if (left.size() < right.size()) {
        return true;
    }
    if (left.size() > right.size()) {
        return false;
    }
    for (int i = left.size() - 1; i >= 0; --i) {
        if (left.longnumber()[i] > right.longnumber()[i]) {
            return false;
        }
        if (left.longnumber()[i] < right.longnumber()[i]) {
            return true;
        }
    }
    return false;
}
bool operator<(const BigInteger& left, const BigInteger& right) {
    if (left.is_pos() and right.is_pos()) {
        return abs_less(left, right);
    }
    if (left.is_pos() and !right.is_pos()) {
        return false;
    }
    if (!left.is_pos() and right.is_pos()) {
        return true;
    }
    if (!left.is_pos() and !right.is_pos()) {
        return !abs_less(left, right);
    }
    return false;
}
bool operator<=(const BigInteger& left, const BigInteger& right) {
    return(left < right) or left == right;
}
bool operator>(const BigInteger& left, const BigInteger& right) {
    return  !(left < right) and left != right;
}
bool operator>=(const BigInteger& left, const BigInteger& right) {
    return !(left < right);
}


BigInteger operator-(BigInteger other) {
    other.minus();
    return other;
}

BigInteger operator+(BigInteger other) {
    return other;
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {

    if (left.is_pos() and !right.is_pos()) {
        return left - (-right);
    }
    if (!left.is_pos() and !right.is_pos()) {
        return -((-left) + (-right));
    }

    if (!left.is_pos() and right.is_pos()) {
        return left - (-right);
    }


    BigInteger res = 0;
    res.set_sign();
    long long int current = 0;
    long long int carry = 0;
    int min_size = (left.size() < right.size() ? left.size() : right.size());
    int max_size = (left.size() > right.size() ? left.size() : right.size());
    for (int i = 0; i < min_size; ++i) {

        current = carry + left.longnumber()[i] + right.longnumber()[i];
        res.push_back(current % base);
        carry = current / base;
    }
    if (min_size < max_size) {
        for (int i = min_size; i < max_size; ++i) {
            if (left.size() > right.size()) {
                current = carry + static_cast<long long>(left.longnumber()[i]);
                res.push_back(current % base);
                carry = current / base;
            }
            else {
                current = carry + static_cast<long long>(right.longnumber()[i]);
                res.push_back(current % base);
                carry = current / base;
            }
        }
    }
    if (carry != 0) { res.push_back(carry); }


    return res;
}
BigInteger operator-(const BigInteger& left, const BigInteger& right) {

    if (left.is_pos() and !right.is_pos()) {
        return left + (-right);
    }
    if (!left.is_pos() and right.is_pos()) {
        return -(-left + right);
    }
    if (!left.is_pos() and !right.is_pos()) {
        return (-right) - (-left);
    }

    BigInteger res = 0;
    res.set_sign();
    long long int borrow = 0;
    bool isneg = 0;
    if (!abs_less(left, right)) {


        for (size_t i = 0; i < left.size() || i < right.size() || borrow != 0; ++i) {
            long long int sub = static_cast<unsigned long long>(i < left.size() ? left.longnumber()[i] : 0) - borrow - static_cast<unsigned long long>(i < right.size() ? right.longnumber()[i] : 0);
            if (sub < 0) {
                sub = sub + base;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            res.push_back(sub);
        }
    }


    else {
        for (size_t i = 0; i < right.size() || i < left.size() || borrow != 0; ++i) {
            long long int sub = static_cast<long long>(i < right.size() ? right.longnumber()[i] : 0) - borrow - static_cast<long long>(i < left.size() ? left.longnumber()[i] : 0);
            if (sub < 0) {
                sub += base;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            res.push_back(sub);
        }
        isneg = 1;
    }
    while (res.size() > 1 and res.longnumber()[res.size() - 1] == 0) {

        res.pop_back();

    }

    if (isneg) {
        return -res;
    }
    return res;
}
BigInteger operator*(const BigInteger& left, const BigInteger& right) {
    BigInteger res = 0;
    if (left == 0 or right == 0) {
        return res;
    }
    if ((!left.is_pos() and right.is_pos()) or (left.is_pos() and !right.is_pos())) {
        res.minus();
    }
    for (int x = 0; x < left.size() * right.size() + 1; ++x) {
        res.push_back(0);
    }
    for (int i = 0; i < left.size(); ++i) {
        unsigned long long carry = 0;
        for (int j = 0; j < right.size() || carry != 0; ++j) {
            unsigned long long cur = 0;
            cur = static_cast<unsigned long long>(res.longnumber()[i + j]) + carry + static_cast<unsigned long long>(left.longnumber()[i]) * static_cast<unsigned long long>((j < right.size() ? right.longnumber()[j] : 0));
            unsigned int  x = cur % base;
            res.real_number()[i + j] = (x);
            carry = cur / base;

        }
    }
    while (res.size() > 1 and res.longnumber()[res.size() - 1] == 0) {

        res.pop_back();

    }

    return res;
}
BigInteger operator/(const BigInteger& left, const BigInteger& right) {
    BigInteger dividend = left;
    BigInteger divisor = right;
    BigInteger res;
    if (dividend == 0) {
        return 0;
    }
    if (!left.is_pos()) { dividend.minus(); }
    if (!right.is_pos()) { divisor.minus(); }
    if (divisor == 1) {
        res = left;
        if (left.is_pos()) {
            return res;
        }
        return -res;
    }

    if (divisor == -1) {
        res = left;
        if (left.is_pos()) {
            return -res;
        }
        return -res;
    }


    BigInteger quotient = 0;
    BigInteger currentChunk = 0;

    for (int i = dividend.size() - 1; i >= 0; --i) {
        currentChunk = currentChunk * base + dividend.longnumber()[i];
        BigInteger cnt = 0;

        BigInteger low = 0;
        BigInteger high = currentChunk + 1;
        while (low < high) {
            BigInteger mid = low + div2(high - low);
            if (mid * divisor <= currentChunk) {
                cnt = mid;
                low = mid + 1;
            }
            else {
                high = mid;
            }
        }

        currentChunk -= cnt * divisor;
        quotient = quotient * base + cnt;
    }

    res = quotient;

    if ((!left.is_pos() and right.is_pos()) or (left.is_pos() and !right.is_pos())) {

        res.minus();
    }
    return res;


}

BigInteger operator%(const BigInteger& left, const BigInteger& right) {
    BigInteger dividend = left;
    BigInteger divisor = right;
    if (!left.is_pos()) { dividend.minus(); }
    if (!right.is_pos()) { divisor.minus(); }

    BigInteger quotient = 0;
    BigInteger currentChunk = 0;

    for (int i = dividend.size() - 1; i >= 0; --i) {
        currentChunk = currentChunk * base + dividend.longnumber()[i];
        BigInteger cnt = 0;

        BigInteger low = 0;
        BigInteger high = currentChunk + 1;
        while (low < high) {
            BigInteger mid = low + div2(high - low);
            if (mid * divisor <= currentChunk) {
                cnt = mid;
                low = mid + 1;
            }
            else {
                high = mid;
            }
        }

        currentChunk -= cnt * divisor;
        quotient = quotient * base + cnt;
    }

    if ((!left.is_pos() and right.is_pos()) or (left.is_pos() and !right.is_pos())) {
        quotient.minus();
    }
    BigInteger resto = left - (quotient)*right;

    return resto;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    *this = *this + other;
    return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this = *this - other;
    return *this;
}
BigInteger& BigInteger::operator*=(const BigInteger& other) {
    *this = *this * other;
    return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& other) {
    *this = *this / other;
    return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this % other;
    return *this;
}



BigInteger BigInteger::operator++() {
    BigInteger res = *this;
    res += 1;
    return res;
}

BigInteger BigInteger::operator++(int)
{
    BigInteger* tmp = this;
    *this += 1;
    return *tmp;
}

BigInteger BigInteger::operator--() {
    BigInteger res = *this - 1;

    return res;
}

BigInteger BigInteger::operator--(int)
{
    BigInteger* tmp = this;
    *this -= 1;
    return *tmp;
}


std::ostream& operator<<(std::ostream& out, const BigInteger& X) {
    if (!X.is_pos()) {
        out << "-";
    }
    int i = 0;
    while (i < X.size()) {

        out << " ";//X.to_string();
        ++i;
    }
    return out;
}

void BigInteger::push_back(const unsigned int X) {

    if (this->data_ != nullptr and (this->size_ == 1 and this->data_[0] == 0) and this->is_empty == 1) {
        data_[0] = X;
        this->is_empty = 0;
    }

    else {
        unsigned int* tmp = new unsigned int[size_ + 1];
        for (std::size_t i = 0; i < size_; ++i) {
            unsigned int tmp_tmp = tmp[i];
            tmp[i] = (data_[i]);
            data_[i] = tmp_tmp;
        }
        delete[] data_;
        data_ = tmp;
        data_[size_] = X;
        ++size_;
    }
}

void BigInteger::pop_back() {

    unsigned int* tmp = new unsigned int[size_ - 1];
    for (std::size_t i = 0; i < size_ - 1; ++i) {
        tmp[i] = std::move(data_[i]);

    }
    delete[] data_;
    data_ = tmp;
    --size_;


}


std::string to_string(const BigInteger X) {
    std::string res = "0";
    unsigned int mask;
    bool one;
    for (int i = X.size() - 1; i >= 0; --i) {
        mask = 1 << 30;
        for (size_t j = 0; j < 31; ++j) {
            one = (X.longnumber()[i] & mask) ? true : false;
            res = multiplyBy2(res);
            if (one) {
                res = addOne(res);
            }
            mask >>= 1;
        }
    }
    
    return res;
}



BigInteger div2(const BigInteger& X) {
    BigInteger res = X;
    int size = X.size();
    int ost = 0;
    int tost = 0;
    for (int i = size - 1; i >= 0; i--) {
        if (res.real_number()[i] % 2 != 0) {
            ost = 1;
        }
        res.real_number()[i] = (res.real_number()[i] + base * tost) / 2;
        tost = ost;
        ost = 0;
    }
    return res;

}

std::string multiplyBy2(const std::string& input) {
    std::string result;
    int carry = 0;
    for (int i = input.size() - 1; i >= 0; --i) {
        int digit = (input[i] - '0') * 2 + carry;
        carry = digit / 10;
        digit %= 10;
        result.insert(0, 1, (char)(digit + '0'));
    }
    if (carry > 0) {
        result.insert(0, 1, (char)(carry + '0'));
    }
    return result;
}

std::string addOne(const std::string& input) {
    std::string result = input;
    int i = result.size() - 1;
    while (i >= 0) {
        if (result[i] == '9') {
            result[i] = '0';
            --i;
        }
        else {
            ++result[i];
            break;
        }
    }
    if (i < 0) {
        result.insert(0, 1, '1');
    }
    return result;
}