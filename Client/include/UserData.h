//
// Created by abualhig on 1/15/20.
//

#ifndef ASS3_USERDATA_H
#define ASS3_USERDATA_H

#include <unordered_map>
#include<vector>

using namespace std;

class UserData {
private:
    string username;
    string password;
    vector<pair<string, string>> receiptsids;//the key is the receipt id and the value is receipt command
    vector<pair<string, vector<string>>> borrowRequests;//the key is the genre and the value is the bookname that i request to borrow
    vector<pair<string, pair<string, bool>>> borrowBooks;// the key is the name of the book and the value is the lender username
    vector<pair<int,string>> subscriptionids;//the first in the pair is the id and the second is the genre
    vector<pair<int,string>> subscribtionsReceipts;//the key is the reciptid and the value the topic
    vector<pair<int, string>> UnsubscribtionsReceipts;//the key is the recipt id and the value is the unsubscrtion topics
   // vector<pair<string , string>> subreceiptandidies; // each receipt with its subscription id
public:
    UserData();

    string getusername();

    void setbool(string);

    void setbool2(string);

    void addreceipt(const string&, const string&);

    void addsubsid( int,string );

    void addborrowrequest(string , string);

    void addborrowedbook(string , string);

    vector<pair<string, string>> getreceiptsids();

    vector<pair<std::string, vector<std::string>>> getBorrowRequests();

    vector<pair<string, pair<string, bool>>> getBorrowBooks();

    vector<pair<int ,string>> getSubscriptionsIDS();

    vector<pair<int, std::string>> getSubscribtionsreceipts();

    vector<pair<int, std::string>> getUNsubscribtionsreceipts();

    void addSubscribtionsreceipts(pair<int,string> p);

    void addUNSubscribtionsreceipts(pair<int,string> p);

    void editUserData(string , string);

    void setborrowrequests(vector<pair<string, vector<string>>> a);
};

#endif //ASS3_USERDATA_H
