/*
The program must accept and integer as the goal, and
up to 3 numbers that will be taken as the knowledge base,
then it should return both the process and the truth value of the 
goal number
*/
#include <thread>
#include <mutex>
#include <string>
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

//Special variables for forward process threads
int unknownFacts = INT_MAX;
int bestIndex = 0;

std::mutex mtx;

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

//Backward reasoning section, fully functional
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

bool solveBackwardTree(int goal)
{
    vector<int> requirements = lookForBestBackwardRule(goal);
    vector<int> unknownRequirements = {};

    //Meaning no rule for the goal was found, thus false, base case
    if (requirements.size() == 0)
        return false;

    //Checking how many requirements do we know
    unknownRequirements = getUnknownRequirements(requirements);

    //If unknown requirements exists then solveBackwardTree is called recursively again

    if (unknownRequirements.size() > 0)
    {
        for (int subGoal : unknownRequirements)
        {
            solveBackwardTree(subGoal);
        }
    }

    //If there are not, then goal requirements are fulfilled and goal is added to the knowledge base
    if (getUnknownRequirements(requirements).size() == 0)
    {
        cout << "Rule: [";
        coutArray(requirements);
        cout << "]\t -> \t" << goal << "\n";
        if (!contains(facts, goal))
            facts.push_back(goal);
        return true;
    }

    return false;
}

//Forward reasoning section,still on progress, it actually doesnt solve the problem

void bestRuleHelper(int i, vector<pair<vector<int>, int>> matchedRules)
{
    vector<int> requirements = matchedRules[i].first;
    int actualUnknownFacts = 0;

    actualUnknownFacts = getUnknownRequirements(requirements).size();

    if (actualUnknownFacts < unknownFacts)
    {
        mtx.lock();
        swap(actualUnknownFacts, unknownFacts);
        bestIndex = i;
        mtx.unlock();
    }
}

pair<vector<int>, int> lookForBestForwardRule(int condition)
{
    vector<pair<vector<int>, int>> matchedRules = {};
    bestIndex = 0;
    unknownFacts = INT_MAX;

    vector<thread> threadList;

    for (pair<vector<int>, int> rule : knowledgeBase)
    {
        if (contains(rule.first, condition))
            matchedRules.push_back(rule);
    }

    for (int i = 0; i < matchedRules.size(); i++)
    {
        threadList.push_back(std::thread(bestRuleHelper, i, matchedRules));
    }

    for (auto &th : threadList)
    {
        th.join();
    }

    if (matchedRules.size() > 0)
    {
        return matchedRules[bestIndex];
    }

    return pair<vector<int>, int>{{}, 0};
}

bool applyForwardSolving(int goal)
{
    for (int i = 0; i < facts.size(); i++)
    {
        int fact = facts[i];
        cout << "Now using: " << fact << "\n";
        pair<vector<int>, int> bestRule = lookForBestForwardRule(fact);

        //Meaning there is not matched rule, so the branch needs to finish
        if (bestRule.first.size() == 0)
        {
            break;
        }

        //So all the requirements are satisfied for this rule
        if (getUnknownRequirements(bestRule.first).size() == 0)
        {
            if (!contains(facts, bestRule.second))
                facts.push_back(bestRule.second);
            coutArray(facts);
            cout << "\n";

            //If the goal was reached there is no need to iterate anymore
            if (bestRule.second == goal)
                break;
        }
    }

    if (contains(facts, goal))
        return true;
    return false;
}

vector<int> stringToFacts(string s)
{
    vector<int> facts;
    string subMember = "";
    for (char x : s)
    {
        if (x == ' ')
        {
            facts.push_back(stoi(subMember));
            subMember = "";
        }
        else
        {
            subMember += x;
        }
    }

    facts.push_back(stoi(subMember));
    return facts;
}

int main()
{

    //Getting input from user
    vector<int> originalFacts; //'Cause after any solving the facts get messed for the next one
    string stringFacts;
    int goal;
    cout << "Please enter the initial facts separated by space: ";
    getline(cin, stringFacts);
    originalFacts = stringToFacts(stringFacts);
    facts = originalFacts;
    coutArray(originalFacts);
    cout << "\nNow type the goal u wanna test: ";
    cin >> goal;

    /*Applying forward method*/
    cout << "Applying forward reasoning now: \n";
    bool forwardResult = applyForwardSolving(goal);
    forwardResult ? cout << "Successful" : cout << "Unable to solve";

    facts = originalFacts; //cleaning array of facts

    //Backard process section
    cout << "\nNow trying with backward method: \n";
    vector<thread> threadList;

    vector<int> requirements = lookForBestBackwardRule(goal);
    for (int x : requirements)
    {
        //Dig tree is the main solving function, it takes
        //a goal as a param and traverse over the knowledge base tree
        //Until all its subgoals are digged, then the backtracking
        //gets the job done.
        threadList.push_back(std::thread(solveBackwardTree, x));
    }

    for (auto &th : threadList)
        th.join();

    if (getUnknownRequirements(requirements).size() == 0 && requirements.size() != 0)
    {
        cout << "Goal acompplished, final facts = ";
        facts.push_back(goal);
    }
    else
        cout << "Failed, the final facts are: ";
    coutArray(facts);
}