
#include "utility\io.h"

#include <fstream>

std::string *io::read(const char *filename)
{
    std::string *text = new std::string();
    std::ifstream fs(filename);
    if (fs.is_open())
    {
        std::string line;
        while (getline(fs, line))
        {
            text->append(line).append("\n");
        }
        fs.close();
    }
    else
    {
        delete text;
        throw std::exception("File could not be opened.");
    }
    return text;
}
