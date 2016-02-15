#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>

#include "XWindowObject.h"


XWindowObject::XWindowObject(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts)
  : display_base(disp_base), height_(height), width_(width)
{
#ifdef DEBUG
  std::cout << "[XWindowObject::XWindowObject]" << std::endl;
#endif // DEBUG
  open_window(name, n_contexts);
  XFlush(display_base->xDisplay);
}



XWindowObject::~XWindowObject() {
#ifdef DEBUG
  std::cout << "[XWindowObject::~XWindowObject]" << std::endl;
#endif // DEBUG
  XDestroyWindow(display_base->xDisplay, window_);
#ifdef DEBUG
  std::cout << "[XWindowObject::~XWindowObject] window destroyed" << std::endl;
#endif // DEBUG
}



void
XWindowObject::open_window(std::string name, unsigned int n_contexts)
{
#ifdef DEBUG
  std::cout << "[XWindowObject::open_window]" << std::endl;
#endif  // DEBUG
  window_ = XCreateSimpleWindow(display_base->xDisplay,display_base->rootWindow,0, 0, width_, height_, 0,display_base->whiteColour, display_base->whiteColour);

  char* window_name = new char[32];
  std::strcpy(window_name, name.c_str());
  XStoreName(display_base->xDisplay, window_, window_name);
	  
  XSelectInput(display_base->xDisplay, window_, StructureNotifyMask | ExposureMask|ButtonPressMask);
  XMapWindow(display_base->xDisplay, window_);

  for (unsigned int i=0; i<n_contexts; i++ ){
    XGCValues values;
    values.foreground = display_base->sectorColour[i%8]; // have to find something better to automatically create colours either in XDisplayBase or here.
    unsigned long valuemask = GCForeground;
    contexts_.push_back( XCreateGC(display_base->xDisplay, window_, valuemask, &values) );
    if ( contexts_.back() < 0 )
      std::cerr << "XCreateGC failed" << std::endl;
  }
}


void
XWindowObject::update(const Data& data) {
  //  std::cout << "[XWindowObject::increase_sector]" << std::endl;
  XClearWindow(display_base->xDisplay, window_);
  XGetWindowAttributes(display_base->xDisplay, window_, &window_attributes_);
  width_ = window_attributes_.width;
  height_ = window_attributes_.height;
}


