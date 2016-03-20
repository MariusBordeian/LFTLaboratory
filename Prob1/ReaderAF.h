#pragma once

#include<fstream>
#include <vector>

using namespace std;

typedef struct triplete2 {
	int qx;
	string a;
	int qy;
} DeltaPairs2;

class ReaderAF
{
public:
	explicit ReaderAF(string);
	~ReaderAF();
	
	int readConfig();
	string minimizeAFD() const;
	void generateGrammar();
	int indexOfSymb(string) const;
	int indexOfState(string) const;
	int indexOfStateFinal(string) const;
	static bool isIn(string, vector<string>);
	static bool isIn(int, vector<int>);
	void showPathsMatrix() const;
	int validateWord(string);
	void showPath(string, string);
	void updateAccesible();
	void updateInaccesible();
	void updateFinalized();
	void updateUnfinalized();
	void analyzeStates();

	string filePath;
	string currentState;
	string initialState;
	int cntStatesAll;
	int cntSymb;
	int cntStatesFinal;
	vector<string> statesAll;
	vector<string> statesFinal;
	vector<string> symbols;
	vector<vector<string>> pathsMatrix;
	vector<string> &split(const string &s, char delim, vector<string> &elems) const;
	vector<string> split(const string &s, char delim) const;
	static vector<string> arraysMinus(const vector<string>, const vector<string>);
	struct infoStruct
	{
		vector<string> accesible;
		vector<string> inaccesible;
		vector<string> finalized;
		vector<string> unfinalized;
	} statesInfo;

private:
	string getNextState(string);
	
	int indexCurrentState;
	fstream confFile;

};
