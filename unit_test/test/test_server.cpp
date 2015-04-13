/*
 * test_factorial.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: prof-ahmed-saad-isa
 */

# include "gtest/gtest.h"
# include "Server.h"

 TEST(ServerTest, SimulateServer)
 {
	 Server server;
	 server.SimulateServer();
 }

TEST(ServerTest, SimulateSingleRandomCreditTransaction)
{
	Server server;

	srand(time(0));
	Transaction trans;
	trans.AccountNumber = rand() % server.Accounts.size();
	trans.Amount = rand() % 10000;
	trans.bDebit = false;

	double exp = server.Accounts[trans.AccountNumber] + trans.Amount;

	server.SimulateTransaction(trans);

	double act = server.Accounts[trans.AccountNumber];
	EXPECT_FLOAT_EQ(exp, act);
}

TEST(ServerTest, SimulateMultipleRandomCreditTransaction)
{
	Server server;

	//First transaction
	Transaction trans;
	trans.AccountNumber = rand() % server.Accounts.size();
	trans.Amount = rand() % 10000;
	trans.bDebit = false;

	double exp = server.Accounts[trans.AccountNumber] + trans.Amount;

	server.SimulateTransaction(trans);

	double act = server.Accounts[trans.AccountNumber];
	EXPECT_FLOAT_EQ(exp, act);


	//Second Transaction
	trans.AccountNumber = rand() % server.Accounts.size();
	trans.Amount = rand() % 10000;
	trans.bDebit = false;

	exp = server.Accounts[trans.AccountNumber] + trans.Amount;

	server.SimulateTransaction(trans);

	act = server.Accounts[trans.AccountNumber];
	EXPECT_FLOAT_EQ(exp, act);
}

TEST(ServerTest, SimulateSimpleDebitTransaction)
{
	Server server;

	Transaction trans;
	trans.AccountNumber = rand() % server.Accounts.size();
	trans.Amount = server.Accounts[trans.AccountNumber] - 1;
	trans.bDebit = true;

	double exp = 1;

	server.SimulateTransaction(trans);

	double act = server.Accounts[trans.AccountNumber];
	EXPECT_FLOAT_EQ(exp, act);
}

TEST(ServerTest, SimulateDebitExccedingTransaction)
{
	Server server;

	Transaction trans;
	trans.AccountNumber = rand() % server.Accounts.size();
	trans.Amount = server.Accounts[trans.AccountNumber] + 1;
	trans.bDebit = true;

	double exp = server.Accounts[trans.AccountNumber];

	server.SimulateTransaction(trans);

	double act = server.Accounts[trans.AccountNumber];
	EXPECT_FLOAT_EQ(exp, act);
}


