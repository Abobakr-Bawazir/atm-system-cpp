
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;
const string ClientsFileName = "Clients.txt";

void ShowMainMenu();
void QucikWithdrawScreen();
void GoBackToMainMenuATM();
void Login();
void ShowQuickWithdrawScreen();
void GoBackToMainMenuATM();

enum enQucikWithdraw { enWithdraw20 = 1, enWithdraw50 = 2, enWithdraw100 = 3, enWithdraw200 = 4, enWithdraw400 = 5, enWithdraw600 = 6, enWithdraw800 = 7, enWithdraw1000 = 8, enExit = 9 };
enum enMainMenueOption { enQuickWIthDraw = 1, enNoramlWithdraw = 2, Deposit = 3, CheckPalcane = 4, Logout = 5 };
struct sClient {

    string AccountNumber;
    string PinCode;
    string NameClinet;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;

};
sClient CurrentClient;

vector<string> SplitStrign(string S1, string Delim) {

    vector<string> sVector;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos) {

        sWord = S1.substr(0, pos);
        if (sWord != "") {

            sVector.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());


    }

    if (S1 != "") {
        sVector.push_back(S1);
    }
    return sVector;
}
sClient ConvertLineToRecord(string Line, string Sapareter = "#//#") {

    sClient Client;
    vector<string> vClientData;
    vClientData = SplitStrign(Line, Sapareter);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.NameClinet = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;

}
vector<sClient> LoadCleintsDataFromFile(string FileName) {


    vector<sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        string Line;
        sClient Client;

        while (getline(MyFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
bool FindClientByAccountNumberAndPIN(string AccountNumber, string PIN, sClient& CurrentClinet)
{
    vector <sClient> vUsers = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient C : vUsers)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PIN)
        {
            CurrentClinet = C;
            return true;
        }
    }
    return false;
}
string ConvertRecordToLine(sClient Client, string Seperater = "#//#") {

    string sClientRecord = "";

    sClientRecord += Client.AccountNumber + Seperater;
    sClientRecord += Client.PinCode + Seperater;
    sClientRecord += Client.NameClinet + Seperater;
    sClientRecord += Client.Phone + Seperater;
    sClientRecord += to_string(Client.AccountBalance) + Seperater;


    return sClientRecord;
}
bool FindClientByAccountNumber(string AccountNumber, sClient& Client)
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool LoadClinetInfo(string AccountNumber, string PIN) {

    if (FindClientByAccountNumberAndPIN(AccountNumber, PIN, CurrentClient))
        return true;
    else
        return false;
}
vector <sClient> SaveClientDataToFile(string FileName, vector<sClient> vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string DataLine;


        for (sClient C : vClients) {

            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

// Diposit
double ReadDepositAmount() {
    double Deposit;
    cout << "\nPlease enter deposit amount: ";
    cin >> Deposit;
    return Deposit;
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, int Amount, vector <sClient>& vClients) {

    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction y / n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                CurrentClient.AccountBalance = C.AccountBalance;
                SaveClientDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}
void ShowDepositScreen() {
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClients);
}

// Normal withdrawal 
int ReadWithdrawAmount() {
    int Withdrew;
    cout << "\nEnter amount multiple of 5's: ";
    cin >> Withdrew;
    return Withdrew;
}
bool isMultipleOf5(int Amount) {
    if (Amount % 5 == 0)
        return true;
    else
        return false;

}
void ShowWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\tNormal Withdrawal Screen\n";
    cout << "===========================================\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);


    int Amount = 0;
    do
    {
        cout << "\nEnter an amount multiple of 5's: ";
        cin >> Amount;

    } while (!isMultipleOf5(Amount));




    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
        cout << "\nEnter amount multiple of 5's: ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
}

// qucik Withdrawal 
int ReadOptionQucikWithdraw() {
    int QucikWithdraw = 0;
    cin >> QucikWithdraw;
    return QucikWithdraw;
}
void QucikWithDrawal(int Amount)
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);



    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, make another choice.\n";

        cin >> ws, Amount;
    }
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
}

short getQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9)//exit
        return;

    short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;

}
short ReadQuicWithdrawOption() {

    short Choice = 0;
    while (Choice < 1 || Choice > 9) {
        cout << "Choose what to withdraw from [1] to [8] : ";
        cin >> Choice;
    }
    return Choice;
}
void ShowQuickWithdrawScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t[2] 50\n";
    cout << "\t[3] 100\t[4] 200\n";
    cout << "\t[5] 400\t[6] 600\n";
    cout << "\t[7] 800\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your balance is: " << CurrentClient.AccountBalance << endl;
    PerfromQuickWithdrawOption(ReadQuicWithdrawOption());

}

// Check Balance

void GoBackToMainMenuATM() {

    cout << "\n\nPress any key to go back to Main Menu ATM...";
    system("pause");
    ShowMainMenu();
}
void CheckBalanceScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\tATM Main Menu Screen\n";
    cout << "===========================================\n";
}
void ShowCheckBalance() {
    CheckBalanceScreen();
    cout << "\nYour balance is: " << CurrentClient.AccountBalance;
}









short ReadMainMenuOption() {
    cout << "Choose what do you want to do? [1 to 5]? ";

    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerformATMMenu(enMainMenueOption ATMOprins) {

    switch (ATMOprins) {

    case enMainMenueOption::enQuickWIthDraw:
        QucikWithdrawScreen();
        break;

    case enMainMenueOption::enNoramlWithdraw:
        ShowWithDrawScreen();
        GoBackToMainMenuATM();
        break;

    case enMainMenueOption::Deposit:
        ShowDepositScreen();
        GoBackToMainMenuATM();
        break;

    case enMainMenueOption::CheckPalcane:
        ShowCheckBalance();
        GoBackToMainMenuATM();
        break;

    case enMainMenueOption::Logout:
        system("cls");
        Login();
        break;

    }
}

void ShowMainMenu() {

    system("cls");
    cout << "===========================================\n";
    cout << "\tATM Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Diposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerformATMMenu((enMainMenueOption)ReadMainMenuOption());
}

void LoginScreen() {
    cout << "\n---------------------------------------\n";
    cout << "\tLogin Screen\n";
    cout << "---------------------------------------\n";
}

void Login() {

    bool LoginFaild = false;

    string AccountNumber, PIN;

    do
    {
        system("cls");
        LoginScreen();

        if (LoginFaild) {
            cout << "Invalid AccountNumber/PIN!\n";
        }

        cout << "Enter AccoutnNumber? ";
        cin >> AccountNumber;
        cout << "Enter PIN? ";
        cin >> PIN;
        LoginFaild = !LoadClinetInfo(AccountNumber, PIN);

    } while (LoginFaild);

    ShowMainMenu();
}

int main()
{
    Login();

    system("pause>0");
    return 0;
}



