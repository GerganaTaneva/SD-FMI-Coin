#include "Wallet.h"
Wallet::Wallet() : owner(NULL), fiatMoney(0.0) 
{
	this->id = generateId();
}

Wallet::Wallet(const double fiatMoney, const char* owner)
{
	this->id = generateId();
	setFiatMoney(fiatMoney);
	setOwner(owner);
}

Wallet::Wallet(const Wallet& other) : owner(NULL) 
{
	copy(other);
}

Wallet& Wallet::operator=(const Wallet& other) 
{
	if (this != &other)
	{
		delete[] this->owner;
		copy(other);
	}
	return *this;
}

Wallet::~Wallet() 
{
	delete[] this->owner;
}

const char* Wallet::getOwner() const
{
	return this->owner;
}

const double Wallet::getFiatMoney() const
{
	return this->fiatMoney;
}

const unsigned Wallet::getID() const 
{
	return this->id;
}

void Wallet::setFiatMoney(const double fiatMoney) 
{
	if (fiatMoney < 0) 
	{
		throw std::exception("Invalid input!");
	}
	this->fiatMoney = fiatMoney;
}

void Wallet::setOwner(const char* owner) 
{
	if (owner == NULL) 
	{
		throw std::exception("Invalid input!");
	}
	delete[] this->owner;
	this->owner = new char[strlen(owner) + 1];
	strcpy_s(this->owner, sizeof(owner), owner);
}

void Wallet::copy(const Wallet& other)
{
	this->owner = new char[strlen(other.getOwner()) + 1];
	strcpy_s(this->owner, sizeof(other.getOwner()), other.getOwner());
	this->id = other.getID();
	this->fiatMoney = other.getFiatMoney();
}

const unsigned Wallet::generateId() const
{
	//srand(time(NULL));
	return rand() % ULONG_MAX;
}