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
		cntNetermsFinal{ other.cntNetermsFinal },
		cntPaths{ other.cntPaths },
		netermsAll{ other.netermsAll },
		netermsFinal{ other.netermsFinal },
		symbols{ other.symbols },
		pathsMatrix{ other.pathsMatrix },
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

	Grammar() : cntNetermsAll(0), cntSymb(0), cntNetermsFinal(0), cntPaths(0), indexCurrentNeterm(0)
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
	int indexOfSymb(string) const;
	int indexOfNeterm(string) const;
	int indexOfNetermFinal(string) const;
	static bool isEmpty(vector<string>);
	void showPathsMatrix() const;
	int validateWord(string);
	void showPath(string, string);
	void updateAccesible();
	void updateInaccesible();
	void updateFinalized();
	void updateUnfinalized();
	void analyzeNeterms();
	map<int, char> getLabeledNetermd() const;

	string filePath;
	string currentNeterm;
	string initialNeterm;
	int cntNetermsAll;
	int cntSymb;
	int cntNetermsFinal;
	int cntPaths;
	unsigned int nrOfPaths();
	vector<string> netermsAll;
	vector<string> netermsFinal;
	vector<string> symbols;
	vector<vector<string>> pathsMatrix;
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

};
