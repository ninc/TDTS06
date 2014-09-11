#include "url_filter.hpp"
#include <vector>
#include <sstream>

using namespace std;

url_filter::url_filter()
{
	key_words.push_back("SpongeBob");
	key_words.push_back("Britney Spears");
	key_words.push_back("Paris Hilton");
	key_words.push_back("Norrköping");
}

url_filter::~url_filter()
{

}

string url_filter::filter(string http)
{
  cout<< http<< endl;
  string request;
  string url;
  stringstream ss;
  ss << http;
  ss >> request >> url;
  cout << endl << endl << endl;
  cout << request << endl;

  cout << url << endl;
  //Look for key word in string
	/*for (auto key : key_words)
	{
		found = http.find(key);


		if (found != string::npos)
		{
			cout << key <<" found at: " << found << endl;
		}
		
		}*/
  return url;
	


}
