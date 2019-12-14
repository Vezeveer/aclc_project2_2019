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

void depositing(int, string), withdrawing(int, string);
void summaryAdmin(), summary(int, string); //takes in index
void title(std::string pTitle, std::string pOptions);
void createAccount();
void home(), homeUser(), homeAdmin();
void deleteAccount();
bool isItANum(bool yesNo);
double getCheckAmount(int, string, string);
void editAccount();
void displayAllAccounts();
int checkChoice(string, string, bool);
bool isNameValid(string);
string firstLastName(string);
bool isPassInvalid(string);

class Account
{
  //private variables
  string firstName, lastName, password, fullName;
  double amount = 0;

public:
  //constructor (initializes an account)
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
  void changeName(string, string); //declared
};

//member function defined outside the class
void Account::changeName(string xName, string yName)
{
  firstName = xName;
  lastName = yName;
  fullName = xName + " " + yName;
}

//this will act as our database
std::vector<Account> dbAccounts = {Account("James",
                                           "Bond",
                                           "007",
                                           20000.0)};
bool found;
string currentAccount;

int main()
{
  home();

  system("pause");
  return 0;
}

void home()
{
  int x;
  title("Select an option:",
        "1. User\n2. Admin\n");
  cout << "-----------------------------------\n>";
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
    home();
  }
}

void homeAdmin()
{
  int option;
  title("Welcome. Please select one of\nthe following:",
        "1. Summary\n2. Deposit\n3. Withdraw\n4. Create\n");
  cout << "5. Delete\n6. Edit Account\n7. Switch to user";
  cout << "\n-----------------------------------\n>";
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
    createAccount();
    break;
  case 5:
    deleteAccount();
    break;
  case 6:
    editAccount();
    break;
  case 7:
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
    title("Enter full name of your account:", ">");
    std::getline(cin, currentAccount);
  }

  for (int i = 0; i < dbAccounts.size(); i++) //acc index finder
  {
    if (currentAccount == "home")
      home();
    if (currentAccount == dbAccounts[i].getFullName())
    {
      found = true;
      title("Welcome, " + dbAccounts[i].getFullName() +
                ".\nPlease select one of the following:",
            "1. Summary\n2. Deposit\n3. Withdraw\n4. Switch to admin");
      cout << "\n-----------------------------------\n>";
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

  if (found == false) //loop back if not found
  {
    cout << "Account not found.\nTry again or "
         << "type \"home\" to go home"
         << "\n\n";
    system("pause");
    homeUser();
  }
}

void createAccount() //done
{
  string firstName, lastName, password;
  double initialAmount;
  bool invalidPassword = false;

  firstName = firstLastName("first");
  lastName = firstLastName("last");

  do
  {
    if (initialAmount > 50000)
      cout << "Please enter 50000 or less:\n";
    else
      title("Initial deposit: ", ">");
    cin >> initialAmount;
    if (cin.fail())
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
      cin.ignore();
  } while (initialAmount > 50000);

  do
  {
    if (invalidPassword)
      title("Enter password: ", "Invalid Password.\n>");
    else
      title("Enter password: ", ">");
    getline(cin, password);
    invalidPassword = isPassInvalid(password);
  } while (invalidPassword);

  dbAccounts.push_back(Account( //add account
      firstName,
      lastName,
      password,
      initialAmount));

  homeAdmin();
}

bool isPassInvalid(string iPassword)
{
  for (int i = 0; i < iPassword.size(); i++)
  {
    if (isspace(iPassword[i])) //check for spaces
      return true;
  }
  return false;
}

string firstLastName(string fl)
{
  string iName;
  bool namePassed = true;
  do
  {
    if (namePassed == false)
      title("Create Account. Your " + fl + " name: ",
            "Invalid name.\n>");
    else
      title("Create Account. Your " + fl + " name: ", ">");
    std::getline(cin, iName);
    namePassed = isNameValid(iName);
  } while (namePassed == false);
  return iName;
}

bool isNameValid(string chkName) //return true if valid
{
  for (int i = 0; i < chkName.size(); i++)
  {
    if (isdigit(chkName[i])) //checks for digits
      return false;
    else if (isspace(chkName[i])) //space checking
      return false;
    else if (i > 0 && isupper(chkName[i])) //capitals
      return false;
  } //checks if name exists and 1st letter Capitalized
  return chkName.size() && isupper(chkName[0]);
}

void summaryAdmin() //done
{
  int x;
  title("SUMMARY of which account?", "");
  for (int j = 0; j < dbAccounts.size(); j++)
  {
    cout << j + 1 << ". " << dbAccounts[j].getFullName() << "\n";
  }

  cout << "-----------------------------------\n>";
  cin >> x; //pass index of acc in db
  cin.ignore();

  summary(x - 1, "admin");
}

void summary(int i, string adminOrUser) //done
{
  title("SUMMARY", "");
  cout << std::fixed << std::setprecision(0) //removes scientific notation
       << "Account Name: " << dbAccounts[i].getFullName()
       << "\nAmount: " << dbAccounts[i].getAmount() << " PHP"
       << "\n-----------------------------------\n";

  system("pause");
  if (adminOrUser == "admin")
    homeAdmin();
  else
    homeUser();
}

void withdrawing(int i, string adminOrUser)
{
  string adminTitle = "Withdraw from which account?";
  string askTitle = "How much would you like to withdraw?";

  if (adminOrUser == "user")
  {
    title(askTitle, "Amount: ");
    dbAccounts[i].withdrawAmount(getCheckAmount(i, "withdraw", askTitle));
    cout << "Success.\n";
    cout << "Current Amount: " << dbAccounts[i].getAmount() << " PHP\n\n";
    system("pause");
    homeUser();
  }

  if (adminOrUser == "admin")
  {
    int iAdmin;
    title(adminTitle, "");
    displayAllAccounts();
    cout << "-----------------------------------\n";
    cout << ">";
    iAdmin = checkChoice(adminTitle, "", true);
    iAdmin -= 1;
    i = iAdmin;

    title(askTitle, "");
    cout << ">";
    dbAccounts[i].withdrawAmount(getCheckAmount(i, "withdraw", askTitle));
    cout << "Success.\n";
    cout << "Current Amount: " << dbAccounts[i].getAmount() << " PHP\n\n";
    system("pause");
    homeAdmin();
  }
}

void depositing(int i, string adminOrUser) //done
{
  int iAdmin;
  double amount2Deposit;
  string xTitle = "Specify amount to deposit between\n500-50000";
  string yTitle = "Deposit to which account?";

  if (adminOrUser == "admin")
  {
    title(yTitle, "");
    displayAllAccounts();
    cout << "-----------------------------------\n";
    cout << ">";
    iAdmin = checkChoice(yTitle, "", true); //will be used as index
    iAdmin -= 1;
    i = iAdmin;
  }

  title(xTitle, "");
  cout << ">";
  amount2Deposit = getCheckAmount(i, "deposit", xTitle);

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

int checkChoice(string xTitle, string sTitle, bool xDisplayAll)
{
  int xChoice;
  bool invalidPassword = true;
  while (invalidPassword)
  {
    cin >> xChoice;
    if (cin.fail()) //cin.fail() will return true if invalid
    {
      title(xTitle, sTitle);
      if (xDisplayAll)
        displayAllAccounts();
      cout << "-----------------------------------\n";
      cout << "Invalid Input\n>";
      invalidPassword = true;
      cin.clear(); //resets cin.fail() to false
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
      invalidPassword = false;
  }
  //cout << "Reached here: ";
  //cin.ignore();
  return xChoice;
}

void displayAllAccounts()
{
  for (int j = 0; j < dbAccounts.size(); j++)
  {
    cout << j + 1 << ". " << dbAccounts[j].getFullName() << "\n";
  }
}

void searchAcc()
{
}

void deleteAccount()
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

void editAccount()
{
  int xElement, xC, xIndex;
  string xfName, xlName;
  title("Which account?", "");
  for (int i = 0; i < dbAccounts.size(); i++)
  {
    cout << i + 1 << ". " << dbAccounts[i].getFullName() << "\n";
  }
  cin >> xElement;
  xIndex = xElement - 1;
  cin.ignore();
  title("Change what?", "1. Name\n");
  cin >> xC;
  cin.ignore();
  if (xC == 1)
  {
    cout << "Enter new first name: ";
    getline(cin, xfName);
    //cin.ignore();
    cout << "Enter new last name: ";
    getline(cin, xlName);
    //cin.ignore();
    //Error here...
    dbAccounts[xIndex].changeName(xfName, xlName);
    summary(xIndex, "admin");
  }
  cout << "Invalid Input.\n";
  homeAdmin();
}

double getCheckAmount(int i, string depositOrWithdraw, string askTitle)
{
  double xCash;
  bool invalidPassword = true;
  double balance = dbAccounts[i].getAmount();
  do
  {
    if (cin.fail())
    {
      title("Invalid amount.\nAmount should be 500 - 50000", ">");
      cin.clear(); //resets cin.fail() to false
      cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    cin >> xCash;
    invalidPassword = cin.fail();
    if (cin.fail() == false)
    {
      if (depositOrWithdraw == "deposit")
      {
        if (xCash > 50000 || xCash < 500)
        {
          title("Invalid amount.\nAmount should be 500 - 50000", ">");
          invalidPassword = true;
        }
        else
          invalidPassword = false;
      }
      else if (depositOrWithdraw == "withdraw")
      {
        if (xCash > balance && xCash > 0 || xCash <= 0)
        {
          system("cls");
          cout << "Invalid amount.\nMaximum amount to draw: "
               << balance << " PHP\n"
               << "-----------------------------------\n>";
          invalidPassword = true;
        }
        else
          invalidPassword = false;
      }
    }
    else
      invalidPassword = cin.fail();
  } while (invalidPassword);

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
