#pragma once
#include <vector>
#include <map>

using namespace std;

typedef struct triplete {
	string qx;
	string a;
	string qy;
} DeltaPairs;

class RegularExpression
{
public:
	char Eps = 'E';
	char Vid = 'O';
	vector<char> Sigma;
	vector<char> multime;
	vector<char> operatori = { '*', '#', '.', '+', '(', ')' };
	map<char, int> pos = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',0 },{ '_',-1 } };
	map<char, int> poe = { { '*',3 },{ '#',3 },{ '.',2 },{ '+',1 },{ '(',4 },{ '_',-1 } };
	

	explicit RegularExpression(string);
	~RegularExpression();

	string completeWithOperators();
	string postFixedForm();
	string getUserString() const { return w1_userInput; }
	string getCompleteForm() const { return w2_complete; }
	string getPostFixed() const { return w3_postFixed; }

	string generateST();
	static string myToString(char);
	string getFileName() const;
	static bool isIn(char, vector<char>);

private:
	string w0_cleanInput;
	string w1_userInput;
	string w2_complete;
	string w3_postFixed;
};

