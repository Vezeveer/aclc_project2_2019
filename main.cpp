//Description: This program will perform most of the
//things that a normal bank account system would do
//Title: Simple Account App
//Programmers: Emmanuel Valdueza, Jake Ogsimer, Mark Perez

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
void depositing(int, string), withdrawing(int, string);
void summaryAdmin(), summary(int, string); //takes in index
void title(std::string pTitle, std::string pOptions);
void createAcc();
void home(), homeUser(), homeAdmin();
void deleteAcc();
bool isItANum(bool yesNo);
double getCheckAmount(int, string);

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
  else if (x == 2)
    homeAdmin();
  else
  {
    cout << "\nInvalid Input or Option.\n\n";
    system("pause");
    if (cin.fail())
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

void homeAdmin()
{
  int option;
  title("Welcome. Please select one of\nthe following:",
        "1. Summary\n2. Deposit\n3. Withdraw\n4. Create\n");
  cout << "5. Delete\n6. Switch to user";
  cout << "\n-----------------------------------\n\n";
  cin >> option;
  cin.ignore();
  switch (option)
  {
  case 1:
    summaryAdmin();
    break;
  case 2:
    depositing(0, "admin"); //0 will never be used
    break;
  case 3:
    withdrawing(0, "admin");
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
    cout << "\nInvalid Input or Option.\n\n";
    system("pause");
    if (cin.fail())
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    homeAdmin();
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
  for (int i = 0; i < dbAccounts.size(); i++) //acc index finder
  {
    if (currentAccount == dbAccounts[i].getFullName())
    {
      found = true;
      title("Welcome, user. Please select one of\nthe following:",
            "1. Summary\n2. Deposit\n3. Withdraw\n4. Switch to admin");
      cout << "\n-----------------------------------\n\n";
      std::getline(cin, option);
      if (option == "1")
        summary(i, "user");
      if (option == "2")
        depositing(i, "user");
      if (option == "3")
        withdrawing(i, "user");
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
  dbAccounts.push_back(Account( //add account
      firstName,
      lastName,
      password,
      initialAmount));
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
  cin >> x; //pass index of acc in db
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

void withdrawing(int i, string adminOrUser)
{
  int iAdmin;
  title("How much would you like to withdraw?", "Amount: ");

  if (adminOrUser == "user")
    dbAccounts[i].withdrawAmount(getCheckAmount(i, "withdraw"));
  cout << "Success.\n";
  cout << "Current Amount: " << dbAccounts[i].getAmount() << " PHP\n\n";
  system("pause");
  homeUser();

  if (adminOrUser == "admin")
  {
    title("Withdraw from which account?", "");
    cout << "\n";
    for (Account acc : dbAccounts) //display all accounts
    {
      cout << "1. " << acc.getFullName();
    }

    cout << "\n\n";
    cin >> iAdmin; //pass index of acc in db
    cin.ignore();
    iAdmin -= 1;
    i = iAdmin;
    dbAccounts[i].withdrawAmount(getCheckAmount(i, "withdraw"));
    cout << "Success.\n";
    cout << "Current Amount: " << dbAccounts[i].getAmount() << " PHP\n\n";
    system("pause");
    homeAdmin();
  }
}

void depositing(int i, string adminOrUser)
{
  int iAdmin;
  double amount2Deposit;

  if (adminOrUser == "admin")
  {
    title("Deposit to which account?", "");
    cout << "\n";
    for (Account acc : dbAccounts) //display all accounts
    {
      cout << "1. " << acc.getFullName();
    }

    cout << "\n\n";
    cin >> iAdmin; //pass index of acc in db
    iAdmin -= 1;
    i = iAdmin;
    cin.ignore();
  }

  title("How much would you like to deposit?", "");
  amount2Deposit = getCheckAmount(i, adminOrUser);

  dbAccounts[i].depositAmount(amount2Deposit);
  cout << "Deposit success.\n";
  cout << "Current Amount is now: "
       << dbAccounts[i].getAmount() << " PHP\n\n";
  system("pause");

  if (adminOrUser == "admin") //return to home
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
  title("What account would you like to delete?",
        "1. placeholder\n1. placeholder\n");
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

double getCheckAmount(int i, string depositOrWithdraw) //checks if is a number
{
  double xCash;
  bool keepLooping = true;
  do
  {
    if (cin.fail())
    {
      cout << "Invalid Input or Amount.\n";
      cin.clear(); //resets cin.fail() to false
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    cin >> xCash;
    keepLooping = cin.fail();
    if (cin.fail() == false)
    {
      if (depositOrWithdraw == "deposit")
      {
        if (xCash > 50000 || xCash < 500 && xCash > 0)
        {
          cout << "Amount should be 500 - 50000\n=>";
          keepLooping = true;
        }
        else
          keepLooping = false;
      }
      else if (depositOrWithdraw == "withdraw")
      {
        if (xCash > dbAccounts[i].getAmount() && xCash > 0)
        {
          cout << "Amount should be less than "
               << dbAccounts[i].getAmount() << " PHP\n=>";
          keepLooping = true;
        }
        else
          keepLooping = false;
      }
    }
    else
      keepLooping = cin.fail();
  } while (keepLooping);

  cin.ignore();
  return xCash;
}

//Used later

bool isItANum(bool yesNo) //returns true if valid
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
