#ifndef DEALER_HEADER_INCLUDED
#define DEALER_HEADER_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <streambuf>

#include "AIMP2_SDK.h"

void get_response(void *pInterface, std::string& response, char *query_string);

void parse_params(std::string& str, std::map<std::string, std::string>& params);

void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");


#endif // DEALER_HEADER_INCLUDED
