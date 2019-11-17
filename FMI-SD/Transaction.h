#pragma once
#include <iostream>
#include <chrono>
class Transaction
{
public:
	Transaction();
	Transaction(const unsigned sender, const unsigned receiver, const double fmiCoins);
	Transaction(const Transaction& other);
	Transaction& operator=(const Transaction& other);
	~Transaction();

public:
	const long long getTime() const;
	const unsigned getSenderID() const;
	const unsigned getReceiverId() const;
	const double getFmiCoins() const;

	void setTime(const long long time);
	void setSenderId(const unsigned senderId);
	void setReceiverId(const unsigned receiverId);
	void setFmiCoins(const double fmiCoins);

private:
	void copy(const Transaction& other);

private:
	long long time;
	unsigned senderId;
	unsigned receiverId;
	double fmiCoins;
};

