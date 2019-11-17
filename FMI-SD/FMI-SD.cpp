#include <iostream>
#include <fstream>
#include "Wallet.h"
#include "Transaction.h"
#include "DynamicArr.h"
#include "Order.h"

using namespace std;

DynamicArr<Wallet> wallets;
DynamicArr<Transaction> transactions;
DynamicArr<Order> orders;

void createWallet(const double fiatMoney, const char* name)
{
	Wallet wallet(fiatMoney, name);
	Transaction initialTrans(ULONG_MAX - 1, wallet.getID(), fiatMoney/375);
	wallets.add(wallet);
	transactions.add(initialTrans);
}

void quit()
{
	ofstream os("wallets.dat", ios::app);
	for (int i = 0; i < wallets.getSize(); i++)
	{
		size_t ownerLen = strlen(wallets[i].getOwner());
		char* owner = new char[ownerLen + 1];
		if (os.is_open())
		{
			double fiatMoney = wallets[i].getFiatMoney();
			unsigned id = wallets[i].getID();
			strcpy_s(owner, sizeof(wallets[i].getOwner()), wallets[i].getOwner());
			os.write((char*)&ownerLen, sizeof(ownerLen));
			os.write((char*)&owner, ownerLen);
			delete[] owner;
			os.write((char*)&fiatMoney, sizeof(fiatMoney));
			os.write((char*)&id, sizeof(id));
		}
	}
	os.close();
}


void readBinary()
{
	ifstream ifs("wallets.dat", ios::binary);
	size_t ownerLen = 0;
	unsigned id = 0;
	double  money = 0.0;
	ifs.read((char*)&ownerLen, sizeof(ownerLen));
	char* owner = new char[ownerLen + 1];
	ifs.read((char*)&owner, sizeof(owner));
	ifs.read((char*)&money, sizeof(money));
	ifs.read((char*)&id, sizeof(id));

	cout << "ID: " << id << endl /*<< "Owner " << owner << endl*/ << "Money " << money << endl;
	ifs.close();

	delete[] owner;
}

void walletInfo(unsigned walletId)
{
	double fmiCoins = 0.0;
	for (int i = 0; i < transactions.getSize(); i++)
	{
		if (transactions[i].getReceiverId() == walletId)
		{
			fmiCoins += transactions[i].getFmiCoins();
		}
	}

	for (int i = 0; i < wallets.getSize(); i++)
	{
		if (wallets[i].getID() == walletId)
		{
			cout << "Wallet with owner " << wallets[i].getOwner() << " has " << wallets[i].getFiatMoney();
			cout << " money and " << fmiCoins << " FMI coins" << endl;
		}
	}
}

int main()
{
	srand(time(NULL));

	createWallet(12.5, "GERI");
	walletInfo(123435);
	//readBinary();
}
	