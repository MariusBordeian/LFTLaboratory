#pragma once
#include "vector"

using namespace std;

typedef struct triplete {
	string qx;
	string a;
	string qy;
} DeltaPairs;

class RegularExpression
{
public:
	vector<char> Sigma = { 'a','b','c','d' };
	vector<char> multime = { 'a','b','c','d','e','o' };

	explicit RegularExpression(string);
	~RegularExpression();

	string completeWithOperators() const;
	string postFixedForm() const;
	string getUserString() const { return w1_userInput; }
	string getCompleteForm() const { return w2_complete; }
	string getPostFixed() const { return w3_postFixed; }

	static bool isIn(char, vector<char>);
	void generateST() const;

private:
	string w1_userInput;
	string w2_complete;
	string w3_postFixed;
};

