#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>
#include <condition_variable>

#include "XDisplayBase.h"
#include "XBeachBall.h"
#include "XDrawAverages.h"
#include "Data.h"

std::mutex mex;
std::condition_variable gdata_available;           
std::condition_variable gread_finished; 



void producer(unsigned int sectors, Data& data)
{
#ifdef DEBUG
      std::cout << "[producer] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist_int(0, sectors-1);
  std::normal_distribution<double> dist_norm(500,100);
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(80));  // slow down data generation a bit
    {  // The class lock_guard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block. -> {}
      std::lock_guard<std::mutex> lock(mex);
      data.sector = dist_int(generator);
      data.energy = dist_norm(generator);
#ifdef DEBUG
      std::cout << "[producer] data = " << data.sector << std::endl;
#endif  // DEBUG
    }
    gdata_available.notify_all();
    //    gread_finished.wait();
  }
}



void calculate_averages(XDrawAverages& draw_ave, const Data& data)
{
#ifdef DEBUG
      std::cout << "[calculate_averages] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  while (true) {
    std::unique_lock<std::mutex> lock(mex);
    gdata_available.wait(lock);
    draw_ave.update(data);
    gread_finished.notify_one();
  }
}



void fill_chart(XBeachBall& beach_ball, const Data& data)
{
#ifdef DEBUG
  std::cout << "[fill_chart] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  while (true) {
    std::unique_lock<std::mutex> lock(mex);
    gdata_available.wait(lock);
#ifdef DEBUG
      std::cout << "[fill_chart] data = " << data.sector << std::endl;
#endif  // DEBUG
    beach_ball.update(data);
    gread_finished.notify_one();
  }
}





int main()
{
  unsigned int sectors = 8;
  std::shared_ptr<XDisplayBase>display_base = std::make_shared<XDisplayBase>();
  XBeachBall beach_ball(display_base, sectors, 3);
  XDrawAverages averages(display_base, sectors, 200);
  Data data;
  std::thread t1( producer, sectors, std::ref(data) );
  std::thread t2( fill_chart, std::ref(beach_ball), std::cref(data));
  std::thread t3( calculate_averages, std::ref(averages), std::cref(data));

  t1.join();
  t2.join();
  t3.join();
}
