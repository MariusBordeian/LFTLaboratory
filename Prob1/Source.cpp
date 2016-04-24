#include "ReaderAF.h"
#include "ReaderST.h"
#include "RegularExpression.h"
#include "Grammar.h"
#include <iostream>
#include <string>

using namespace std;

int AF()
{
	char path[256] = "..\\data\\";
	string filePath_2(path);
	string filePath(path);
	string w;
	cout << "enter a filename from " + filePath + " : ";
	//cout << filePath << endl;
	cin >> w;
	filePath += w;

	ReaderAF r(filePath);

	if (r.readConfig() == 1)
	{
		cout << "\nsomething went wrong!\n";
		system("pause");
		return 1;
	};

	r.showPathsMatrix();

	int user;
	string s;
	do
	{
		user = 2;
		cout << "\n\n\
			0 : previous\n\
			1 : analyze M\n\
			2 : show matrix\n\
			3 : check word for L(M)\n\
			4 : show path for w from initial state\n\
			5 : show path for w from given state\n\
			6 : genrate Grammar\n\
			7 : minimize AFD\n\
			8 : compare\n";
		cout << "\nselect option : ";
		cin >> user;
		cin.clear();
		cin.ignore(256, '\n');

		switch (user)
		{
		case 0:
			cout << "\nsee ya!\n";
			break;
		case 1:
			r.analyzeStates();
			break;
		case 2:
			r.showPathsMatrix();
			break;
		case 3:
			cout << "\nword : ";
			getline(cin, w);
			cout << w << ", " << ((r.validateWord(w) == 0) ? "invalid for L(M)" : "valid for L(M)") << endl;
			break;
		case 4:
			cout << "\nword : ";
			getline(cin, w);
			r.showPath(w, r.initialState);
			break;
		case 5:
			cout << "\nstate to begin : ";
			cin >> s;
			while (r.indexOfState(s) == -1)
			{
				cout << "\nnot a valid state!\n";
				cout << "\nstate to begin : ";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> s;
			}
			cout << "word : ";
			getline(cin, w);
			r.showPath(w, s);
			break;
		case 6:
			r.generateGrammar();
			break;
		case 7:
		{
			ReaderAF r_(r);
			r_.minimizeAFD();
			r_.showPathsMatrix();
		}
			break;	
		case 8:
		{
			cout << "enter second AFD filename, from " + filePath_2 + " : ";
			//cout << filePath << endl;
			cin >> w;
			filePath_2 += w;

			ReaderAF r_1(filePath);
			r_1.readConfig();
			ReaderAF r_2(filePath_2);
			r_2.readConfig();
			r_2.showPathsMatrix();

			cout << endl << (r_1 == r_2 ? filePath + " is equivalent with " + filePath_2 : filePath + " is NOT equivalent with " + filePath_2) << endl;
		}
			break;
		}

		filePath_2 = string(path);
		filePath = string(path);
	} while (user != 0);

	return 0;
}

int ST()
{
	char path[256] = "..\\data\\";
	string filePath(path);
	string w;
	cout << "enter a filename from " + filePath + " : ";
	//cout << filePath << endl;
	cin >> w;
	filePath += w;

	ReaderST st(filePath);

	if (st.readConfig() == 1)
	{
		cout << "\nsomething went wrong!\n";
		system("pause");
		return 1;
	};

	st.showPathsMatrix();

	int user;
	string s;
	do
	{
		user = 2;
		cout << "\n\n\
			0 : previous\n\
			1 : show matrix\n\
			2 : ST2AFD\n";
		cout << "\nselect option : ";
		cin >> user;
		cin.clear();
		cin.ignore(256, '\n');

		switch (user)
		{
		case 0:
			cout << "\nsee ya!\n";
			break;
		case 1:
			st.showPathsMatrix();
			break;
		case 2:
			st.ST2AFD();
			break;
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);

	return 0;
}

int RE()
{
	int user;
	string expression;

	cout << "\t eg. : ((a+bc)*bc(d+ab)#)* \n";
	cout << endl << "Forma initiala\t\t: ";
	cin >> expression;

	RegularExpression re(expression);
	
	cout << "Forma completata\t: " << re.completeWithOperators() << endl;
	cout << "Forma postfixata\t: " << re.postFixedForm() << endl;

	do
	{
		user = 3;
		cout << "\n\n\
			0 : previous\n\
			1 : Get_S.T.\n";
		cout << "\nselect option : ";
		cin >> user;
		cin.clear();
		cin.ignore(256, '\n');

		switch (user)
		{
		case 0:
			cout << "\nsee ya!\n";
			break;
		case 1:
			cout << re.generateST();
			break;
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);

	return 0;
}

int GR()
{
	char path[256] = "..\\data\\";
	string filePath_2(path);
	string filePath(path);
	string w;
	cout << "enter a filename from " + filePath + " : ";
	//cout << filePath << endl;
	cin >> w;
	filePath += w;
	Grammar g(filePath);

	if (g.readConfig() == 1)
	{
		cout << "\nsomething went wrong!\n";
		system("pause");
		return 1;
	};

	g.showRules();

	int user;
	string s;
	do
	{
		user = 2;
		cout << "\n\n\
			0 : previous\n\
			1 : show rules\n\
			2 : Clean\n\
			3 : Remove left redundancy\n\
			4 : check word for this grammar.\t eg.: baaba\n\
			5 : First/Follow\n";
		cout << "\nselect option : ";
		cin >> user;
		cin.clear();
		cin.ignore(256, '\n');

		switch (user)
		{
		case 0:
			cout << "\nsee ya!\n";
			break;
		case 1:
			g.showRules();
			break;
		case 2: {
				Grammar g2(g);
				g2.cleanUp();
			} break;
		case 3: {
				Grammar g3(g);
				g3.removeLeftRecursion();
				g3.showRulesType2();
			} break;
		case 4: {
				string word;
				cout << "\nEnter word to check : ";
				cin >> word;
				cout << endl << (g.checkWordForGrammar(word) ? word + " poate fi generat de catre gramatica" : word + " NU poate fi generat de catre gramatica") << endl;
			} break;
		case 5: {
			g.firstFollow();

		} break;
		}

		filePath_2 = string(path);
		filePath = string(path);
	} while (user != 0);

	return 0;
}

int main()
{
	auto result = 0;

	int user;
	string w;
	do
	{
		user = -1;
		cout << "\n\n\
			0 : exit\n\
			1 : A.F.\n\
			2 : S.T.\n\
			3 : R.E.\n\
			4 : Grammar\n";
		cout << "\nselect option : ";
		cin >> user;
		cin.clear();
		cin.ignore(256, '\n');

		switch (user)
		{
		case 0:
			cout << "\nbye!\n";
			break;
		case 1:
			result = AF();
			break;
		case 2:
			result = ST();
			break;
		case 3:
			result = RE();
			break;
		case 4:
			result = GR();
			break;
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);

	//system("pause");
	return result;
}
