#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cmath>

#include "Data.h"

#include "XDrawAverages.h"



XDrawAverages::XDrawAverages(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts)
  : XWindowObject(name, disp_base, width, height, n_contexts), sums_(n_contexts,0.0), values_(n_contexts)
{
#ifdef DEBUG
  std::cout << "[XDrawAverages::XDrawAverages]" << std::endl;
#endif // DEBUG
  n_sectors_ = n_contexts;
}




void
XDrawAverages::update(const Data& data) {
#ifdef DEBUG
  std::cout << "[XDrawAverages::update] " << data.sector << " - " << data.energy << std::endl;
#endif // DEBUG
  
  XWindowObject::update(data);

  const unsigned int& sector = data.sector;
  const double& energy = data.energy;
  std::deque<double>& values = values_.at(sector);
  double& sum = sums_.at(sector);
  
  values.push_back(energy);
  sum += energy;
  if ( values.size() > average_over_ ) {
    sum -= values.front();
    values.pop_front();
  }
  
  XLockDisplay(display());
  for ( unsigned int i = 0; i < n_sectors_; ++i ) {
    double average = sums_.at(i) / values_.at(i).size();
    int height_i = height() / n_sectors_;
    std::string out_string = "sector " + std::to_string(i) + ": " + std::to_string(average);
    XDrawString ( display(), window(), context(i), 0, height_i * (i+0.5), out_string.c_str(), out_string.size() );
  }
  XFlush(display());
  XUnlockDisplay(display());
}


