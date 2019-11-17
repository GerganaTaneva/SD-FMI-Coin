#include "Order.h"

Order::Order() : type(), walletId(0), fmiCoins(0.0) 
{
}

Order::Order(const Type type, const unsigned walletId, const double fmiCoins)
{
	this->type = type;
	this->walletId = walletId;
	this->fmiCoins = fmiCoins;
}

Order::Order(const Order& other)
{
	copy(other);
}

Order::~Order() {}

const Order::Type Order::getType() const
{
	return this->type;
}

const unsigned Order::getWalletId() const
{
	return this->walletId;
}

const double Order::getFmiCoins() const
{
	return this->fmiCoins;
}

Order& Order::operator=(const Order& other)
{
	if (this != &other)
	{
		copy(other);
	}
	return *this;
}

void Order::setType(const Type type)
{
	this->type = type;
}

void Order::setWalletId(const unsigned walletId)
{
	this->walletId = walletId;
}

void Order::setFmiCoins(const double dmiCoins)
{
	if (fmiCoins < 0)
	{
		throw std::exception("Invalid input!");
	}
	this->fmiCoins = fmiCoins;
}

void Order::copy(const Order& other)
{
	this->type = other.type;
	this->walletId = other.walletId;
	this->fmiCoins = other.fmiCoins;
}

