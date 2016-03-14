#pragma once
#include "string"
#include "vector"
#include <map>

using namespace std;

class RegularExpression
{

public:
	vector<char> sigma = { 'a','b','c','d','e','o' };
	map<char, int> pos = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',0 },{ '_',-1 } };
	map<char, int> poe = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',4 },{ '_',-1 } };

	explicit RegularExpression();

	string completeWithOperators(string);
	string postFixedForm(string);
	string generateST(string);
	string myToString(char);
	bool isIn(char, vector<char>);

	~RegularExpression();
};

