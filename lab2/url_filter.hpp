#ifndef URL_FILTER_HPP
#define URL_FILTER_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class url_filter{
public:
	url_filter(int port);
	~url_filter();
	int filter(string http);
private:
	vector<string> key_words;

};
#endif
