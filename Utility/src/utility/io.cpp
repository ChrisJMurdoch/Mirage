
#include "utility/io.hpp"

#include <fstream>

std::string *io::read(char const *filename)
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

void io::write(char const *filename, char const *text)
{
    std::ofstream os;
    os.open(filename);
    os << text;
    os.close();
}
