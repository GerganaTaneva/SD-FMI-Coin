#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

struct Wallet {
    char owner[256];
    unsigned id;
    double fiatMoney;
};

struct Transaction {
    long long time;
    unsigned senderId;
    unsigned receiverId;
    double fmiCoins;
};

struct Order {
    enum Type { SELL, BUY } type;
    unsigned walletId;
    double fmiCoins;
};

vector<Wallet> wallets;
vector<Transaction> transactions;
vector<Order> orders;
vector<bool> activeOrders;

int findWalletIndex(unsigned id) {
    for (int i = 0; i < wallets.size(); i++) {
        if (id == wallets[i].id) {
            return i;
        }
    }
    return -1;
}

double findAmountOfCoins(unsigned id) {
    double result = 0;
    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].senderId == id) {
            result -= transactions[i].fmiCoins;
        }
        if (transactions[i].receiverId == id) {
            result += transactions[i].fmiCoins;
        }
    }
    return result;
}

bool compareWallets(Wallet w1, Wallet w2) {
    return (findAmountOfCoins(w1.id) > findAmountOfCoins(w2.id));
}



int main() {
    ifstream wallets_file_in("wallets.dat", ios::binary);
    ifstream transactions_file_in("transactions.dat", ios::binary);
    ifstream orders_file_in("orders.dat", ios::binary);

    while (wallets_file_in.is_open() && !wallets_file_in.eof()) {
        Wallet w;
        if (wallets_file_in.read((char*) &w, sizeof(Wallet))) {
            wallets.push_back(w);
        }
    }
    wallets_file_in.close();
    while (transactions_file_in.is_open() && !transactions_file_in.eof()) {
        Transaction t;
        if (transactions_file_in.read((char*) &t, sizeof(Transaction))) {
            transactions.push_back(t);
        }
    }
    transactions_file_in.close();
    while (orders_file_in.is_open() && !orders_file_in.eof()) {
        Order o;
        if (orders_file_in.read((char*) &o, sizeof(Order))) {
            orders.push_back(o);
            activeOrders.push_back(true);
        }
    }
    orders_file_in.close();

    unsigned id = 0;

    while (true) {
        string commandLine;
        getline(cin, commandLine);
        istringstream commandStream(commandLine);
        string command;
        commandStream >> command;
        if (command == "quit") {
            break;
        }

        if (command == "add-wallet") {
            Wallet w;
            commandStream >> w.fiatMoney;
            commandStream >> w.owner;
            while (findWalletIndex(id) > -1 || id == 4294967295) {
                id++;
            }
            w.id = id;
            wallets.push_back(w);

            Transaction t;
            t.fmiCoins = w.fiatMoney / 375;
            t.senderId = 4294967295;
            t.receiverId = w.id;
            t.time = time(nullptr);
            transactions.push_back(t);
            continue;
        }

        if (command == "wallet-info") {
            unsigned walletId;
            commandStream >> walletId;
            int walletIndex = findWalletIndex(walletId);
            cout << "Name: " << wallets[walletIndex].owner << endl;
            cout << "Amount of money: " << wallets[walletIndex].fiatMoney << endl;
            cout << "Amount of coins: " << findAmountOfCoins(walletId) << endl;
            cout << endl;

            continue;
        }

        if (command == "transfer") {
            Transaction t;
            commandStream >> t.senderId;
            commandStream >> t.receiverId;
            commandStream >> t.fmiCoins;
            if (findWalletIndex(t.senderId) > -1 && findWalletIndex(t.receiverId) > -1) {
                if (findAmountOfCoins(t.senderId) > t.fmiCoins) {
                    t.time = time(nullptr);
                    transactions.push_back(t);
                    cout << "Transaction was successful!" << endl;
                } else {
                    cout << "The sender does not have enough coins for this transaction!" << endl;

                }
            } else {
                cout << "The sender and/or receiver does not exist!" << endl;
            }
            continue;
        }

        if (command == "make-order") {
            Order o;
            string type;
            commandStream >> type;
            commandStream >> o.fmiCoins;
            commandStream >> o.walletId;
            if (type == "SELL") {
                o.type = Order::Type::SELL;
            } else if (type == "BUY") {
                o.type = Order::Type::BUY;
            } else {
                cout << "Invalid order type!" << endl;
                continue;
            }
            if (o.type == Order::Type::SELL) {
                double availableCoins = findAmountOfCoins(o.walletId);
                for (int i = 0; i < orders.size(); i++) {
                    if (o.walletId != orders[i].walletId || !activeOrders[i]) {
                        continue;
                    }
                    if (orders[i].type == Order::Type::SELL) {
                        availableCoins -= orders[i].fmiCoins;
                    } else {
                        availableCoins += orders[i].fmiCoins;
                    }
                }
                if (availableCoins < o.fmiCoins) {
                    cout << "There are not enough coins in this wallet." << endl;
                    continue;
                }
                string orderFileName = to_string(o.walletId) + "_" + to_string(time(nullptr)) + ".txt";
                ofstream order_file(orderFileName);
                int countTransactions = 0;
                double sumOfAllMoney = 0;
                for (int i = 0; i < orders.size(); i++) {
                    if (orders[i].type == Order::Type::SELL || !activeOrders[i]) {
                        continue;
                    }
                    countTransactions++;
                    if (orders[i].fmiCoins <= o.fmiCoins) {
                        Transaction t;
                        t.fmiCoins = orders[i].fmiCoins;
                        t.senderId = o.walletId;
                        t.receiverId = orders[i].walletId;
                        t.time = time(nullptr);
                        transactions.push_back(t);
                        int senderWalletIndex = findWalletIndex(t.senderId);
                        int receiverWalletIndex = findWalletIndex(t.receiverId);
                        wallets[senderWalletIndex].fiatMoney += t.fmiCoins * 375;
                        wallets[receiverWalletIndex].fiatMoney -= t.fmiCoins * 375;
                        o.fmiCoins -= t.fmiCoins;
                        activeOrders[i] = false;
                        order_file << "Sender: " << wallets[senderWalletIndex].owner << " ";
                        order_file << "Receiver: " << wallets[receiverWalletIndex].owner << " ";
                        order_file << "FMI Coins: " << t.fmiCoins << endl;
                        sumOfAllMoney += t.fmiCoins * 375;
                    } else {
                        Transaction t;
                        t.fmiCoins = o.fmiCoins;
                        t.senderId = o.walletId;
                        t.receiverId = orders[i].walletId;
                        t.time = time(nullptr);
                        transactions.push_back(t);
                        int senderWalletIndex = findWalletIndex(t.senderId);
                        int receiverWalletIndex = findWalletIndex(t.receiverId);
                        wallets[senderWalletIndex].fiatMoney += t.fmiCoins * 375;
                        wallets[receiverWalletIndex].fiatMoney -= t.fmiCoins * 375;
                        o.fmiCoins -= t.fmiCoins;
                        orders[i].fmiCoins -= t.fmiCoins;
                        order_file << "Sender: " << wallets[senderWalletIndex].owner << " ";
                        order_file << "Receiver: " << wallets[receiverWalletIndex].owner << " ";
                        order_file << "FMI Coins: " << t.fmiCoins << endl;
                        sumOfAllMoney += t.fmiCoins * 375;
                    }
                    if (o.fmiCoins < 1e-6) {
                        break;
                    }
                }
                order_file << "Number of transactions: " << countTransactions << endl;
                order_file << "Sum in real money: " << sumOfAllMoney << endl;
                order_file.close();
            } else {
                double availableMoney = wallets[findWalletIndex(o.walletId)].fiatMoney;
                for (int i = 0; i < orders.size(); i++) {
                    if (o.walletId != orders[i].walletId || !activeOrders[i]) {
                        continue;
                    }
                    if (orders[i].type == Order::Type::SELL) {
                        availableMoney += orders[i].fmiCoins * 375;
                    } else {
                        availableMoney -= orders[i].fmiCoins * 375;
                    }
                }
                if (availableMoney < o.fmiCoins * 375) {
                    cout << "There is not enough money in this wallet." << endl;
                    continue;
                }
                string orderFileName = to_string(o.walletId) + "_" + to_string(time(nullptr)) + ".txt";
                ofstream order_file(orderFileName);
                int countTransactions = 0;
                double sumOfAllMoney = 0;
                for (int i = 0; i < orders.size(); i++) {
                    if (orders[i].type == Order::Type::BUY || !activeOrders[i]) {
                        continue;
                    }
                    countTransactions++;
                    if (orders[i].fmiCoins <= o.fmiCoins) {
                        Transaction t;
                        t.fmiCoins = orders[i].fmiCoins;
                        t.senderId = orders[i].walletId;
                        t.receiverId = o.walletId;
                        t.time = time(nullptr);
                        transactions.push_back(t);
                        int senderWalletIndex = findWalletIndex(t.senderId);
                        int receiverWalletIndex = findWalletIndex(t.receiverId);
                        wallets[senderWalletIndex].fiatMoney += t.fmiCoins * 375;
                        wallets[receiverWalletIndex].fiatMoney -= t.fmiCoins * 375;
                        o.fmiCoins -= t.fmiCoins;
                        activeOrders[i] = false;
                        order_file << "Sender: " << wallets[senderWalletIndex].owner << " ";
                        order_file << "Receiver: " << wallets[receiverWalletIndex].owner << " ";
                        order_file << "FMI Coins: " << t.fmiCoins << endl;
                        sumOfAllMoney += t.fmiCoins * 375;
                    } else {
                        Transaction t;
                        t.fmiCoins = o.fmiCoins;
                        t.senderId = orders[i].walletId;
                        t.receiverId = o.walletId;
                        t.time = time(nullptr);
                        transactions.push_back(t);
                        int senderWalletIndex = findWalletIndex(t.senderId);
                        int receiverWalletIndex = findWalletIndex(t.receiverId);
                        wallets[senderWalletIndex].fiatMoney += t.fmiCoins * 375;
                        wallets[receiverWalletIndex].fiatMoney -= t.fmiCoins * 375;
                        o.fmiCoins -= t.fmiCoins;
                        orders[i].fmiCoins -= t.fmiCoins;
                        order_file << "Sender: " << wallets[senderWalletIndex].owner << " ";
                        order_file << "Receiver: " << wallets[receiverWalletIndex].owner << " ";
                        order_file << "FMI Coins: " << t.fmiCoins << endl;
                        sumOfAllMoney += t.fmiCoins * 375;
                    }
                    if (o.fmiCoins < 1e-6) {
                        break;
                    }
                }

                order_file << "Number of transactions: " << countTransactions << endl;
                order_file << "Sum in real money: " << sumOfAllMoney << endl;
                order_file.close();
            }
            if (o.fmiCoins > 1e-6) {
                orders.push_back(o);
                activeOrders.push_back(true);
            }

            continue;
        }

        if (command == "attract-investors") {
            // Not sure how to read all the files in the folder
            // And this is the only place where the orders are actually stored :(

            sort(wallets.begin(), wallets.end(), compareWallets);
            int endIndex = wallets.size() > 10 ? 10 : wallets.size();

            for (int i = 0; i < endIndex; i++) {
                cout << i+1 << ":" << endl;
                cout << "Owner: " << wallets[i].owner << endl;
                cout << "fmiCoins: " << findAmountOfCoins(wallets[i].id);
                cout << endl;
            }

            continue;
        }

        cout << "Invalid command!\n";
    }
    ofstream wallets_file_out("wallets.dat", ios::binary);
    ofstream transactions_file_out("transactions.dat", ios::binary);
    ofstream orders_file_out("orders.dat", ios::binary);

    for (int i = 0; i < wallets.size(); i++) {
        wallets_file_out.write((char*) &wallets[i], sizeof(Wallet));
    }
    wallets_file_out.close();
    for (int i = 0; i < transactions.size(); i++) {
        transactions_file_out.write((char*) &transactions[i], sizeof(Transaction));
    }
    transactions_file_out.close();
    for (int i = 0; i < orders.size(); i++) {
        if (activeOrders[i]) {
            orders_file_out.write((char*) &orders[i], sizeof(Order));
        }
    }
    orders_file_out.close();

    return 0;
}
