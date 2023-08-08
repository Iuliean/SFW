SRC=Socket.cpp \
	Connection.cpp \
	SocketDescriptor.cpp \
	ThreadPool.cpp \
	Server.cpp \
	ServerConnectionHandler.cpp \
	utils.cpp \
	LoggerManager.cpp \
	Logger.cpp
OBJECT=$(addprefix $(INTERMEDIATES), $(SRC:.cpp=.o))

.PHONY: all

all: $(OBJECT)

$(INTERMEDIATES)%.o:%.cpp
	$(CXX) -c -o $@ $< $(INCLUDE) $(CFLAGS)