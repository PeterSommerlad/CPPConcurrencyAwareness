// adapted from listing 5.5 ccia
// https://github.com/anthonywilliams/ccia_code_samples/blob/main/listings/listing_5.5.cpp
#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);
    y.store(true,std::memory_order_relaxed);
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));
    if(x.load(std::memory_order_relaxed)) // could read zero
        ++z;
}

int main()
{
    x=false;
    y=false;
    z=0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load()!=0); // might fire
    return z.load();
}
