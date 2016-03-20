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

vector<string> ReaderAF::arraysMinus(const vector<string> fromArray, const vector<string> whatArray)
{
	vector<string> result;
	for (unsigned int it = 0; it < fromArray.size(); ++it)
	{
		if (!isIn(fromArray[it], whatArray))
		{
			result.push_back(fromArray[it]);
		}
	}
	
	return result;
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

bool ReaderAF::isIn(string a, vector<string> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i].compare(a) == 0)
			return true;
	}
	return false;
}

bool ReaderAF::isIn(int a, vector<int> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i] == a)
			return true;
	}
	return false;
}

bool ReaderAF::isEmpty(vector<string> vec)
{
	unsigned int sumchek = 0;

	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i].compare("-") == 0)
			sumchek++;

	return sumchek == vec.size();
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

void ReaderAF::updateAccesible()
{
	vector<int> mystack;
	vector<bool> mycheck;
	mycheck.resize(cntStatesAll);
	auto initialStateIndex = indexOfState(initialState);
	statesInfo.accesible.clear();
	statesInfo.accesible.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
	{
		if (i != initialStateIndex)
		{
			statesInfo.accesible[i] = "-";
			mycheck[i] = false;
		}
		else
		{
			statesInfo.accesible[i] = initialState;
			mycheck[i] = true;
		}
	}
	
	for (auto it = pathsMatrix[initialStateIndex].begin(); it != pathsMatrix[initialStateIndex].end(); ++it)
	{
		auto auxIndex = indexOfState(*it);
		if (auxIndex > -1) 
		{
			mystack.push_back(auxIndex);
		}
	}

	auto i = mystack.begin();
	while ( i != mystack.end() )
	{
		auto localIndex = *i;
		mystack.erase(i);
		mycheck[localIndex] = true;
		
		statesInfo.accesible[localIndex] = statesAll[localIndex];
		
		for (auto j = pathsMatrix[localIndex].begin(); j != pathsMatrix[localIndex].end(); ++j)
		{
			auto auxIndex = indexOfState(*j);
			if (auxIndex > -1 && !mycheck[auxIndex])
			{
				mystack.push_back(auxIndex);				
			}
		}
		i = mystack.begin();
	}
}

void ReaderAF::updateInaccesible()
{
	if (statesInfo.accesible.size() == 0)
		updateAccesible();

	statesInfo.inaccesible.clear();
	statesInfo.inaccesible.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.inaccesible[i] = "-";

	for (auto i = 0; i < cntStatesAll; i++)
	{
		if (statesInfo.accesible[i].compare("-") == 0)
		{
			statesInfo.inaccesible[i] = statesAll[i];
		}
	}
}

void ReaderAF::updateFinalized()
{
	if (statesInfo.unfinalized.size() == 0)
		updateAccesible();

	statesInfo.finalized.clear();
	statesInfo.finalized.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
	{
		if (isIn(statesAll[i], statesInfo.accesible) &&
			indexOfStateFinal(statesAll[i]) > -1)
		{
			statesInfo.finalized[i] = statesAll[i];
		}
		else
		{
			statesInfo.finalized[i] = "-";
		}
	}
	

	/* // not sure of this solution
	auto symCheck = 0;
	for (auto i = 0; i < cntStatesAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			if (isIn(pathsMatrix[i][j], statesInfo.accesible))
			{
				if (pathsMatrix[i][j] == "-")
					symCheck++;
			}
			else
				break;
		}
		if ((symCheck == cntSymb) && (indexOfStateFinal(statesAll[i]) > -1))
		{
			statesInfo.finalized[i] = statesAll[i];
			symCheck = 0;
		}
		else
		{
			symCheck = 0;
		}
	}
	//*/
}

void ReaderAF::updateUnfinalized()
{	// accesibil, not final, si dead-end
	if (statesInfo.inaccesible.size() == 0)
		updateAccesible();

	statesInfo.unfinalized.clear();
	statesInfo.unfinalized.resize(cntStatesAll);
	for (auto i = 0; i < cntStatesAll; i++)
		statesInfo.unfinalized[i] = "-";

	auto symCheck = 0;
	for (auto i = 0; i < cntStatesAll; i++)
	{
		for (auto j = 0; j < cntSymb; j++)
		{
			if (isIn(statesAll[i], statesInfo.accesible))
			{
				if (pathsMatrix[i][j] == "-")
					symCheck++;
			}
			else
			{
				symCheck = 0;
				break;
			}
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
}

void ReaderAF::analyzeStates()
{
	// NOTE : order matters!
	updateAccesible();
	updateInaccesible();
	updateUnfinalized();
	updateFinalized();
	
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
	while (!confFile.eof())
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

string ReaderAF::minimizeAFD()
{
	removeUselessStates();

	unsigned int i = 0, j = 0;
	unsigned int cardQ = statesAll.size();
	auto** A = new int*[cardQ];
	for (i = 0; i < cardQ; i++)
	{
		A[i] = new int[cardQ];
	}

	for (i = 0;i < cardQ; i++)
		for (j = 0; j < cardQ; j++)
			A[i][j] = 0;

	vector<string> stateAllWithoutFinal = arraysMinus(statesAll, statesFinal);

	for (i = 0; i < cardQ - 1; i++)
		for (j = i + 1; j < cardQ; j++)
			if (
				(isIn(statesAll[i], statesFinal) && isIn(statesAll[j], stateAllWithoutFinal)) || 
				(isIn(statesAll[i], stateAllWithoutFinal) && isIn(statesAll[j], statesFinal))
				)
			{
				A[i][j] = 1;
				A[j][i] = 1;
			}

	bool modif;
	do {
		modif = false;

		for (i = 0;i < cardQ - 1; i++)
		{
			for (j = i + 1;j < cardQ; j++)
			{
				if (A[i][j] == 0)
				{
					for (unsigned int a = 0; a < symbols.size(); ++a)
					{
						if ((pathsMatrix[i][a].compare("-") != 0 && pathsMatrix[j][a].compare("-") == 0) ||
							(pathsMatrix[j][a].compare("-") != 0 && pathsMatrix[i][a].compare("-") == 0))
						{
							A[i][j] = 1;
							A[j][i] = 1;
							modif = true;
							break;
						}
						else
						{
							if (pathsMatrix[i][a].compare("-") != 0 && pathsMatrix[j][a].compare("-") != 0)
							{
								auto x = indexOfState(pathsMatrix[i][a]);
								auto y = indexOfState(pathsMatrix[j][a]);
								if (A[x][y] != 0)
								{
									A[i][j] = 1;
									A[j][i] = 1;
									modif = true;
									break;
								}
							}
						}
					}
				}
			}
		}		
	} while (modif);
	
	//interpretarea claselor de echivalenta
	vector<int> classes;
	classes.resize(cardQ);
	for (i = 0; i < cardQ; i++)
	{
		classes[i] = -1; //neclasificat
	}
	int nrClase = -1;
	vector<int> reprezentantClasa;
	vector<int> newFinalStates;
	for (i = 0;i < cardQ; i++)
	{
		if (classes[i] == -1) //starea qi nu a fost inclusa inca in nici o clasa
		{
			classes[i] = ++nrClase;
			reprezentantClasa.push_back(i);
			if (isIn(statesAll[i], statesFinal) && !isIn(nrClase, newFinalStates))
				newFinalStates.push_back(nrClase);
			for (j = i + 1;j < cardQ ;j++)
				if (A[i][j] == 0)
					classes[j] = nrClase;
		}
	}

	vector<int> newStates;
	for (int k = 0; k <= nrClase; k++)
	{
		newStates.push_back(k);
	}
	int newCardQ = newStates.size();
	// M'=(Q', 6, 7', q0', F')

	vector<DeltaPairs2> delta;
	for (int k = 0; k < newCardQ; k++)
	{
		for (j = 0; j < symbols.size(); ++j)
		{
			auto localIndex = indexOfState(pathsMatrix[reprezentantClasa[k]][j]);
			if (localIndex > -1) 
			{
				delta.push_back(DeltaPairs2{ k, symbols[j], classes[localIndex] });
			}			
		}
	}

	int newInitialState = classes[indexOfState(initialState)];
	
	string output = "";

	output.append(to_string(newCardQ) + "\n");
	for (i = 0; i < newStates.size(); i++)
	{
		output.append(to_string(newStates[i]) + " ");
	}
	output.append("\n");
	
	output.append(to_string(cntSymb) + "\n");
	for (i = 0; i < symbols.size(); i++)
	{
		output.append(symbols[i] + " ");
	}
	output.append("\n");

	output.append(to_string(newInitialState) + "\n");
	output.append(to_string(newFinalStates.size()) + "\n");
	for (i = 0; i < newFinalStates.size(); i++)
	{
		output.append(to_string(newFinalStates[i]) + " ");
	}
	output.append("\n");

	for (i = 0; i < delta.size(); i++)
	{
		output.append(to_string(delta[i].qx) + " " + delta[i].a + " " + to_string(delta[i].qy));
		if (i < delta.size() - 1)
			output.append("\n");
	}

	fstream minAFDFile;
	auto minAFDPath = filePath + ".minAFD";
	minAFDFile.open(minAFDPath, ios::out);

	if (minAFDFile.is_open())
	{
		minAFDFile << output;
		minAFDFile.close();
		cout << "\nCheck out " + minAFDPath << endl;
	}
	else
	{
		cout << "\nsomething went wrong!\ncounld not create " + minAFDPath << endl;
	}

	return output;
}

void ReaderAF::removeUselessStates()
{
	updateAccesible();
	updateInaccesible();
	updateFinalized();
	updateUnfinalized();

	if (!isEmpty(statesInfo.inaccesible)) 
	{
		vector<vector<string>> paths;
		vector<string> states;
		vector<string> finals;

		for (unsigned int i = 0; i < statesInfo.accesible.size(); i++)
		{
			auto localIndex = indexOfState(statesInfo.accesible[i]);
			if (localIndex > -1)
			{
				paths.push_back(pathsMatrix[localIndex]);
				states.push_back(statesInfo.accesible[i]);
				if (indexOfStateFinal(statesInfo.accesible[i]) > -1)
					finals.push_back(statesInfo.accesible[i]);
			}
		}

		pathsMatrix = paths;
		statesAll = states;
		statesFinal = finals;

		cntStatesAll = statesAll.size();
		cntStatesFinal = statesFinal.size();

		currentState = initialState;
		indexCurrentState = indexOfState(initialState);

		updateAccesible();
		updateInaccesible();
		updateFinalized();
		updateUnfinalized();
	}
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
