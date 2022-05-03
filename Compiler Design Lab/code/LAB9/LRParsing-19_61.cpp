#include <bits/stdc++.h>
using namespace std;

vector<string> split(string s)
{
    vector<string> ret;
    stringstream x(s);
    string tmp;
    while (getline(x, tmp, ' '))
    {
        ret.push_back(tmp);
    }

    return ret;
}

map<char, vector<int>> make_map(vector<string> row)
{
    map<char, vector<int>> m;
    for (string s : row)
    {
        m[s[0]] = {-1};
    }
    return m;
}

void insert_map(vector<string> row, vector<string> key, map<char, vector<int>> &m)
{
    for (int i = 0; i < row.size(); i++)
    {
        m[key[i][0]].push_back(stoi(row[i]));
    }
}

void print_stack(stack<pair<char, int>> st)
{
    int l = st.size();
    while (!st.empty())
    {
        auto top = st.top();
        cout << top.first << top.second << ",";
        st.pop();
    }
    cout << setw(30 - 3 * l) << "";
}

string string_matching(string str, vector<pair<string, string>> grammar, map<char, vector<int>> action, map<char, vector<int>> gotu)
{
    cout << "STACK" << setw(32) << "ACTION\n";
    stack<pair<char, int>> st;
    pair<char, int> top = {'$', 1};
    int ind = 0;
    st.push(top);
    while (!st.empty())
    {
        if (ind == str.length())
            return "String Not Accepted\n";
        print_stack(st);
        cout << str.substr(ind);
        top = st.top();
        int act = action[str[ind]][top.second];

        if (act == 99)
        {
            cout << " -> acc\n";
            return "\nString is Accepted\n";
        }

        if (act == 0)
        {
            return "\nString Not Accepted\n";
        }
        else if (act > 0)
        {
            cout << " -> shift " << str[ind] << act << endl;
            st.push({str[ind++], act});
        }
        else
        {
            cout << " -> reduce " << -(act + 1) << endl;
            auto prod = grammar[-(act + 1)];

            for (int i = prod.second.length() - 1; i >= 0; i--)
            {

                if (st.empty())
                {
                    return "\nString Not Accepted\n";
                }

                if (st.top().first == prod.second[i])
                {

                    st.pop();
                }
                else
                {
                    return "\nString Not Accepted\n";
                }
            }
            if (st.empty() || gotu[prod.first[0]][st.top().second] == 0)
            {
                return "\nString Not Accepted\n";
            }

            st.push({prod.first[0], gotu[prod.first[0]][st.top().second]});
        }
    }
    return "String not Accepted\n";
}

int main()
{
    cout << "By: Tanmay Vig \t Roll No.: 19BCS061\n";
    vector<pair<string, string>> grammer;
    map<char, vector<int>> action, gotu;
    vector<string> terminal, non_terminal;
    string str;

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
        getline(file, line);
        stringstream x(line);
        string tmp;
        vector<string> row;

        while (getline(x, tmp, '|'))
        {
            row.push_back(tmp);
        }

        terminal = split(row[0]);
        non_terminal = split(row[1]);
        action = make_map(terminal);
        gotu = make_map(non_terminal);

        while (getline(file, line))
        {
            row.clear();
            stringstream x(line);
            while (getline(x, tmp, '|'))
            {
                row.push_back(tmp);
            }
            insert_map(split(row[0]), terminal, action);
            insert_map(split(row[1]), non_terminal, gotu);
        }
    }

    cout << "Enter String: ";
    cin >> str;
    cout << string_matching(str, grammer, action, gotu) << endl;
    return 0;
}