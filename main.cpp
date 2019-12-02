//Description: This program will perform most of the things that
// a normal bank account system would do
//Title: Simple Account App

#include <iostream>
#include <string>

using std::cin;
using std::cout;

int option = 0;
double savings = 0, withdraw = 0, deposit = 0;

void other();
void depositing();
void home();
void withdrawing();
void summary();
void title(std::string pTitle, std::string pOptions);

int main()
{
  depositing();
  home();

  system("pause");
  return 0;
}

void home()
{
  title("Welcome. Please select one of the following:",
        "1. Withdraw\n2. Deposit\n3. Account Summary\n4. Other");
  cout << "\n-----------------------------------\n\n";
  cin >> option;
  switch (option)
  {
  case 1:
    summary();
    break;
  case 2:
    depositing();
    break;
  case 3:
    summary();
    break;
  case 4:
    other();
    break;
  default:
    break;
  }
}

void newAccount()
{
  title("Enter details for new account.\n", "");
  cout << "Account Name: "
       //cin
       << "Initial Amount: ";
  //cin
}

void summary()
{
  title("SUMMARY", "");
  cout << "Amount: "
       << savings << " PHP\n"
       << "-----------------------------------\n"
       << "Enter 1 to go to menu: ";
  cin >> option;
  if (option == 1)
  {
    home();
  }
}

void withdrawing()
{
  title("How much would you like to withdraw?", "Amount: ");
}

void depositing()
{
  title("How much would you like to deposit?", "");
  cin >> deposit;
  savings = deposit;
}

void other()
{
  title("What would you like to do?",
        "1. New account\n2. Search Account\n3. Delete Account\n");
}

void searchAcc()
{
}

void deleteAcc()
{
  title("What account would you like to delete?", "1. placeholder\n1. placeholder\n");
  //array of accounts... or i dunno leme see
}

void title(std::string pTitle, std::string pOptions)
{
  system("cls");
  cout << pTitle
       << "\n-----------------------------------\n"
       << pOptions;
}
/*
void editAccount()
{
  system("cls");
  cout << "What would you like to change?\n"
       << "-----------------------------------\n"
       << "1. Name\n"
       << "2. Edit Account\n"
       << "3. Delete Account\n";
}
*/