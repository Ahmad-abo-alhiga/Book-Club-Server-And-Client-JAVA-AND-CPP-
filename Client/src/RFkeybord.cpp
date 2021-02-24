//
// Created by abualhig on 1/12/20.
//
#include <printf.h>
#include "../include/RFkeybord.h"

using namespace std;

RFkeybord::RFkeybord(ConnectionHandler &conh, UserData *user) : ch(conh), user(user), terminate(false), id(1),
                                                                receipt(-1) {}

void RFkeybord::operator()() {
    const short s = 1024;
    int sp;// to find the first space ' '
    while (!terminate) {//when to stop the loop???? it will be stopped from read to server when we accept and receipt of logout
        char inp[s];
        std::cin.getline(inp, s);
        string command(inp);
        sp = command.find_first_of(' ');

        string help = command.substr(0, sp);
        if (help == "login") {
            handlelogin(command, sp);
        }

        if (help == "join") {
            handlejoin(command, sp);
        }

        if (help == "exit") {
            handleexit(command, sp);
        }

        if (help == "add") {
            handleadd(command, sp);
        }

        if (help == "borrow") {
            handleborrow(command, sp);
        }

        if (help == "return") {
            handlereturn(command, sp);
        }

        if (help == "status") {
            handlestatus(command, sp);
        }

        if (help == "logout") {
            Terminate();
            handlelogout();
        }
    }

}

bool RFkeybord::shouldterminate() { return terminate; }

void RFkeybord::Terminate() { terminate = true; }

void RFkeybord::handleadd(string command, int space) {
    command = command.substr(space + 1);
    int i = command.find_first_of(' ');
    string gen = command.substr(0, i);
    i++;
    string bookname = command.substr(i, command.length());
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:" + gen;
    tosend += '\n';
    tosend += '\n';
    tosend += this->user->getusername();//here i want to set the activeuser name
    tosend += " has added the book ";
    tosend += bookname;
    tosend += '\n';
//    tosend += '\0';

    ch.sendLine(tosend);
}

void RFkeybord::handleborrow(string command, int space) {
    //TODO i should here add the book to borrowsrequests (userData)
    command = command.substr(space+1);
    //___________taking the gen______________________
    int x1 = command.find_first_of(' ');
    string gen = command.substr(0, x1);
    command = command.substr(x1+1);
    //___________taking the bookname_________________
    string bookname = command;
    //__________handling the opcode__________________
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:";
    tosend += gen + '\n' + '\n';
    tosend += user->getusername() + " wish to borrow " + bookname + '\n';
//    tosend += '\0';
    user->addborrowrequest(gen, bookname);
    ch.sendLine(tosend);
}

void RFkeybord::handleexit(string command, int space) {
    command = command.substr(space + 1);//TODO i changed to +1
    string gen = command;
    string tosend = "UNSUBSCRIBE";
    tosend += '\n';
    tosend += "id:";
    int id11 = 0;
    bool found = false;
    for (int i = 0; i < this->user->getSubscriptionsIDS().size() && !found; i++) {
        if (this->user->getSubscriptionsIDS().at(i).second == gen) {
            id11 = this->user->getSubscriptionsIDS().at(i).first;
            found = true;
        }
    }
    tosend += std::to_string(id11);
    tosend += '\n';
    tosend += "receipt:";
    tosend += to_string(receipt) + '\n';
    user->addreceipt(to_string(receipt), "UNSUBSCRIBE");
    user->addUNSubscribtionsreceipts(make_pair(receipt,gen));
    receipt--;
    tosend += "\n\n";
//    tosend += '\0';
    ch.sendLine(tosend);
}


void RFkeybord::handlejoin(string command, int space) {
    command = command.substr(space + 1);
    string gen = command;
    string tosend = "SUBSCRIBE";
    tosend += '\n';
    tosend += "destination:";
    tosend += gen;
    tosend += '\n';
    tosend += "id:";
    tosend += std::to_string(id);
    user->addsubsid(id, gen);// from here i yesha5zer the id for the sent frame in the unsubscribe to this topic
    tosend += '\n';
    tosend += "receipt:";
    tosend += std::to_string(receipt);
    string a = "SUBSCRIBE";
    string b = "" + to_string(receipt);
    user->addreceipt(b, a);//here i added the command with the recipt id
    this->user->addSubscribtionsreceipts(make_pair(receipt , gen));
    id++;
    receipt--;
    tosend += "\n\n";
//    tosend += '\0';
    ch.sendLine(tosend);
}

void RFkeybord::handlelogin(string command, int space) {
    command = command.substr(space);
    //___________host____________________
    int x1 = command.find_first_of(':');// to take the host
    string host = command.substr(0, x1);
    command = command.substr(x1 + 1);
    //___________port____________________
    int x2 = command.find_first_of(' ');
    string port = command.substr(0, x2);
    command = command.substr(x2 + 1);
    //__________username_________________
    int x3 = command.find_first_of(' ');
    string username = command.substr(0, x3);
    command = command.substr(x3 + 1);
    //_________password__________________
    string password = command;

    string tosend = "CONNECT";
    tosend += '\n';
    tosend += "accept-version:1.2";
    tosend += '\n';
    tosend += "host:";
    tosend += host;
    tosend += '\n';
    tosend += "login:";
    tosend += username;
    tosend += '\n';
    tosend += "passcode:";
    tosend += password;
    tosend += "\n\n";
//    tosend += '\0';
    this->user->editUserData(username, password);

    ch.sendLine(tosend);
}

void RFkeybord::handlelogout() {
    //TODO here i should add to reciptsid (in UserData) <id,<DISCONNECT,"somthing">>
    string tosend = "DISCONNECT";
    tosend += '\n';
    tosend += "receipt:";
    tosend += std::to_string(receipt);
    tosend += "\n\n";
    string a = "DISCONNECT";
    user->addreceipt(std::to_string(receipt), a);
    receipt++;
    ch.sendLine(tosend);
}

void RFkeybord::handlereturn(string command, int space) {
    command = command.substr(space);
    //___________genre______________________________________
    int x1 = command.find_first_of(' ');
    string gen = command.substr(0, x1);
    command = command.substr(x1);
    //___________book name__________________________________
    string bookname = command;

    //_______________handling the frame_____________________
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:";
    tosend += gen;
    tosend += "\n\n";
    tosend += "Returning " + bookname + " to " + user->getusername() + '\n';
//    tosend += '\0';

    //----------------delete from the borrowrequest-----------------------------
    for (pair<string, vector<string>> a : user->getBorrowRequests()) {
        if (a.first == gen) {
            for (size_t i = 0; i < a.second.size(); i++) {
                string s = a.second[i];
                if (s == bookname) {
                    a.second.erase(a.second.begin() + i);
                }
            }
        }
    }
    //-----------------delete from borrowed books--------------------------------
    for (size_t i = 0; i < user->getBorrowBooks().size(); i++) {
        pair<string, pair<string, bool>> m = user->getBorrowBooks().at(i);
        if (m.first == bookname) {
            user->getBorrowBooks().erase(user->getBorrowBooks().begin() + i);
        }
    }
    ch.sendLine(tosend);
}

void RFkeybord::handlestatus(string command, int space) {
    command = command.substr(space+1);
    int x1 = command.find_first_of(' ');
    string gen = command.substr(0, x1);
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:" + gen;
    tosend += "\n\n";
    tosend += "book status\n";
//    tosend += '\0';
    ch.sendLine(tosend);
}