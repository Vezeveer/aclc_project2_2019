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
double getCheckAmount(int, string, string, string);
void editAccount();
string displayAllAccounts();
int checkChoice(string, string, int);
bool isNameValid(string);
string firstLastName(string, string);
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
bool userFound;
string userAccInput;

int main()
{
  home();

  system("pause");
  return 0;
}

void home()
{
  int x, maxChoices = 2;
  string leadTitle = "Select an option:",
         optionsTitle = "1. User\n2. Admin";
  title(leadTitle,
        optionsTitle);
  x = checkChoice(leadTitle, optionsTitle, maxChoices);

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
      cin.ignore(std::
                     numeric_limits<std::streamsize>::
                         max(),
                 '\n');
    }
    home();
  }
}

void homeAdmin()
{
  int option, maxChoices = 7;
  string leadTitle = "Welcome. Please select one"
                     " of\nthe following:",
         optionsTitle = "1. Summary\n2. Deposit"
                        "\n3. Withdraw\n4. Create\n"
                        "5. Delete\n6. Edit Account"
                        "\n7. Switch to user";

  title(leadTitle, optionsTitle);
  option = checkChoice(leadTitle, optionsTitle, maxChoices);

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

  if (!userFound)
  {
    title("Enter full name of your account:", ">");
    std::getline(cin, userAccInput);
  }

  for (int i = 0; i < dbAccounts.size(); i++) //acc index finder
  {
    userFound = userAccInput == dbAccounts[i].getFullName();
    if (userFound)
    {
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
        userFound = false;
        home();
      }
    }
    if (userAccInput == "home")
      home();
  }

  if (userFound == false) //loop back if not userFound
  {
    cout << "Account not userFound.\nTry again or "
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

  firstName = firstLastName("first", "new");
  lastName = firstLastName("last", "new");

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

string firstLastName(string fl, string newOrEdit) //return name if valid
{
  string iName;
  bool namePassed = true;

  if (newOrEdit == "edit")
    newOrEdit = "Editing Account.\nYour new " + fl + " name:";
  else
    newOrEdit = "Creating Account.\nYour " + fl + " name:";

  do
  {
    if (namePassed == false)
      title(newOrEdit,
            "Invalid name.\n>");
    else
      title(newOrEdit, ">");
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
  int x, maxChoices = dbAccounts.size();
  string optionsTitle = displayAllAccounts(),
         leadTitle = "SUMMARY of which account?";
  title(leadTitle, displayAllAccounts());
  x = checkChoice(leadTitle, optionsTitle, maxChoices);

  summary(x - 1, "admin");
}

void summary(int i, string adminOrUser) //done
{
  string actualAmount = std::to_string(dbAccounts[i].getAmount());
  //removes the last 4 digits
  actualAmount.erase(actualAmount.size() - 4, 4);

  string optionsTitle = "Account Name: " +
                        dbAccounts[i].getFullName() +
                        "\nAmount: " +
                        actualAmount +
                        " PHP";

  title("Account Summary", optionsTitle);
  system("pause");

  if (adminOrUser == "admin")
    homeAdmin();
  else
    homeUser();
}

void withdrawing(int i, string adminOrUser)
{
  bool keepLooping = true;
  double withDrawAmount = 0;
  string leadTitle = "WITHDRAWING";
  string adminTitle = "\nWithdraw from which account?";
  string askTitle = "\nHow much would you like to withdraw?";
  string optionsTitle = "1. 500\n2. 1500"
                        "\n3. 3000\n4. 5000\n5. Custom";

  if (adminOrUser == "user")
  {
    title(leadTitle, askTitle);
    dbAccounts[i].withdrawAmount(getCheckAmount(i,
                                                "withdraw",
                                                leadTitle,
                                                optionsTitle));
    cout << "Success.\n";
    cout << "Current Amount: "
         << dbAccounts[i].getAmount()
         << " PHP\n\n";

    system("pause");
    homeUser();
  }

  if (adminOrUser == "admin")
  {
    int iAdmin,
        maxAccChoices = dbAccounts.size(),
        maxDrawChoices = 5;

    //Ask account
    title(leadTitle + adminTitle, displayAllAccounts());
    iAdmin = checkChoice(leadTitle + adminTitle,
                         displayAllAccounts(),
                         maxAccChoices);
    iAdmin -= 1;
    i = iAdmin;

    //Ask Preset Amount
    do
    {
      title(leadTitle + askTitle, optionsTitle);
      int choice = checkChoice(leadTitle + adminTitle,
                               optionsTitle,
                               maxDrawChoices);
      switch (choice)
      {
      case 1:
        withDrawAmount = 500;
        break;
      case 2:
        withDrawAmount = 1500;
        break;
      case 3:
        withDrawAmount = 3000;
        break;
      case 4:
        withDrawAmount = 5000;
        break;
      case 5:
        //Ask Custom Amount
        dbAccounts[i].withdrawAmount(getCheckAmount(i,
                                                    "withdraw",
                                                    leadTitle,
                                                    optionsTitle));
        break;
      default:
        break;
      }

      if (withDrawAmount <= dbAccounts[i].getAmount())
      {
        dbAccounts[i].withdrawAmount(withDrawAmount);
        keepLooping = false;
      }
      else
        keepLooping = true;
    } while (keepLooping);

    //convert doubles to strings
    string crAmount = std::to_string(dbAccounts[i].getAmount());
    crAmount.erase(crAmount.size() - 4, 4);
    string strAmount = std::to_string(withDrawAmount);
    strAmount.erase(strAmount.size() - 4, 4);

    title(leadTitle, "Success.\n"
                     "Amount Drawn: " +
                         strAmount +
                         "\nCurrent Balance: " +
                         crAmount + " PHP");

    system("pause");
    homeAdmin();
  }
}

void depositing(int i, string adminOrUser) //done
{
  int iAdmin, maxChoices = dbAccounts.size();
  double amount2Deposit;
  string leadTitle = "DEPOSITING";
  string specifyOptionsTitle = "Specify amount"
                               " to deposit between"
                               "\n500-50000";
  string askAccTitle = "Deposit to which account?";
  string optionsTitle = "...none for now";

  if (adminOrUser == "admin")
  {
    title(askAccTitle, displayAllAccounts());

    iAdmin = checkChoice(askAccTitle, "", maxChoices);
    iAdmin -= 1;
    i = iAdmin;
  }

  title(leadTitle, specifyOptionsTitle);
  amount2Deposit = getCheckAmount(i,
                                  "deposit",
                                  leadTitle,
                                  optionsTitle);

  dbAccounts[i].depositAmount(amount2Deposit);

  string strDeposit = std::to_string(amount2Deposit);
  strDeposit.erase(strDeposit.size() - 4, 4);
  string strBalance = std::to_string(dbAccounts[i].getAmount());
  strBalance.erase(strBalance.size() - 4, 4);

  title(leadTitle, "Deposit Success.\n"
                   "Amount Deposited: " +
                       strDeposit +
                       "\nCurrent Balance: " +
                       strBalance + " PHP");

  system("pause");

  if (adminOrUser == "admin") //return to home
    homeAdmin();
  else
    homeUser();
}

//Asks for a number. Returns number if valid
int checkChoice(string leadTitle,
                string optionsTitle,
                int maxChoices)
{
  int inputChoice;
  bool invalidChoice = true;

  while (invalidChoice)
  {
    cin >> inputChoice;
    if (cin.fail() ||
        inputChoice >
            maxChoices ||
        inputChoice == 0) //goes thru if input is invalid
    {
      title(leadTitle, optionsTitle);

      cout << "Invalid Input\n>";
      invalidChoice = true;
      cin.clear(); //resets cin.fail() to false
      cin.ignore(std::
                     numeric_limits<std::streamsize>::
                         max(),
                 '\n'); //erase input characters
    }
    else
      invalidChoice = false; //else exit loop & return input
  }

  return inputChoice;
}

string displayAllAccounts()
{
  string aContainer = "", lastLine = "\n";
  for (int j = 0; j < dbAccounts.size(); j++)
  {
    if (j == (dbAccounts.size() - 1))
      lastLine = "";
    aContainer.append(std::to_string(j + 1) + ". " +
                      dbAccounts[j].getFullName() +
                      lastLine);
  }
  return aContainer;
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

void title(std::string leadTitle, std::string optionsTitle)
{
  system("cls");
  cout << leadTitle
       << "\n-----------------------------------\n"
       << optionsTitle
       << "\n-----------------------------------\n>";
}

void editAccount()
{
  int dbElement, xC, xIndex, maxChoices = dbAccounts.size();
  string xfName, xlName;

  title("Which account?", displayAllAccounts());
  for (int i = 0; i < dbAccounts.size(); i++)
  {
    cout << i + 1 << ". " << dbAccounts[i].getFullName() << "\n";
  }
  dbElement = checkChoice("Which account?", "", maxChoices);
  xIndex = dbElement - 1;

  title("Change what?", "1. Name\n");
  cout << "-----------------------------------\n>";
  xC = checkChoice("Change what?", "1. Name\n", 1);
  cin.ignore();
  if (xC == 1)
  {
    title("Enter new first name: ", ">");
    xfName = firstLastName("first", "edit");
    title("Enter new last name: ", ">");
    xlName = firstLastName("last", "edit");

    dbAccounts[xIndex].changeName(xfName, xlName);
    summary(xIndex, "admin");
  }
  cout << "Invalid Input.\n";
  homeAdmin();
}

//returns amount if valid
double getCheckAmount(int i,
                      string depositOrWithdraw,
                      string leadTitle,
                      string optionsTitle)
{
  double balance = dbAccounts[i].getAmount();
  //remove scientific notation
  string strBalance = std::to_string(balance);
  strBalance.erase(strBalance.size() - 4, 4);
  double xCash;
  bool invalidInput = true;

  do
  {
    if (cin.fail()) //if input are letters
    {
      title(leadTitle,
            "Invalid amount."
            "\nAmount should be 500 - 50000");
      cin.clear(); //resets cin.fail() to false
      cin.ignore(
          std::numeric_limits<std::streamsize>::max(),
          '\n');
    }

    title(leadTitle + "\nEnter the amount to " +
              depositOrWithdraw,
          "Current Balance: " + strBalance);
    cin >> xCash; //Actual Input
    invalidInput = cin.fail();

    if (cin.fail() == false)
    {
      //DEPOSIT ERROR
      if (depositOrWithdraw == "deposit")
      {
        if (xCash > 50000 || xCash < 500)
        {
          title(leadTitle,
                "Invalid amount."
                "\nAmount should be 500 - 50000");
          system("pause");
          invalidInput = true;
        }
        else
          invalidInput = false;
      }
      //WITHDRAW ERROR
      else if (depositOrWithdraw == "withdraw")
      {
        if (xCash > balance && xCash > 0 || xCash <= 0)
        {
          title(leadTitle,
                "Invalid amount."
                "\nMaximum to draw: " +
                    strBalance +
                    " PHP");
          invalidInput = true;
        }
        else
          invalidInput = false;
      }
    }
    else
      invalidInput = cin.fail();
  } while (invalidInput);

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
