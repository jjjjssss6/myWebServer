#include "maincode/EventLoop.h"
#include "maincode/Server.h"
#include "maincode/Timer/Timer.h"

int main()
{
    Timer *timer = new Timer();
    EventLoop *loop = new EventLoop(timer);
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}