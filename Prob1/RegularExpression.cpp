#include "RegularExpression.h"
#include <vector>
#include <stack>
#include <string>

using namespace std;


RegularExpression::RegularExpression() { }

string RegularExpression::completeWithOperators(string in) {

	vector<char> X = { 'a','b','c','d',')','*','#','e','o' };
	vector<char> Y = { 'a','b','c','d','(','e','o' };
	int counter = 0;

	if (in.size() != 0) {
		unsigned int i = 1, j = 1;
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

string RegularExpression::postFixedForm(string in) {
	stack<char> S;
	string out = "";
	S.push('_');
	vector<char> multime = { 'a','b','c','d','e','o' };
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




RegularExpression::~RegularExpression() {}