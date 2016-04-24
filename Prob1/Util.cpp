#include "Util.h"

bool isIn(int a, vector<int> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i] == a)
			return true;
	}
	return false;
}

bool isIn(char a, vector<string> v)
{
	string character(1, a);
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i].compare(character)==0)
			return true;
	}
	return false;
}

bool isIn(string a, vector<string> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i].compare(a) == 0)
			return true;
	}
	return false;
}

bool isIn(string a, vector<tuple<string, int, int>> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (a.compare(get<0>(v[i])) == 0)
			return true;
	}
	return false;
}

bool isIn(tuple<string, int, int> t, vector<tuple<string, int, int>> v)
{
	string t_a = get<0>(t);
	int t_b = get<1>(t);
	int t_c = get<2>(t);

	for (unsigned int i = 0; i < v.size(); ++i)
	{
		string v_a = get<0>(v[i]);
		int v_b = get<1>(v[i]);
		int v_c = get<2>(v[i]);

		if (v_a.compare(t_a) == 0
			&& v_b == t_b
			&& v_c == t_c)
			return true;
	}
	return false;
}

bool areEqual(vector<string> vector_1, vector<string> vector_2)
{
	auto result = true;

	if (vector_1.size() == vector_2.size())
	{
		for (unsigned int i = 0; i < vector_2.size(); i++)
		{
			if (!isIn(vector_1[i], vector_2) || !isIn(vector_2[i], vector_1))
			{
				result = false;
				break;
			}
		}
	}
	else
	{
		result = false;
	}

	return result;
}


vector<string>& split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string> arraysMinus(const vector<string> fromArray, const vector<string> whatArray)
{
	vector<string> result;
	for (unsigned int it = 0; it < fromArray.size(); ++it)
	{
		if (!isIn(fromArray[it], whatArray))
		{
			result.push_back(fromArray[it]);
		}
	}

	return result;
}

vector<string> arraysIntersection(const vector<string> v1, const vector<string> v2)
{
	vector<string> result;

	for (auto it = v1.begin(); it != v1.end(); ++it)
	{
		if (isIn(*it, v2) && !isIn(*it, result))
		{
			result.push_back(*it);
		}
	}

	for (auto it = v2.begin(); it != v2.end(); ++it)
	{
		if (!isIn(*it, result) && isIn(*it, v1))
		{
			result.push_back(*it);
		}
	}

	return result;
}

vector<string> arraysUnion(const vector<string> v1, const vector<string> v2)
{
	vector<string> result;

	for (auto it = v1.begin(); it != v1.end(); ++it)
	{
		if (!isIn(*it, result))
		{
			result.push_back(*it);
		}
	}

	for (auto it = v2.begin(); it != v2.end(); ++it)
	{
		if (!isIn(*it, result))
		{
			result.push_back(*it);
		}
	}

	return result;
}

void printMap(map<string, vector<string>> in) {
	for (auto pair : in) {
		cout << pair.first << " -> ";
		for (auto val : pair.second) {
			cout << val << " ";
		}
		cout << endl;
	}
}

void printMap(map < string, string> in) {
	for (auto pair : in) {
		cout << pair.first << " -> " << pair.second << endl;
	}
}

void printMap(map<string, vector<pair<string, vector<string>>>> in) {
	for (auto pair : in) {
		for (auto vec : pair.second) {
			cout << "< " << pair.first << " | " << vec.first << " > : { ";
			for (auto vec2 : vec.second) {
				cout << vec2 << " ";
			}
			cout << "}" << endl;
		}
		cout << endl;
	}
}
