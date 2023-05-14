#include "Connection.h"
#include "Server.h"
#include "ServerConnectionHandler.h"
#include "Socket.h"
#include "Task.h"
#include "Callback.h"
#include "ThreadPool.h"
#include <cstdlib>
#include <mutex>
#include <pthread.h>
#include <thread>

class test
{
private:
    int i{89};
public:
    test() = default;
    void callme(iu::Connection&& c)
    {
        std::cout << "Method " << i << '\n';
    }
};

static std::mutex m;

void lol(iu::Connection&& c)
{
    for(size_t i = 0; i < 10; i++)
    {
        {
            std::lock_guard l(m);
            std::cout << std::this_thread::get_id() <<" : "<< i <<'\n';
        }

        std::this_thread::sleep_for(std::chrono::seconds(rand()%10));
    }
}

int main()
{
    //1. decide if a packet parser is needed and decide architecture
    //2. maby constexpr handling of blocking or nonblocking sockets (?)
    //5. figure out how to stop handlers with infinite loops

    iu::DistributedServer server("0.0.0.0", 4000, 2);

    std::thread t([&server](){server.Run();});

    int i = 0;

    std::cin >> i;

    server.Stop();
    t.join();

    return 0;
}