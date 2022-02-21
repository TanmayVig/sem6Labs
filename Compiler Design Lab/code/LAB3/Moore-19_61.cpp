// By - Tanmay Vig
// Teaching assistant - vikas yadav
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

string moore_outPut(int dfa[][100], int initial_state, int n, int m)
{
    string str;
    cout << "Enter string: ";
    cin >> str;

    int curr_state = dfa[initial_state][str[0] - '0'];

    int i = 1;
    string op(1, (char)dfa[initial_state][m - 1]);
    // cout << dfa[initial_state][2 * (str[0] - '0') + 1] << "::" << str[0] - '0' + 1 << endl;
    cout << "Transition: " << op[0];
    while (i < str.size() && curr_state != -1)
    {

        op = op + (char)(dfa[curr_state][m - 1]);
        cout << "->" << op[i];
        // cout << curr_state << "::" << str[0] - '0' + 1 << endl;
        curr_state = dfa[curr_state][str[i++] - '0'];
    }
    cout << "\n";

    return op;
}

int main()
{
    ifstream file;
    int dfa[100][100], initial_state;
    file.open("./Moore.txt");
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

            dfa[n][m++] = ((temp >= "a" && temp <= "z") || (temp >= "A" && temp <= "Z")) ? temp[0] : stoi(temp);
        }
        n++;
    }

    string op = moore_outPut(dfa, initial_state, n, m);
    cout << "Output: " << op << endl;

    return 0;
}