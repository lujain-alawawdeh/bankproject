#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string filename = "bankclient.txt";
const string userfilename = "users.txt";

struct sclient {
    string name;
    string pincode;
    string accountnumber;
    double accountbalance;
    string phone;
    bool mark = false;
};

struct suser {
    string name;
    string password;
    int permission;
    bool mark = false;
};

suser usercurrent;

void ShowMainMenue();
void showtransactionmenu();
short readtransactionmenu();
void showtotalbalance();
void ShowManageUsersMenu();
void login();
void showdenidscreen();

vector<string> splitstring(string s, string seperator = "##") {
    short pos = 0;
    string word = "";
    vector<string> vclient;

    while ((pos = s.find(seperator)) != string::npos) {
        word = s.substr(0, pos);
        if (word != "") {
            vclient.push_back(word);
        }
        s.erase(0, pos + seperator.length());
    }

    if (s != "")
        vclient.push_back(s);

    return vclient;
}

string converttoline(sclient client, string sep = "##") {
    string oneline = "";
    oneline += client.name + sep;
    oneline += client.pincode + sep;
    oneline += client.accountnumber + sep;
    oneline += client.phone + sep;
    oneline += to_string(client.accountbalance);
    return oneline;
}

sclient converttorecord(string line, string sep = "##") {
    vector<string> vclient;
    sclient client;

    vclient = splitstring(line, sep);

    if (vclient.size() >= 5) {
        client.name = vclient[0];
        client.pincode = vclient[1];
        client.accountnumber = vclient[2];
        client.phone = vclient[3];
        client.accountbalance = stod(vclient[4]);
    }

    return client;
}

bool clientexistbyaccountnumber(string accountnumber, string filename) {
    fstream myfile;
    myfile.open(filename, ios::in);

    if (myfile.is_open()) {
        string line;
        sclient client;

        while (getline(myfile, line)) {
            client = converttorecord(line);

            if (client.accountnumber == accountnumber) {
                myfile.close();
                return true;
            }
        }

        myfile.close();
    }

    return false;
}

sclient readclient() {
    sclient client;

    cout << "please enter account number\n";
    getline(cin >> ws, client.accountnumber);

    while (clientexistbyaccountnumber(client.accountnumber, filename)) {
        cout << "Client with [" << client.accountnumber << "] already exists, Enter another Account Number: ";
        getline(cin, client.accountnumber);
    }

    cout << "please enter name\n";
    getline(cin, client.name);

    cout << "please enter pincode\n";
    getline(cin, client.pincode);

    cout << "please enter phone \n";
    getline(cin, client.phone);

    cout << "please enter account balance \n";
    cin >> client.accountbalance;

    return client;
}

vector<sclient> loaddatatofile(string filename) {
    fstream myfile;
    vector<sclient> vclient;

    myfile.open(filename, ios::in);

    if (myfile.is_open()) {
        string line;
        sclient client;

        while (getline(myfile, line)) {
            if (line != "") {
                client = converttorecord(line);
                vclient.push_back(client);
            }
        }

        myfile.close();
    }

    return vclient;
}

void printclientrecord(sclient client) {
    cout << "| " << setw(15) << left << client.accountnumber;
    cout << "| " << setw(20) << left << client.name;
    cout << "| " << setw(10) << left << client.pincode;
    cout << "| " << setw(15) << left << client.phone;
    cout << "| " << setw(12) << left << client.accountbalance << "\n";
}

void printclientrecordbalance(sclient client) {
    cout << "| " << setw(15) << left << client.accountnumber;
    cout << "| " << setw(20) << left << client.name;
    cout << "| " << setw(12) << left << client.accountbalance << "\n";
}

void showclientscreen() {
    vector<sclient> vclient = loaddatatofile(filename);

    cout << "\n \t\t Client List (" << vclient.size() << ") Client(s).\n";
    cout << "___________________________________________________________________________________\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(20) << "Client Name";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(15) << "Phone";
    cout << "| " << left << setw(12) << "Balance" << "\n";
    cout << "___________________________________________________________________________________\n";

    if (vclient.size() == 0) {
        cout << "\t\t\t\tNo Clients Available In the System!\n";
    }
    else {
        for (sclient c : vclient) {
            printclientrecord(c);
        }
    }

    cout << "___________________________________________________________________________________\n";
}

void printclientcard(sclient client) {
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number: " << client.accountnumber;
    cout << "\nPin Code     : " << client.pincode;
    cout << "\nName         : " << client.name;
    cout << "\nPhone        : " << client.phone;
    cout << "\nAccount Balance: " << client.accountbalance;
    cout << "\n-----------------------------------\n";
}

bool findbyaccountnumber(string acc, vector<sclient> vclient, sclient& client) {
    for (sclient& c : vclient) {
        if (c.accountnumber == acc) {
            client = c;
            return true;
        }
    }

    return false;
}

sclient changeclient(string accountnumber) {
    sclient client;

    client.accountnumber = accountnumber;

    cout << "please enter name\n";
    getline(cin >> ws, client.name);

    cout << "please enter pincode \n";
    getline(cin, client.pincode);

    cout << "please enter phone\n";
    getline(cin, client.phone);

    cout << "please enter account balance \n";
    cin >> client.accountbalance;

    return client;
}

bool markfordeletebyaccountnumber(string account, vector<sclient>& vclient) {
    for (sclient& c : vclient) {
        if (c.accountnumber == account) {
            c.mark = true;
            return true;
        }
    }

    return false;
}

vector<sclient> saveclient(string filename, vector<sclient> vclient) {
    fstream myfile;
    myfile.open(filename, ios::out);

    string line;

    if (myfile.is_open()) {
        for (sclient c : vclient) {
            if (c.mark == false) {
                line = converttoline(c);
                myfile << line << endl;
            }
        }

        myfile.close();
    }

    return vclient;
}

void adddatatofile(string filename, string line) {
    fstream myfile;
    myfile.open(filename, ios::out | ios::app);

    if (myfile.is_open()) {
        myfile << line << endl;
        myfile.close();
    }
}

void addnewclient() {
    sclient client;
    client = readclient();
    adddatatofile(filename, converttoline(client));
}

void addnewclients() {
    char more = 'y';

    do {
        system("cls");

        cout << "Adding New Client:\n\n";

        addnewclient();

        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> more;

    } while (more == 'y' || more == 'Y');
}

bool deletebyaccountnumber(string accountnumber, vector<sclient>& vclient) {
    sclient client;
    char answer = 'n';

    if (findbyaccountnumber(accountnumber, vclient, client)) {
        printclientcard(client);

        cout << "\nAre you sure you want delete this client? y/n? ";
        cin >> answer;

        if (answer == 'y' || answer == 'Y') {
            markfordeletebyaccountnumber(accountnumber, vclient);
            saveclient(filename, vclient);
            vclient = loaddatatofile(filename);

            cout << "\nClient Deleted Successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << accountnumber << ") is Not Found!\n";
        return false;
    }

    return false;
}

bool updateclientbyaccountnumber(string acc, vector<sclient>& vclient) {
    sclient client;
    char answer = 'n';

    if (findbyaccountnumber(acc, vclient, client)) {
        printclientcard(client);

        cout << "\nAre you sure you want update this client? y/n? ";
        cin >> answer;

        if (answer == 'y' || answer == 'Y') {
            for (sclient& c : vclient) {
                if (c.accountnumber == acc) {
                    c = changeclient(acc);
                    break;
                }
            }

            saveclient(filename, vclient);

            cout << "\nClient Updated Successfully.\n";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << acc << ") is Not Found!\n";
        return false;
    }

    return false;
}

string readaccountnumber() {
    string acc;

    cout << "Please enter Account Number? ";
    cin >> acc;

    return acc;
}

void showdeleteclient() {
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-----------------------------------\n";

    vector<sclient> vclient = loaddatatofile(filename);
    string accountnum = readaccountnumber();

    deletebyaccountnumber(accountnum, vclient);
}

void showupdateclient() {
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen\n";
    cout << "-----------------------------------\n";

    vector<sclient> vclient = loaddatatofile(filename);
    string accountnum = readaccountnumber();

    updateclientbyaccountnumber(accountnum, vclient);
}

void showaddclient() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "-----------------------------------\n";

    addnewclients();
}

void showfindclient() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "-----------------------------------\n";

    vector<sclient> vclient = loaddatatofile(filename);
    sclient client;
    string accnum = readaccountnumber();

    if (findbyaccountnumber(accnum, vclient, client)) {
        printclientcard(client);
    }
    else {
        cout << "\nClient with Account Number [" << accnum << "] is not found!\n";
    }
}

void showendscreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

enum menuoption {
    elistclient = 1,
    eaddclient = 2,
    edeleteclient = 3,
    eupdateclient = 4,
    efindclient = 5,
    etransaction = 6,
    emanageusers = 7,
    eexist = 8
};

bool checkpermission(int permission) {
    if (usercurrent.permission == -1)
        return true;

    if ((usercurrent.permission & permission) == permission) {
        return true;
    }
    else {
        return false;
    }
}

void gotomenu() {
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenue();
}

short readptiononmenu() {
    short num;

    do {
        cout << "Choose what do you want to do? [1 to 8]? ";
        cin >> num;

        if (cin.fail() || num < 1 || num > 8) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input, please enter a number between 1 and 8.\n";
        }
        else {
            break;
        }

    } while (true);

    return num;
}

void mainmenu(menuoption eoption) {
    system("cls");

    switch (eoption) {
    case menuoption::elistclient:
        if (!checkpermission(1)) {
            showdenidscreen();
        }
        else {
            showclientscreen();
        }

        gotomenu();
        break;

    case menuoption::eaddclient:
        if (!checkpermission(2)) {
            showdenidscreen();
        }
        else {
            showaddclient();
        }

        gotomenu();
        break;

    case menuoption::edeleteclient:
        if (!checkpermission(4)) {
            showdenidscreen();
        }
        else {
            showdeleteclient();
        }

        gotomenu();
        break;

    case menuoption::eupdateclient:
        if (!checkpermission(8)) {
            showdenidscreen();
        }
        else {
            showupdateclient();
        }

        gotomenu();
        break;

    case menuoption::efindclient:
        if (!checkpermission(16)) {
            showdenidscreen();
        }
        else {
            showfindclient();
        }

        gotomenu();
        break;

    case menuoption::etransaction:
        if (!checkpermission(32)) {
            showdenidscreen();
            gotomenu();
        }
        else {
            showtransactionmenu();
        }

        break;

    case menuoption::emanageusers:
        if (!checkpermission(64)) {
            showdenidscreen();
            gotomenu();
        }
        else {
            ShowManageUsersMenu();
        }

        break;

    case menuoption::eexist:
        showendscreen();
        break;
    }
}

void ShowMainMenue() {
    system("cls");

    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Exit.\n";
    cout << "===========================================\n";

    mainmenu((menuoption)readptiononmenu());
}

bool depositountnumber(string acc, double amount, vector<sclient>& vclient) {
    char answer = 'n';

    cout << "are you sure you want to perform this transaction? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y') {
        for (sclient& c : vclient) {
            if (c.accountnumber == acc) {
                c.accountbalance += amount;

                saveclient(filename, vclient);

                cout << "your balance is " << c.accountbalance << "\n";
                return true;
            }
        }

        cout << "client account number " << acc << " not found \n";
    }

    return false;
}

void showdeposit() {
    cout << "\n _________________________\n";
    cout << "\tDeposit Screen\n";
    cout << " _________________________\n";

    sclient client;
    vector<sclient> vclient = loaddatatofile(filename);

    string accountnum = readaccountnumber();

    while (!findbyaccountnumber(accountnum, vclient, client)) {
        cout << "client account number " << accountnum << " not found \n";
        accountnum = readaccountnumber();
    }

    printclientcard(client);

    double amount = 0;

    cout << "please enter amount \n";
    cin >> amount;

    depositountnumber(accountnum, amount, vclient);
}

void showdraw() {
    cout << "\n _________________________\n";
    cout << "\tDraw Screen\n";
    cout << " _________________________\n";

    sclient client;
    vector<sclient> vclient = loaddatatofile(filename);

    string accountnum = readaccountnumber();

    while (!findbyaccountnumber(accountnum, vclient, client)) {
        cout << "client account number " << accountnum << " not found \n";
        accountnum = readaccountnumber();
    }

    printclientcard(client);

    double amount = 0;

    cout << "please enter amount \n";
    cin >> amount;

    while (amount > client.accountbalance) {
        cout << "Amount Exceeds the balance, you can withdraw up to : "
            << client.accountbalance << endl;

        cout << "Please enter another amount? ";
        cin >> amount;
    }

    depositountnumber(accountnum, -amount, vclient);
}

void showtotalbalance() {
    vector<sclient> vclient = loaddatatofile(filename);
    double totalbalance = 0;

    cout << "\n\t\tBalances List (" << vclient.size() << ") Client(s).\n";
    cout << "_____________________________________________________\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(20) << "Client Name";
    cout << "| " << left << setw(12) << "Balance" << "\n";
    cout << "_____________________________________________________\n";

    if (vclient.size() == 0) {
        cout << "\t\tNo Clients Available In the System!\n";
    }
    else {
        for (sclient c : vclient) {
            printclientrecordbalance(c);
            totalbalance += c.accountbalance;
        }
    }

    cout << "_____________________________________________________\n";
    cout << "\t\t Total Balances = " << totalbalance << "\n";
}

void showtotalbalancescreen() {
    showtotalbalance();
}

enum etransactionoption {
    edepost = 1,
    edraw = 2,
    etotal = 3,
    exist = 4
};

void gobacktotransaction() {
    cout << "\n\nPress any key to go back to transaction menu...\n";
    system("pause>0");
    showtransactionmenu();
}

void performtransaction(etransactionoption transaction) {
    switch (transaction) {
    case etransactionoption::edepost:
        showdeposit();
        gobacktotransaction();
        break;

    case etransactionoption::edraw:
        showdraw();
        gobacktotransaction();
        break;

    case etransactionoption::etotal:
        showtotalbalancescreen();
        gobacktotransaction();
        break;

    case etransactionoption::exist:
        ShowMainMenue();
        break;
    }
}

short readtransactionmenu() {
    short choice = 0;

    do {
        cout << "enter your choice [1-4]\n";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice, please enter a number between 1 and 4.\n";
        }
        else {
            break;
        }

    } while (true);

    return choice;
}

void showtransactionmenu() {
    system("cls");

    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";

    performtransaction((etransactionoption)readtransactionmenu());
}

void showdenidscreen() {
    cout << "\n------------------------------------\n";
    cout << "Access Denied,\nYou don't Have Permission To Do this,\nPlease Contact Your Admin.";
    cout << "\n------------------------------------\n";
}

suser convertuserlinetorecord(string line, string seperator = "##") {
    vector<string> vuser;
    suser user;

    vuser = splitstring(line, seperator);

    if (vuser.size() >= 3) {
        user.name = vuser[0];
        user.password = vuser[1];
        user.permission = stoi(vuser[2]);
    }

    return user;
}

string convertusertoline(suser user, string seperator = "##") {
    string oneline = "";

    oneline += user.name + seperator;
    oneline += user.password + seperator;
    oneline += to_string(user.permission);

    return oneline;
}

vector<suser> loaduserdatafromfile(string filename) {
    vector<suser> vuser;
    fstream myfile;

    myfile.open(filename, ios::in);

    if (myfile.is_open()) {
        string line;

        while (getline(myfile, line)) {
            if (line != "")
                vuser.push_back(convertuserlinetorecord(line));
        }

        myfile.close();
    }

    return vuser;
}

void saveuserdatatofile(string filename, vector<suser>& vuser) {
    fstream myfile;

    myfile.open(filename, ios::out);

    if (myfile.is_open()) {
        for (suser u : vuser) {
            if (u.mark == false)
                myfile << convertusertoline(u) << endl;
        }

        myfile.close();
    }
}

bool finduserbyusernameandpassword(string name, string password, suser& user) {
    vector<suser> vuser = loaduserdatafromfile(userfilename);

    for (suser& u : vuser) {
        if (u.name == name && u.password == password) {
            user = u;
            return true;
        }
    }

    return false;
}

void login() {
    bool logfaild = false;
    string username, password;

    do {
        system("cls");

        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (logfaild) {
            cout << "invalid password & username \n";
        }

        cout << "enter username \n";
        cin >> username;

        cout << "enter password \n";
        cin >> password;

        logfaild = !finduserbyusernameandpassword(username, password, usercurrent);

    } while (logfaild == true);

    ShowMainMenue();
}

bool finduserbyusername(string name, vector<suser> vuser, suser& user) {
    for (suser u : vuser) {
        if (u.name == name) {
            user = u;
            return true;
        }
    }

    return false;
}

void printuserrecord(suser user) {
    cout << "| " << setw(15) << left << user.name;
    cout << "| " << setw(10) << left << user.password;
    cout << "| " << setw(10) << left << user.permission << "\n";
}

void showlistuserscreen() {
    vector<suser> vuser = loaduserdatafromfile(userfilename);

    cout << "___________________________________________________\n";
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(10) << "Permissions" << "\n";
    cout << "___________________________________________________\n";

    if (vuser.size() == 0) {
        cout << "no users available on system \n";
    }
    else {
        for (suser u : vuser)
            printuserrecord(u);
    }

    cout << "___________________________________________________\n";
}

int readpermission() {
    int permission = 0;
    char answer = 'n';

    cout << "do you want to give full access y/n \n";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        return -1;

    cout << "\nDo you want to give access to : \n";

    cout << "\nShow Client List? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 1;

    cout << "\nAdd New Client? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 2;

    cout << "delete client ?y/n \n";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 4;

    cout << "\nUpdate Client? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 8;

    cout << "\nFind Client? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 16;

    cout << "\nTransactions? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 32;

    cout << "\nManage Users? y/n? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
        permission += 64;

    return permission;
}

suser readnewuser() {
    suser user;

    cout << "enter user name \n ";
    cin >> user.name;

    vector<suser> vuser = loaduserdatafromfile(userfilename);
    suser tempuser;

    while (finduserbyusername(user.name, vuser, tempuser)) {
        cout << "user with: " << user.name << " already exist \n";
        cin >> user.name;
    }

    cout << "enter password \n";
    cin >> user.password;

    user.permission = readpermission();

    return user;
}

void addnewuser() {
    suser user = readnewuser();

    fstream myfile;
    myfile.open(userfilename, ios::out | ios::app);

    if (myfile.is_open()) {
        myfile << convertusertoline(user) << endl;
        myfile.close();
    }
}

void ShowAddNewUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen\n";
    cout << "-----------------------------------\n";

    char more = 'y';

    do {
        addnewuser();

        cout << "\nUser Added Successfully, do you want to add more users? y/n? ";
        cin >> more;

    } while (more == 'y' || more == 'Y');
}

void PrintUserCard(suser user) {
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << user.name;
    cout << "\nPassword    : " << user.password;
    cout << "\nPermissions : " << user.permission;
    cout << "\n-----------------------------------\n";
}

bool markuser(string username, vector<suser>& vuser) {
    for (suser& u : vuser) {
        if (u.name == username) {
            u.mark = true;
            return true;
        }
    }

    return false;
}

bool deleteuserbyusername(string username, vector<suser>& vuser) {
    if (username == "Admin") {
        cout << "\n\nYou cannot Delete this User (Admin is protected)!\n";
        return false;
    }

    suser user;
    char answer = 'n';

    if (finduserbyusername(username, vuser, user)) {
        PrintUserCard(user);

        cout << "Are you sure you want to delete user y/n \n";
        cin >> answer;

        if (answer == 'y' || answer == 'Y') {
            markuser(username, vuser);
            saveuserdatatofile(userfilename, vuser);
            vuser = loaduserdatafromfile(userfilename);

            cout << "user delete successfully \n";
            return true;
        }
    }
    else {
        cout << "user with user name : " << username << " not found \n";
        return false;
    }

    return false;
}

void ShowDeleteUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Users Screen\n";
    cout << "-----------------------------------\n";

    vector<suser> vUsers = loaduserdatafromfile(userfilename);
    string username;

    cout << "Please enter Username? ";
    cin >> username;

    deleteuserbyusername(username, vUsers);
}

suser changeuserrecord(string username) {
    suser user;

    user.name = username;

    cout << "enter password \n";
    cin >> user.password;

    user.permission = readpermission();

    return user;
}

bool updateuserbyusername(string username, vector<suser>& vuser) {
    suser user;
    char answer = 'n';

    if (finduserbyusername(username, vuser, user)) {
        PrintUserCard(user);

        cout << "Are you sure you want to update user y/n \n";
        cin >> answer;

        if (answer == 'y' || answer == 'Y') {
            for (suser& u : vuser) {
                if (u.name == username) {
                    u = changeuserrecord(username);
                    break;
                }
            }

            saveuserdatatofile(userfilename, vuser);

            cout << "update user successfully \n";
            return true;
        }
    }
    else {
        cout << "user not found \n";
        return false;
    }

    return false;
}

void ShowUpdateUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Users Screen\n";
    cout << "-----------------------------------\n";

    vector<suser> vUsers = loaduserdatafromfile(userfilename);
    string username;

    cout << "Please enter Username? ";
    cin >> username;

    updateuserbyusername(username, vUsers);
}

void ShowFindUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen\n";
    cout << "-----------------------------------\n";

    vector<suser> vUsers = loaduserdatafromfile(userfilename);
    string username;

    cout << "Please enter Username? ";
    cin >> username;

    suser user;

    if (finduserbyusername(username, vUsers, user))
        PrintUserCard(user);
    else
        cout << "\nUser with Username [" << username << "] is not found!\n";
}

enum eusermenuoptions {
    elistuser = 1,
    eadduser = 2,
    edeleteuser = 3,
    eupdateuser = 4,
    efinduser = 5,
    emainmenu = 6
};

void gobacktomanagemrnu() {
    cout << "\n\nPress any key to go back to Manage Users Menu...";
    system("pause>0");
    ShowManageUsersMenu();
}

void performmanageuser(eusermenuoptions option) {
    switch (option) {
    case eusermenuoptions::elistuser:
        showlistuserscreen();
        gobacktomanagemrnu();
        break;

    case eusermenuoptions::eadduser:
        ShowAddNewUserScreen();
        gobacktomanagemrnu();
        break;

    case eusermenuoptions::eupdateuser:
        ShowUpdateUserScreen();
        gobacktomanagemrnu();
        break;

    case eusermenuoptions::edeleteuser:
        ShowDeleteUserScreen();
        gobacktomanagemrnu();
        break;

    case eusermenuoptions::efinduser:
        ShowFindUserScreen();
        gobacktomanagemrnu();
        break;

    case eusermenuoptions::emainmenu:
        ShowMainMenue();
        break;
    }
}

short ReadManageUsersMenueOption() {
    short choice;

    do {
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 6) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice, please enter a number between 1 and 6.\n";
        }
        else {
            break;
        }

    } while (true);

    return choice;
}

void ShowManageUsersMenu() {
    system("cls");

    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";

    performmanageuser((eusermenuoptions)ReadManageUsersMenueOption());
}

int main() {
    login();
    return 0;
}