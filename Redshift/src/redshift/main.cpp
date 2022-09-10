
#include "display/display.hpp"

int main()
{
    Display display{"Redshift", 1000, 600};

    while(!display.shouldClose())
        display.frame();
}
