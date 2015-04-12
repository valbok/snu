/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "ConnectomeParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace NSnu
{

ConnectomeParser::ConnectomeParser(const char* filename) throw()
    : mFilename(filename)
{
}

bool ConnectomeParser::parse(TConnections& result)
{
    ifstream file(mFilename.c_str());
    if (!file.is_open())
    {
        return false;
    }

    while (file.good())
    {
        string line;
        file >> line;
        istringstream ss(line);
        string token;
        vector<string> splitted;
        for (int i = 0; i < 3 && getline(ss, token, ','); ++i)
        {
            splitted.push_back(token);
        }

        if (splitted.size() < 3)
        {
            continue;
        }

        result.push_back(make_tuple(splitted[0], splitted[1], stoi(splitted[2])));
    }

    return !result.empty();
}

} // namespace NSin
