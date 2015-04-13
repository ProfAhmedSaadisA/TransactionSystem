/*
 * Server.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: prof-ahmed-saad-isa
 */

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "Server.h"
#include "ctime"
using namespace std;

vector<double> Server::Accounts;
vector<pthread_mutex_t> Server::account_mutexes;
ofstream Server::lout("text/Log.txt");
string TransactionTypes[2] = {"Credit", "Debit"};

//Write Log
void Server::WriteLog(string strLog)
{
#ifdef ENABLE_LOGGING
	//pthread_mutex_lock(&log_mutex);
	Server::lout<<strLog;
	//pthread_mutex_unlock(&log_mutex);
#endif
}

//Initialize Accounts
void Server::InitAccounts()
{
	//It's already initialized
	if(Accounts.size())
		return;
	WriteLog("Initializing Accounts...\n\n");
	ifstream fin("text/Accounts.txt");
	int balance;
	long i = 0;
	while(fin>>balance)
	{
		ostringstream strLog;
		strLog 	<< "Account#" << i++
				<< ". Balance: " << balance << endl;
		WriteLog(strLog.str());
		Accounts.push_back(balance);
	}
}

//Initialize Mutexes
void Server::InitMutexes()
{
	//it's already initialized
	if(account_mutexes.size())
		return;
	WriteLog("Initializing Mutexes...\n\n");
	int nAccounts = Accounts.size();
	account_mutexes.resize(nAccounts);
	for (long i = 0; i < nAccounts; i++)
	{
		pthread_mutex_init(&account_mutexes[i], NULL);
	}
}

//Initialize Accounts and Mutexes
void Server::Init()
{
	//pthread_mutex_init(&log_mutex, NULL);
	WriteLog("Initializing Server...\n\n");
	InitAccounts();
	InitMutexes();
}

//Initialize Transaction
Transaction Server::InitTransaction()
{
	Transaction trans;
	trans.AccountNumber = rand() % Server::Accounts.size();
	trans.Amount = rand() % MAX_TRANS;
	trans.bDebit = rand() % 2;
	return trans;
}

//Initialize Transactions
void Server::InitTransactions()
{
	WriteLog("Initializing Transactions...\n\n");
	srand(time(NULL));
	int nThreads = rand() % MAX_THREADS;
	Transactions.resize(nThreads);
	for (long i = 0; i < nThreads; i++)
	{
		Transactions[i] = InitTransaction();
	}
}

//Transaction thread
void* Server::Transact(void *arg)
{
	Transaction* trans = (Transaction*) arg;
	pthread_mutex_lock(&Server::account_mutexes[trans->AccountNumber]);

	TransactionOutput* transOut = new TransactionOutput;
	transOut->transaction = *trans;
	transOut->bSucceed = true;
	int ID = trans->AccountNumber;
	if (trans->bDebit)
	{
		if (Accounts[ID] > trans->Amount)
			Accounts[ID] -= trans->Amount;
		else
			transOut->bSucceed = false;
	}
	else
	{
		Accounts[ID] += trans->Amount;
	}
	pthread_mutex_unlock(&account_mutexes[ID]);
	//
	//pthread_exit((void*) 0);
	return (void*) transOut;
}

//Create Thread
pthread_t Server::CreateThread(Transaction &trans)
{
	ostringstream strLog;
	strLog	<< "Performing "<<TransactionTypes[trans.bDebit]
	    	<< " transaction for Account#" << trans.AccountNumber
	    	<< " with amount: "<< trans.Amount << endl;
	WriteLog(strLog.str());

	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&thread, &attr, &Server::Transact, (void *) &trans);

	pthread_attr_destroy(&attr);

	return thread;
}

//Create Threads
void Server::CreateThreads()
{
	int nTransactions = Transactions.size();
	callThd.resize(nTransactions);
	for (long i = 0; i < nTransactions; i++)
	{
		callThd[i] = CreateThread(Transactions[i]);
	}
}

//Wait Thread
void Server::WaitThread(pthread_t thread)
{
	//Wait on the thread
	void *status;
	pthread_join(thread, &status);
	TransactionOutput* transOut = (TransactionOutput*) status;
	if (!transOut->bSucceed)
	{
		ostringstream strLog;
		strLog	<< "Sorry! No enough balance to complete the debit transaction for Account#"
				<< transOut->transaction.AccountNumber << " with amount: "
				<< transOut->transaction.Amount << endl;
		WriteLog(strLog.str());
	}
	delete transOut;
}



//Wait Threads
void Server::WaitThreads()
{
	//Wait on the other threads
	void *status;
	int nThreads = callThd.size();

	for (long i = 0; i < nThreads; i++)
	{
		pthread_join(callThd[i], &status);
		TransactionOutput* transOut = (TransactionOutput*) status;
		if (!transOut->bSucceed)
		{
			ostringstream strLog;
			strLog	<< "Sorry! No enough balance to complete the debit transaction for Account#"
					<< transOut->transaction.AccountNumber << " with amount: "
					<< transOut->transaction.Amount << endl;
			WriteLog(strLog.str());
		}
		delete transOut;
	}
}

//Destroy Mutuxes
void Server::DestroyMutexes()
{
	WriteLog("\nDestroying Mutexes...\n\nAccounts Summary:\n\n");
	long nMutexes = account_mutexes.size();
	for (long i = 0; i < nMutexes; i++)
	{
		pthread_mutex_destroy(&account_mutexes[i]);
		ostringstream strLog;
		strLog << "Account #" << i << ": " << Accounts[i] << endl;
		WriteLog(strLog.str());
	}
	//pthread_mutex_destroy(&log_mutex);
}

//Save Accounts physically
void Server::SaveAccounts()
{
	ofstream fout("text/Accounts.txt");
	int nAccounts = Server::Accounts.size();
	for(long i = 0 ; i < nAccounts ; i++)
	{
		fout<<Server::Accounts[i]<<endl;
	}
	fout.close();
}

//Simulate Transaction
void Server::SimulateTransaction(Transaction &trans)
{
	pthread_t thread = CreateThread(trans);
	WaitThread(thread);
}

//Simulate Server
void Server::SimulateServer()
{
	InitTransactions();
	CreateThreads();
	WaitThreads();
}

//Constructor
Server::Server()
{
    time_t tnow = time(0);
	WriteLog("\nLogging:" + string(ctime(&tnow)) + "\nConstructing Server...\n\n");
	Init();
}

//Destructor
Server::~Server()
{
	DestroyMutexes();
	//SaveAccounts();
}




