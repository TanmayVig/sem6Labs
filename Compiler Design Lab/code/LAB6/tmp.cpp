#include <bits/stdc++.h>
using namespace std;
void split(string s, char c, vector<string> &p)
{
    int i = 0;
    int j = s.find(c);
    while (j >= 0)
    {
        p.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);
    }
    if (j < 0)
    {
        p.push_back(s.substr(i, s.length()));
        // cout<<s.length();
    }
}
unordered_map<string, int> kt;
bool check(unordered_map<string, vector<string>> mp, string sentence, int j, string var)
{
    if (j == -1 && kt[var] == 1)
        return true;
    if (j < 0)
    {
        return false;
    }
    for (int i = 0; i < mp[var].size(); i++)
    {
        if (mp[var][i].size() == 1)
        {
            if ((j == 0 && sentence[j] == mp[var][i][0]))
            {
                return true;
            }
        }
        else
        {
            if (sentence[j] == mp[var][i][1])
            {
                string str(1, mp[var][i][0]); // iif the production doesnt give answer backtrack
                // to original value and see next elemment
                bool val = check(mp, sentence, j - 1, str);
                if (val == true)
                    return val;
            }
        }
    }
    return false;
}
int main()
{
    printf("%s\n%s\n", "Mohammad Abdul Basit", "18BCS064");
    ifstream fin("RG.txt");
    unordered_map<string, vector<string>> mp;
    string str;
    string index;
    while (getline(fin, str))
    {
        vector<string> productions;
        int j = str.find('-', 0);
        index = str.substr(0, j);
        // cout<<index;
        str = str.substr(j + 2, str.length());
        split(str, '|', productions);
        mp[index] = productions;
    }
    string sentence;
    cout << "Input Sentence :";
    cin >> sentence;
    string status;
    if (check(mp, sentence, sentence.length() - 1, "S") == 1)
    {
        status = "\nAccepted";
    }
    else
    {
        status = "\nNot Accepted";
    }
    cout << status;
}