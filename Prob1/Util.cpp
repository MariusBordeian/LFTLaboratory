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