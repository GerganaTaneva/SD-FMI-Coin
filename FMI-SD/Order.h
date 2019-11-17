#pragma once
#include <iostream>
class Order
{
public:
	enum Type { SELL, BUY };

public:
	Order();
	Order(const Type type, const unsigned walletId, const double fmiCoins);
	Order(const Order& other);
	Order& operator=(const Order& other);
	~Order();

	const Type getType() const;
	const unsigned getWalletId() const;
	const double getFmiCoins() const;

	void setType(const Type type);
	void setWalletId(const unsigned walletId);
	void setFmiCoins(const double dmiCoins);

private:
	void copy(const Order& other);

private:
	Type type;
	unsigned walletId;
	double fmiCoins;
};

