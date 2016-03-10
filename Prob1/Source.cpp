#include "ReaderAF.h"
#include "ReaderST.h"
#include <iostream>
#include <string>

using namespace std;

int AF(string filePath)
{
	ReaderAF r(filePath);

	if (r.readConfig() == 1)
	{
		cout << "\nsomething went wrong!\n";
		system("pause");
		return 1;
	};

	r.showPathsMatrix();

	int user;
	string w;
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
			6 : genrate Grammar\n";
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
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);

	return 0;
}

int ST(string filePath)
{
	ReaderST st(filePath);

	if (st.readConfig() == 1)
	{
		cout << "\nsomething went wrong!\n";
		system("pause");
		return 1;
	};

	st.showPathsMatrix();

	int user;
	string w;
	string s;
	do
	{
		user = 2;
		cout << "\n\n\
			0 : previous\n\
			1 : show matrix\n\
			2 : show eps_inch\n\
			3 : ST2AFD\n";
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
			st.showEpsInch();
			break;
		case 3:
			st.ST2AFD();
			break;
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);

	return 0;
}

string pas_1(string w1)
{
	string w2 = w1;



	return w2;
}

string pas_2(string w2)
{
	string w3 = w2;



	return w3;
}

int main()
{
	char path[256] = "..\\data\\";
	string filePath(path);
	auto result = 0;
	
	int user;
	string w;
	string w1; // user input
	string w2; // complet
	string w3; // postfixed
	do
	{
		user = -1;
		cout << "\n\n\
			0 : exit\n\
			1 : A.F.\n\
			2 : S.T.\n\
			3 : RegExpr\n";
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
			cout << "enter a filename from " + filePath + " : ";
			//cout << filePath << endl;
			cin >> w;
			result = AF(filePath + w);
			break;
		case 2:
			cout << "enter a filename from " + filePath + " : ";
			//cout << filePath << endl;
			cin >> w;
			result = ST(filePath + w);
			break;
		case 3:
			cout << "\nw1 : ";
			cin >> w1;
			w2 = pas_1(w1);
			cout << "w2 : " << w2 << endl;
			w3 = pas_2(w2);
			cout << "w3 : " << w3 << endl;
			//pas_3(w3);
			break;
		default:
			cout << "\nselect a valid option!\n";
			break;
		}

	} while (user != 0);
		
	//system("pause");
	return result;
}