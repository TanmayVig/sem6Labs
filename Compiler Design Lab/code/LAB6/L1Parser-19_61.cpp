#include <bits/stdc++.h>
using namespace std;
// static int hh=0;
struct values
{
    vector<int> val;
};
set<char> first(char ele, unordered_map<char, set<int>> left, vector<vector<char>> prod, unordered_map<char, set<char>> First)
{
    if (First.find(ele) != First.end())
        return First[ele];
    set<char> ans;
    for (auto itr = left[ele].begin(); itr != left[ele].end(); itr++)
    {
        if (prod[*itr][1] >= 'a' && prod[*itr][1] <= 'z')
        {
            ans.insert(prod[*itr][1]);
        }

        int c = 1;
    back:
        if ((c < (prod[*itr].size())) && prod[*itr][c] >= 'A' && prod[*itr][c] <= 'Z')
        {
            // cout<<prod[*itr][c]<<"\t";
            set<char> a = first(prod[*itr][c], left, prod, First);
            for (auto i = a.begin(); i != a.end(); i++)
            {
                ans.insert(*i);
            }
            if (a.find('e') != a.end() && prod[*itr][c + 1] != ele)
            {
                c = c + 1;
                goto back; // if Y! Y2 is there and Y1 has epsilon consider Y2 also then for first
            }
        }
    }
    return ans;
}
set<char> follow(char ele, unordered_map<char, set<int>> right, vector<vector<char>> prod, unordered_map<char, set<char>> First)
{
    set<char> ans;
    for (auto it = right[ele].begin(); it != right[ele].end(); it++)
    {
        int i;
        if (ele == 'S')
            ans.insert('$');
        for (i = 1; i < prod[*it].size(); i++)
        {
            if (prod[*it][i] == ele)
                break;
        }
        if (i == prod[*it].size() - 1)
        {
            // cout<<ele<<" "<<prod[*it][0]<<endl;
            if (ele != prod[*it][0])
            {
                set<char> a = follow(prod[*it][0], right, prod, First);
                for (auto j = a.begin(); j != a.end(); j++)
                {
                    ans.insert(*j);
                }
            }
        }
        else
        {
            if (prod[*it][i + 1] >= 'a' && prod[*it][i + 1] <= 'z')
                ans.insert(prod[*it][i + 1]);
            if (prod[*it][i + 1] >= 'A' && prod[*it][i + 1] <= 'Z')
            {
                // cout<<ele<<" "<<prod[*it][i+1]<<endl;
                for (auto j = First[prod[*it][i + 1]].begin(); j != First[prod[*it][i + 1]].end(); j++)
                {
                    // cout<<*j;
                    ans.insert(*j);
                }
                if (First[prod[*it][i + 1]].find('e') != First[prod[*it][i + 1]].end()) // if the next element contains epsilon
                {
                    if (ele != prod[*it][0])
                    {
                        set<char> a = follow(prod[*it][0], right, prod, First);
                        for (auto j = a.begin(); j != a.end(); j++) // if first of beta conatin epsilon add follow of first element too
                        {
                            ans.insert(*j);
                        }
                    }
                }
            }
        }
    }
    ans.erase('e');
    return ans;
}
int main()
{
    ifstream fin("./FirstAndFollow.txt");
    vector<vector<char>> prod;
    string str;
    while (getline(fin, str))
    {
        vector<char> a;
        int j = str.find('-', 0);
        a.push_back(str[0]);
        str = str.substr(j + 2, str.length());
        for (int i = 0; i < str.length(); i++)
        {
            a.push_back(str[i]);
        }
        prod.push_back(a);
    }
    unordered_map<char, set<int>> left, right;
    for (int i = 0; i < prod.size(); i++)
    {
        left[prod[i][0]].insert(i);
        for (int j = 1; j < prod[i].size(); j++)
        {
            if (prod[i][j] >= 'A' && prod[i][j] <= 'Z')
                right[prod[i][j]].insert(i);
        }
    }

    // First
    unordered_map<char, set<char>> First;
    for (auto it = left.begin(); it != left.end(); it++)
    {
        set<char> a = first(it->first, left, prod, First);
        First[it->first] = a;
    }
    cout << "First:\n";
    for (auto i = First.begin(); i != First.end(); i++)
    {
        cout << i->first << "\t";
        for (auto itr = i->second.begin(); itr != i->second.end(); itr++)
            cout << *itr << "\t";
        cout << endl;
    }
    unordered_map<char, set<char>> Follow;
    for (auto it = left.begin(); it != left.end(); it++)
    {
        set<char> a = follow(it->first, right, prod, First);
        Follow[it->first] = a;
    }
    cout << "Follow:\n";
    for (auto i = Follow.begin(); i != Follow.end(); i++)
    {
        cout << i->first << "\t";
        for (auto itr = i->second.begin(); itr != i->second.end(); itr++)
            cout << *itr << "\t";
        cout << endl;
    }
    cout << "\n\n LL Parsing table\n\n";
    set<int> table[32][32];
    for (int i = 0; i < prod.size(); i++)
    {
        int flag = 0;
        int j = 1;
        // repeat loop until we find no more null element or last element
        while (1)
        {
            char element = prod[i][j];
            int flagLocal = 0;
            if (element == 'e')
            {
                flagLocal = 1;
                flag = 1;
                goto b;
            }
            if (element >= 'a' && element <= 'z')
            {
                table[abs((prod[i][0] - 'A')) % 19][element - 'a'].insert(i);
            }
            else
            {
                // checking first
                for (auto itr = First[element].begin(); itr != First[element].end(); itr++)
                {
                    if (*itr == 'e')
                    {
                        flagLocal = 1;
                        flag = 1;
                    }
                    else
                    {
                        table[abs((prod[i][0] - 'A')) % 19][*itr - 'a'].insert(i);
                    }
                }
            }
            // see if current production had an epsilon if it had, then see first of next or stop if no localEpsilons present
            if (flagLocal == 1)
            {
                if (j == prod[i].size() - 1)
                    break;
                j++;
                continue;
            }
            break;
        }
    // if epsilon in first, do follow also
    b:
        if (flag == 1)
        {
            for (auto itr = Follow[prod[i][0]].begin(); itr != Follow[prod[i][0]].end(); itr++)
            {
                if (*itr == '$')
                {
                    table[abs((prod[i][0] - 'A')) % 19][31].insert(i);
                }
                else
                {
                    table[abs((prod[i][0] - 'A')) % 19][*itr - 'a'].insert(i);
                }
            }
        }
    }
    // display S=18 A=0,B=1, $ at column 31
    for (int i = 18; i == 18 || i < 2; i++)
    {
        for (int j = 0; j < 3 || j == 31; j++)
        {
            if (table[i][j].size() >= 1)
            {
                for (auto itr = table[i][j].begin(); itr != table[i][j].end(); itr++)
                {
                    cout << *itr << ",";
                }
            }
            else
            {
                cout << "-1";
            }
            if (j == 2)
            {
                j = 30;
            }
            cout << "\t";
        }
        cout << "\n";
        if (i == 18)
            i = -1;
    }
}