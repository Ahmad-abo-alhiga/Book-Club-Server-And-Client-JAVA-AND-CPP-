//
// Created by abualhig on 1/12/20.
//

#include "connectionHandler.h"
#include "UserData.h"

#ifndef BOOST_ECHO_CLIENT_STOMPENCDEC_H
#define BOOST_ECHO_CLIENT_STOMPENCDEC_H
using namespace std;

class RFkeybord {
private:
    ConnectionHandler &ch;
    bool terminate;
    UserData *user;
    int id;
    int receipt;
public:
    RFkeybord(ConnectionHandler & , UserData*);

    void Terminate();

    bool shouldterminate();

    void operator()();

    void handlelogin(string command, int space);

    void handlejoin(string command, int space);

    void handleexit(string command, int space);

    void handleadd(string command, int space);

    void handleborrow(string command, int space);

    void handlereturn(string command, int space);

    void handlestatus(string command, int space);

    void handlelogout();
};
#endif //BOOST_ECHO_CLIENT_STOMPENCDEC_H

