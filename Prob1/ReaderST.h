#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

class ReaderST
{
public:
	explicit ReaderST(string);
	~ReaderST();
	
	int ST2AFD();
	int readConfig();
	int indexOfSymb(string) const;
	int indexOfState(string) const;
	int indexOfStateFinal(string) const;
	//int validateWord(string);
	static bool areStatesEqual(vector<string>, vector<string>);
	bool contains(vector<vector<string>>, vector<string>);
	bool contains(vector<string>, string);
	bool hasElementFrom(vector<string>, vector<string>);
	vector<string> getNextStates(vector<string>, string);
	//void generateGrammar();
	void showPathsMatrix() const;
	//void showPath(string word, string startState);
	void precalcEpsInch();
	void showEpsInch();
	vector<string> getEpsInch(string state);
	vector<string> getEpsInch(vector<string> states);
	string vectorToString(vector<string>);

	string Eps = "E"; // eps
	string filePath;
	string currentState;
	string initialState;
	int cntStatesAll;
	int cntSymb;
	int cntStatesFinal;
	map<string, vector<string>> states_ST2AF;
	map<string, vector<string>> states_eps_inch;
	vector<string> statesAll;
	vector<string> statesFinal;
	vector<string> symbols;
	vector<vector<vector<string>>> pathsMatrix;
	vector<string> &split(const string &s, char delim, vector<string> &elems) const;
	vector<string> split(const string &s, char delim) const;

private:
	vector<string> getNextState(string fromState, string withSymbol);
	
	const vector<string> emptyState = { "-" };
	int indexCurrentState;
	fstream confFile;

};
