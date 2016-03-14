#include "RegularExpression.h"
#include <vector>
#include <stack>
#include <string>
#include <tuple>
#include <algorithm>
using namespace std;


RegularExpression::RegularExpression() { }

string RegularExpression::completeWithOperators(string in) {
	vector<char> X = { 'a','b','c','d',')','*','#','e','o' };
	vector<char> Y = { 'a','b','c','d','(','e','o' };
	string out = "";
	if (in.size() != 0) {
		out += in[0];
		unsigned int i = 1, j = 1;
		for (i = 1; i < in.size(); i++)
		{
			if (isIn(in[i], Y) && isIn(in[i - 1], X))
			{
				out += '.';
			}
			out += in[i];
		}
	}
	return out;
}

string RegularExpression::postFixedForm(string in) {
	stack<char> S;
	string out = "";
	S.push('_');

	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (isIn(in[i], sigma))
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

string RegularExpression::generateST(string in) {
	stack<tuple<string, string>> stivaST;
	vector<tuple<string, string, string>> delta;
	tuple<string, string> M1;
	tuple<string, string> M2;
	int stCrt = -1;
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (isIn(in[i], sigma))
		{
			++stCrt;
			stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
			if (in[i] != 'o')
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
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString('e'), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString('e'), get<0>(M2)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), to_string(stCrt + 1)));
				delta.push_back(tuple<string, string, string>(get<1>(M2), myToString('e'), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			case '.':
				M2 = stivaST.top();
				stivaST.pop();
				M1 = stivaST.top();
				stivaST.pop();
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), get<0>(M2)));
				stivaST.push(tuple<string, string>(get<0>(M1), get<1>(M2)));
				break;
			case '#':
				M1 = stivaST.top();
				stivaST.pop();
				++stCrt;
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString('e'), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			case '*':
				M1 = stivaST.top();
				stivaST.pop();
				++stCrt;
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString('e'), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), get<0>(M1)));
				delta.push_back(tuple<string, string, string>(get<1>(M1), myToString('e'), to_string(stCrt + 1)));
				delta.push_back(tuple<string, string, string>(to_string(stCrt), myToString('e'), to_string(stCrt + 1)));
				stivaST.push(tuple<string, string>(to_string(stCrt), to_string(stCrt + 1)));
				++stCrt;
				break;
			}
		}
	}
	M1 = stivaST.top();
	stivaST.pop();
	int modulQ;
	modulQ = stCrt + 1;
	string q0 = get<0>(M1);
	string F = get<1>(M1);

	sort(begin(delta), end(delta), [](auto const &t1, auto const &t2) {
		return atoi(get<0>(t1).c_str()) < atoi(get<0>(t2).c_str());
	});

	string toBePrinted = "";
	toBePrinted.append("Initial state : ").append(q0).append("\nFinal state : ").append(F).append("\nDelta : \n");
	for (unsigned int i = 0; i < delta.size(); i++) {
		toBePrinted.append(get<0>(delta[i])).append("\t").append(get<1>(delta[i])).append("\t").append(get<2>(delta[i])).append("\n");
	}

	return toBePrinted;
}

bool RegularExpression::isIn(char character, vector<char> set) {
	for (unsigned int i = 0; i < set.size(); i++) {
		if (set[i] == character) {
			return true;
		}
	}
	return false;
}

string RegularExpression::myToString(char a) {
	string s = "";
	s += a;
	return s;
}


RegularExpression::~RegularExpression() {}