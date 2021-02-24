//
// Created by abualhig on 1/12/20.
//
using namespace std;

#include "../include/RFserver.h"
#include <iostream>

RFserver::RFserver(ConnectionHandler &conh, UserData *user) : ch(conh), userdata(user), terminate(false) {}


void RFserver::operator()() {
    while (!terminate) {
        string s1;
        ch.getLine(s1);
        cout << "***************start" << endl << s1 << "************end" << endl;
        int x1 = s1.find_first_of('\n');
        string help = s1.substr(0, x1);
        if (help == "CONNECTED")
            handleconnectedframe();
        if (help == "RECEIPT") {
            handlereceiptframe(s1, x1);
        }
        if (help == "MESSAGE") {
            handlemessageframe(s1, x1);
        }
        if (help == "ERROR") {
            handleerrorframe(s1, x1);
        }
    }
}


void RFserver::handleconnectedframe() {
    terminate = false;
    cout << "Login successful" << endl;
}

void RFserver::handlemessageframe(string frame, int space) {

    frame = frame.substr(space + 1);
    //___________ taking the subscription of the message
    int x1 = frame.find_first_of('\n');
    string subscription = frame.substr(0, x1);
    int x111 = subscription.find_first_of(':');
    string subsid = subscription.substr(x111 + 1, subscription.length());// i would use it in returning
    frame = frame.substr(x1 + 1);//note that heere we also substr the '\n'
    //______________________handling the Messageid_________________-
    int x2 = frame.find_first_of('\n');
    string messageid = frame.substr(0, x2);
    frame = frame.substr(x2 + 1, frame.length() - 1);
    //____________-handling the destination____________-
    int x3 = frame.find_first_of('\n');
    string destination = frame.substr(0, x3);
    int x55 = destination.find_first_of(':');
    string gen11 = destination.substr(x55 + 1, destination.length());
    frame = frame.substr(x3 + 2, frame.length() - 1);// after this i will get the body
    // ________________handling the body_______________________________
    int x4 = frame.find_first_of('\n');
    string body = frame.substr(0, x4);

    int firstspace = body.find_first_of(' ');
    int lastspace = body.find_last_of(' ');
    string firstword = body.substr(0, firstspace);

    bool handeled = false;
    //___________here we handle Taking bookname from username;
    if (lastspace >= 0) {
        if (firstword == "Taking") {
            handeled = true;
            string h = body.substr(firstspace + 1);
            int sp = h.find_first_of(' ');
            string bookname = h.substr(0, sp);
            int sp1 = body.find_last_of(' ');
            string owner = body.substr(sp1 + 1, body.length());//the lender
            if (this->userdata->getusername() == owner) {//here the lender is i, so i should remove the book from me
                removeBook(bookname);
            } else { //here the client is not the lender so i want to check if i want to borrow this book and take it
                bool found = false;
                for (auto &a :userdata->getBorrowBooks()) {
                    if (a.first == bookname) {
                        found = true;
                    }
                }
                if (found) {
                    for (pair<string, vector<string>> &a:this->userdata->getBorrowRequests()) {
                        for (int i = 0; i < a.second.size(); i++) {
                            if (a.second[i] == bookname) {
                                userdata->setbool2(bookname);
                            }
                        }
                    }
                } else {
                    userdata->addborrowedbook(bookname, gen11);
                }
                //------------------ adding the borrowed book to my books ----------------
                bool found1 = false;
                for (auto &b : books) {
                    if (b.first == gen11) {
                        found1 = true;
                        b.second.push_back(bookname);
                    }
                }
                if (!found1) {
                    vector<string> toadd;
                    toadd.push_back(bookname);
                    books.insert(make_pair(gen11, toadd));
                }
            }
        }



        //------------------------ has added ----------------------------------------
        string identifier = this->userdata->getusername() + " has added";
        if (body.substr(0, identifier.length()) == identifier) {
            handeled = true;
            //___saving the bookname_____
            int z = body.find_last_of(' ');
            string bookname1 = body.substr(z + 1, body.length());
//__saving the gen
            int i = destination.find_first_of(':');
            string gen = destination.substr(i + 1, destination.length());//taking the gen
            bool found = false;

            for (auto& a:books) {
                if (a.first == gen) {
                    found = true;
                    addbook(gen, bookname1);
                }
            }
            if (!found) {//here the gen is not existed in the books
                vector<string> toput;
                toput.push_back(bookname1);
                pair<string, vector<string>> toadd = make_pair(gen, toput);
                books.insert(toadd);
                cout << bookname1 << " was succesfully added to " << gen << endl;
            }
        } else {
//_______________________handling has message(john has book)________________
            string helper3 = body.substr(firstspace + 1);
            int c = helper3.find_first_of(' ');
            string op2 = helper3.substr(0, c);
            string bookname2 = body.substr(lastspace + 1, body.length());
            if (op2 == "has") {
                handeled = true;
                for (auto& j:this->userdata->getBorrowRequests()) {
                    for (int i = 0; i < j.second.size(); i++) {
                        if (j.second[i] == bookname2) {
                            SendBack2(firstword, bookname2, gen11);
                        }
                    }
                }
            }
        }
//_______________________handling status______________________
        if (body == "Book status") {
            handeled = true;
            string tosend = "SEND";
            tosend += '\n';
            tosend += "destination:" +
                      gen11;
            tosend += '\n';
            tosend += '\n';
            tosend += this->userdata->

                    getusername()

                      + ":";
            vector<string> allbooks;
            for (auto& a:books) {
                if (a.first == gen11) {
                    vector<string> vec = a.second;
                    for (const auto &i: vec) {
                        allbooks.push_back(i);
                    }
                }
            }

            if (allbooks.size() == 1) {
                tosend += allbooks[0];
            } else {
                for (int i = 0; i < allbooks.size(); i++) {
                    if (i == allbooks.size() - 1) {
                        tosend += " and " + allbooks[i];
                    } else {
                        tosend += allbooks[i];
                        if (!(i == allbooks.size() - 2)) {
                            tosend += ",";
                        }
                    }
                }
            }
            ch.sendLine(tosend);
        }

//_________here we handle wish to borrow_____________-

        int sp = body.find_first_of(' ');
        string name = body.substr(sp);
        string helper = body.substr(sp + 1, body.length());//here i cut the username
        int helper1 = helper.find_first_of(' ');
        string op = helper.substr(0, helper1);
        if (op == "wish") {     // thats mean that someone wants to borrow the book
            handeled = true;
            int z = body.find_last_of(' ');
            string bookname1 = body.substr(z + 1, body.length());
            if (this->userdata->getusername() != name) {//i want to check if i have the book
                for (auto& a:this->books) {
                    for (const auto &i: a.second) {
                        if (i == bookname1) {
                            cout << "SendBack1" << endl;
                            sendBack(bookname1, gen11);
                        }
                    }
                }
            }
        }
//________________________handling returning message________________
        if (firstword == "Returning") {
            handeled = true;
            string help = body.substr(firstspace + 1);
            int b = help.find_first_of(' ');
            string bookname = help.substr(b);
            string lender = body.substr(lastspace + 1, body.length());
            if (this->userdata->getusername() == lender) {// if im the lender
                string genre;
                vector<pair<int, string>> vec = this->userdata->getSubscriptionsIDS();
                for (auto &i: vec) {
                    if (i.first == stoi(subsid)) {
                        genre = i.second;
                    }
                }
//----------------------- add the book to books -----------------------------------------
                bool found = false;
                for (auto& a:this->books) {
                    if (a.first == genre) {
                        a.second.push_back(bookname);
                        found = true;
                    }
                }
                if (!found) {
                    vector<string> books1;
                    books1.push_back(bookname);
                    this->books.insert(make_pair(genre, books1));
                }
//----------------------- if i requested it i need to add it to the borrowedbooks --------------
                for (auto& a:this->userdata->getBorrowRequests()) {
                    if (a.first == genre) {
                        vector<string> vec1 = a.second;
                        for (const auto &s: vec1) {
                            if (s == bookname) {
                                for (auto& booklender:this->userdata->getBorrowBooks()) {
                                    if (booklender.first == bookname) {
                                        booklender.second.second = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }


        }
    }
    if (!handeled) {//here it means that we take a some client book status so i want to print uts inventory

        cout << body << endl;

    }
}

void RFserver::handlereceiptframe(string frame, int space) {
    frame = frame.substr(space + 1);
    string receiptid;
    bool found = false;
    for (int i = 0; i < frame.length() && frame.at(i) != '\n'; i++) {
        if (!found) {
            if (frame.at(i) == ':') {
                found = true;
            }
        } else {
            receiptid += frame.at(i);
        }
    }
    string command;
    vector<pair<std::string, std::string>> map = this->userdata->getreceiptsids();
    for (size_t i = 0; i < map.size(); i++) {
        if (map.at(i).first == receiptid) {
            command = map.at(i).second;
        }
    }
    string gen;
    if (command.empty()) {
        cout << "receipt hasnt been received for this command" << endl;
    }
    //_________________handling the subscribe receipt____________
    if (command == "SUBSCRIBE") {
        for (size_t i = 0; i < userdata->getSubscribtionsreceipts().size(); i++) {
            if (userdata->getSubscribtionsreceipts().at(i).first == stoi(receiptid)) {
                gen = userdata->getSubscribtionsreceipts().at(i).second;
            }
        }
        cout << "Joined club " + gen << endl;
    }
    //____________handling the unsubscribereceipt_____________
    if (command == "UNSUBSCRIBE") {
        for (size_t i = 0; i < userdata->getUNsubscribtionsreceipts().size(); i++) {
            if (userdata->getUNsubscribtionsreceipts().at(i).first == stoi(receiptid)) {
                gen = userdata->getUNsubscribtionsreceipts().at(i).second;
            }
        }
        cout << "Exited club " + gen << endl;
    }
    //___________handling the logout receipt____________________
    if (command == "DISCONNECT") {
        ch.close();
        terminate = true;
    }
}

void RFserver::handleerrorframe(string frame, int space) {
    int x1 = frame.find_first_of('\n');
    frame = frame.substr(x1 + 2);
    int x2 = frame.find_first_of('\n');
    string toprint = frame.substr(0, x2);
    cout << toprint << endl;
}

void
RFserver::sendBack(const string &book, const string &destination) {// a helper function that send to the server frame
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:" + destination;
    tosend += '\n';
    tosend += '\n';
    tosend += this->userdata->getusername() + " has " + book;
    tosend += '\n';
//    tosend += '\0';
    ch.sendLine(tosend);
}

void RFserver::removeBook(const string &book) {
    for (auto &a:books) {
        for (int i = 0; i < a.second.size(); ++i) {
            if (a.second[i] == book) {
                a.second.erase(a.second.begin() + i);
            }
        }
    }
    for (int i = 0; i < this->userdata->getBorrowBooks().size(); ++i) {
        if (this->userdata->getBorrowBooks().at(i).second.first == book) {
            userdata->setbool(book);
        }
    }
}

void RFserver::SendBack2(const string &name, const string &book, const string &destination) {
    string tosend = "SEND";
    tosend += '\n';
    tosend += "destination:" + destination;
    tosend += "\n\n";
    tosend += "Taking " + book + " from " + name;
    tosend += '\n';
    ch.sendLine(tosend);
}

void RFserver::addbook(string gen, string bookname) {
    vector<string> vec;
    bool stop = false;
    for (auto& a : books) {
        if (a.first == gen && (!stop)) {
            stop = true;
            for (int i = 0; i < a.second.size(); ++i) {
                vec.push_back(a.second[i]);
            }
        }
    }
    vec.push_back(bookname);
    books.erase(gen);
    pair<string, vector<string>> toadd = make_pair(gen, vec);
    books.insert(toadd);
    cout << bookname << " was succesfully added to " << gen << endl;
}