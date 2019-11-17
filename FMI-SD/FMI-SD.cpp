#include <iostream>
#include <fstream>
#include "Wallet.h"
#include "Transaction.h"
#include "DynamicArr.h"

using namespace std;

DynamicArr<Wallet> wallets;
DynamicArr<Transaction> transactions;
//DynamicArr<Order> orders;

void createWallet(const double fiatMoney, const char* name)
{
	Wallet wallet(fiatMoney, name);
	Transaction initialTrans(ULONG_MAX - 1, wallet.getID(), fiatMoney/375);
	wallets.add(wallet);
	transactions.add(initialTrans);

	//ofstream os("wallets.dat", ios::app);
	//size_t ownerLen = strlen(name);
	////char* owner = new char[ownerLen + 1];
	//char owner[] = { 'g', 'e', 'r', 'i' };
	//if (os.is_open())
	//{
	//	unsigned id = 123435;
	//	//strcpy_s(owner, sizeof(name), name);
	//	os.write((char*)&ownerLen, sizeof(ownerLen));
	//	os.write((char*)&owner, ownerLen);
	//	//delete[] owner;

	//	os.write((char*)&fiatMoney, sizeof(fiatMoney));
	//	os.write((char*)&id, sizeof(id));
	//	//os << ownerLen << "GERI" << id << fiatMoney << endl;
	//}
	//os.close();
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

int main()
{
	srand(time(NULL));
	/*for (int i = 0; i < 10; i++) 
	{
		Wallet wallet;
		cout << time << endl;
	}*/

	createWallet(12.5, "GERI");

	//readBinary();

	
}
