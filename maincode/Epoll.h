#pragma
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
public:
    Epoll();
    ~Epoll();

    int getEpfd();

    void updateChannel(Channel*);
    void deleteChannel(Channel*);

    std::vector<Channel*> poll(int timeout = -1);  

private:
    int epfd;
    struct epoll_event *events;
};