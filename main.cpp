#include <bits/stdc++.h>
using namespace  std;

const string FILE_NAME = "automata_1.txt";

void ReadAutomata(const string fileName, int &alphabetSize, int &stateCount, int &s0, vector<int> &finalStates, vector<vector<int>> &transition)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return;
    }

    file >> alphabetSize >> stateCount >> s0;
    int finalStateCount;
    file >> finalStateCount;

    finalStates.resize(finalStateCount);
    for (int i=0; i<finalStateCount; ++i)
    {
        file >> finalStates[i];
    }

    int s1, s2;
    char a;
    transition.resize(stateCount, vector<int>(alphabetSize, -1));
    while (file >> s1 >> a >> s2)
    {
        transition[s1][a - 'a'] = s2;
    }

    file.close();
}

int ProcessWord(int s0, string w, vector<vector<int>> transition)
{
    int curState = s0;
    for (char a : w)
    {
        curState = transition[curState][a - 'a'];
        if (curState == -1)
            return -1;
    }
    return curState;
}

void dfs(int curState, int alphabetSize, vector<vector<int>> transition, vector<bool> &visited)
{
    visited[curState] = true;
    for (int i=0; i<alphabetSize; ++i)
    {
        int nextState = transition[curState][i];
        if (nextState != -1 && !visited[nextState])
        {
            dfs(nextState, alphabetSize, transition, visited);
        }
    }
}

bool IsAcceptedByAutomata(int s0, string w1, string w2, int alphabetSize, int stateCount, vector<int> finalStates, vector<vector<int>> transition)
{
    int s1 = ProcessWord(s0, w1, transition);
    if (s1 == -1) return false;

    vector<bool> visited(stateCount, false);
    dfs(s1, alphabetSize, transition, visited);

    for (int state=0; state<stateCount; ++state)
    {
        if (visited[state])
        {
            int s2 = ProcessWord(state, w2, transition);
            if ( find(finalStates.begin(), finalStates.end(), s2) != finalStates.end() )
                return true;
        }
    }
    return false;
}

int main()
{
    int alphabetSize, stateCount, s0;
    vector<int> finalStates;
    vector<vector<int>> transition;

    ReadAutomata(FILE_NAME, alphabetSize, stateCount, s0, finalStates, transition);

    string w1, w2;
    cout << "Enter w1: ";
    cin >> w1;
    cout << "Enter w2: ";
    cin >> w2;

    bool isAccepted = IsAcceptedByAutomata(s0, w1, w2, alphabetSize, stateCount, finalStates, transition);
    if (isAccepted)
        cout << "The automata can accept words w = w1w0w2\n";
    else
        cout << "The automata can NOT accept words w = w1w0w2\n";

    return 0;
}
