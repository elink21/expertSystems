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
        cout << " " << x;
    }
}

bool contains(vector<int> array, int value)
{
    return find(array.begin(), array.end(), value) != array.end();
}

vector<int> getUnknownRequirements(vector<int> requirements)
{
    vector<int> unknownRequirements = {};
    for (int requirement : requirements)
    {
        if (!contains(facts, requirement))
            unknownRequirements.push_back(requirement);
    }
    return unknownRequirements;
}

//Main functions
pair<vector<int>, int> lookForBestForwardRule(int condition)
{
    vector<pair<vector<int>, int>> matchedRules = {};
    int bestIndex = 0;
    int unknownFacts = INT_MAX;
    for (pair<vector<int>, int> rule : knowledgeBase)
    {
        if (contains(rule.first, condition))
            matchedRules.push_back(rule);
    }

    for (int i = 0; i < matchedRules.size(); i++)
    {
        vector<int> requirements = matchedRules[i].first;
        int actualUnknownFacts = 0;

        actualUnknownFacts = getUnknownRequirements(requirements).size();

        if (actualUnknownFacts < unknownFacts)
        {
            swap(actualUnknownFacts, unknownFacts);
            bestIndex = i;
        }
    }

    if (matchedRules.size() > 0)
    {
        return matchedRules[bestIndex];
    }

    return pair<vector<int>, int>{{}, 0};
}

vector<int> lookForBestBackwardRule(int goal)
{
    vector<vector<int>> matchedRules = {};
    int bestIndex = 0;
    int knownFacts = 0;

    for (pair<vector<int>, int> fact : knowledgeBase)
    {
        if (fact.second == goal)
            matchedRules.push_back(fact.first);
    }

    for (int i = 0; i < matchedRules.size(); i++)
    {
        vector<int> requirements = matchedRules[i];
        int actualKnownFacts = 0;
        for (int requirement : requirements)
        {
            if (contains(facts, requirement))
                actualKnownFacts++;
        }
        if (actualKnownFacts > knownFacts)
        {
            swap(actualKnownFacts, knownFacts);
            bestIndex = i;
        }
    }

    if (matchedRules.size() > 0)
        return matchedRules[bestIndex];
    return vector<int>{};
}

bool digTree(int goal)
{
    vector<int> requirements = lookForBestBackwardRule(goal);
    vector<int> unknownRequirements = {};

    //Meaning no rule for the goal was found, thus false, base case
    if (requirements.size() == 0)
        return false;

    //Checking how many requirements do we know
    unknownRequirements = getUnknownRequirements(requirements);

    //If unknown requirements exists then digTree is called recursively again

    if (unknownRequirements.size() > 0)
    {
        for (int subGoal : unknownRequirements)
        {
            digTree(subGoal);
        }
    }

    //If there are not, then goal requirements are fulfilled and goal is added to the KB
    if (getUnknownRequirements(requirements).size() == 0)
    {
        cout << "Rule: [";
        coutArray(requirements);
        cout << "]\t -> \t" << goal << "\n";
        facts.push_back(goal);
        return true;
    }

    return false;
}

bool forwardProcess(int goal)
{
    return false;   
}

int main()
{
    pair<vector<int>, int> best = lookForBestForwardRule(7);
    cout << "Rule: [";
    coutArray(best.first);
    cout << "]\t";
    cout << best.second;

    /*
    cout << digTree(2) << "\n";
    coutArray(facts);
    */
}