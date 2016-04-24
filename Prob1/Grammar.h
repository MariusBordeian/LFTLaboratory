#pragma once

#include <map>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


class Grammar
{
public:
	Grammar(string);

	Grammar(const Grammar& other)
		: filePath{ other.filePath },
		currentNeterm{ other.currentNeterm },
		initialNeterm{ other.initialNeterm },
		cntNetermsAll{ other.cntNetermsAll },
		cntSymb{ other.cntSymb },
		cntPaths{ other.cntPaths },
		netermsAll{ other.netermsAll },
		symbols{ other.symbols },
		rules{ other.rules },
		netermsInfo{ other.netermsInfo.accesible,
		other.netermsInfo.inaccesible,
		other.netermsInfo.finalized,
		other.netermsInfo.unfinalized },
		indexCurrentNeterm{ other.indexCurrentNeterm }
	{
		confFile.open(filePath, ios::in);
	}

	Grammar operator=(Grammar other)
	{
		return Grammar(other);
	}

	Grammar() : cntNetermsAll(0), cntSymb(0), cntPaths(0), indexCurrentNeterm(0)
	{ }
	~Grammar();

	friend bool operator==(const Grammar& lhs, const Grammar& rhs);

	friend bool operator!=(const Grammar& lhs, const Grammar& rhs)
	{
		return !(lhs == rhs);
	}

	int readConfig();
	string minimizeAFD();
	void removeUselessNeterms();
	void generateGrammar();
	void firstFollow();
	vector<string> getFirst(string);
	vector<string> getFollow(string);
	int indexOfSymb(string) const;
	vector<vector<string>> getRecursionInfo(vector<string>, int);
	vector<string> getEligibleRulesForReplacing(vector<string>, unsigned int);
	int indexOfNeterm(string) const;
	int indexOfNetermFinal(string) const;
	static bool isEmpty(vector<string>);
	void showRules() const;
	void showRulesType2() const;
	void cleanUp();
	vector<string> getContainingNeterms(string) const;
	vector<string> getContainingNeterms(vector<string>) const;
	vector<string> getTerms(vector<string>) const;
	vector<tuple<string, int, int>> getKeysContainingValue(string) const;
	map<string,vector<string>> getRulesContainingRightValue(string);
	vector<string> getRulesContainingKeyAndValue(string,string);
	vector<string> getRulesByKey(string);
	
	bool checkWordForGrammar(string word);
	int validateWord(string);
	void showPath(string, string);
	void updateAccesible();
	void updateInaccesible();
	void updateFinalized();
	void updateUnfinalized();
	void analyzeNeterms();
	void removeLeftRecursion();
	void parse(string, int, int);
	map<int, char> getLabeledNetermd() const;

	string filePath;
	string currentNeterm;
	string initialNeterm;
	int cntNetermsAll;
	int cntSymb;
	int cntPaths;
	unsigned int nrOfPaths();
	vector<string> netermsAll;
	vector<string> symbols;
	map<string,vector<string>> rules;
	vector<vector<vector<tuple<string, int, int>>>> V;		// CYK
	
	map<string, vector<string>> first;						// First
	map<string, vector<string>> follow;						// Follow

	struct infoStruct
	{
		vector<string> accesible;
		vector<string> inaccesible;
		vector<string> finalized;
		vector<string> unfinalized;
	} netermsInfo;

private:
	string getNextNeterm(string);

	int indexCurrentNeterm;
	fstream confFile;

	unsigned int parse_stack_calls = 0;
};
