#include "RegularExpression.h"
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


RegularExpression::RegularExpression(string in) { w1_userInput = in; }

RegularExpression::~RegularExpression() { }

string RegularExpression::completeWithOperators() const {
	string in = w1_userInput;
	vector<char> X = { 'a','b','c','d',')','*','#','e','o' };
	vector<char> Y = { 'a','b','c','d','(','e','o' };
	int counter = 0;

	if (in.size() != 0) {
		unsigned int i = 1;
		for (i = 1; i < in.size(); i++)
		{
			if (isIn(in[i], Y) && isIn(in[i - 1], X))
			{
				counter++;
			}
		}
	}
	// not efficient but whatever, it works :))
	string out(in.size() + counter, ' ');

	if (in.size() != 0) {
		out[0] = in[0];
		unsigned int i = 1, j = 1;
		for (i = 1; i < in.size(); i++)
		{
			if (isIn(in[i], Y) && isIn(in[i - 1], X))
			{
				out[j++] = '.';
			}
			out[j++] = in[i];
		}
	}
	return out;
}

string RegularExpression::postFixedForm() const {
	string in = w2_complete;
	stack<char> S;
	string out = "";
	S.push('_');
	map<char, int> pos = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',0 },{ '_',-1 } };
	map<char, int> poe = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',4 },{ '_',-1 } };

	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (isIn(in[i], multime))
			out += in[i];
		else
			if (in[i] == ')')
			{
				while (S.top() != '(')
				{
					out += S.top();
					S.pop();
				}
				S.pop();
			}
			else {
				while (pos[S.top()] >= poe[in[i]])
				{
					out += S.top();
					S.pop();
				}
				S.push(in[i]);
			}
	}
	while (S.top() != '_') {
		out += S.top();
		S.pop();
	}
	S.pop();
	return out;
}

bool RegularExpression::isIn(char character, vector<char> set) {
	for (unsigned int i = 0; i < set.size(); i++) {
		if (set[i] == character) {
			return true;
		}
	}
	return false;
}

void RegularExpression::generateST() const
{
	auto stCrt = -1;
	stack<pair<string, string>> stivaST;
	pair<string, string> M1;
	pair<string, string> M2;
	vector<DeltaPairs> delta;

	for (unsigned int i = 0; i < w3_postFixed.length(); ++i)
	{
		if (isIn(w3_postFixed[i], multime))
		{
			++stCrt;
			stivaST.push(pair<string, string>(to_string(stCrt), to_string(stCrt + 1)));
			if (w3_postFixed[i] != 'o')	// multimea "vida"
			{
				delta.push_back(DeltaPairs{ to_string(stCrt), to_string(w3_postFixed[i]), to_string(stCrt + 1) });
			}
			++stCrt;
		}
		else
		{
			switch(w3_postFixed[i])
			{
			case '+':
				M2 = stivaST.top();
				stivaST.pop();
				M1 = stivaST.top();
				stivaST.pop();
				
				++stCrt;

				delta.push_back(DeltaPairs{ to_string(stCrt), "e", M1.first });
				delta.push_back(DeltaPairs{ to_string(stCrt), "e", M2.first });
				delta.push_back(DeltaPairs{ M1.second, "e", to_string(stCrt + 1) });
				delta.push_back(DeltaPairs{ M2.second, "e", to_string(stCrt + 1) });

				stivaST.push(pair<string, string>(to_string(stCrt), to_string(stCrt + 1)));

				++stCrt;

				break;

			case '.':
				M2 = stivaST.top();
				stivaST.pop();
				M1 = stivaST.top();
				stivaST.pop();

				delta.push_back(DeltaPairs{ M1.first, "e", M2.second });

				stivaST.push(pair<string, string>(M1.first, M2.second));

				break;

			case '#':
				M1 = stivaST.top();
				stivaST.pop();

				++stCrt;

				delta.push_back(DeltaPairs{ to_string(stCrt), "e", M1.first });
				delta.push_back(DeltaPairs{ M1.second, "e", M1.first });
				delta.push_back(DeltaPairs{ M1.second, "e", to_string(stCrt + 1) });
				delta.push_back(DeltaPairs{ to_string(stCrt), "e", to_string(stCrt + 1) });

				stivaST.push(pair<string, string>(to_string(stCrt), to_string(stCrt + 1)));

				++stCrt;

				break;

			case '*':
				M1 = stivaST.top();
				stivaST.pop();

				++stCrt;

				delta.push_back(DeltaPairs{ to_string(stCrt), "e", M1.first });
				delta.push_back(DeltaPairs{ M1.second, "e", M1.first });
				delta.push_back(DeltaPairs{ M1.second, "e", to_string(stCrt + 1) });
				delta.push_back(DeltaPairs{ to_string(stCrt), "e", to_string(stCrt + 1) });

				stivaST.push(pair<string, string>(to_string(stCrt), to_string(stCrt + 1)));

				++stCrt;

				break;
			}
		}
	}

	M1 = stivaST.top();
	stivaST.pop();

	auto nrStari = stCrt + 1;
	// same Sigma (alfabet) => Sigma = this->Multime
	// delta 
	string initialState = M1.first;
	vector<string> finalStates;
	finalStates.push_back(M1.second);

	// TODO : create the conf file to write into
	
	fstream STFile;
	string STPath = "..\\data\\" + w1_userInput + ".ST";	// not sure all symbols from w1_userInput is supported as filename

	STFile.open(STPath, ios::out);

	if (STFile.is_open())
	{
		// dump method output to STFile aka S.T. config file

		
		STFile.close();
		cout << "\ncheck " + STPath << endl;
	}
	else
	{
		cout << "\nsomething went wrong!\nCould not opet file to write into." << endl;
	}
}
