#include <bits/stdc++.h>
using namespace std;

int main()
{
    vector<pair<string, string>> grammer;
    map<string, vector<int>> action, gotu;

    string line;
    ifstream file;
    file.open("./CFG.txt");
    if (file.is_open())
    {
        while (getline(file, line))
        {
            size_t brk = line.find("->");
            string state = line.substr(0, brk),
                   transition = line.substr(brk + 2);
            grammer.push_back({state, transition});
        }
    }
    file.close();
    file.open("./table.txt");
    if (file.is_open())
    {
        while (getline(file, line))
        {
        }
    }

    return 0;
}