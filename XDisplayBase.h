#ifndef XDisplayBase_h
#define XDisplayBase_h

#include <string>

#include <X11/Xlib.h>


class XDisplayBase{

 public:
  XDisplayBase();
  ~XDisplayBase();
  //	 unsigned long getColour(char* colourName);
  unsigned long getColour( const std::string& colourName);
  bool initCheck;
  int screenNum;
  Window rootWindow;
  unsigned long blackColour, whiteColour;
  Display* xDisplay;
  Colormap colourMap;
  unsigned long sectorColour[8];
};


#endif
