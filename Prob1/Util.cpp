#include "Util.h"

bool isIn(string a, vector<string> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i].compare(a) == 0)
			return true;
	}
	return false;
}

bool isIn(int a, vector<int> v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		if (v[i] == a)
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
