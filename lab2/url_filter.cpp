#include "url_filter.hpp"
#include <vector>

using namespace std;

url_filter::url_filter(int port)
{
	key_words.push_back("SpongeBob");
	key_words.push_back("Britney Spears");
	key_words.push_back("Paris Hilton");
	key_words.push_back("Norrköping");
}

url_filter::~url_filter()
{

}

int url_filter::filter(string http)
{

	//Look for key word in string
	for (auto key : key_words)
	{
		found = http.find(key);


		if (found != string::npos)
		{
			cout << key <<" found at: " << found << endl;
		}
		
	}
	


}