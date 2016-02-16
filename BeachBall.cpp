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



void producer(Data& data, unsigned int sectors, unsigned int wait_ms, unsigned int nthreads)
{
#ifdef DEBUG
  std::cout << "[producer] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  std::mutex m;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist_int(0, sectors-1);
  std::normal_distribution<double> dist_norm(500,100);
  std::this_thread::sleep_for(std::chrono::milliseconds(80));  // make sure consumers are ready
  while (true) {
    // {  The class lock_guard is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block. -> {}
    //	std::lock_guard<std::mutex> lock(mex);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));  // slow down data generation a bit
    std::unique_lock<std::mutex> lock(mex);
#ifdef DEBUG
    std::cout << "[producer] has lock" << std::endl;
#endif  // DEBUG
    data.sector = dist_int(generator);
    data.energy = dist_norm(generator);
#ifdef DEBUG
    std::cout << "[producer] data = " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
#ifdef DEBUG
    std::cout << "[producer] notified all"  << std::endl;
#endif  // DEBUG

    for ( unsigned int i = 0; i < nthreads; ++i ){
      gdata_available.notify_one();
      gread_finished.wait(lock);
#ifdef DEBUG
      std::cout << "[producer] got notified" << std::endl;
#endif  // DEBUG
    }
  }
}




void calculate_averages(XDrawAverages& draw_ave, const Data& data)
{
#ifdef DEBUG
      std::cout << "[calculate_averages] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  while (true) {
    std::unique_lock<std::mutex> lock(mex);
    //    gdata_available.wait(lock, [data] {return data.has_data;} );
    gdata_available.wait(lock);
#ifdef DEBUG
      std::cout << "[calculate_averages] got lock; data: " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
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
      std::cout << "[fill_chart] got lock; data: " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
    beach_ball.update(data);
    gread_finished.notify_one();
  }
}





int main()
{
  unsigned int sectors = 8;
  unsigned int nthreads = 2;
  unsigned int wait_ms = 30;  // how long the producer waits before producing data.
  
  std::shared_ptr<XDisplayBase>display_base = std::make_shared<XDisplayBase>();
  XBeachBall beach_ball("Beachball", display_base, 300, 300, sectors);
  beach_ball.set_step_size(2);
  XDrawAverages averages("Average Energies", display_base, 150, 200, sectors);
  Data data;

  std::thread t1( producer, std::ref(data), sectors, wait_ms, nthreads );
  std::thread t2( fill_chart, std::ref(beach_ball), std::cref(data));
  std::thread t3( calculate_averages, std::ref(averages), std::cref(data));

  t1.join();
  t2.join();
  t3.join();
}
