/*
 ------------------------
 Name : Nishan Singh
 Email: nishan-singh1@myseneca.ca
 ------------------------
*/



#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstring>
#include<iomanip>
#include <occi.h>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee
{
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char email[100];
	char phone[50];
	char extension[10];
	char reportsTo[100];
	char jobTitle[50];
	char city[50];

};


int menu();
int findEmployee(Connection *conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection *conn, struct Employee emp);
void displayAllEmployees(Connection *conn);


int main(void)
{	
	Environment* env = nullptr;
	Connection* conn = nullptr;

	string usr = "";
	string pass = "";
	string srv = "";
	int empNo, found = 0;
	bool flag = true;

	struct Employee emp;
	emp.reportsTo[0] = '\0';

	try
	{
		
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);

		//int x = findEmployee(conn, 1056, &emp);
		//cout << x;
		
		while (flag)
		{
			int ch = menu();

			switch (ch)
			{
			case 1:

				cout << "Enter the Employee number: ";

				cin >> empNo;

				found = findEmployee(conn, empNo, &emp);

				if (found)
				{
					displayEmployee(conn, emp);
				}
				else
				{
					cout << "Employee " << empNo << " does not exist";
				}
				break;
		 	case 2:
				displayAllEmployees(conn);
				break;
			case 0: 
				flag = false;
			default:
				break;
			}
		}

		
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	

	//int x = menu();
	return 0;
}

int menu()
{	
	bool flag = true;
	int ch;

	cout << "\n********************* HR Menu *********************" << endl;
	cout << "1) Find Employee" << endl;
	cout << "2) Employees Report" << endl;
	cout << "3) Add Employee" << endl;
	cout << "4) Update Employee" << endl;
	cout << "5) Remove Employee" << endl;
	cout << "0) Exit" << endl;

	cout << "Enter: ";

	do{
		cin >> ch;

		if (ch >= 0 && ch <= 5){
			flag = false;
		} else{
			cout << "Please enter a valid option [0-5]: ";
		}

	} while (flag);
	
	return ch;

}

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp)
{	
	bool isAvailable = false;
	bool found = false;
	int reportsToID;
	string fname, lname;


	try
	{
		Statement* stmt = conn->createStatement();
		ResultSet* rs = stmt->executeQuery("SELECT employeenumber FROM employees");
		 
		string query = "Select employeenumber, lastname, firstname, email, phone, extension, reportsTo, jobtitle, city FROM employees e JOIN offices o ON o.officecode = e.officecode WHERE employeenumber =";
		string empNo = to_string(employeeNumber);
		string query2 = "SELECT firstName, Lastname FROM employees where employeenumber =";

		while (rs->next() && !found)
		{
			int eId = rs->getInt(1);
			if (employeeNumber == eId)
			{
				isAvailable = true;
				found = true;
			} 
		}
		if (isAvailable)
		{
			query = query + to_string(employeeNumber);
			rs = stmt->executeQuery(query);
			while (rs->next())
			{
				

				 
				emp->employeeNumber = rs->getInt(1);
				strcpy(emp->lastName, rs->getString(2).c_str());
				strcpy(emp->firstName, rs->getString(3).c_str());
				strcpy(emp->email, rs->getString(4).c_str());
				strcpy(emp->phone, rs->getString(5).c_str());
				strcpy(emp->extension, rs->getString(6).c_str());
				reportsToID = rs->getInt(7);
				query2 = query2 + to_string(reportsToID);
				strcpy(emp->jobTitle, rs->getString(8).c_str());
				strcpy(emp->city, rs->getString(9).c_str());
			}
			ResultSet* rs = stmt->executeQuery(query2);
			while (rs->next())
			{
				

				fname = rs->getString(1);
				lname = rs->getString(2);
				char fullname[100] = {};
				strcat(fullname, fname.c_str());
				strcat(fullname, " ");
				strcat(fullname, lname.c_str());
				strcpy(emp->reportsTo, fullname);

			}
		}

		conn->terminateStatement(stmt);

	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return isAvailable;
}

void displayEmployee(Connection* conn, struct Employee emp)
{

		cout << "\nemployeeNumber = " << emp.employeeNumber << endl;
		cout << "lastName = " << emp.lastName << endl;
		cout << "firstName = " << emp.firstName << endl;
		cout << "email = " << emp.email << endl;
		cout << "phone = " << emp.phone << endl;
		cout << "extension = " << emp.extension << endl;
		cout << "reportsTo = " << emp.reportsTo << endl;
		cout << "jobTitle = " << emp.jobTitle << endl;
		cout << "city = " << emp.city << endl;
	
}

void displayAllEmployees(Connection* conn)
{	

	struct Employee y;
	y.reportsTo[0] = '\0';
	try
	{

		Statement* stmt = conn->createStatement();
		ResultSet* rx = stmt->executeQuery("SELECT employeenumber FROM employees");

		cout  << setw(5) << left << "\nE" << setw(25) <<  "Employee Name" << setw(32) <<"Email" << setw(17) <<"Phone" << setw(6) << "Ext" << setw(20) << "Manager" << endl;

		while (rx->next())
		{
			int x = findEmployee(conn,rx->getInt(1), &y);
			char name[100];
			strcpy(name, y.firstName);
			strcat(name, " ");
			strcat(name, y.lastName);
			cout << setw(5) << left << y.employeeNumber << setw(25) << name <<  setw(32)  << y.email << setw(17) << y.phone << setw(6) << y.extension << setw(20) << y.reportsTo << endl;
		}
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}
