#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <string.h>
#include <queue>
#include <fstream>
using namespace std;

set<string> firstOf(string str, map<string, vector<string>> prod)
{
    set<string> Set;
    if (prod.find(str) == prod.end())
    {
        if (str.size() == 1)
            Set.insert(str);
        else
        {
            string x("");
            x += str[0];
            set<string> fiS = firstOf(x, prod);
            Set.insert(fiS.begin(), fiS.end());
        }
        return Set;
    }
    for (string x : prod[str])
    {
        string s("");
        s += x[0];
        set<string> fiO = firstOf(s, prod);
        Set.insert(fiO.begin(), fiO.end());
    }
    return Set;
}
set<string> followOf(string str, map<string, vector<string>> prod, string start)
{

    set<string> Set;
    if (str == start)
        Set.insert("$");
    for (map<string, vector<string>>::iterator ii = prod.begin(); ii != prod.end(); ++ii)
    {
        string key = (*ii).first;
        for (string x : (*ii).second)
        {
            string y = x;
            // cout<<y<<endl;
            for (int i = 0; i < y.size(); i++)
            {
                string s("");
                s += y[i];
                if (s == str)
                {
                    if (i == x.size() - 1)
                    {
                        if (key != str)
                        {
                            set<string> foO = followOf(key, prod, start);
                            Set.insert(foO.begin(), foO.end());
                        }
                    }
                    else
                    {
                        string st("");
                        st += y[i + 1];
                        set<string> fiO = firstOf(st, prod);
                        Set.insert(fiO.begin(), fiO.end());
                        if (Set.find("e") != Set.end())
                        {
                            Set.erase("e");
                            y[i + 1] = y[i];
                        }
                    }
                }
            }
        }
    }
    return Set;
}

int split(vector<string> &arr, string line, char delimiter)
{
    line += delimiter;
    int i = 0, len = line.length();
    string str = "";
    for (int j = 0; j < len; j++)
    {
        if (line[j] == delimiter)
        {
            arr.push_back(str);
            str = "";
        }
        else
            str += line[j];
    }
    return i;
}
int main()
{
    map<string, vector<string>> map1;
    vector<string> arl;
    ifstream file;
    file.open("./FirstAndfollow.txt", ios::in);
    if (file.is_open())
    {
        string line, initial;

        getline(file, initial);
        while (getline(file, line))
        {
            size_t brk = line.find("->");
            string state = line.substr(0, brk);
            string transition = line.substr(brk + 2, line.length());
            split(arl, transition, '|');
            map1[state] = arl;
            arl.clear();
        }
        // for (map<string, vector<string>>::iterator ii = map1.begin(); ii != map1.end(); ++ii)
        for (auto ii : map1)
        {
            cout << ii.first << ": ";
            for (int i = 0; i < (ii).second.size(); i++)
            {
                cout << (ii).second[i] << " ";
            }
            cout << endl;
        }
        set<string>::iterator itr;
        cout << "First of :" << endl;
        for (map<string, vector<string>>::iterator ii = map1.begin(); ii != map1.end(); ++ii)
        {
            set<string> fiS = firstOf((*ii).first, map1);
            cout << ((*ii).first) << ": ";
            for (itr = fiS.begin(); itr != fiS.end(); itr++)
            {
                cout << *itr << " ";
            }
            cout << endl;
        }
        cout << "Follow of:" << endl;
        for (map<string, vector<string>>::iterator ii = map1.begin(); ii != map1.end(); ++ii)
        {
            set<string> foO = followOf((*ii).first, map1, initial);
            cout << (*ii).first << ": ";
            for (itr = foO.begin(); itr != foO.end(); itr++)
            {
                cout << *itr << " ";
            }
            cout << endl;
        }
    }
    return 0;
}