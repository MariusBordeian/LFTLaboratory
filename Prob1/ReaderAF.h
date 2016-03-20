#pragma once

#include <fstream>
#include <vector>
#include <map>

using namespace std;

typedef struct triplete2 {
	int qx;
	string a;
	int qy;
} DeltaPairs2;

class ReaderAF
{
public:
	ReaderAF(string);

	ReaderAF(const ReaderAF& other)
		: filePath{other.filePath},
		  currentState{other.currentState},
		  initialState{other.initialState},
		  cntStatesAll{other.cntStatesAll},
		  cntSymb{other.cntSymb},
		  cntStatesFinal{other.cntStatesFinal},
		  cntPaths{other.cntPaths},
		  statesAll{other.statesAll},
		  statesFinal{other.statesFinal},
		  symbols{other.symbols},
		  pathsMatrix{other.pathsMatrix},
		  statesInfo{other.statesInfo.accesible, 
					other.statesInfo.inaccesible, 
					other.statesInfo.finalized, 
					other.statesInfo.unfinalized},
		  indexCurrentState{other.indexCurrentState}
	{
		confFile.open(filePath, ios::in);
	}

	ReaderAF operator=(ReaderAF other)
	{
		return ReaderAF(other);
	}

	ReaderAF(): cntStatesAll(0), cntSymb(0), cntStatesFinal(0), cntPaths(0), indexCurrentState(0)
	{ }
	~ReaderAF();

	friend bool operator==(const ReaderAF& lhs, const ReaderAF& rhs);

	friend bool operator!=(const ReaderAF& lhs, const ReaderAF& rhs)
	{
		return !(lhs == rhs);
	}

	int readConfig();
	string minimizeAFD();
	void removeUselessStates();
	void generateGrammar();
	int indexOfSymb(string) const;
	int indexOfState(string) const;
	int indexOfStateFinal(string) const;
	static bool isEmpty(vector<string>);
	void showPathsMatrix() const;
	int validateWord(string);
	void showPath(string, string);
	void updateAccesible();
	void updateInaccesible();
	void updateFinalized();
	void updateUnfinalized();
	void analyzeStates();
	map<int, char> getLabeledStated() const;

	string filePath;
	string currentState;
	string initialState;
	int cntStatesAll;
	int cntSymb;
	int cntStatesFinal;
	int cntPaths;
	unsigned int nrOfPaths();
	vector<string> statesAll;
	vector<string> statesFinal;
	vector<string> symbols;
	vector<vector<string>> pathsMatrix;
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
