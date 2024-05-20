
#include "BigFloat.h"
#include "BigInteger.h"



BigFloat::BigFloat(BigInteger num1, BigInteger den1) {
    if (num1 >= 0) {
        num = num1;
    }
    else {
        num = -num1;
    }
    if (den1 >= 0) {
        den = den1;
    }
    else {
        den = -den1;
    }
    if ((num1.is_pos() and den1.is_pos()) or (!num1.is_pos() and !den1.is_pos())) {
        sign = 1;
    }
    else {
        sign = -1;
    }
}

BigFloat::BigFloat(BigInteger x) {
    den = BigInteger(1);
    sign = 1;
    if (x >= 0) {
        num = x;
        
    }
    else {
        num = -x;
        x.minus();
    }
    num = x;
    
}

BigFloat::BigFloat(int x) {
    den = BigInteger(1);
    
    num = BigInteger(x);
    sign = 1;
}

bool BigFloat::is_pos() const {
    if (sign == 1) { return 1; }
    return 0;
}

BigFloat& BigFloat::operator=(const BigFloat& other) {
    num = other.num;
    den = other.den;
    sign = other.sign;

    return *this;
}
//
//BigFloat& BigFloat::operator=(BigFloat&& other) {
//    if (this != &other) {
//        swap_mv(other);
//    }
//    return *this;
//}

BigFloat::BigFloat(const BigFloat& other) {//êîïèðóþùèé êîíñòðóêòîð
    num = other.num;
    den=  other.den;
    sign = other.sign;
    socraschen();

}
/*
BigFloat::BigFloat(BigFloat&& other) {
    num = 0;
    den = 1;
    sign = 1;
    swap_mv(other);
}*/

bool BigFloat::operator==(const BigFloat& right) const {
    if (sign != right.sign) {
        return false;
    }
    if (num * right.den == den * right.num) {
        return true;
    }
    return false;
}

bool operator!=(const BigFloat& left, const BigFloat& right) {
    return !(left == right);
}

bool BigFloat::abs_less(const BigFloat& right) const {
    if (num * right.den <den * right.num) {
        return true;
    }
    return false;
}

bool operator<(const BigFloat& left, const BigFloat& right) {
    if (left.is_pos() and right.is_pos()) {
        return left.abs_less(right);
    }
    if (left.is_pos() and !right.is_pos()) {
        return false;
    }
    if (!left.is_pos() and right.is_pos()) {
        return true;
    }
    if (!left.is_pos() and !right.is_pos()) {
        return !left.abs_less(right);
    }
    return false;
}

bool operator<=(const BigFloat& left, const BigFloat& right) {
    return(left < right) or left == right;
}
bool operator>(const BigFloat& left, const BigFloat& right) {
    return  !(left < right) and left != right;
}
bool operator>=(const BigFloat& left, const BigFloat& right) {
    return !(left < right);
}

BigFloat operator-(BigFloat other) {
    other.minus();
    return other;
}

BigFloat operator+(BigFloat other) {
    return other;
}

BigFloat operator+(const BigFloat& left, const BigFloat& right) {
    if (left.is_pos() and !right.is_pos()) {
        return left - (-right);
    }
    if (!left.is_pos() and !right.is_pos()) {
        return -((-left) + (-right));
    }

    if (!left.is_pos() and right.is_pos()) {
        return left - (-right);
    }

    BigFloat x(left.getnum() * right.getden() + right.getnum() * left.getden(), left.getden() * right.getden());
    x.socraschen();
    return x;
    
}

BigFloat operator-(const BigFloat& left, const BigFloat& right) {
    if (left.is_pos() and !right.is_pos()) {
        return left + (-right);
    }
    if (!left.is_pos() and right.is_pos()) {
        return -(-left + right);
    }
    if (!left.is_pos() and !right.is_pos()) {
        return (-right) - (-left);
    }

    BigFloat x;
    if (left > right) {
        x = BigFloat(left.getnum() * right.getden() - right.getnum() * left.getden(), left.getden() * right.getden());
    }
    else {
        x = BigFloat(-left.getnum() * right.getden() + right.getnum() * left.getden(), left.getden() * right.getden());
        x.minus();
    }
    x.socraschen();
    return x;

}

BigFloat operator*(const BigFloat& left, const BigFloat& right) {
    
    BigFloat x = BigFloat(left.getnum() * right.getnum(), left.getden() * right.getden());

    if (left == 0 or right == 0) {
        return 0;
    }
    if (!((left.is_pos() and right.is_pos()) or (!left.is_pos() and !right.is_pos()))) {
        x.minus();
    }
    x.socraschen();
    return x;
}

BigFloat operator/(const BigFloat& left, const BigFloat& right) {
    if (left == 0 or right == 0) {
        return 0;
    }
    BigFloat x(left.getnum() * right.getden(), left.getden() * right.getnum());
    if (!((left.is_pos() and right.is_pos()) or (!left.is_pos() and !right.is_pos()))) {
        x.minus();
    }
    x.socraschen();
    return x;
}

BigFloat& BigFloat::operator+=(const BigFloat& other) {
    *this = *this + other;
    return *this;
}
BigFloat& BigFloat::operator-=(const BigFloat& other) {
    *this = *this - other;
    return *this;
}
BigFloat& BigFloat::operator*=(const BigFloat& other) {
    *this = *this * other;
    return *this;
}
BigFloat& BigFloat::operator/=(const BigFloat& other) {
    *this = *this / other;
    return *this;
}

BigInteger NOD(BigInteger x, BigInteger y) {
    if (!x.is_pos()) {
        x.minus();
    }
    if (!y.is_pos()) {
        y.minus();
    }
    if (x % y == 0)
        return y;
    if (y % x == 0)
        return x;
    if (x > y)
        return NOD(x % y, y);
    return NOD(x, y % x);
}

void BigFloat::socraschen() {
    BigInteger nodto = NOD(num, den);
    if (nodto == 1) { return; }
    num = num / nodto;
    den = den / nodto;
    
}




std::ostream& operator<<(std::ostream& out, const BigFloat& X) {
    if (!X.is_pos()) {
        out << "-";
    }
    int i = 0;
    out << to_string(X.getnum());
    if (X.getden() != 1) {
        out << "/"<<to_string(X.getden());
    }
    return out;
}


std::string to_string(const BigFloat& X) {
    std::string res = "";
    if (X.getnum() != 0) {
        if (X < 0) { res += "-"; }
        res += to_string(X.getnum());
        if (X.getden() != 1) {
            res = res + "/" + to_string(X.getden());
        }
    }
    else { res += "0"; }

    return res;

}