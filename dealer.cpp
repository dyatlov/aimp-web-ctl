#include "dealer.h"
#include "Responder.h"

RESPONSE_TYPE get_response(void *pInterface, std::string& response, char *query_string)
{
	std::string str(query_string);
	
	std::map<std::string, std::string> params;
	parse_params(str, params);
	
	CResponder responder(reinterpret_cast<IAIMP2Controller *>(pInterface), params);
	
	response = responder.GetResponse();

	return responder.GetResponseType();
}

void parse_params(std::string& str, std::map<std::string, std::string>& params)
{
	std::vector<std::string> tokens;
	std::string delimiters("&");
	tokenize(str, tokens, delimiters);
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> mypair;
		delimiters = "=";
		tokenize(tokens.at(i), mypair, delimiters);
		if(mypair.size() > 1)
		{
			params.insert(std::pair<std::string,std::string>(mypair.at(0), mypair.at(1)));
		}
	}
}

void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}
