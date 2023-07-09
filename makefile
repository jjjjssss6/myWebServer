CXX = g++

CXXFLAGS = -lpthread

DEBUG = 1

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
else
	CXXFLAGS += -O2
endif

server: server.cpp ./maincode/Acceptor.cpp ./maincode/Buffer.cpp ./maincode/Connection.cpp ./maincode/Channel.cpp ./maincode/Epoll.cpp ./maincode/EventLoop.cpp ./maincode/Socket.cpp ./maincode/Server.cpp ./maincode/util.cpp ./maincode/myThreadPool/ThreadPool.h ./maincode/myThreadPool/SafeQueue.h ./maincode/Timer/Timer.cpp
		$(CXX) -o server $^ $(CXXFLAGS)

cleanserver: 
		rm -r server

client: client.cpp ./maincode/Buffer.cpp ./maincode/Socket.cpp ./maincode/util.cpp
		$(CXX) -o client $^ $(CXXFLAGS)

cleanclient: 
		rm -r client

clean:
		rm -r server client
//aaaaa
