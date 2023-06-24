CXX = g++

CXXFLAGS = -O2 -lpthread


server: server.cpp ./maincode/Acceptor.cpp ./maincode/Buffer.cpp ./maincode/Connection.cpp ./maincode/Channel.cpp ./maincode/Epoll.cpp ./maincode/EventLoop.cpp ./maincode/Socket.cpp ./maincode/Server.cpp ./maincode/util.cpp ./maincode/myThreadPool/ThreadPool.h ./maincode/myThreadPool/SafeQueue.h
		$(CXX) -o server $^ $(CXXFLAGS)

cleanserver: 
		rm -r server

client: client.cpp ./maincode/Acceptor.cpp ./maincode/Buffer.cpp ./maincode/Connection.cpp ./maincode/Channel.cpp ./maincode/Epoll.cpp ./maincode/EventLoop.cpp ./maincode/Socket.cpp ./maincode/Server.cpp ./maincode/util.cpp ./maincode/myThreadPool/ThreadPool.h ./maincode/myThreadPool/SafeQueue.h
		$(CXX) -o client $^ $(CXXFLAGS)

cleanclient: 
		rm -r client

clean:
		rm -r server client