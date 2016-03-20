#include "ReaderST.h"
#include "Util.h"
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

ReaderST::ReaderST(string path): cntStatesAll(0), cntSymb(0), cntStatesFinal(0), indexCurrentState(0)
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

ReaderST::~ReaderST()
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

vector<string>& ReaderST::split(const string &s, char delim, vector<string> &elems) const
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> ReaderST::split(const string &s, char delim) const
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

int ReaderST::indexOfSymb(string c) const
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

int ReaderST::indexOfState(string c) const
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

int ReaderST::indexOfStateFinal(string c) const
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

void ReaderST::showPathsMatrix() const
{
	cout << "\nMatrix : \n\t";
	for (auto i = 0; i < cntSymb; i++)
		cout << symbols[i] << "\t\t";
	cout << endl;
	for (auto i = 0; i < cntStatesAll; i++)
	{
		cout << statesAll[i] << "\t";
		for (auto j = 0; j < cntSymb; j++)
		{
			auto k_len = pathsMatrix[i][j].size();
			for (unsigned int k = 0; k < k_len; ++k)
			{
				cout << pathsMatrix[i][j][k] << " ";
			}
			cout << "\t\t";
		}
		cout << endl;
	}
}

/*
int ReaderST::validateWord(string w)
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
			if (areStatesEqual(getNextState(wordSymbols[i]), emptyState))
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
//*/

bool ReaderST::areStatesEqual(vector<string> first, vector<string> second)
{
	bool result;
	
	if (first.size() == second.size())
	{
		sort(first.begin(), first.end());
		sort(second.begin(), second.end());
		
		result = equal(first.begin(), first.end(), second.begin());
	}
	else
	{
		result = false;
	}

	return result;
}

/*
void ReaderST::showPath(string w, vector<string> startState)
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
//*/
bool ReaderST::contains(vector<vector<string>> container, vector<string> toBeContained) {
	for (unsigned int i = 0; i < container.size(); i++) {
		if (areStatesEqual(container[i], toBeContained)) {
			return true;
		}
	}
	return false;
}

bool ReaderST::contains(vector<string> container, string toBeContained) {
	for (unsigned int i = 0; i < container.size(); i++) {
		if (container[i].compare(toBeContained)==0) {
			return true;
		}
	}
	return false;
}

vector<string> ReaderST::getNextStates(vector<string> fromStates, string withSymbol) {
	vector<string> nextStates;
	for (unsigned int i = 0; i < fromStates.size(); i++) {
		vector<string> partialNextStates = getNextState(fromStates[i], withSymbol);
		for (unsigned int j = 0; j < partialNextStates.size(); j++) {
			if (partialNextStates[j].compare("-") != 0 && !contains(nextStates, partialNextStates[j]) ) {
				nextStates.push_back(partialNextStates[j]);
			}
		}
	}
	return nextStates;
}

vector<string> ReaderST::getEpsInch(vector<string> states) {
	vector<string> local_eps_inch;
	for (unsigned int i = 0; i < states.size(); i++) {
		vector<string> partial_eps_inch=getEpsInch(states[i]);
		if(partial_eps_inch.size()!=0){
			for (unsigned int j = 0; j < partial_eps_inch.size(); j++) {
				if (!contains(local_eps_inch, partial_eps_inch[j])) {
					local_eps_inch.push_back(partial_eps_inch[j]);
				}
			}
		}
		
	}
	return local_eps_inch;
}
string ReaderST::vectorToString(vector<string> inputVector) {
	string out = "-";
	if (inputVector.size() != 0) {
		out = "";
		for (unsigned int i = 0; i < inputVector.size(); i++) {
			if (i == 0)
				out.append("{");

			out.append(inputVector[i]);

			if (i < inputVector.size() - 1)
				out.append(",");

			if (i == inputVector.size() - 1)
				out.append("}");
		}
	}
	return out;
}
bool ReaderST::hasElementFrom(vector<string> vect1, vector<string> vect2) {
	for (unsigned int i = 0; i < vect2.size(); i++) {
		for (unsigned int j = 0; j < vect1.size(); j++) {
			if (vect2[i].compare(vect1[j]) == 0) {
				return true;
			}
		}
	}
	return false;
}

int ReaderST::ST2AFD()
{
	auto result = -1;
	fstream AFDFile;
	auto AFDPath = filePath + ".AFD";

	AFDFile.open(AFDPath, ios::out);

	if (AFDFile.is_open())
	{
		vector<string> startStateEPS_inch = getEpsInch(initialState);
		vector<vector<string>> afd_states;
		queue<vector<string>> states_queue;
		vector<string> afdFinalStates;
		string afdStartState;
		
		vector<string> afdMatrixString;

		if (startStateEPS_inch.size() != 0) {
			sort(startStateEPS_inch.begin(), startStateEPS_inch.end());
			states_queue.push(startStateEPS_inch);
		}
		while (!states_queue.empty()) {
			vector<string> queue_state = states_queue.front();
			states_queue.pop();
			if (!contains(afd_states, queue_state)) {
				sort(queue_state.begin(), queue_state.end());
				afd_states.push_back(queue_state);
				for (unsigned int i = 0; i < symbols.size(); i++) {
					if (symbols[i].compare(Eps) != 0) {
						vector<string> symbolStates=getNextStates(queue_state, symbols[i]);
						vector<string> eps_inch_per_symbol_state = getEpsInch(symbolStates);

						if (!contains(afd_states, eps_inch_per_symbol_state)) {
							if (eps_inch_per_symbol_state.size() != 0) {
								sort(eps_inch_per_symbol_state.begin(), eps_inch_per_symbol_state.end());
								states_queue.push(eps_inch_per_symbol_state);
							}
						}
						if (contains(queue_state,initialState) && !hasElementFrom(queue_state, statesFinal)) {
							sort(queue_state.begin(), queue_state.end());
							afdStartState = vectorToString(queue_state);
						}else if (hasElementFrom(queue_state, statesFinal)) {
							if (!contains(afdFinalStates, vectorToString(queue_state))) {
								sort(queue_state.begin(), queue_state.end());
								afdFinalStates.push_back(vectorToString(queue_state));
							}
						}
						if(eps_inch_per_symbol_state.size()!=0){
							sort(eps_inch_per_symbol_state.begin(), eps_inch_per_symbol_state.end());
							string row = "";
							row.append(vectorToString(queue_state)).append("\t").append(symbols[i]).append("\t").append(vectorToString(eps_inch_per_symbol_state));
							afdMatrixString.push_back(row);
						}
					}
				}
			}
		}


		AFDFile << afd_states.size() << endl;
		for (unsigned int i = 0; i < afd_states.size(); i++) {
			AFDFile << vectorToString(afd_states[i]) << " ";
		}

		AFDFile << endl << symbols.size()-1 << endl;

		for (unsigned int i = 0; i < symbols.size(); i++) {
			if(symbols[i].compare(Eps)!=0)
				AFDFile << symbols[i] << " ";
		}

		AFDFile << endl << afdStartState << endl;

		AFDFile  << afdFinalStates.size() << endl;

		for (unsigned int i = 0; i < afdFinalStates.size(); i++) {
			AFDFile << afdFinalStates[i] << " ";
		}

		AFDFile << endl;

		for (unsigned int i = 0; i < afdMatrixString.size(); i++) {
			AFDFile << afdMatrixString[i];
			if (i != afdMatrixString.size() - 1)
				AFDFile << endl;
		}

		AFDFile.close();

		cout << "\nCheck out " + AFDPath << endl;

		result = 0;
	}
	else
	{
		cout << "\nsomething went wrong!\ncounld not create " + AFDPath << endl;
	}

	return result;
}

void ReaderST::precalcEpsInch()
{
	auto len = statesAll.size();
	for (unsigned int i = 0; i < len; ++i)
	{
		if (!areStatesEqual(pathsMatrix[i][0], emptyState))
		{
			auto tmp = getEpsInch(statesAll[i]);
			tmp.push_back(statesAll[i]);
			sort(tmp.begin(), tmp.end());
			states_eps_inch.insert(pair<string, vector<string>>(statesAll[i], tmp));
		}
		else
		{
			vector<string> tmp;
			tmp.push_back(statesAll[i]);
			states_eps_inch.insert(pair<string, vector<string>>(statesAll[i], tmp));
		}
	}

	for (auto it = states_eps_inch.begin(); it != states_eps_inch.end(); ++it)
	{
		auto i = it->second.begin();
		while (i != it->second.end())
		{
			auto aux = *i;
			++i;
			if (i != it->second.end() && aux == *i)
			{
				i = it->second.erase(i);
				--i;
			}
		}
	}
}

vector<string> ReaderST::getEpsInch(string state)
{
	vector<string> eps_inch;
	auto state_i = indexOfState(state);

	if (state_i > -1)
	{
		auto history = states_eps_inch.find(state);

		if (history != states_eps_inch.end())
		{
			eps_inch = history->second;
		}
		else
		{
			for (unsigned int i = 0; i < pathsMatrix[state_i][0].size(); ++i)
			{
				if (pathsMatrix[state_i][0][i].compare("-") != 0)
				{
					auto tmp = getEpsInch(pathsMatrix[state_i][0][i]);
					tmp.push_back(pathsMatrix[state_i][0][i]);
					eps_inch.insert(eps_inch.begin(), tmp.begin(), tmp.end());
				}
				else
				{
					eps_inch.push_back(state);
				}
			}

			if (eps_inch.size() > 1)
				sort(eps_inch.begin(), eps_inch.end());
		}
	}

	return eps_inch;
}

vector<string> ReaderST::getNextState(string fromState, string withSymbol)
{
	vector<string> result;
	auto indexSymb = indexOfSymb(withSymbol);
	auto indexState = indexOfState(fromState);

	if (indexSymb != -1 && 
		indexState != -1 &&
		!areStatesEqual(pathsMatrix[indexState][indexSymb], emptyState))
	{
		result.insert(result.begin(), pathsMatrix[indexState][indexSymb].begin(), pathsMatrix[indexState][indexSymb].end());
	}
	else
	{
		result.insert(result.begin(), emptyState.begin(), emptyState.end());
	}

	return result;
}

int ReaderST::readConfig()
{
	if (!confFile.is_open())
	{
		cout << "\nconfig file not opened!\n";
		return 1;
	}

	statesAll.clear();
	statesFinal.clear();
	symbols.clear();
	auto states_i = pathsMatrix.size();
	if (states_i > 0)
	{
		for (unsigned int i = 0; i < states_i; i++)
		{
			auto symbols_j = pathsMatrix[i].size();

			if (symbols_j > 0)
			{
				for (unsigned int j = 0; j < symbols_j; ++j)
				{
					pathsMatrix[i][j].clear();
				}
			}
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
			pathsMatrix[i][j].clear();
			pathsMatrix[i][j].insert(pathsMatrix[i][j].begin(), emptyState.begin(), emptyState.end());
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
			pathsMatrix[i][j].push_back(cs);
		}
		else
		{
			cout << "\nwrong paths matrix in config file!\n";
			return 1;
		}
	}

	for (auto a = 0; a < cntStatesAll; a++)
	{
		for (auto b = 0; b < cntSymb; b++)
		{
			if ( pathsMatrix[a][b].size() > 1 )
				pathsMatrix[a][b].erase(pathsMatrix[a][b].begin());
		}
	}

	precalcEpsInch();

	return 0;
}

void ReaderST::showEpsInch()
{
	cout << "\n\ninchiderea epsilon : \n";
	for (auto it = states_eps_inch.begin(); it != states_eps_inch.end(); ++it)
	{
		cout << it->first << " { ";

		for (auto sec = it->second.begin(); sec != it->second.end(); ++sec)
		{
			cout << *sec << " ";
		}

		cout << "}; " << endl;
	}
	cout << endl;
}

/*
void ReaderST::generateGrammar()
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
//*/