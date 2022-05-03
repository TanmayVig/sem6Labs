// identify leaders and then blocks
#include <bits/stdc++.h>
using namespace std;

int create_num(string s, size_t found)
{
    int num = 0, ind = found + 5;
    while (isdigit(s[ind]))
    {
        num = num * 10 + (s[ind++] - '0');
    }
    return num;
}

class Block
{
public:
    vector<string> code;
    vector<int> gotu;
    pair<int, int> boundry;

public:
    Block(vector<string> tac, int i, int j)
    {
        for (int k = i - 1; k < j; k++)
            code.push_back(tac[k]);
        boundry = {i, j};
        size_t found_if = tac[j - 1].find("if");
        size_t found_goto = tac[j - 1].find("goto");
        if (found_if != string::npos || found_goto == string::npos)
        {
            gotu.push_back(j + 1);
        }
        if (found_goto != string::npos)
        {
            gotu.push_back(create_num(tac[j - 1], found_goto));
        }
    }
};

vector<vector<bool>> construct_matrix(vector<Block> blocks)
{
    int n = blocks.size();
    vector<vector<bool>> mat(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++)
    {
        set<int> s(blocks[i].gotu.begin(), blocks[i].gotu.end());
        for (int j = 0; j < n; j++)
        {
            int leader = blocks[j].boundry.first;
            if (s.find(leader) != s.end())
                mat[i][j] = true;
        }
    }
    return mat;
}

int main()
{
    cout << "TANMAY VIG\n19BCS061\n";
    set<int> leaders_set;
    vector<int> leaders;
    vector<string> tac;
    string line;
    fstream file;
    file.open("./TAC.txt", ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            tac.push_back(line);
        }
    }
    file.close();

    leaders_set.insert(1);
    int ind = 1;
    for (string s : tac)
    {
        size_t found = s.find("goto");
        if (found != string::npos)
        {
            leaders_set.insert(ind + 1);
            leaders_set.insert(create_num(s, found));
        }
        ind++;
    }

    leaders.assign(leaders_set.begin(), leaders_set.end());
    vector<Block> blocks;
    for (int i = 0; i < leaders.size(); i++)
    {
        blocks.push_back(Block(tac, leaders[i], (i == leaders.size() - 1) ? tac.size() : leaders[i + 1] - 1));
    }

    int c = 0;
    cout << "Block\tLeader\tGoto\n";
    for (Block b : blocks)
    {
        cout << "B" << c++ << ":\t";
        cout << b.boundry.first << "\t";
        for (int i : b.gotu)
        {
            cout << i << ",";
        }
        cout << endl;
    }

    cout << endl;

    vector<vector<bool>> adj_matrix = construct_matrix(blocks);

    cout << "Adjacency Matrix of flow graph:\n"
         << setw(3) << "";
    for (int i = 0; i < adj_matrix.size(); i++)
        cout << "B" << i << " ";
    cout << endl;

    file.open("./AdjacencyMatrix.txt", ios::out);
    for (int v = 0; v < adj_matrix.size(); v++)
    {
        cout << "B" << v << ": ";
        for (auto i : adj_matrix[v])
        {
            cout << i << "  ";
            file << i << "  ";
        }
        cout << endl;
        file << endl;
    }
    return 0;
}