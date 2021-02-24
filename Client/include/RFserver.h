//
// Created by abualhig on 1/12/20.
//
#include <vector>
#include "../include/connectionHandler.h"
#include "RFkeybord.h"
#include <unordered_map>

using namespace std;
#ifndef BOOST_ECHO_CLIENT_RFSERVER_H
#define BOOST_ECHO_CLIENT_RFSERVER_H

class RFserver {
private:
    UserData* userdata;
    ConnectionHandler &ch;
    bool terminate;
    unordered_map<string, vector<string>> books;//the key is the genre and the value is the books in this genre that the client add
public:
    RFserver(ConnectionHandler &conh , UserData*);

    void addbook(string , string);

    void operator()();

    void handleconnectedframe();

    void handlemessageframe(string frame, int space);

    void handlereceiptframe(string frame, int space);

    void handleerrorframe(string frame, int space);

    void sendBack(const string& book, const string& destination);

    void SendBack2(const string& name, const string& book, const string& destination);

    void removeBook(const string& book);

};

#endif //BOOST_ECHO_CLIENT_RFSERVER_H
