#include "Timer.h"
#include <unistd.h>

using namespace std;

void print()
{
    printf("ring ring ring\n");
}
int main()
{
    Timer t;
    t.addToTimer(1, 100, std::bind(&print));
    sleep(1);
    t.handleTimeoutEvents();
    t.handleTimeoutEvents();
    return 0;
}