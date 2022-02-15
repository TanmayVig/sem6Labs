// By - Tanmay Vig
// Teaching assistant - vikas yadav
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

string mealy_outPut(int dfa[][100], int initial_state)
{
    string str;
    cout << "Enter string: ";
    cin >> str;

    int curr_state = dfa[initial_state][2 * (str[0] - '0')];

    int i = 1;
    string op(1, (char)dfa[initial_state][2 * (str[0] - '0') + 1]);
    // cout << dfa[initial_state][2 * (str[0] - '0') + 1] << "::" << str[0] - '0' + 1 << endl;
    while (i < str.size() && curr_state != -1)
    {

        op = op + (char)(dfa[curr_state][2 * (str[i] - '0') + 1]);
        // cout << curr_state << "::" << str[0] - '0' + 1 << endl;
        curr_state = dfa[curr_state][2 * (str[i++] - '0')];
    }

    return op;
}

int main()
{
    ifstream file;
    int dfa[100][100], initial_state;
    file.open("./Mealy.txt");
    int n = 0, m = 0;
    bool counting = true;
    string str, temp;
    getline(file, str);
    initial_state = stoi(str);
    int i = 0;
    while (getline(file, str))
    {
        cout << "Q" << n << ": " << str << endl;
        stringstream X(str);
        m = 0;
        while (getline(X, temp, ' '))
        {

            if ((temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z"))
            {
                dfa[n][m++] = temp == "-1" ? '!' : temp[0];
            }
            else
                dfa[n][m++] = stoi(temp);
        }
        n++;
    }

    string op = mealy_outPut(dfa, initial_state);
    cout << "Output: " << op << endl;

    return 0;
}