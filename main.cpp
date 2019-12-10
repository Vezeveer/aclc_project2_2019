//Description: This program will perform most of the
//things that a normal bank account system would do
//Title: Simple Account App

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

class Account
{
  //private variables
  string firstName, lastName, password;
  double amount;
  int id;

public:
  //constructor (initializes account)
  Account(string fName,
          string lName,
          string xPass,
          double xAmount,
          int xId)
      : firstName(fName),
        lastName(lName),
        password(xPass),
        amount(xAmount),
        id(xId)
  {
  }

  std::string getFullName()
  {
    string fullName = firstName + " " + lastName;
    return fullName;
  }
  std::string getFirstName()
  {
    return firstName;
  }
  std::string getLastName()
  {
    return lastName;
  }
  double getAmount()
  {
    return amount;
  }
  void depositAmount(double xAmount)
  {
    amount += xAmount;
  }
  void withdrawAmount(double yAmount)
  {
    amount -= yAmount;
  }
  int getId()
  {
    return id;
  }
};

int idIncrementer = 0;
int option = 0;
double savings = 0, withdraw = 0, deposit = 0;

void other();
void depositing(), withdrawing();
void summaryAdmin(), summary(int);
void title(std::string pTitle, std::string pOptions);
void createAcc();
void home(), homeUser(), homeAdmin();
void deleteAcc();

std::vector<Account> dbAccounts; //will act as our database

int main()
{
  createAcc();

  home();

  system("pause");
  return 0;
}

void homeAdmin()
{

  title("Welcome. Please select one of the following:",
        "1. Summary\n2. Deposit\n3. Withdraw\n4. Create\n5. Delete\n6. Switch to user");
  cout << "\n-----------------------------------\n\n";
  cin >> option;
  switch (option)
  {
  case 1:
    summaryAdmin();
    break;
  case 2:
    depositing();
    break;
  case 3:
    withdrawing();
    break;
  case 4:
    createAcc();
    break;
  case 5:
    deleteAcc();
    break;
  case 6:
    homeUser();
    break;
  default:
    home();
    break;
  }
}

void home()
{
  int x;
  title("Select an option:",
        "1. User\n2. Admin\n");
  cout << "\n-----------------------------------\n\n";
  cin >> x;
  if (x == 1)
    homeUser();
  else
    homeAdmin();
}

void homeUser()
{
  //undefined
  //undefined
  //undefined
}

void createAcc() //done
{
  string firstName, lastName, password;
  double initialAmount;
  title("Create Account", "Your first name: \n");
  cin >> firstName;
  cout << "Your last name: \n";
  cin >> lastName;
  cout << "Initial amount: \n";
  cin >> initialAmount;
  cout << "Password: \n";
  cin >> password;
  dbAccounts.push_back(Account(firstName, lastName, password, initialAmount, idIncrementer));

  idIncrementer++;
}

void newAccount()
{
  title("Enter details for new account.\n", "");
  cout << "Account Name: "
       //cin
       << "Initial Amount: ";
  //cin
}

void summaryAdmin()
{
  int x;
  title("SUMMARY of which account?", "");
  cout << "\n";
  for (Account acc : dbAccounts)
  {
    cout << "1. " << acc.getFullName();
  }

  cout << "\n";
  cin >> x;
  summary(x);

  system("pause");
  homeAdmin();
}

void summary(int option)
{
  for (int i = 0; i < dbAccounts.size(); i++)
  {
    if (option - 1 == dbAccounts[i].getId())
    {
      title("SUMMARY", "");
      cout << "Account Name: " << dbAccounts[i].getFullName()
           << "\nAmount: " << dbAccounts[i].getAmount() << " PHP\n"
           << std::endl;
    }
  }
}

void withdrawing()
{
  title("How much would you like to withdraw?", "Amount: ");
}

void depositing()
{
  double x;
  title("How much would you like to deposit?", "");
  cin >> x;
  //undefined
  //undefined
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