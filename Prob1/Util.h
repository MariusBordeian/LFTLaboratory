#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <sstream>

using namespace std;

bool isIn(int a, vector<int> v);
bool isIn(char a, vector<string> v);
bool isIn(string a, vector<string> v);
bool isIn(string a, vector<tuple<string, int, int>> v);
bool isIn(tuple<string, int, int> t, vector<tuple<string, int, int>> v);
bool areEqual(vector<string> vector_1, vector<string> vector_2);

vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
vector<string> arraysMinus(const vector<string>, const vector<string>);
vector<string> arraysIntersection(const vector<string>, const vector<string>);
vector<string> arraysUnion(const vector<string>, const vector<string>);