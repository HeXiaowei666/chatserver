#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// 匹配User表的ORM类 
class User 
{
public:
    User(int id=-1, string name="", string pwd="", string state="offline")
    {
        this->id = id;
        this->name = name;
        this->password = password;
        this->state = state;
    }

    void setId (int id) {this->id = id;} // 一定要加this 因为和形参名字一样
    void setName (string name) {this->name = name;}
    void setPwd (string pwd) {this->password = password;}
    void setState (string state) {this->state = state;}

    int getId () {return this->id;} 
    string getName () {return this->name;}
    string getPwd () {return this->password;}
    string getState () {return this->state;}

protected:
    int id;
    string name;
    string password;
    string state;
};

#endif 