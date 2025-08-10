#pragma once
#include <utility>
#include <string>
#include <limits>
const unsigned long long int base = (2147483648); //íîâàÿ áàçà, ÷òîáû èñïîëüçîâàòü âñå áèòû ÷èñëà

class BigInteger
{
	unsigned int* data_;//ìàññèâ èíòîâ - ïîëíûõ
	std::size_t size_; //ðàçìåð ìàññèâà
	int sign_;//çíàê
	void swap_mv(BigInteger& other) { //"ìóâ ñåìàíòèêà" äëÿ êîíñòðóêòîðîâ
		unsigned int* temp_data = data_;
		std::size_t temp_size = size_;
		int temp_sign = sign_;

		data_ = other.data_;
		size_ = other.size_;
		sign_ = other.sign_;

		other.data_ = temp_data;
		other.size_ = temp_size;
		other.sign_ = temp_sign;

	}

public:


	std::size_t size() const { return size_; } //ðàçìåð
	const unsigned int* longnumber() const { return data_; }//ïîëó÷åíèå ýëåìåíòà ìàññèâà
	unsigned int* real_number() { return data_; }//ïîëó÷åíèå ýëåìåíòà ìàññèâà(ñ âîçìîæíîñòüþ èçìåíåíèÿ)
	bool is_pos() const { return sign_ > 0; }//ïðîâåðêà íà çíàê
	BigInteger() : data_(), size_(1), sign_(1) {//êîíñòðóêòîð ïî óìîë÷àíèþ
		data_ = new unsigned int[1];
		data_[0] = 0;
		is_empty = 1;
	}
	void minus() { //èçìåíåíèå çíàêà
		sign_ *= -1;
	}
	void set_sign() {//âûñòàâèòü çíàê(÷òîáû íå áûë ðàâåí íóëþ, ãäå-òî ïðèãîæäàåòñÿ)
		sign_ = 1;
	}
	bool is_empty; //÷òîáû îòëè÷àòü 0 ïî óìîë÷àíèþ è çíà÷àùèå íóëè
	BigInteger& operator=(const BigInteger&);//ïðèñâàèâàíèÿ
	BigInteger& operator=(BigInteger&&);//ïåðåìåùåíèÿ

	BigInteger& operator+=(const BigInteger&);
	BigInteger& operator-=(const BigInteger&);
	BigInteger& operator*=(const BigInteger&);
	BigInteger& operator/=(const BigInteger&);
	BigInteger& operator%=(const BigInteger&);

	BigInteger operator++();
	BigInteger operator++(int);
	BigInteger operator--();
	BigInteger operator--(int);

	bool operator==(const BigInteger& right) const;

	void push_back(const unsigned int X);
	void pop_back();


	BigInteger(const std::string& str);//êîíñòðóêòîð îò ñòðîêè
	BigInteger(long long int x);//îò öåëî÷èñëåííûõ òèïîâ
	BigInteger(const BigInteger& other);//êîïèðîâàíèÿ
	BigInteger(BigInteger&& other);//ïåðåìåù
	~BigInteger();//äåñòðóêòîð

};


std::string to_string(const BigInteger X);//ïåðåâîä â ñòðîêó

bool operator!=(const BigInteger& left, const BigInteger& right);//ñðàâíåíèÿ
bool operator<(const BigInteger& left, const BigInteger& right);
bool operator<=(const BigInteger& left, const BigInteger& right);
bool operator>(const BigInteger& left, const BigInteger& right);
bool operator>=(const BigInteger& left, const BigInteger& right);

BigInteger operator-(BigInteger other);//óíàðíûå + è -
BigInteger operator+(BigInteger other);

BigInteger operator+(const BigInteger& left, const BigInteger& right);//ñëîæåíèå
BigInteger operator-(const BigInteger& left, const BigInteger& right);//âû÷èòàíèå
BigInteger operator*(const BigInteger& left, const BigInteger& right);//óìíîæåíèå "ñòîëáèêîì"
BigInteger operator/(const BigInteger& left, const BigInteger& right);//äåëåíèå ñòîëáèêîì(ñ áèíàðíûì ïîèñêîì)
BigInteger operator%(const BigInteger& left, const BigInteger& right);//íàõîæäåíèå îñòàòêà(÷åðåç äåëåíèå)

BigInteger div2(const BigInteger& X);//äåëåíèå íà 2(äëÿ áèíàðíîãî ïîèñêà â ðåàëèçàöèè îïåðàòîðà äåëåíèÿ)
#include <iostream>
std::ostream& operator<<(std::ostream& out, const BigInteger& X);

std::string multiplyBy2(const std::string& input);
std::string addOne(const std::string& input);