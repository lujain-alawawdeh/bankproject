// bankproject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const string filename = "client.txt";

struct sclient {
    string name;
    string pincode;
    string accountnum;
    double accountbalance;
    bool mark = false;
};

sclient readclient() {
    sclient client;
    cout << "please enter name\n";
    getline(cin >> ws, client.name);
    cout << "please enter pincode\n";
    getline(cin, client.pincode);
    cout << "enter account number \n";
    getline(cin, client.accountnum);
    cout << "enter account balance\n";
    cin >> client.accountbalance;
    return client;
}

string converttoline(sclient client, string seperator = "##") {
    string line = "";
    line += client.name + seperator;
    line += client.pincode + seperator;
    line += client.accountnum + seperator;
    line += to_string(client.accountbalance);
    return line;
}

vector<string> split(string s, string sep) {
    short pos = 0;
    vector<string> vstring;
    string word;
    while ((pos = s.find(sep)) != string::npos) {
        word = s.substr(0, pos);
        if (word != "")
            vstring.push_back(word);
        s.erase(0, pos + sep.length());
    }
    if (s != "")
        vstring.push_back(s);
    return vstring;
}

sclient converttorecord(string line, string seperator = "##") {
    vector<string> vclient = split(line, seperator);
    sclient client;
    if (vclient.size() >= 4) {
        client.name = vclient[0];
        client.pincode = vclient[1];
        client.accountnum = vclient[2];
        client.accountbalance = stod(vclient[3]);
    }
    return client;
}

void printclientcard(sclient client) {
    cout << "\n-----------------------------------";
    cout << "\nName           : " << client.name;
    cout << "\nPin Code       : " << client.pincode;
    cout << "\nAccount Number : " << client.accountnum;
    cout << "\nAccount Balance: " << client.accountbalance;
    cout << "\n-----------------------------------\n";
}

void addtofile(string name, string data) {
    fstream myfile;
    myfile.open(name, ios::out | ios::app);
    if (myfile.is_open()) {
        myfile << data << endl;
    }
    myfile.close();
}

void addnewclient() {
    sclient client = readclient();
    addtofile(filename, converttoline(client));
}

void addclient() {
    char addmore = 'Y';
    do {
        addnewclient();
        cout << "\nDo you want to add more ? y/n \n";
        cin >> addmore;
    } while (toupper(addmore) == 'Y');
}

vector<sclient> showfile(string filename) {
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
    }
    myfile.close();
    return vclient;
}

bool findbyaccountnum(string accountnum, sclient& client, vector<sclient>& vclient) {
    for (sclient& c : vclient) {
        if (c.accountnum == accountnum) {
            client = c;
            return true;
        }
    }
    return false;
}

string readaccnum() {
    string num;
    cout << "please enter account number\n";
    cin >> num;
    return num;
}

vector<sclient> saveclientsdatatofile(string filename, vector<sclient>& vclient) {
    fstream myfile;
    myfile.open(filename, ios::out);
    if (myfile.is_open()) {
        for (sclient& c : vclient) {
            if (!c.mark) {
                myfile << converttoline(c) << endl;
            }
        }
        myfile.close();
    }
    return vclient;
}

bool markclientfordelete(string accnum, vector<sclient>& vclient) {
    for (sclient& c : vclient) {
        if (c.accountnum == accnum) {
            c.mark = true;
            return true;
        }
    }
    return false;
}

bool deleteclientbyaccountnumber(string accnum, vector<sclient>& vclient) {
    sclient client;
    char answer = 'n';

    if (findbyaccountnum(accnum, client, vclient)) {
        printclientcard(client);
        cout << "\nare you sure you want to delete y/n ?\n";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            markclientfordelete(accnum, vclient);
            saveclientsdatatofile(filename, vclient);
            vclient = showfile(filename);
            cout << "\ndelete successfully\n";
            return true;
        }
    }
    else {
        cout << "\n" << accnum << " not found\n";
    }
    return false;
}

int main() {
    vector<sclient> vclient = showfile(filename);
    string accnum = readaccnum();
    deleteclientbyaccountnumber(accnum, vclient);
    return 0;
}