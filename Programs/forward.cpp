/*
The program must accept and integer as the goal, and
up to 3 numbers that will be taken as the knowledge base,
then it should return both the process and the truth value of the 
goal number
*/
#include <bits/stdc++.h>
using namespace std;

/*Declaring global variables*/
vector<pair<vector<int>, int>> knowledgeBase = {
    {{8, 6, 5}, 4},
    {{6, 3}, 9},
    {{7, 4}, 9},
    {{8}, 1},
    {{6}, 5},
    {{9, 1}, 2},
    {{7}, 6},
    {{1, 7}, 9},
    {{1, 8}, 6}};

vector<int> facts = {7, 8};

/*Auxiliar functions*/
void coutArray(vector<int> array)
{
    for (int x : array)
    {
        cout << x << "\t";
    }
    cout << "\n";
}

vector<int> lookForBestRule(int goal)
{
    vector<vector<int>> matchedRules;
    int bestIndex = 0;
    int knownFacts = 0;

    for (pair<vector<int>, int> fact : knowledgeBase)
    {
        if (fact.second == goal)
            matchedRules.push_back(fact.first);
    }

    //Just in case the user prompted a non-valid goal
    if (matchedRules.size() == 0)
    {
        throw "Goal must be searchable in knowledge base";
    }

    for (int i = 0; i < matchedRules.size(); i++)
    {
        vector<int> requirements = matchedRules[i];
        int actualKnownFacts = 0;
        for (int requirement : requirements)
        {
            if (find(facts.begin(), facts.end(), requirement) != facts.end())
                actualKnownFacts++;
        }
        if (actualKnownFacts > knownFacts)
        {
            swap(actualKnownFacts, knownFacts);
            bestIndex = i;
        }
    }

    return matchedRules[bestIndex];
}

int main()
{
    coutArray(lookForBestRule(6));
}