#include "RegularExpression.h"
#include <vector>
#include <stack>
#include <map>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;


RegularExpression::RegularExpression(string in) { 
	w1_userInput = in; 
	w0_cleanInput = getFileName();

	for (auto i = w0_cleanInput.begin(); i != w0_cleanInput.end(); ++i)
	{
		if ((*i != Eps || *i != Vid) && !isIn(*i, Sigma))
			Sigma.push_back(*i);
	}

	multime.insert(multime.begin(), Sigma.begin(), Sigma.end());
	multime.push_back(Eps);
	multime.push_back(Vid);
}

RegularExpression::~RegularExpression() { }

string RegularExpression::completeWithOperators() 
{
	string in = w1_userInput;
	
	vector<char> X = { ')','*','#',Eps,Vid };
	X.insert(X.end(), Sigma.begin(), Sigma.end());

	vector<char> Y = { '(',Eps,Vid };
	Y.insert(Y.end(), Sigma.begin(), Sigma.end());
	
	string out = "";
	if (in.size() != 0) {
		out += in[0];
		unsigned int i = 1;
		for (i = 1; i < in.size(); i++)
		{
			if (isIn(in[i], Y) && isIn(in[i - 1], X))
			{
				out += '.';
			}
			out += in[i];
		}
	}

	w2_complete = out;
	
	return out;
}

string RegularExpression::postFixedForm() 
{
	string in = w2_complete;
	stack<char> S;
	string out = "";
	S.push('_');

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
				while (pos.at(S.top()) >= poe.at(in[i]))
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

	w3_postFixed = out;

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

string RegularExpression::generateST() 
{
	string in = w3_postFixed;
	stack<tuple<string, string>> stivaST;
	vector<tuple<string, string, string>> delta;
	tuple<string, string> M1;
	tuple<string, string> M2;
	int stCrt = -1;
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (isIn(in[i], multime))
		{
			++stCrt;
			stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
			if (in[i] != Vid)
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(in[i]), to_string(stCrt + 1)));
			++stCrt;
		}
		else
		{
			switch (in[i])
			{
			case '+':
				M2 = stivaST.top();
				stivaST.pop();
				M1 = stivaST.top();
				stivaST.pop();
				++stCrt;
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(Eps), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(Eps), get<0>(M2)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), to_string(stCrt + 1)));
				delta.push_back(tuple<string, string, string>(get<1>(M2), myToString(Eps), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			case '.':
				M2 = stivaST.top();
				stivaST.pop();
				M1 = stivaST.top();
				stivaST.pop();
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), get<0>(M2)));
				stivaST.push(tuple<string, string>(get<0>(M1), get<1>(M2)));
				break;
			case '#':
				M1 = stivaST.top();
				stivaST.pop();
				++stCrt;
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(Eps), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			case '*':
				M1 = stivaST.top();
				stivaST.pop();
				++stCrt;
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(Eps), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString(Eps), to_string(stCrt + 1)));
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString(Eps), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			}
		}
	}
	M1 = stivaST.top();
	stivaST.pop();
	auto nrStari = stCrt + 1;
	string q0 = get<0>(M1);
	string F = get<1>(M1);

	sort(begin(delta), end(delta), [](auto const &t1, auto const &t2) {
		return atoi(get<0>(t1).c_str()) < atoi(get<0>(t2).c_str());
	});

	string toBePrinted = "";
	toBePrinted.append("Initial state\t: ").append(q0).append("\nFinal state\t: ").append(F).append("\nDelta: \n");
	for (unsigned int i = 0; i < delta.size(); i++) {
		toBePrinted.append(get<0>(delta[i])).append("\t").append(get<1>(delta[i])).append("\t").append(get<2>(delta[i])).append("\n");
	}

	fstream STFile;
	string STPath = "..\\data\\" + w0_cleanInput + ".ST";

	STFile.open(STPath, ios::out);

	if (STFile.is_open())
	{
		STFile << nrStari << endl;

		for (auto i = 0; i < nrStari; ++i)
		{
			STFile << i << " ";
		}
		STFile << endl;

		STFile << Sigma.size() + 1 << endl;

		for (auto it = Sigma.begin(); it != Sigma.end(); ++it)
		{
			STFile << *it << " ";
		}
		STFile << Eps << endl;

		STFile << q0 << endl;

		STFile << "1" << endl;

		STFile << F << endl;
		
		for (unsigned int it = 0; it < delta.size(); ++it)
		{
			STFile << get<0>(delta[it]) << " " << get<1>(delta[it]) << " " << get<2>(delta[it]);
			if (it != delta.size() - 1)
				STFile << endl;
		}

		STFile.close();
		cout << "\n\ncheck " + STPath << endl << endl;
	}
	else
	{
		cout << "\nsomething went wrong!\nCould not opet file to write into." << endl;
	}

	return toBePrinted;
}

string RegularExpression::myToString(char a) 
{
	string s = "";
	s += a;
	return s;
}

string RegularExpression::getFileName() const
{
	string input = w1_userInput;
	
	auto it = input.begin();	
	while (it != input.end())
	{
		if (isIn(*it, operatori))
		{
			it = input.erase(it);
		}
		else
		{
			++it;
		}
	}

	return input;
}
