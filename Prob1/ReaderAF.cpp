#include "ReaderAF.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

ReaderAF::ReaderAF(string path): cntStatesAll(0), cntSymb(0), cntStatesFinal(0), indexCurrentState(0)
{
	filePath = path;
	confFile.open(filePath, ios::in);
	
	if (!confFile)
	{
		cout << "Wrong file path!";
		system("pause");
		exit(1);
	}
}

ReaderAF::~ReaderAF()
{
	confFile.close();

	/*
	for (auto i = 0; i < cntSymb; i++)
	{
		delete[] pathsMatrix[i];
	}

	delete[] pathsMatrix;
	delete[] statesAll;
	delete[] statesFinal;
	delete[] symbols;
	//* why this fails here ???
	delete[] statesInfo.accesible;
	delete[] statesInfo.inaccesible;
	delete[] statesInfo.finalized;
	delete[] statesInfo.unfinalized;
	//*
	
	delete statesInfo;

	filePath = nullptr;
	pathsMatrix = nullptr;
	statesAll = nullptr;
	statesFinal = nullptr;
	symbols = nullptr;
	statesInfo.accesible = nullptr;
	statesInfo.inaccesible = nullptr;
	statesInfo.finalized = nullptr;
	statesInfo.unfinalized = nullptr;
	//*/
}

vector<string>& ReaderAF::split(const string &s, char delim, vector<string> &elems) const
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ReaderAF::split(const string &s, char delim) const
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

int ReaderAF::indexOfSymb(string c) const
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

int ReaderAF::indexOfState(string c) const
{
	int i, f = 0;
	for (i = 0; i < cntStatesAll; i++)
	{
		if (statesAll[i].compare(c) == 0)
		{
			f = 1;
			break;
		}
	}
	return (f == 0) ? -1 : i;
}

int ReaderAF::indexOfStateFinal(string c) const
{
	int i, f = 0;
	for (i = 0; i < cntStatesFinal; i++)
	{
		if (statesFinal[i].compare(c) == 0)
		{
			f = 1;
			break;
		}
	}
	return (f == 0) ? -1 : i;
}

void ReaderAF::showPathsMatrix() const
{
	cout << "\nMatrix : \n\t";
	for (auto i = 0; i < cntSymb; i++)
		cout << symbols[i] << "\t";
	cout << endl;
	for (auto i = 0; i < cntStatesAll; i++)
	{
		cout << statesAll[i] << "\t";
		for (auto j = 0; j < cntSymb; j++)
		{
			cout << pathsMatrix[i][j] << "\t";
		}
		cout << endl;
	}
}

int ReaderAF::validateWord(string w)
{
	auto result = 0, valid = 1;
	auto wordSymbols = split(w,' ');
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
			if (getNextState(wordSymbols[i]).compare("-") == 0)
			{
				break;
			}
		}

		if (indexOfStateFinal(currentState) != -1)
			result = 1;
	}
	else
	{
		cout << "invalid word!\n";
	}
	currentState = initialState;
	indexCurrentState = indexOfState(currentState);

	return result;
}

void ReaderAF::showPath(string w, string startState)
{
	currentState = startState;
	indexCurrentState = indexOfState(currentState);
	
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
		cout << "path for \"" << w << "\" from state \"" << startState << "\" is : " << currentState;
		for (auto i = 0; i < len; i++)
		{
			auto s = getNextState(wordSymbols[i]);
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

	currentState = initialState;
	indexCurrentState = indexOfState(currentState);
}

void ReaderAF::analyzeStates()
{
	statesInfo.accesible.clear();
	statesInfo.accesible.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.accesible[i] = "-";

	statesInfo.inaccesible.clear();
	statesInfo.inaccesible.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.inaccesible[i] = "-";

	statesInfo.finalized.clear();
	statesInfo.finalized.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.finalized[i] = "-";

	statesInfo.unfinalized.clear();
	statesInfo.unfinalized.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.unfinalized[i] = "-";


	for (auto i = 0; i < cntStatesAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			auto index = indexOfState(pathsMatrix[i][j]);
			if (index > -1)
				statesInfo.accesible[index] = pathsMatrix[i][j];
		}
	}

	for (auto i = 0; i < cntStatesAll; i++)
	{
		if (statesInfo.accesible[i].compare("-") == 0)
		{
			statesInfo.inaccesible[i] = statesAll[i];
		}
	}

	auto symCheck = 0;
	for (auto i = 0; i < cntStatesAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			if (pathsMatrix[i][j] == "-")
				symCheck++;
		}
		if ((symCheck == cntSymb) && (indexOfStateFinal(statesAll[i]) == -1))
		{
			statesInfo.unfinalized[i] = statesAll[i];
			symCheck = 0;
		}
		else
		{
			symCheck = 0;
		}
	}

	for (auto i = 0; i < cntStatesAll; i++)
	{
		if ((statesInfo.unfinalized[i].compare("-") == 0) && 
			(indexOfStateFinal(statesAll[i]) == -1))
		{
			statesInfo.finalized[i] = statesAll[i];
		}
	}


	cout << "\nfor initial state \"" << initialState << "\" following are true";

	cout << "\naccesible states\t: ";
	for (auto i = 0; i < cntStatesAll; i++)
		cout << statesInfo.accesible[i] << " ";

	cout << "\ninaccesible states\t: ";
	for (auto i = 0; i < cntStatesAll; i++)
		cout << statesInfo.inaccesible[i] << " ";

	cout << "\nfinalized states\t: ";
	for (auto i = 0; i < cntStatesAll; i++)
		cout << statesInfo.finalized[i] << " ";

	cout << "\nunfinalized states\t: ";
	for (auto i = 0; i < cntStatesAll; i++)
		cout << statesInfo.unfinalized[i] << " ";
}

string ReaderAF::getNextState(string symb)
{
	string result = "-";
	auto indexSymb = indexOfSymb(symb);

	if (indexSymb != -1 && pathsMatrix[indexCurrentState][indexSymb].compare("-") != 0)
	{
		result = pathsMatrix[indexCurrentState][indexSymb];
		currentState = result;
		indexCurrentState = indexOfState(currentState);
	}

	return result;
}

int ReaderAF::readConfig()
{
	if (!confFile.is_open())
	{
		cout << "\nconfig file not opened!\n";
		return 1;
	}

	statesAll.clear();
	statesFinal.clear();
	symbols.clear();
	int p_m_len = pathsMatrix.size();
	if (p_m_len > 0)
	{
		for (auto i = 0; i < p_m_len; i++)
		{
			pathsMatrix[i].clear();
		}

		pathsMatrix.clear();
	}

	string tmpRead;

	cout << "\n***********************************************************************";
	confFile >> cntStatesAll;
	cout << endl << cntStatesAll << " : states\t\t:";

	for (auto i = 0; i < cntStatesAll; i++)
	{
		confFile >> tmpRead;
		statesAll.push_back(tmpRead);
		cout << " " << statesAll[i];
	}

	confFile >> cntSymb;
	cout << endl << cntSymb << " : symbols\t\t:";

	for (auto i = 0; i < cntSymb; i++)
	{
		confFile >> tmpRead;
		symbols.push_back(tmpRead);
		cout << " " << symbols[i];
	}

	confFile >> initialState;
	cout << endl << initialState << " : initial state";
	currentState = initialState;
	indexCurrentState = indexOfState(currentState);

	confFile >> cntStatesFinal;
	cout << endl << cntStatesFinal << " : final states\t:";

	for (auto i = 0; i < cntStatesFinal; i++)
	{
		confFile >> tmpRead;
		statesFinal.push_back(tmpRead);
		cout << " " << statesFinal[i];
	}

	cout << "\n***********************************************************************\n";

	pathsMatrix.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
	{
		pathsMatrix[i].resize(cntSymb);
	}

	for (auto i = 0; i < cntStatesAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			pathsMatrix[i][j] = "-";
		}
	}

	string ci, cj, cs;
	int i, j;
	while (!confFile.eof() && confFile.peek() != '\n')
	{
		confFile >> ci;
		confFile >> cj;
		confFile >> cs;

		if ((i = indexOfState(ci)) != -1 &&
			(j = indexOfSymb(cj)) != -1 &&
			(indexOfState(cs)) != -1)
		{
			pathsMatrix[i][j] = cs;
		}
		else
		{
			cout << "\nwrong paths matrix in config file!\n";
			return 1;
		}
	}

	return 0;
}

void ReaderAF::generateGrammar()	// TODO: S -> epsilon => S'(just not to remove S) -> epsilon
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
		grammarFile << cntStatesAll << endl;
		for (auto i = 0;i < cntStatesAll; i++)
		{
			grammarFile << ((statesAll[i].compare(initialState) == 0) ? netermStart : ((indexOfStateFinal(statesAll[i]) > -1) ? netermFinal + to_string(i) : netermPrefix + to_string(i)) ) << " ";
		}
		grammarFile << endl;

		grammarFile << cntSymb << endl;
		for (auto i = 0;i < cntSymb; i++)
		{
			grammarFile << symbols[i] << " ";
		}
		grammarFile << endl;

		grammarFile << netermStart << endl;

		grammarFile << cntStatesFinal << endl;
		for (auto i = 0; i < cntStatesFinal; i++)
		{
			grammarFile << netermFinal + to_string(indexOfState(statesFinal[i])) << " ";
		}
		grammarFile << endl;

		for (auto i = 0; i < cntStatesAll; i++)
		{
			auto localState = statesAll[i];
			auto localStateIndex = indexOfState(localState);

			for (auto j = 0; j < cntSymb; j++)
			{
				auto localNext = pathsMatrix[i][j];
				auto localNextIndex = indexOfState(localNext);
				
				if (localNextIndex > -1)
				{
					if (localState.compare(initialState) == 0)
					{
						if (indexOfStateFinal(localNext) != -1)
						{
							grammarFile << netermStart << " -> " << symbols[j] << netermFinal + to_string(localNextIndex) << endl;
							grammarFile << netermStart << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermStart << " -> " << symbols[j] << (localNext.compare(initialState) == 0 ? netermStart : netermPrefix + to_string(localNextIndex)) << endl;
						}
					}
					else if (indexOfStateFinal(localState) > -1)
					{
						if (indexOfStateFinal(localNext) != -1)
						{
							grammarFile << netermFinal + to_string(localStateIndex) << " -> " << symbols[j] << (
								(indexOfStateFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialState) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
							grammarFile << netermFinal + to_string(localStateIndex) << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermFinal + to_string(localStateIndex) << " -> " << symbols[j] << (
								(indexOfStateFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialState) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
						}
					}
					else
					{
						if (indexOfStateFinal(localNext) != -1)
						{
							grammarFile << netermPrefix + to_string(localStateIndex) << " -> " << symbols[j] << (
								(indexOfStateFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialState) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
							grammarFile << netermPrefix + to_string(localStateIndex) << " -> " << symbols[j] << endl;
						}
						else
						{
							grammarFile << netermPrefix + to_string(localStateIndex) << " -> " << symbols[j] << (
								(indexOfStateFinal(localNext) != -1) ? netermFinal + to_string(localNextIndex) : (
									(localNext.compare(initialState) == 0) ? netermStart : netermPrefix + to_string(localNextIndex)
									)
								) << endl;
						}
					}
				}
			}
		}

		for (auto i = 0; i < cntStatesFinal; i++)
		{
			grammarFile << netermFinal + to_string(indexOfState(statesFinal[i])) << " -> epsilon" << endl;
		}

		cout << "check " << grammarPath << endl;
		grammarFile.close();
	}
	else
	{
		cout << "\nError create grammar file!\n";
	}
}
