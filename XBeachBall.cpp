#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cmath>


#include "XBeachBall.h"



XBeachBall::XBeachBall(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts)
  : XWindowObject(name, disp_base, width, height, n_contexts)
{
#ifdef DEBUG
  std::cout << "[XBeachBall::XBeachBall]" << std::endl;
#endif // DEBUG
  n_sectors_ = n_contexts;
  zero_sectors();
}




void
XBeachBall::zero_sectors(){
  sector_count.clear();
  for (unsigned int i =0; i < n_sectors_; i++)
    sector_count.push_back(0);
}


void
XBeachBall::check_size(unsigned int counts){
  if (counts >= width()/2) zero_sectors();
}



void
XBeachBall::update(const Data& data) {
  //  std::cout << "[XBeachBall::increase_sector]" << std::endl;
  XWindowObject::update(data);
  
  const unsigned int& sector = data.sector;
  check_size(sector_count[sector]);

  sector_count[sector] += step_size_;
  
  for ( unsigned int i = 0; i < n_sectors_; ++i ) {
    XFillArc(display(), window(), context(i), width()/2-sector_count[i], width()/2-sector_count[i], 2*sector_count[i], 2*sector_count[i], 64*(90+i*360/n_sectors_), 360/n_sectors_*64);
  }
  XFlush(display());
}


