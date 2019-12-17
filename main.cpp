//Description: This program will perform most of the
//things that a normal bank account system would do
//Title: Simple Account App
//Programmers: Emmanuel Valdueza, Jake Ogsimer, Mark Perez

#include <iostream>
#include <string>
#include <vector>
#include <limits>  //gets cin size to handle bad input
#include <iomanip> //removes scientific notation
#include <fstream> //handles files

using std::cin;
using std::cout;
using std::string;

void depositing(int, string), withdrawing(int, string);
void summaryAdmin(), summary(int, string); //takes in index
void title(std::string pTitle, std::string pOptions);
void createAccount();
void home(), homeUser(string, string, int), homeAdmin(string);
void deleteAccount();
bool isItANum(bool didItFail);
double checkGetAccAmount(int, string, string, string);
void editAccount();
string displayAllAccounts();
int checkChoice(string, string, int);
bool isNameValid(string);
string checkName(string, string);
bool isPassInvalid(string);
double getMoneyInput(string, string, string, double, double);
bool isItAValidNumber(bool);
int pauseScreen(int);
void writeDBToFile(string, int);
void readDBFromFile();
string encryptData(string);

class Account
{
  //private variables
  string firstName,
      lastName,
      password,
      fullName,
      encryptedBal;
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

  //2nd constructor (encryption purposes)
  Account(string fName,
          string lName,
          string xPass,
          string eBal)
      : firstName(fName),
        lastName(lName),
        password(xPass),
        encryptedBal(eBal)
  {
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
  string getPassword()
  {
    return password;
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

std::vector<Account> dbAccounts; //objects of accounts
bool userLoggedIn = false;
string adminPassword = "123";
string userAccInput; //compare input with db
std::vector<std::vector<int>> decryptDBKeys;
std::vector<int> decryptKey;
string strTempDB;
int tempOpenIteration = 0; //delete Later
int tempCloseIteration = 0;

int main()
{
  readDBFromFile(); //loads database

  home();

  pauseScreen(0);
  return 0;
}

void readDBFromFile()
{
  std::ifstream dbFile; //create object
  std::ifstream keyFile;

  string frstName, scndName, passWord, bal;

  dbFile.open("dbAccounts.txt"); //open files
  keyFile.open("key.txt");

  if (dbFile.fail() || keyFile.fail()) //checks if file exists
  {
    title("ACCOUNT CREATION",
          "No accounts exist."
          "\nWe shall create a new one.");
    cout << "Press enter to continue...";
    pauseScreen(0);
    dbFile.close();
    keyFile.close();
    createAccount(); //close everything, redirect to new user
  }

  //Decrypt & store in object
  while (dbFile.eof() == false)
  {
    int keyIteration = 0;
    int iteration = 0;

    dbFile >> strTempDB; //get iteration

    while (keyFile.eof() == false) //Decrypt
    {
      keyFile >> keyIteration;

      strTempDB[iteration] -= keyIteration;

      iteration++;
    }

    //Seperate items
    while (strTempDB.size() > 0)
    {
      frstName = strTempDB.substr(0, strTempDB.find(","));
      strTempDB.erase(0, frstName.size() + 1);
      scndName = strTempDB.substr(0, strTempDB.find(","));
      strTempDB.erase(0, scndName.size() + 1);
      passWord = strTempDB.substr(0, strTempDB.find(","));
      strTempDB.erase(0, passWord.size() + 1);
      bal = strTempDB.substr(0, strTempDB.find(","));
      strTempDB.erase(0, bal.size() + 1);

      //Object creation
      dbAccounts.push_back(Account(frstName,
                                   scndName,
                                   passWord,
                                   std::stod(bal)));
    }
  }
  //close files
  dbFile.close();
  keyFile.close();
}

void writeDBToFile(string exitOrNot, int userIndex)
{
  std::ofstream dbFile; //create object
  std::ofstream keyFile;
  string temporaryDB; //temporary
  string seperator = ",";

  dbFile.open("dbAccounts.txt");
  keyFile.open("key.txt");

  if (dbFile.fail() || keyFile.fail())
  {
    cout << "Error reading file\n";
    pauseScreen(0);
    exit(1);
  }

  //store entire db to temporary string vector
  for (int i = 0; i < dbAccounts.size(); i++)
  {
    if (i + 1 == dbAccounts.size())
      seperator = "";
    temporaryDB.append(dbAccounts[i].getFirstName() + "," +
                       dbAccounts[i].getLastName() + "," +
                       dbAccounts[i].getPassword() + "," +
                       std::to_string(dbAccounts[i].getAmount()) +
                       seperator);
  }

  //encrypt data & send all accounts
  temporaryDB = encryptData(temporaryDB);

  //save encrypted data to file
  dbFile << temporaryDB;

  //save decrypt keys to file
  if (decryptDBKeys.size() != 0)
  {
    string spacer = " ";
    for (int i = 0; i < decryptDBKeys.size(); i++)
    {
      for (int j = 0; j < decryptDBKeys[i].size(); j++)
      {
        if (i + 1 == decryptDBKeys.size() && j + 1 == decryptDBKeys[i].size())
          spacer = "";
        keyFile << std::to_string(decryptDBKeys[i][j]) + spacer;
      }
    }
  }

  dbFile.close();  //close file
  keyFile.close(); //close keys file

  if (exitOrNot == "exit")
  {
    title("THANK YOU", "Bye.");
    cout << "Press any key to exit";
    pauseScreen(0);
    exit(1);
  }
  else if (exitOrNot == "admin")
    homeAdmin("passwordOff");
  else if (exitOrNot == "user")
    homeUser("passwordOff", "", userIndex);

  //fallThrough. Will continue to next line
}

//encrypts all accoounts
string encryptData(string unencryptedData)
{
  srand(time(NULL)); //prevents constant random by seeding with time
  string encryptedData = "";

  decryptKey.clear(); //make sure there are no left over keys
  decryptDBKeys.clear();

  for (int i = 0; i < unencryptedData.size(); i++)
  {                                                          //fstream reads spaces as new item
    int randomNum = 1 + (rand() % 5);                        //get random number
    decryptKey.push_back(randomNum);                         //append to keyfile
    encryptedData.push_back(unencryptedData[i] + randomNum); //append to
  }

  decryptDBKeys.push_back(decryptKey); //send sequence to db

  return encryptedData;
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
    homeUser("passwordOn", "", 0);
  else if (x == 2)
    homeAdmin("passwordOn");
}

void homeAdmin(string passwordOnOff)
{
  int option, maxChoices = 8;
  bool locked = true;
  string leadTitle = "Welcome, Admin.\n"
                     "Please select one"
                     " of the following:",
         optionsTitle = "1. Summary\n2. Deposit"
                        "\n3. Withdraw\n4. Create\n"
                        "5. Delete\n6. Edit Account"
                        "\n7. Switch to user"
                        "\n8. Exit",
         inputPassword;

  if (passwordOnOff == "passwordOn")
  {
    title("Welcome, Admin.", "Enter password to proceed");
    do
    {
      getline(cin, inputPassword);
      if (inputPassword == adminPassword)
        locked = false;
      else
        title("Welcome, Admin.", "Invalid password.\n"
                                 "Please enter correct"
                                 " password to\nproceed"
                                 " or type \"home\" to go"
                                 " back.");
      if (inputPassword == "home")
        home();
    } while (locked);
  }
  else
    locked = false;

  if (locked == false)
  {
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
      homeUser("passwordOn", "", 0);
      break;
    case 8:
      exit(1);
      break;
    default:
      break;
    }
  }
}

void homeUser(string passwordOnOff, string chosenAccount, int accIndex)
{
  int option, maxChoices = 5;
  bool keepLooping = false, innerKeepLooping = false;
  string inputPassword;

  if (passwordOnOff == "passwordOn")
  {
    do
    {
      if (keepLooping == false)
        title("USER ACCOUNT LOGIN",
              "Enter full name of your account");
      else
        title("USER ACCOUNT LOGIN",
              "Account not found."
              "\nTry again or type \"home\" to go back");

      std::getline(cin, userAccInput);

      for (int i = 0; i < dbAccounts.size(); i++) //acc index finder
      {
        userLoggedIn = userAccInput == dbAccounts[i].getFullName();
        if (userLoggedIn)
        {
          accIndex = i;
          chosenAccount = dbAccounts[i].getFullName();
          do
          {
            if (innerKeepLooping == false)
              title("Welcome, " + chosenAccount,
                    "Enter your password"
                    " to continue");
            else
              title("Welcome, " + chosenAccount,
                    "Entered password is invalid.\n"
                    "Try again or type \"home\" to go back");
            std::getline(cin, inputPassword);

            if (inputPassword == dbAccounts[i].getPassword())
              innerKeepLooping = false;
            else
              innerKeepLooping = true;

          } while (innerKeepLooping);

          keepLooping = false;
          break;
        }
        else
          keepLooping = true;
      }
      //Go back home
      if (userAccInput == "home")
        home();
    } while (keepLooping);
  }

  string wlcTitle = "Welcome, " + chosenAccount +
                    ".\nPlease select one of the following:";
  string optionsTitle = "1. Summary\n2. Deposit\n"
                        "3. Withdraw\n4. Switch to admin"
                        "\n5. Exit";

  //Ask get choice
  title(wlcTitle, optionsTitle);
  option = checkChoice(wlcTitle, optionsTitle, maxChoices);

  if (option == 1)
    summary(accIndex, "user");
  if (option == 2)
    depositing(accIndex, "user");
  if (option == 3)
    withdrawing(accIndex, "user");
  if (option == 4)
  {
    userLoggedIn = false;
    home();
  }
  if (option == 5)
    exit(1);
}

void createAccount()
{
  string firstName, lastName, password, fullName;
  double initialAmount = 0;
  bool invalidPassword = false, loopNameGet = false;

  do
  { //get names
    firstName = checkName("first", "new");
    lastName = checkName("last", "new");
    fullName = firstName + " " + lastName;

    //and checks database for duplicates
    for (int i = 0; i < dbAccounts.size(); i++)
    {
      if (fullName == dbAccounts[i].getFullName())
      {
        loopNameGet = true;
        title("CREATING", "Name already exist."
                          "\nTry another name.");
        cout << "Press enter to start over";
        pauseScreen(0);
      }
      else
        loopNameGet = false;
    }
  } while (loopNameGet);

  do //Get Initial Deposit
  {
    if (initialAmount > 50000 || initialAmount < 500)
      cout << "Please enter 50000 or less:\n";
    else
      title("Initial deposit: ", "Amount must be between"
                                 "\n500 - 50000");

    initialAmount = getMoneyInput("CREATING",
                                  "Amount must be between"
                                  "\n500 - 50000",
                                  "INVALID amount."
                                  " Should be between"
                                  "\n500 - 50000",
                                  500, 50000);
  } while (initialAmount > 50000 || initialAmount < 500);

  do //Get Password
  {
    if (invalidPassword)
      title("CREATING", "Invalid Password.\n"
                        "Must contain no spaces");
    else
      title("CREATING", "Enter new password");
    getline(cin, password);
    invalidPassword = isPassInvalid(password);
  } while (invalidPassword);

  dbAccounts.push_back(Account( //Add Account Details
      firstName,
      lastName,
      password,
      initialAmount));

  writeDBToFile("fallThrough", 0);
  title("CREATING", "Success.");
  cout << "Press enter to continue...";
  pauseScreen(0);
  homeAdmin("passwordOff");
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

//return name if valid
string checkName(string firstOrLast, string newOrEdit)
{
  string newName, leadTitle, optionsTitle;

  if (newOrEdit == "edit")
  {
    leadTitle = "EDITING";
    optionsTitle = "Your new " + firstOrLast + " name";
  }
  else
  {
    leadTitle = "CREATING";
    optionsTitle = "Your " + firstOrLast + " name";
  }

  title(leadTitle, optionsTitle);
  std::getline(cin, newName);

  while (isNameValid(newName) == false)
  {
    title(leadTitle, "Invalid name.\n"
                     "Name must not contain spaces\n"
                     "or special characters");
    std::getline(cin, newName);
  }

  return newName;
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

void summary(int index, string adminOrUser) //done
{
  int i = index;
  string actualAmount = std::to_string(dbAccounts[i].getAmount());
  //removes the last 4 digits
  actualAmount.erase(actualAmount.size() - 4, 4);

  string optionsTitle = "Account Name: " +
                        dbAccounts[i].getFullName() +
                        "\nAmount: " +
                        actualAmount +
                        " PHP";

  title("Account Summary", optionsTitle);
  cout << "Press enter to continue...";
  pauseScreen(0);

  if (adminOrUser == "admin")
    homeAdmin("passwordOff");
  else
    homeUser("passwordOff", dbAccounts[i].getFullName(), index);
}

void withdrawing(int index, string adminOrUser)
{
  int i = index;
  bool keepLooping = true;
  double withDrawAmount = 0;
  int maxDrawChoices = 5;
  string strCurrentBalance = std::to_string(dbAccounts[i].getAmount());
  strCurrentBalance.erase(strCurrentBalance.size() - 4, 4);
  string leadTitle = "WITHDRAWING";
  string adminTitle = "\nWithdraw from which account?";
  string askTitle = "\nHow much would you like to withdraw?";
  string optionsTitle = "1. 500\n2. 1500"
                        "\n3. 3000\n4. 5000\n5. Custom";
  string invalidTitle = "Invalid Input.\n"
                        "Amount should be less than " +
                        strCurrentBalance + " PHP";

  if (adminOrUser == "admin") //if admin, overwrite variables
  {                           //show which accounts
    int maxAccChoices = dbAccounts.size();
    maxDrawChoices = 5;

    //Ask account index
    title(leadTitle + adminTitle, displayAllAccounts());
    i = checkChoice(leadTitle + adminTitle,
                    displayAllAccounts(),
                    maxAccChoices) -
        1;
  }

  //Ask Preset Amount, admin & user
  do
  {
    title(leadTitle + askTitle, optionsTitle);
    int choice = checkChoice(leadTitle + askTitle,
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
      withDrawAmount = getMoneyInput(leadTitle,
                                     "Enter the custom "
                                     "amount to draw",
                                     invalidTitle,
                                     0,
                                     dbAccounts[i].getAmount());
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

  //convert doubles to strings & display success
  string postBalance = std::to_string(dbAccounts[i].getAmount());
  postBalance.erase(postBalance.size() - 4, 4);
  string strDrawAmount = std::to_string(withDrawAmount);
  strDrawAmount.erase(strDrawAmount.size() - 4, 4);

  writeDBToFile("fallThrough", 0);
  title(leadTitle, "Success.\n"
                   "Amount Drawn: " +
                       strDrawAmount +
                       "\nCurrent Balance: " +
                       postBalance + " PHP");

  cout << "Press enter to continue...";
  pauseScreen(0);
  if (adminOrUser == "admin")
    homeAdmin("passwordOnOff");
  homeUser("passwordOnOff", dbAccounts[i].getFullName(), i);
}

void depositing(int i, string adminOrUser) //done
{
  int iAdmin, maxChoices = dbAccounts.size();
  double amount2Deposit;
  string leadTitle = "DEPOSITING";
  string specifyOptionsTitle = "Specify amount"
                               " to deposit";
  string askAccTitle = "Deposit to which account?";
  string optionsTitle = "...none for now";

  if (adminOrUser == "admin")
  {
    title(askAccTitle, displayAllAccounts());

    iAdmin = checkChoice(askAccTitle, displayAllAccounts(), maxChoices);
    iAdmin -= 1;
    i = iAdmin;
  }
  amount2Deposit = getMoneyInput(leadTitle,
                                 specifyOptionsTitle,
                                 "Invailid Amount\n"
                                 "Must be between "
                                 "500 - 50000",
                                 500, 50000);
  dbAccounts[i].depositAmount(amount2Deposit);

  //convert to string for display
  string strDeposit = std::to_string(amount2Deposit);
  strDeposit.erase(strDeposit.size() - 4, 4);
  string strBalance = std::to_string(dbAccounts[i].getAmount());
  strBalance.erase(strBalance.size() - 4, 4);

  title(leadTitle, "Deposit Success.\n"
                   "Amount Deposited: " +
                       strDeposit +
                       "\nCurrent Balance: " +
                       strBalance + " PHP");
  writeDBToFile("fallThrough", 0);
  cout << "Press enter to continue...";
  pauseScreen(0);

  if (adminOrUser == "admin")
    homeAdmin("passwordOff");
  else
    homeUser("passwordOff", dbAccounts[i].getFullName(), i);
}

//Asks for a number. Returns number if valid
int checkChoice(string leadTitle,
                string optionsTitle,
                int maxChoices)
{
  int inputChoice;
  bool keepLooping = true;

  do
  {
    cin >> inputChoice;

    if (isItAValidNumber(cin.fail()))
    {
      keepLooping = false;
    }
    else //loop if not a number
      keepLooping = true;
    //loop if not within limit
    if (inputChoice > maxChoices ||
        inputChoice <= 0)
    {
      title(leadTitle, optionsTitle);
      cout << "Invalid Input\n>";
      keepLooping = true;
    }
  } while (keepLooping);

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

void searchAcc() //feature coming soon
{
}

void deleteAccount()
{
  string leadTitle = "ACCOUNT DELETION";
  int choice, maxChoices = dbAccounts.size();
  title(leadTitle + "\nWhat account would you like to delete?",
        displayAllAccounts());
  choice = checkChoice(leadTitle +
                           "\nWhat account would you like to delete?",
                       displayAllAccounts(), maxChoices) -
           1;

  dbAccounts.erase(dbAccounts.begin() + choice);

  title(leadTitle, "Success.");
  cout << "Press enter to continue...";
  pauseScreen(0);

  if (dbAccounts.size() == 0)
  {
    title("NO ACCOUNTS LEFT IN DATABASE",
          "We will create a new account.");
    cout << "Press enter to continue...";
    pauseScreen(0);
    createAccount();
  }
  writeDBToFile("fallThrough", 0);
  homeAdmin("passwordOff");
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
  int dbElement, xOption, xIndex, maxChoices = dbAccounts.size();
  string xfName, xlName, leadTitle = "EDITING";

  title(leadTitle + "\nWhich account?", displayAllAccounts());
  dbElement = checkChoice(leadTitle + "\nWhich account?",
                          displayAllAccounts(), maxChoices);
  xIndex = dbElement - 1;

  title(leadTitle + "\nChange what?", "1. Name");
  xOption = checkChoice(leadTitle + "\nChange what?", "1. Name", 1);

  if (xOption == 1)
  {
    title("Enter new first name: ", ">");
    xfName = checkName("first", "edit");
    title("Enter new last name: ", ">");
    xlName = checkName("last", "edit");

    dbAccounts[xIndex].changeName(xfName, xlName);
    writeDBToFile("fallThrough", 0);
    summary(xIndex, "admin");
  }

  //delete later... might never go through
  cout << "Invalid Input.\n";
  homeAdmin("passwordOff");
}

double getMoneyInput(string leadTitle,
                     string optionsTitle,
                     string invalidTitle,
                     double min,
                     double max)
{
  double moneyInput;
  bool keepLooping = false;

  do
  {
    if (keepLooping == false)
      title(leadTitle, optionsTitle);
    else
      title(leadTitle, invalidTitle);

    cin >> moneyInput;

    //check if it is a number
    if (isItAValidNumber(cin.fail()))
      keepLooping = false;
    else
      keepLooping = true;

    //check if it is within min max
    if (moneyInput < min || moneyInput > max)
      keepLooping = true;

  } while (keepLooping);

  return moneyInput;
}

//might delete this function
double checkGetAccAmount(int i,
                         string depositOrWithdraw,
                         string leadTitle,
                         string optionsTitle)
{
  string invalidTitle = "Wrong amount";
  double balance = dbAccounts[i].getAmount();
  //remove scientific notation
  string strBalance = std::to_string(balance);
  strBalance.erase(strBalance.size() - 4, 4);
  double xCash;
  bool invalidInput = true;

  xCash = getMoneyInput(leadTitle, optionsTitle, invalidTitle, 500, 50000);

  return xCash;
}

bool isItAValidNumber(bool didItFail) //returns true if valid
{
  if (didItFail)
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
      letters and temporaryDB after will be discarded.
    */
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
  }
}

int pauseScreen(int returnNothing)
{
  //we can also use cin.ignore to wait until enter is pressed
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return returnNothing;
}