#include "Grammar.h"
#include "Util.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

Grammar::Grammar(string path) : cntNetermsAll(0), cntSymb(0), cntPaths(0), indexCurrentNeterm(0)
{
	filePath = path;
	confFile.open(filePath, ios::in);
}
Grammar::~Grammar()
{
	confFile.close();

	/*
	for (auto i = 0; i < cntSymb; i++)
	{
	delete[] pathsMatrix[i];
	}

	delete[] pathsMatrix;
	delete[] netermsAll;
	delete[] netermsFinal;
	delete[] symbols;
	//* why this fails here ???
	delete[] netermsInfo.accesible;
	delete[] netermsInfo.inaccesible;
	delete[] netermsInfo.finalized;
	delete[] netermsInfo.unfinalized;
	//*

	delete netermsInfo;

	filePath = nullptr;
	pathsMatrix = nullptr;
	netermsAll = nullptr;
	netermsFinal = nullptr;
	symbols = nullptr;
	netermsInfo.accesible = nullptr;
	netermsInfo.inaccesible = nullptr;
	netermsInfo.finalized = nullptr;
	netermsInfo.unfinalized = nullptr;
	//*/
}
/*
int Grammar::indexOfSymb(string c) const
{
	int i, f = 0;
	for (i = 0; i < cntSymb; i++)
	{
		if (symbols[i].compare(c) == 0)
		{
			f = 1;
			break;
		}
	}
	return (f == 0) ? -1 : i;
}
*/
int Grammar::indexOfNeterm(string c) const
{
	int i, f = 0;
	for (i = 0; i < cntNetermsAll; i++)
	{
		if (netermsAll[i].compare(c) == 0)
		{
			f = 1;
			break;
		}
	}
	return (f == 0) ? -1 : i;
}
/*
int Grammar::indexOfNetermFinal(string c) const
{
	int i, f = 0;
	for (i = 0; i < cntNetermsFinal; i++)
	{
		if (netermsFinal[i].compare(c) == 0)
		{
			f = 1;
			break;
		}
	}
	return (f == 0) ? -1 : i;
}

bool Grammar::isEmpty(vector<string> vec)
{
	unsigned int sumchek = 0;

	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i].compare("-") == 0)
			sumchek++;

	return sumchek == vec.size();
}
*/
void Grammar::showRules() const
{
	cout << "\nRules : \n";
	for (auto values : rules) {
		cout << values.first << "\t";
		for (auto element : values.second) {
			cout << element << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Grammar::showRulesType2() const
{
	int counter = 0;
	cout << "\nRules : \n";
	for (auto values : rules) {
		cout << values.first << "\t";
		counter = 0;
		for (auto element : values.second) {
			cout << element << ((counter!=values.second.size()-1)?"\t| ":"");
			counter++;
		}
		cout << endl;
	}
	cout << endl;
}
void Grammar::removeLeftRedundancy() {
	map<string, int> priorities;
	//vector<string> additionaNeterms;
	for (unsigned int i = 0; i < netermsAll.size(); i++) {
		priorities.insert(pair<string, int>(netermsAll[i], i));
	}

	for (unsigned int i = 0,n= netermsAll.size(); i < n; i++) {
		vector<string> rulesOfLocalNeterm = rules.at(netermsAll[i]);
		vector<vector<string>> redundancyInfo = getRedundancyInfo(rulesOfLocalNeterm, i);
		if (redundancyInfo[0].size()>0 && redundancyInfo[1].size()>0) {
			rules.erase(netermsAll[i]);
			vector<string> newRule1; // Ai rules
			vector<string> newRule2; // Ai' rules -> noul neterminal care se creeaza

			newRule1.insert(newRule1.begin(), redundancyInfo[1].begin(), redundancyInfo[1].end());
			string newNeterm = netermsAll[i];
			newNeterm.append("'");
			for (auto betaRule : redundancyInfo[1]) {
				newRule1.push_back(betaRule.append(newNeterm));
			}
			newRule2.insert(newRule2.begin(), redundancyInfo[0].begin(), redundancyInfo[0].end());
			for (auto alphaRule : redundancyInfo[0]) {
				newRule2.push_back(alphaRule.append(newNeterm));
			}
			rules.insert(pair<string, vector<string>>(netermsAll[i], newRule1));
			rules.insert(pair<string, vector<string>>(newNeterm, newRule2));
		}

		if (i == n - 1) break;

		for (unsigned int j = 0; j <= i; j++) {
			vector<string> localRules = rules.at(netermsAll[i + 1]);
			vector<string> eligibleRules = getEligibleRulesForReplacing(localRules, j);
			if (eligibleRules.size() > 0) {
				vector<string> oldRules = rules.at(netermsAll[i + 1]);
				vector<string> newRules;
				for (auto r:oldRules) {
					if (r[0] != netermsAll[j][0]) {
						newRules.push_back(r);
					}
				}
				rules.erase(netermsAll[i + 1]);
				vector<string> rulesOfAj = rules.at(netermsAll[j]);
				for (auto gamma : eligibleRules) {
					for (auto jrule : rulesOfAj) {
						newRules.push_back(jrule.append(gamma));
					}
				}
				rules.insert(pair<string,vector<string>>(netermsAll[i+1], newRules));
			}
 		}
	}
	netermsAll.clear();
	for (auto r : rules) {
		netermsAll.push_back(r.first);
	}
	cntNetermsAll = netermsAll.size();
}

vector<string> Grammar::getEligibleRulesForReplacing(vector<string> rulesOfLocalNeterm, unsigned int index) {
	vector<string> results;
	string gamma = "";
	for (auto rule : rulesOfLocalNeterm) {
		if (rule[0] == netermsAll[index][0]) {
			gamma = rule.substr(1, rule.size());
			results.push_back(gamma);
		}
	}
	return results;
}

vector<vector<string>> Grammar::getRedundancyInfo(vector<string> rulesOfLocalNeterm,unsigned int index) {
	vector<string> alpha;
	vector<string> beta;
	vector<vector<string>> combined;
	for (int i = 0; i < rulesOfLocalNeterm.size(); i++) {
		if (rulesOfLocalNeterm[i][0] == netermsAll[index][0]) {
			alpha.push_back(rulesOfLocalNeterm[i].substr(1, rulesOfLocalNeterm[i].size()));
		}
		else if (indexOfNeterm(to_string(rulesOfLocalNeterm[i][0]))==-1 || indexOfNeterm(to_string(rulesOfLocalNeterm[i][0])) > index)  {
			beta.push_back(rulesOfLocalNeterm[i]);
		}
	}
	combined.push_back(alpha);
	combined.push_back(beta);
	return combined;
}

void Grammar::cleanUp() {
	vector<string> N1;
	bool change = false;
	do{
		change = false;
		for (auto pair : rules) {
			for (auto vect : pair.second) {
				vector<string> localContainingNeterms = getContainingNeterms(vect);
				if (localContainingNeterms.size()==0 || (arraysIntersection(localContainingNeterms,N1).size()==localContainingNeterms.size())) {
					if (!isIn(pair.first, N1)) {
						N1.push_back(pair.first);
						change = true;
						break;
					}
				}
			}
			
		}
	} while (change);

	auto i = rules.begin();
	while ( i != rules.end()) {
		if (!isIn(i->first, N1) ) {
			i = rules.erase(i);
			//--i;
		}
		else {
			auto j = i->second.begin();
			while (j != i->second.end()) {
				vector<string> localContainingNeterms = getContainingNeterms(*j);
				if (arraysMinus(localContainingNeterms, N1).size()!=0) {
					j = i->second.erase(j);
					//--j;
				}
				else {
					++j;
				}
			}
			++i;
		}
	}
	for (auto it = netermsAll.begin(); it != netermsAll.end();++it) {
		if (rules.find(*it) == rules.end()) {
			it = netermsAll.erase(it);
		}
	}

	//Step 2
	map<string, bool> marcaj;
	for (auto it:netermsAll) {
		marcaj.insert(pair<string, bool>(it, false));
	}
	queue<string> netermFinal;
	netermFinal.push(initialNeterm);
	while (!netermFinal.empty()) {
		string localNeterm = netermFinal.front();
		netermFinal.pop();
		marcaj.at(localNeterm) = true;
		vector<string> localNeterms = getContainingNeterms(rules.at(localNeterm));
		for (auto lNeterm : localNeterms) {
			if (!marcaj.at(lNeterm)) {
				netermFinal.push(lNeterm);
			}
		}
	}

	for (auto it = rules.begin(); it != rules.end();) {
		if (!marcaj.at(it->first)) {
			it = rules.erase(it);
		}
		else {
			it++;
		}
	}

	for (auto it = netermsAll.begin(); it != netermsAll.end(); ++it) {
		if (rules.find(*it) == rules.end()) {
			it = netermsAll.erase(it);
		}
	}

	vector<string> terms;
	for (auto it : rules) {
		terms = arraysUnion(terms, getTerms(it.second));
	}
	symbols = terms;
	cntNetermsAll = netermsAll.size();
	cntSymb = symbols.size();
	
	fstream grammarFile;
	string grammarPath(filePath);
	grammarPath += ".cleanGrm";
	grammarFile.open(grammarPath, ios::out);

	if (grammarFile.is_open())
	{
		grammarFile << cntNetermsAll << endl;

		for (auto n : netermsAll) {
			grammarFile << n << " ";
		}
		
		grammarFile << endl << cntSymb << endl;

		for (auto t : symbols) {
			grammarFile << t << " ";
		}

		grammarFile << endl << initialNeterm << endl;

		auto i = 0, j = 0;
		for (auto rule : rules) {
			i++;
			for (auto v : rule.second) {
				j++;
				grammarFile << rule.first << " " << v;
				if (!(j == rule.second.size() && i == rules.size())) {
					grammarFile << endl;
				}
			}
			j = 0;
		}

		cout << "check " << grammarPath << endl;
		grammarFile.close();
	}
	else
	{
		cout << "\nError create grammar file!\n";
	}
}
vector<string> Grammar::getContainingNeterms(string in) {
	vector<string> temp;
	for (unsigned int i = 0; i < in.length(); i++) {
		if (isIn(in[i], netermsAll)) {
			if (!isIn(in[i], temp)) {
				temp.push_back(string(1, in[i]));
			}
			
		}
	}
	return temp;
}
vector<string> Grammar::getContainingNeterms(vector<string> in) {
	vector<string> temp;
	for ( auto j:in){
		for (unsigned int i = 0; i < j.length(); i++) {
			if (isIn(j[i], netermsAll)) {
				if (!isIn(j[i], temp)) {
					temp.push_back(string(1, j[i]));
				}
			}
		}
	}
	return temp;
}
vector<string> Grammar::getTerms(vector<string> in) {
	vector<string> temp;
	for (auto j : in) {
		for (unsigned int i = 0; i < j.length(); i++) {
			if (isIn(j[i], symbols)) {
				if (!isIn(j[i], temp)) {
					temp.push_back(string(1, j[i]));
				}
			}
		}
	}
	return temp;
}
/*
int Grammar::validateWord(string w)
{
	auto result = 0, valid = 1;
	auto wordSymbols = split(w, ' ');
	int len = wordSymbols.size();

	for (auto i = 0; i < len; i++)
	{
		if (indexOfSymb(wordSymbols[i]) == -1)
		{
			valid = 0;
			break;
		}
	}

	if (valid != 0)
	{
		for (auto i = 0; i < len; i++)
		{
			if (getNextNeterm(wordSymbols[i]).compare("-") == 0)
			{
				break;
			}
		}

		if (indexOfNetermFinal(currentNeterm) != -1)
			result = 1;
	}
	else
	{
		cout << "invalid word!\n";
	}
	currentNeterm = initialNeterm;
	indexCurrentNeterm = indexOfNeterm(currentNeterm);

	return result;
}

void Grammar::showPath(string w, string startNeterm)
{
	currentNeterm = startNeterm;
	indexCurrentNeterm = indexOfNeterm(currentNeterm);

	auto wordSymbols = split(w, ' ');
	int len = wordSymbols.size();

	auto valid = 1;

	for (auto i = 0; i < len; i++)
	{
		if (indexOfSymb(wordSymbols[i]) == -1)
		{
			valid = 0;
			break;
		}
	}

	if (valid != 0)
	{
		cout << "path for \"" << w << "\" from neterm \"" << startNeterm << "\" is : " << currentNeterm;
		for (auto i = 0; i < len; i++)
		{
			auto s = getNextNeterm(wordSymbols[i]);
			if (s.compare("-") != 0)
			{
				cout << " -> " << s;
			}
			else
			{
				cout << "; dead-end!";
				break;
			}
		}
	}
	else
	{
		cout << "not a valid word!";
	}

	currentNeterm = initialNeterm;
	indexCurrentNeterm = indexOfNeterm(currentNeterm);
}

void Grammar::updateAccesible()
{
	vector<int> mystack;
	vector<bool> mycheck;
	mycheck.resize(cntNetermsAll);
	auto initialNetermIndex = indexOfNeterm(initialNeterm);
	netermsInfo.accesible.clear();
	netermsInfo.accesible.resize(cntNetermsAll);
	for (auto i = 0; i < cntNetermsAll; i++)
	{
		if (i != initialNetermIndex)
		{
			netermsInfo.accesible[i] = "-";
			mycheck[i] = false;
		}
		else
		{
			netermsInfo.accesible[i] = initialNeterm;
			mycheck[i] = true;
		}
	}

	for (auto it = rules[initialNetermIndex].begin(); it != rules[initialNetermIndex].end(); ++it)
	{
		auto auxIndex = indexOfNeterm(*it);
		if (auxIndex > -1)
		{
			mystack.push_back(auxIndex);
		}
	}

	auto i = mystack.begin();
	while (i != mystack.end())
	{
		auto localIndex = *i;
		mystack.erase(i);
		mycheck[localIndex] = true;

		netermsInfo.accesible[localIndex] = netermsAll[localIndex];

		for (auto j = rules[localIndex].begin(); j != rules[localIndex].end(); ++j)
		{
			auto auxIndex = indexOfNeterm(*j);
			if (auxIndex > -1 && !mycheck[auxIndex])
			{
				mystack.push_back(auxIndex);
			}
		}
		i = mystack.begin();
	}
}

void Grammar::updateInaccesible()
{
	if (netermsInfo.accesible.size() == 0)
		updateAccesible();

	netermsInfo.inaccesible.clear();
	netermsInfo.inaccesible.resize(cntNetermsAll);
	for (auto i = 0; i < cntNetermsAll; i++)
		netermsInfo.inaccesible[i] = "-";

	for (auto i = 0; i < cntNetermsAll; i++)
	{
		if (netermsInfo.accesible[i].compare("-") == 0)
		{
			netermsInfo.inaccesible[i] = netermsAll[i];
		}
	}
}

void Grammar::updateFinalized()
{
	if (netermsInfo.unfinalized.size() == 0)
		updateAccesible();

	netermsInfo.finalized.clear();
	netermsInfo.finalized.resize(cntNetermsAll);
	for (auto i = 0; i < cntNetermsAll; i++)
	{
		if (isIn(netermsAll[i], netermsInfo.accesible) &&
			indexOfNetermFinal(netermsAll[i]) > -1)
		{
			netermsInfo.finalized[i] = netermsAll[i];
		}
		else
		{
			netermsInfo.finalized[i] = "-";
		}
	}


	/* // not sure of this solution
	auto symCheck = 0;
	for (auto i = 0; i < cntNetermsAll; i++)
	{
	for (auto j = 0; j < cntSymb; j++)
	{
	if (isIn(pathsMatrix[i][j], netermsInfo.accesible))
	{
	if (pathsMatrix[i][j] == "-")
	symCheck++;
	}
	else
	break;
	}
	if ((symCheck == cntSymb) && (indexOfNetermFinal(netermsAll[i]) > -1))
	{
	netermsInfo.finalized[i] = netermsAll[i];
	symCheck = 0;
	}
	else
	{
	symCheck = 0;
	}
	}
}

void Grammar::updateUnfinalized()
{
	if (netermsInfo.inaccesible.size() == 0)
		updateAccesible();

	netermsInfo.unfinalized.clear();
	netermsInfo.unfinalized.resize(cntNetermsAll);
	for (auto i = 0; i < cntNetermsAll; i++)
		netermsInfo.unfinalized[i] = "-";

	auto symCheck = 0;
	for (auto i = 0; i < cntNetermsAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			if (isIn(netermsAll[i], netermsInfo.accesible))
			{
				if (rules[i][j] == "-")
					symCheck++;
			}
			else
			{
				symCheck = 0;
				break;
			}
		}
		if ((symCheck == cntSymb) && (indexOfNetermFinal(netermsAll[i]) == -1))
		{
			netermsInfo.unfinalized[i] = netermsAll[i];
			symCheck = 0;
		}
		else
		{
			symCheck = 0;
		}
	}
}

void Grammar::analyzeNeterms()
{
	// NOTE : order matters!
	updateAccesible();
	updateInaccesible();
	updateUnfinalized();
	updateFinalized();

	cout << "\nfor initial neterm \"" << initialNeterm << "\" following are true";

	cout << "\naccesible neterms\t: ";
	for (auto i = 0; i < cntNetermsAll; i++)
		cout << netermsInfo.accesible[i] << " ";

	cout << "\ninaccesible neterms\t: ";
	for (auto i = 0; i < cntNetermsAll; i++)
		cout << netermsInfo.inaccesible[i] << " ";

	cout << "\nfinalized neterms\t: ";
	for (auto i = 0; i < cntNetermsAll; i++)
		cout << netermsInfo.finalized[i] << " ";

	cout << "\nunfinalized neterms\t: ";
	for (auto i = 0; i < cntNetermsAll; i++)
		cout << netermsInfo.unfinalized[i] << " ";
}

map<int, char> Grammar::getLabeledNetermd() const
{
	map<int, char> labels;

	for (auto i = 0; i < cntNetermsAll; i++)
	{
		if (isIn(netermsAll[i], netermsFinal))
		{
			labels.insert(pair<int, char>(i, 'F'));
		}
		else
		{
			labels.insert(pair<int, char>(i, 'N'));
		}
	}

	return labels;
}

unsigned int Grammar::nrOfPaths()
{
	unsigned int counter = 0;

	for (auto i = 0; i < cntNetermsAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			if (rules[i][j].compare("-") != 0)
			{
				counter++;
			}
		}
	}

	cntPaths = counter;

	return counter;
}

string Grammar::getNextNeterm(string symb)
{
	string result = "-";
	auto indexSymb = indexOfSymb(symb);

	if (indexSymb != -1 && rules[indexCurrentNeterm][indexSymb].compare("-") != 0)
	{
		result = rules[indexCurrentNeterm][indexSymb];
		currentNeterm = result;
		indexCurrentNeterm = indexOfNeterm(currentNeterm);
	}

	return result;
}

*/
int Grammar::readConfig()
{
	if (!confFile.is_open())
	{
		cout << "\nconfig file not opened!\n";
		return 1;
	}

	netermsAll.clear();
	symbols.clear();
	int p_m_len = rules.size();
	if (p_m_len > 0)
	{
		rules.clear();
	}

	string tmpRead;

	cout << "\n***********************************************************************";
	confFile >> cntNetermsAll;
	cout << endl << cntNetermsAll << " : neterms\t\t:";

	for (auto i = 0; i < cntNetermsAll; i++)
	{
		confFile >> tmpRead;
		netermsAll.push_back(tmpRead);
		cout << " " << netermsAll[i];
	}

	confFile >> cntSymb;
	cout << endl << cntSymb << " : symbols\t\t:";

	for (auto i = 0; i < cntSymb; i++)
	{
		confFile >> tmpRead;
		symbols.push_back(tmpRead);
		cout << " " << symbols[i];
	}

	confFile >> initialNeterm;
	cout << endl << initialNeterm << " : initial neterm";
	currentNeterm = initialNeterm;
	indexCurrentNeterm = indexOfNeterm(currentNeterm);

	cout << "\n***********************************************************************\n";


	string ci, cj;
	while (!confFile.eof())
	{
		confFile >> ci;
		confFile >> cj;

		if (rules.find(ci)!=rules.end())
		{
			rules.at(ci).push_back(cj);
		}
		else
		{
			vector<string> tempVect;
			tempVect.push_back(cj);
			rules.insert(pair<string, vector<string>>(ci, tempVect));
		}
	}
	return 0;
}
/*
void Grammar::removeUselessNeterms()
{
	updateAccesible();
	updateInaccesible();
	updateFinalized();
	updateUnfinalized();

	if (!isEmpty(netermsInfo.inaccesible))
	{
		vector<vector<string>> paths;
		vector<string> neterms;
		vector<string> finals;

		for (unsigned int i = 0; i < netermsInfo.accesible.size(); i++)
		{
			auto localIndex = indexOfNeterm(netermsInfo.accesible[i]);
			if (localIndex > -1)
			{
				paths.push_back(rules[localIndex]);
				neterms.push_back(netermsInfo.accesible[i]);
				if (indexOfNetermFinal(netermsInfo.accesible[i]) > -1)
					finals.push_back(netermsInfo.accesible[i]);
			}
		}

		rules = paths;
		netermsAll = neterms;
		netermsFinal = finals;

		cntNetermsAll = netermsAll.size();
		cntNetermsFinal = netermsFinal.size();

		currentNeterm = initialNeterm;
		indexCurrentNeterm = indexOfNeterm(initialNeterm);

		nrOfPaths();

		updateAccesible();
		updateInaccesible();
		updateFinalized();
		updateUnfinalized();
	}
}

void Grammar::generateGrammar()	// TODO: S -> epsilon => S'(just not to remove S) -> epsilon
{
	string netermStart = "S";
	string netermFinal = "F";
	string netermPrefix = "N";

	fstream grammarFile;

	string grammarPath(filePath);
	grammarPath += ".grammar";

	grammarFile.open(grammarPath, ios::out);

	if (grammarFile.is_open())
	{
		grammarFile << cntNetermsAll << endl;
		for (auto i = 0; i < cntNetermsAll; i++)
		{
			grammarFile << ((netermsAll[i].compare(initialNeterm) == 0) ? netermStart : ((indexOfNetermFinal(netermsAll[i]) > -1) ? netermFinal + to_string(i) : netermPrefix + to_string(i))) << " ";
		}
		grammarFile << endl;

		grammarFile << cntSymb << endl;
		for (auto i = 0; i < cntSymb; i++)
		{
			grammarFile << symbols[i] << " ";
		}
		grammarFile << endl;

		grammarFile << netermStart << endl;

		grammarFile << cntNetermsFinal << endl;
		for (auto i = 0; i < cntNetermsFinal; i++)
		{
			grammarFile << netermFinal + to_string(indexOfNeterm(netermsFinal[i])) << " ";
		}
		grammarFile << endl;

		for (auto i = 0; i < cntNetermsAll; i++)
		{
			auto localNeterm = netermsAll[i];
			auto localNetermIndex = indexOfNeterm(localNeterm);

			for (auto j = 0; j < cntSymb; j++)
			{
				auto localNext = rules[i][j];
				auto localNextIndex = indexOfNeterm(localNext);

				if (localNextIndex > -1)
				{
					if (localNeterm.compare(initialNeterm) == 0)
					{
						if (indexOfNetermFinal(localNext) != -1)
						{
							grammarFile << netermStart << " -> " << symbols[j] << netermFinal + to_string(localNextIndex) << endl;
							grammarFile << netermStart << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermStart << " -> " << symbols[j] << (localNext.compare(initialNeterm) == 0 ? netermStart : netermPrefix + to_string(localNextIndex)) << endl;
						}
					}
					else if (indexOfNetermFinal(localNeterm) > -1)
					{
						if (indexOfNetermFinal(localNext) != -1)
						{
							grammarFile << netermFinal + to_string(localNetermIndex) << " -> " << symbols[j] << (
								(indexOfNetermFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialNeterm) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
							grammarFile << netermFinal + to_string(localNetermIndex) << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermFinal + to_string(localNetermIndex) << " -> " << symbols[j] << (
								(indexOfNetermFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialNeterm) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
						}
					}
					else
					{
						if (indexOfNetermFinal(localNext) != -1)
						{
							grammarFile << netermPrefix + to_string(localNetermIndex) << " -> " << symbols[j] << (
								(indexOfNetermFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialNeterm) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
							grammarFile << netermPrefix + to_string(localNetermIndex) << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermPrefix + to_string(localNetermIndex) << " -> " << symbols[j] << (
								(indexOfNetermFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialNeterm) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
						}
					}
				}
			}
		}

		for (auto i = 0; i < cntNetermsFinal; i++)
		{
			grammarFile << netermFinal + to_string(indexOfNeterm(netermsFinal[i])) << " -> epsilon" << endl;
		}

		cout << "check " << grammarPath << endl;
		grammarFile.close();
	}
	else
	{
		cout << "\nError create grammar file!\n";
	}
}

bool operator==(const Grammar & lhs, const Grammar & rhs)
{
	auto result = true;
	auto l(lhs);
	auto r(rhs);

	if (l.symbols.size() == r.symbols.size() && areEqual(l.symbols, r.symbols))
	{
		l.minimizeAFD();
		r.minimizeAFD();

		if (l.netermsAll.size() == r.netermsAll.size() &&
			l.netermsFinal.size() == r.netermsFinal.size() &&
			l.cntPaths == r.cntPaths)
		{
			vector<bool> l_check;
			vector<bool> r_check;
			for (auto i = 0; i < l.cntNetermsAll; i++)
			{
				l_check.push_back(false);
				r_check.push_back(false);
			}

			queue<int> l_q;
			queue<int> r_q;

			l_q.push(l.indexOfNeterm(l.initialNeterm));
			r_q.push(r.indexOfNeterm(r.initialNeterm));

			map<int, char> l_map = l.getLabeledNetermd();
			map<int, char> r_map = r.getLabeledNetermd();

			while (!l_q.empty() && !r_q.empty())
			{
				auto l_index = l_q.front();
				l_q.pop();
				l_check[l_index] = true;

				auto r_index = r_q.front();
				r_q.pop();
				r_check[r_index] = true;

				if (l_map.at(l_index) == r_map.at(r_index))
				{
					vector<string> l_paths = l.rules[l_index];
					vector<string> r_paths = r.rules[r_index];

					for (auto i = 0; i < l.cntSymb; i++)
					{
						auto l_localIndex = l.indexOfNeterm(l_paths[i]);
						auto r_localIndex = r.indexOfNeterm(r_paths[i]);

						if (l_localIndex != -1 && r_localIndex != -1)
						{
							if (!l_check[l_localIndex] && !r_check[r_localIndex])
							{
								l_q.push(l_localIndex);
								r_q.push(r_localIndex);
							}
							else
							{
								if (!l_check[l_localIndex] != !r_check[r_localIndex])
								{
									result = false;
									break;
								}
							}
						}
						else
						{
							if (l_localIndex != r_localIndex)
							{
								result = false;
								break;
							}
						}
					}
				}
				else
				{
					result = false;
					break;
				}
			}
		}
	}
	else
	{
		result = false;
	}

	return result;
}
*/