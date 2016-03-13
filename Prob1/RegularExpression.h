#pragma once
#include "string"
#include "vector"
#include <map>

using namespace std;

class RegularExpression
{

public:
	explicit RegularExpression();

	string completeWithOperators(string);
	string postFixedForm(string);
	bool isIn(char, vector<char>);

	~RegularExpression();
};

