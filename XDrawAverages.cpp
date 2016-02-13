#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cmath>

#include "XDisplayBase.h"
#include "Data.h"

#include "XDrawAverages.h"



XDrawAverages::XDrawAverages(std::shared_ptr<XDisplayBase> disp_base, unsigned int nsectors, unsigned int ave_over)
  : display_base(disp_base), n_sectors_(nsectors), average_over_(ave_over), sums_(nsectors,0.0), values_(nsectors)
{
#ifdef DEBUG
  std::cout << "[XDrawAverages::XDrawAverages]" << std::endl;
#endif // DEBUG
  open_window();
}


XDrawAverages::~XDrawAverages() {
#ifdef DEBUG
  std::cout << "[XDrawAverages::~XDrawAverages]" << std::endl;
#endif // DEBUG
  XDestroyWindow(display_base->xDisplay, window_);
#ifdef DEBUG
  std::cout << "[XDrawAverages::~XDrawAverages] window destroyed" << std::endl;
#endif // DEBUG
}



void
XDrawAverages::open_window()
{
#ifdef DEBUG
  std::cout << "[XDrawAverages::open_window]" << std::endl;
#endif  // DEBUG
  window_ = XCreateSimpleWindow(display_base->xDisplay,display_base->rootWindow,0, 0, DisplayWidth(display_base->xDisplay, display_base->screenNum)/4, DisplayWidth(display_base->xDisplay, display_base->screenNum)/4, 0,display_base->whiteColour, display_base->whiteColour);

  char* window_name = new char[32];
  std::strcpy(window_name, "Average energies");
  XStoreName(display_base->xDisplay, window_, window_name);
	  
  XSelectInput(display_base->xDisplay, window_, StructureNotifyMask | ExposureMask|ButtonPressMask);
  XMapWindow(display_base->xDisplay, window_);

  for (unsigned int i=0; i<n_sectors_; i++ ){
    XGCValues values;
    values.foreground = display_base->sectorColour[i%8]; // have to find something better to automatically create colours either in XDisplayBase or here.
    unsigned long valuemask = GCForeground;
    sector_context.push_back( XCreateGC(display_base->xDisplay, window_, valuemask, &values) );
    if ( sector_context.back() < 0 )
      std::cerr << "XCreateGC failed" << std::endl;
  }
   XFlush(display_base->xDisplay);
}



void
XDrawAverages::update(const Data& data) {
#ifdef DEBUG
  std::cout << "[XDrawAverages::update] " << data.sector << " - " << data.energy << std::endl;
#endif // DEBUG
  if ( !window_ )
    open_window();

  XClearWindow(display_base->xDisplay, window_);
  XGetWindowAttributes(display_base->xDisplay, window_, &window_attributes_);

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
  
  for ( unsigned int i = 0; i < n_sectors_; ++i ) {
    double average = sums_.at(i) / values_.at(i).size();
    int height = window_attributes_.height / n_sectors_;
    std::string out_string = "sector " + std::to_string(i) + ": " + std::to_string(average);
    XDrawString ( display_base->xDisplay , window_, sector_context.at(i), 0, height * i, out_string.c_str(), out_string.size() );
  }
   XFlush(display_base->xDisplay);
}


