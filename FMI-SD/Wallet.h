#pragma once
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <climits>

class Wallet
{
public:
	Wallet();
	Wallet(const double fiatMoney, const char* owner);
	Wallet(const Wallet& other);
	Wallet& operator=(const Wallet& other);
	~Wallet();

public:
	const char* getOwner() const;
	const unsigned getID() const;
	const double getFiatMoney() const;

	void setOwner(const char* owner);
	void setFiatMoney(const double fiatMoney);

private:
	const unsigned generateId() const;
	void copy(const Wallet& other);

private:
	char* owner;
	unsigned id;
	double fiatMoney;
};

