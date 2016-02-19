
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>

#include <X11/Xlib.h>

#include "XDisplayBase.h"



XDisplayBase::XDisplayBase() {
  XInitThreads();
  xDisplay = XOpenDisplay (0);
  if ( !xDisplay )
    throw std::runtime_error("[XDisplayBase] Error: Couldn't open display.");
  
  screenNum = DefaultScreen( xDisplay );
  rootWindow = RootWindow(xDisplay, screenNum);

  colourMap = DefaultColormap(xDisplay, screenNum);
  blackColour = BlackPixel(xDisplay, screenNum);
  whiteColour = WhitePixel(xDisplay, screenNum);
 
  std::string rgbValues[8] = {"RGBi:0.627451/0.062745/0.09019608","RGBi:0.01960784313725/0.36862745098039/0.60392156862745","RGBi:0/0.6/0.30196078431372","RGBi:0.75294117647059/0.8/0.23137254901961","RGBi:0.33725490196078/0.12941176470588/0.47843137254902","RGBi:0.16862745098039/0.63529411764706/0.58039215686275","RGBi:0.87058823529412/0.55294117647059/0.16470588235294","RGBi:	0.03921568627451/0.13333333333333/0.27058823529412"};
  for (int i=0; i<8; i++ ){
    char* colourName = new char[std::strlen(rgbValues[i].c_str())];
    strcpy(colourName,rgbValues[i].c_str());
    sectorColour[i] = getColour(colourName);
  }
}



unsigned long
XDisplayBase::getColour( const std::string& colourName )
{
  XColor approxColour, trueColour;
  colourMap = DefaultColormap(xDisplay, screenNum);
  XAllocNamedColor( xDisplay, colourMap, colourName.c_str(), &approxColour, &trueColour );
  return( approxColour.pixel );
}



XDisplayBase::~XDisplayBase()
{
#ifdef DEBUG
  std::cout << "[XDisplayBase::~XDisplayBase]" << std::endl;
#endif // DEBUG
  XCloseDisplay(xDisplay);
#ifdef DEBUG
  std::cout << "[XDisplayBase::~XDisplayBase] display closed" << std::endl;
#endif // DEBUG
}
