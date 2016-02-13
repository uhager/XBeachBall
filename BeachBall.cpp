#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>

#include "XDisplayBase.h"
#include "XBeachBall.h"


std::mutex mex;
std::condition_variable gdata_available;           
std::condition_variable gread_finished; 


struct Data
{
  int sector = 0;
  double energy = 0.0;
};


void producer(unsigned int sectors, Data& data)
{
#ifdef DEBUG
      std::cout << "[producer] initial data = " << data << std::endl;
#endif  // DEBUG
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist(0, sectors-1);
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(80));  // slow down data generation a bit
    {  // The class lock_guard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block. -> {}
      std::lock_guard<std::mutex> lock(mex);
      data.sector = dist(generator);
#ifdef DEBUG
      std::cout << "[producer] data = " << data << std::endl;
#endif  // DEBUG
    }
    gdata_available.notify_all();
    //    gread_finished.wait();
  }
}


void fill_chart(XBeachBall& beach_ball, const Data& data)
{
#ifdef DEBUG
      std::cout << "[fill_chart] initial data = " << data << std::endl;
#endif  // DEBUG
  while (true) {
    std::unique_lock<std::mutex> lock(mex);
    gdata_available.wait(lock);
#ifdef DEBUG
      std::cout << "[fill_chart] data = " << data << std::endl;
#endif  // DEBUG
    beach_ball.increase_sector(data.sector);
    gread_finished.notify_one();
  }
}


int main()
{
  unsigned int sectors = 5;
  std::shared_ptr<XDisplayBase>display_base = std::make_shared<XDisplayBase>();
  XBeachBall beach_ball(display_base, sectors, 3);
  Data data;
  std::thread t1( producer, sectors, std::ref(data) );
  std::thread t2( fill_chart, std::ref(beach_ball), std::cref(data));

  t1.join();
  t2.join();
  // for (unsigned int i = 0 ; i < 200 ; ++i ) {
  //   beach_ball.increase_sector(i%8);
  // }
  // sleep(2);
}
