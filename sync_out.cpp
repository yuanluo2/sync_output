/*
    @author yuanluo2
    @brief a way to sync the std::ostream. thanks for https://stackoverflow.com/questions/18277304/using-stdcout-in-multiple-threads Howard Hinnant's great answer.
*/
#include <iostream>
#include <utility>
#include <thread>
#include <mutex>

std::mutex mut;

std::ostream& sync_out_one(std::ostream& os) {
    return os;
}

template<typename Arg0, typename ... Args>
std::ostream& sync_out_one(std::ostream& os, Arg0&& arg0, Args&& ... args) {
    os << arg0;
    return sync_out_one(os, std::forward<Args>(args)...);
}

template<typename ... Args>
std::ostream& sync_out(std::ostream& os, Args&& ... args) {
    std::lock_guard<std::mutex> lgmt{ mut };
    return sync_out_one(os, std::forward<Args>(args)...);
}

int main() {
    std::thread t0 { [](){ sync_out(std::cout, "Good deeds are like water.\n"); } };
    std::thread t1 { [](){ sync_out(std::cout, "Whater is good for all things, ", "and does not compete, \n"); } };
    std::thread t2 { [](){ sync_out(std::cout, "and it is good, ", "for dealing with the evil of others, \n"); } };
    std::thread t3 { [](){ sync_out(std::cout, "so it is few compared to the Tao.\n"); } };

    t0.join();
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
