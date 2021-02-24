//
// Created by abualhig on 1/15/20.
//

#include <iostream>
#include "../include/UserData.h"

void UserData::editUserData(string a, string b) {
    this->username = a;
    this->password = b;
}

UserData::UserData(){}

string UserData::getusername() {
    return this->username;
}

vector<pair<std::string, vector<std::string>>> UserData::getBorrowRequests() {
    return this->borrowRequests;
}

vector<pair<string, pair<string, bool>>> UserData::getBorrowBooks() {
    return this->borrowBooks;
}

vector<pair<int, std::string>> UserData::getSubscriptionsIDS() {
    return this->subscriptionids;
}


void UserData::setborrowrequests(vector<pair<string, vector<string>>> a) {
    this->borrowRequests = a;
}


void UserData::addreceipt(const string &a, const string &b) {
    this->receiptsids.push_back(make_pair(a, b));
}

void UserData::addsubsid(int id, string topic) {
    this->subscriptionids.push_back(make_pair(id, topic));
}

vector<pair<string, string>> UserData::getreceiptsids() {
    return receiptsids;
}

void UserData::setbool(string i) {
    for(auto& a :borrowBooks){
        if(a.first == i){
            a.second.second = false;
        }
    }
}

void UserData::addborrowrequest(string gen, string bookname) {
    bool found = false;
    for (auto &a:borrowRequests) {
        if (a.first == gen) {
            found = true;
            a.second.push_back(bookname);
        }
    }
    if (!found) {
        vector<string> vec;
        vec.push_back(bookname);
        borrowRequests.push_back(make_pair(gen, vec));
    }
}

void UserData::setbool2(string i) {
    for(auto& a :borrowBooks){
        if(a.first == i){
            a.second.second = true;
        }
    }
}

vector<pair<int, std::string>> UserData::getSubscribtionsreceipts() {
    return this->subscribtionsReceipts;
}

vector<pair<int, std::string>> UserData::getUNsubscribtionsreceipts() {
    return this->UnsubscribtionsReceipts;
}

void UserData::addSubscribtionsreceipts(pair<int, string> p) {
    this->subscribtionsReceipts.push_back(p);

}

void UserData::addUNSubscribtionsreceipts(pair<int, string> p) {
    this->UnsubscribtionsReceipts.push_back(p);
}

void UserData::addborrowedbook(string gen, string bookname) {
    borrowBooks.push_back(make_pair(bookname, make_pair(gen, true)));
}

