#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cmath>

#include "XDisplayBase.h"

#include "XBeachBall.h"



XBeachBall::XBeachBall(std::shared_ptr<XDisplayBase> disp_base, unsigned int nsectors, unsigned int step)
  : display_base(disp_base), n_sectors(nsectors), step_size(step)
{
#ifdef DEBUG
  std::cout << "[XBeachBall::XBeachBall]" << std::endl;
#endif // DEBUG
  zero_sectors();
  open_window();
  XFlush(display_base->xDisplay);
}


XBeachBall::~XBeachBall() {
#ifdef DEBUG
  std::cout << "[XBeachBall::~XBeachBall]" << std::endl;
#endif // DEBUG
  XDestroyWindow(display_base->xDisplay, ball_window);
#ifdef DEBUG
  std::cout << "[XBeachBall::~XBeachBall] window destroyed" << std::endl;
#endif // DEBUG
}


void
XBeachBall::open_window()
{
#ifdef DEBUG
  std::cout << "[XBeachBall::open_window]" << std::endl;
#endif  // DEBUG
  ball_window = XCreateSimpleWindow(display_base->xDisplay,display_base->rootWindow,0, 0, DisplayWidth(display_base->xDisplay, display_base->screenNum)/4, DisplayWidth(display_base->xDisplay, display_base->screenNum)/4, 0,display_base->whiteColour, display_base->whiteColour);

  char* window_name = new char[32];
  std::strcpy(window_name, "BeachBall");
  XStoreName(display_base->xDisplay, ball_window, window_name);
	  
  XSelectInput(display_base->xDisplay, ball_window, StructureNotifyMask | ExposureMask|ButtonPressMask);
  XMapWindow(display_base->xDisplay, ball_window);

  for (unsigned int i=0; i<n_sectors; i++ ){
    XGCValues values;
    values.foreground = display_base->sectorColour[i%8]; // have to find something better to automatically create colours either in XDisplayBase or here.
    unsigned long valuemask = GCForeground;
    sector_context.push_back( XCreateGC(display_base->xDisplay, ball_window, valuemask, &values) );
    if ( sector_context.back() < 0 )
      std::cerr << "XCreateGC failed" << std::endl;
  }
}



void
XBeachBall::zero_sectors(){
  sector_count.clear();
  for (unsigned int i =0; i < n_sectors; i++)
    sector_count.push_back(0);
}


void
XBeachBall::check_size(int counts, int width){
  if (counts >= width/2) zero_sectors();
}



void
XBeachBall::increase_sector(unsigned int sector) {
  //  std::cout << "[XBeachBall::increase_sector]" << std::endl;
  if ( !ball_window )
    open_window();

  XClearWindow(display_base->xDisplay, ball_window);
  XGetWindowAttributes(display_base->xDisplay, ball_window, &ball_window_attributes);

  check_size(sector_count[sector], ball_window_attributes.width);

  sector_count[sector] += step_size;
  for ( unsigned int i = 0; i < sector_count.size(); ++i ) {
    XFillArc(display_base->xDisplay, ball_window, sector_context[i], (ball_window_attributes.width)/2-sector_count[i], (ball_window_attributes.width)/2-sector_count[i], 2*sector_count[i], 2*sector_count[i], 64*(90+i*360/n_sectors), 360/n_sectors*64);
  }
   XFlush(display_base->xDisplay);
}


