/*
 * Transaction.h
 *
 *  Created on: Apr 13, 2015
 *      Author: prof-ahmed-saad-isa
 */

#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#pragma once

struct Transaction
{
	long AccountNumber;
	bool bDebit;
	double Amount;
};

struct TransactionOutput
{
	Transaction transaction;
	bool bSucceed;
};

#endif /* TRANSACTION_H_ */
