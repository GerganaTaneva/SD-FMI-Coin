#include "Transaction.h"
Transaction::Transaction() : senderId(0), receiverId(0), fmiCoins(0.0) 
{
	std::chrono::high_resolution_clock m_clock;
	this->time = std::chrono::duration_cast<std::chrono::milliseconds>(m_clock.now().time_since_epoch()).count();
}

Transaction::Transaction(const unsigned sender, const unsigned receiver, const double fmiCoins)
{
	std::chrono::high_resolution_clock m_clock;
	this->time = std::chrono::duration_cast<std::chrono::milliseconds>(m_clock.now().time_since_epoch()).count();
	this->senderId = sender;
	this->receiverId = receiver;
	this->fmiCoins = fmiCoins;
}

Transaction::Transaction(const Transaction& other)
{
	copy(other);
}

Transaction::~Transaction(){}

Transaction& Transaction::operator=(const Transaction& other) 
{
	if (this != &other)
	{
		copy(other);
	}
	return *this;
}

const long long Transaction::getTime() const
{
	return this->time;
}

const unsigned Transaction::getSenderID() const
{
	return this->senderId;
}

const unsigned Transaction::getReceiverId() const
{
	return this->receiverId;
}

const double Transaction::getFmiCoins() const
{
	return this->fmiCoins;
}

void Transaction::setTime(const long long time)
{
	this->time = time;
}

void Transaction::setSenderId(const unsigned senderId)
{
	this->senderId = senderId;
}

void Transaction::setReceiverId(const unsigned receiverId)
{
	this->receiverId = receiverId;
}

void Transaction::setFmiCoins(const double fmiCoins)
{
	if (fmiCoins < 0)
	{
		throw std::exception("Invalid input!");
	}
	this->fmiCoins = fmiCoins;
}

void Transaction::copy(const Transaction& other) 
{
	this->receiverId = other.receiverId;
	this->senderId = other.senderId;
	this->fmiCoins = other.fmiCoins;
	this->time = other.time;
}