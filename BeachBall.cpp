#include <iostream>
#include <memory>
#include <thread>
#include <random>
#include <chrono>

#include "Semaphore.h"

#include "XDisplayBase.h"
#include "XBeachBall.h"
#include "XDrawAverages.h"
#include "XDrawTracks.h"
#include "Data.h"



Semaphore g_sem_consumed(3);
std::vector<Semaphore> g_sem_produced;



void producer(Data& data, unsigned int sectors, unsigned int wait_ms = 0, unsigned int nthreads = 2)
{
#ifdef DEBUG
  std::cout << "[producer] initial data = " << data.sector << std::endl;
#endif  // DEBUG
  std::mutex mex;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dist_sector(0, sectors-1);
  std::normal_distribution<double> dist_energy(7,1);
  std::normal_distribution<double> dist_delta_radius(2,1.5);
  std::uniform_int_distribution<unsigned int> dist_position(5,50);
  std::uniform_int_distribution<unsigned int> dist_tracklength(1,18);

  g_sem_consumed.wait_all();
  while (true) {
    {
      std::mutex mex;
      int sector = dist_sector(generator);
      unsigned int position = dist_position(generator);
      unsigned int tracklength = dist_tracklength(generator);
      std::vector<std::pair<double,double>> track;
      double radius = 0;
      double energy_tot = 0;
      for ( unsigned int i = 0 ; i < tracklength ; ++i ){
	radius += dist_delta_radius(generator);
	double energy = dist_energy(generator);
	energy_tot += energy;
	track.push_back( {radius, energy} );
      }
      std::lock_guard<std::mutex> lk(mex);
      data.sector = sector;
      data.energy = energy_tot;
      data.position = position;
      data.track = track;
      std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));  // slow down data generation a bit
#ifdef DEBUG
      std::cout << "[producer] data = " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
    }

    for (unsigned int i = 0; i < nthreads; ++i ) {
      g_sem_produced.at(i).notify_one();
    }
    g_sem_consumed.wait_all();
  }
}




void calculate_averages(XDrawAverages& draw_ave, const Data& data, unsigned int num = 1)
{
  g_sem_consumed.notify_one();
  while (true) {
      g_sem_produced.at(num).wait_one();
#ifdef DEBUG
      std::cout << "[calculate_averages] got lock; data: " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
    draw_ave.update(data);
    g_sem_consumed.notify_one();
  }
}



void fill_chart(XBeachBall& beach_ball, const Data& data, unsigned int num = 0)
{
  g_sem_consumed.notify_one();
  while (true) {
      g_sem_produced.at(num).wait_one();

#ifdef DEBUG
      std::cout << "[fill_chart] got lock; data: " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
    beach_ball.update(data);
    g_sem_consumed.notify_one();
  }
}


void draw_tracks(XDrawTracks& drawtracks, const Data& data, unsigned int num = 2)
{
  g_sem_consumed.notify_one();
  while (true) {
      g_sem_produced.at(num).wait_one();
#ifdef DEBUG
      std::cout << "[draw_tracks] got lock; data: " << data.sector << ", " << data.energy << std::endl;
#endif  // DEBUG
    drawtracks.update(data);
    g_sem_consumed.notify_one();
  }
}




int main()
{
  unsigned int sectors = 8;
  unsigned int nthreads = 3;
  unsigned int wait_ms = 100;  // how long the producer waits before producing data.
  
  for (unsigned int i = 0; i < nthreads; ++i ) {
     g_sem_produced.emplace_back( Semaphore() );
  }
  
  std::shared_ptr<XDisplayBase>display_base = std::make_shared<XDisplayBase>();
  XBeachBall beach_ball("Beachball", display_base, 300, 300, sectors);
  beach_ball.set_step_size(2);
  XDrawAverages averages("Average Energies", display_base, 150, 200, sectors);
  XDrawTracks drawtracks( "Tracks", display_base, 500,300, sectors);
  Data data;

  std::thread t0( producer, std::ref(data), sectors, wait_ms, nthreads );
  std::thread t1( fill_chart, std::ref(beach_ball), std::cref(data), 0);
  std::thread t2( calculate_averages, std::ref(averages), std::cref(data), 1);
  std::thread t3( draw_tracks, std::ref(drawtracks), std::cref(data), 2);

  t0.join();
  t1.join();
  t2.join();
  t3.join();
}
