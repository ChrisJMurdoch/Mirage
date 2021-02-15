
#include "utility\io.h"

#include <iostream>
#include <fstream>

std::string *io::read(const char* filename)
{
    std::ifstream fs(filename);
    std::string *text = new std::string();
    std::string line;
    if (fs.is_open())
    {
        while (getline(fs, line))
        {
            text->append(line).append("\n");
        }
        fs.close();
    }
    else
    {
        throw std::exception("File could not be opened.");
    }
    return text;
}
