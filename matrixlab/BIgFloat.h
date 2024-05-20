#pragma once
#include "BigInteger.h"
#include <string>
class BigFloat
{
	BigInteger num;
	BigInteger den;
	int sign;
public:

	BigInteger getnum() const{
		return num;
	}
	BigInteger getden() const
	{
		return den;
	}

	bool is_pos() const;
	BigFloat() : num(0), den(1), sign(1) {};
	void swap_mv(BigFloat& other) { //"ìóâ ñåìàíòèêà" äëÿ êîíñòðóêòîðîâ
		BigInteger temp1 = num;
		BigInteger temp2 = den;
		int temp_sign = sign;

		num = other.num;
		den = other.den;
		sign = other.sign;

		other.num = temp1;
		other.den = temp2;
		other.sign = temp_sign;

	}
	void minus(){
		sign *= -1;
	}
	bool abs_less(const BigFloat& right) const;

	BigFloat& operator=(const BigFloat& other);//ïðèñâàèâàíèÿ
	//BigFloat& operator=(BigFloat&&);//ïåðåìåùåíèÿ

	BigFloat& operator+=(const BigFloat&);
	BigFloat& operator-=(const BigFloat&);
	BigFloat& operator*=(const BigFloat&);
	BigFloat& operator/=(const BigFloat&);

	void socraschen();

	bool operator==(const BigFloat& right) const;

	BigFloat(BigInteger num1, BigInteger den1);
	BigFloat(const std::string& str);//êîíñòðóêòîð îò ñòðîêè
	BigFloat(long long int x);//îò öåëî÷èñëåííûõ òèïîâ
	BigFloat(const BigFloat& other);//êîïèðîâàíèÿ
	//BigFloat(BigFloat&& other);//ïåðåìåù
	BigFloat(BigInteger x);
	BigFloat(int x);
	~BigFloat() = default;//äåñòðóêòîð
};

bool operator!=(const BigFloat& left, const BigFloat& right);
bool operator<(const BigFloat& left, const BigFloat& right);
bool operator<=(const BigFloat& left, const BigFloat& right);
bool operator>(const BigFloat& left, const BigFloat& right);
bool operator>=(const BigFloat& left, const BigFloat& right);


BigFloat operator-(BigFloat other);
BigFloat operator+(BigFloat other);

BigFloat operator+(const BigFloat& left, const BigFloat& right);
BigFloat operator-(const BigFloat& left, const BigFloat& right);
BigFloat operator*(const BigFloat& left, const BigFloat& right);
BigFloat operator/(const BigFloat& left, const BigFloat& right);


std::ostream& operator<<(std::ostream& out, const BigFloat& X);
std::string to_string(const BigFloat& X);
