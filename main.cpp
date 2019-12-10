//Description: This program will perform most of the
//things that a normal bank account system would do
//Title: Simple Account App

#include <iostream>
#include <string>
#include <vector>
#include <limits>  //gets cin size to handle bad input
#include <iomanip> //removes scientific notation

using std::cin;
using std::cout;
using std::string;

class Account
{
  //private variables
  string firstName, lastName, password, fullName;
  double amount = 0;

public:
  //constructor (initializes account)
  Account(string fName,
          string lName,
          string xPass,
          double xAmount)
      : firstName(fName),
        lastName(lName),
        password(xPass),
        amount(xAmount)
  {
    fullName = firstName + " " + lastName;
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
};

void other();
void depositing(string), withdrawing();
void summaryAdmin(), summary(int, string); //takes in index
void title(std::string pTitle, std::string pOptions);
void createAcc();
void home(), homeUser(), homeAdmin();
void deleteAcc();

std::vector<Account> dbAccounts; //will act as our database
bool found;
string currentAccount;

int main()
{
  createAcc();

  home();

  system("pause");
  return 0;
}

void home()
{
  int x;
  title("Select an option:",
        "1. User\n2. Admin\n");
  cout << "\n-----------------------------------\n\n";
  cin >> x;
  cin.ignore();
  if (x == 1)
    homeUser();
  else
    homeAdmin();
}

void homeAdmin()
{
  int option;
  title("Welcome. Please select one of the following:",
        "1. Summary\n2. Deposit\n3. Withdraw\n4. Create\n5. Delete\n6. Switch to user");
  cout << "\n-----------------------------------\n\n";
  cin >> option;
  cin.ignore();
  switch (option)
  {
  case 1:
    summaryAdmin();
    break;
  case 2:
    depositing("admin");
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

void homeUser()
{
  string option;
  if (!found)
  {
    title("Enter full name of account:", "");
    std::getline(cin, currentAccount);
  }
  for (int i = 0; i < dbAccounts.size(); i++)
  {
    if (currentAccount == dbAccounts[i].getFullName())
    {
      found = true;
      title("Welcome, user. Please select one of the following:",
            "1. Summary\n2. Deposit\n3. Withdraw\n4. Switch to admin");
      cout << "\n-----------------------------------\n\n";
      std::getline(cin, option);
      if (option == "1")
        summary(i, "user");
      if (option == "4")
      {
        found = false;
        home();
      }
    }
  }
  if (found == false)
  {
    cout << "Account not found." << std::endl;
    system("pause");
    homeUser();
  }
}

void createAcc() //done
{
  string firstName, lastName, password;
  double initialAmount;
  title("Create Account", "Your first name: \n");
  std::getline(cin, firstName);
  cout << "Your last name: \n";
  std::getline(cin, lastName);
  do
  {
    if (initialAmount > 50000)
      cout << "Please enter 50000 or less:\n";
    else
      cout << "Initial amount: \n";
    cin >> initialAmount;
    cin.ignore();
  } while (initialAmount > 50000);
  cout << "Password: \n";
  cin >> password;
  cin.ignore();
  dbAccounts.push_back(Account(firstName, lastName, password, initialAmount));
}

void summaryAdmin() //done
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
  cin.ignore();

  summary(x - 1, "admin");
}

void summary(int i, string adminOrUser) //done
{
  title("SUMMARY", "");
  cout << std::fixed << std::setprecision(0) //removes scientific notation
       << "Account Name: " << dbAccounts[i].getFullName()
       << "\nAmount: " << dbAccounts[i].getAmount() << " PHP\n"
       << std::endl;

  system("pause");
  if (adminOrUser == "admin")
    homeAdmin();
  else
    homeUser();
}

void withdrawing()
{
  title("How much would you like to withdraw?", "Amount: ");
}

void depositing(string adminOrUser)
{
  double x;
  title("How much would you like to deposit?", "");
  cin >> x;
  cin.ignore();

  if (adminOrUser == "admin")
    homeAdmin();
  else
    homeUser();
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

bool isItAValidNumber(bool yesNo) //returns true if valid
{
  if (yesNo)
  {
    cin.clear(); //resets cin.fail() to false
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    /*
      cin.ignore() clears the characters that would make 
			cin.fail() true where the first argument indicates no
			limit to the number of characters to ignore and the
			second argument the character from which to stop ignoring
      which is the character for enter.
    */
    return false;
  }
  else
  {
    /*
      Discards extra input.
      If user types in numbers then letters, the
      letters and everything after will be discarded.
    */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
  }
}