/*
 * Server.h
 *
 *  Created on: Apr 13, 2015
 *      Author: prof-ahmed-saad-isa
 */

#ifndef SERVER_H_
#define SERVER_H_


#pragma once

#include <pthread.h>
#include <vector>
#include <fstream>
#include "Transaction.h"
using namespace std;

//Comment to disable logging
#define ENABLE_LOGGING
#define MAX_TRANS 1000
#define MAX_THREADS 100

class Server
{
/*Attributes*/
private:

	//Vector of critical sections; one for each account
	static vector<pthread_mutex_t> account_mutexes;

	//Output file stream for writing log
	static ofstream lout;

	//Transaction Types
	string TransactionTypes[2];

	//Vector of transactions
	vector<Transaction> Transactions;

	//Vector of threads
	vector<pthread_t > callThd;

	////Mutex for log file
	//pthread_mutex_t log_mutex;

public:

	//Vector of accounts' balances
	static vector<double> Accounts;

/*Methods*/
private:

	//Write Log
	void WriteLog(string strLog);

	//Initialize Accounts
	void InitAccounts();

	//Initialize Mutuxes
	void InitMutexes();

	//Initialize Transaction
	Transaction InitTransaction();

	//Initialize Transactions
	void InitTransactions();

	//Initialize Accounts and Mutexes
	void Init();

	//Transaction thread
	static void *Transact(void *arg);

	//Create Thread
	pthread_t CreateThread(Transaction &trans);

	//Create Threads
	void CreateThreads();

	//Wait Thread
	void WaitThread(pthread_t thread);

	//Wait Threads
	void WaitThreads();

	//Destroy Mutuxes
	void DestroyMutexes();

	//Save Accounts physically
	void SaveAccounts();

public:

	//Costructor
	Server();

	//Destructor
	~Server();

	//Simulate Transaction
	void SimulateTransaction(Transaction &trans);

	//Simulation Server
	void SimulateServer();

};

#endif /* SERVER_H_ */
