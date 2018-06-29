/*
	FileName	: CD_Interest.cc
	Author		: Zachary Zuch
	Course		: CSCI 362-02
	Assignment 	: Lab 2
	Description	: Print out a table of interest values by year.
*/

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <locale.h>

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;


/************************************************************/
// Function prototypes

void
printIntro(); 

void
printTable(unsigned int numRows, double balance, double rate);

void
printRow(unsigned int rowNum, double balance, double interest);

double
calcInterest(double balance, double rate);

/************************************************************/

int
main(int argc, char* argv[])
{
	printIntro();

	cout << "Please enter the initial balance: ";
	double initBalance;
	cin >> initBalance;

	cout << "Please enter the interest rate  : ";
	double rate;
	cin >> rate;
	
	cout << "Please enter the number of years: ";
	unsigned int years;
	cin >> years;

	printTable(years, initBalance, rate);	

	return EXIT_SUCCESS;
}

/************************************************************/

void
printIntro()
{
	cout << "This program will calculate the interest earned" << endl;
	cout << "  on a CD over a period of several years." << endl << endl;
}

/************************************************************/

void
printTable(unsigned int numRows, double balance, double rate)
{
	cout << endl;
	cout << "Year     Balance     Interest     New Balance" << endl;
	cout << "----     -------     --------     -----------" << endl;

	double interest = 0.0;
	for (unsigned int i = 1; i <= numRows; ++i)
	{
		interest = calcInterest(balance, rate);
		printRow(i, balance, interest);
		balance += interest;
	}
	cout << endl;
}

/************************************************************/

void
printRow(unsigned int rowNum, double balance, double interest)
{
	// READD Apostrophes ON Submit
	// setlocale(LC_NUMERIC, "");
	// REMOVE A SPACE AFTER %-4d to align it properly on linux 
	// 	with the apostrophe flag
	printf("%-4d     %7.2f     %8.2f     %11.2f\n",
		rowNum, balance, interest, (balance + interest));
}

/************************************************************/

double
calcInterest(double balance, double rate)
{
	return balance * (0.01 * rate);
}

/************************************************************/