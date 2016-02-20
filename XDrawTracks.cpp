#include <iostream>
#include <vector>

#include "XDrawTracks.h"


using namespace std;


XDrawTracks::XDrawTracks(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts)
  : XWindowObject(name, disp_base, width, height, n_contexts)
{
#ifdef DEBUG
  std::cout << "[XDrawTracks::XDrawTracks]" << std::endl;
#endif // DEBUG
  n_sectors_ = n_contexts;
  for (int i=0; i<2; i++ )
    lineContext[i] = XCreateGC(display(), window(), 0, 0);

  XSetLineAttributes(display(), lineContext[0], 1, LineSolid, CapRound, JoinRound); //width, style, line-end style, line join style
  XSetLineAttributes(display(), lineContext[1], 1, LineOnOffDash, CapRound, JoinRound); //width, style, line-end style, line join style
}




void XDrawTracks::update(const Data& data)
{
#ifdef DEBUG
  std::cout << "[XDrawTracks::update] " << data.sector << " - " << data.energy << std::endl;
#endif // DEBUG
  if (data.sector > n_sectors_ )
    return;

  XWindowObject::update(data);
  double position = data.position ;
  int odd_even = data.sector % 2;
	
  XLockDisplay(display());

  XDrawLine(display(), window(), lineContext[0], 5, 5, width()-5, 5);
  XDrawLine(display(), window(), lineContext[0], 5, height()/2, window()-5, height()/2);
  XDrawLine(display(), window(), lineContext[0], 5, height()-5, window()-5, height()-5);
  XDrawLine(display(), window(), lineContext[1], 5, height()/2 * (height()-10)/2, window()-5, height()/2 * (height()-10)/2);
  XDrawLine(display(), window(), lineContext[1], 5, height()/2 * (height()-10)/2, window()-5, height()/2 * (height()-10)/2);

  for (unsigned int i = 0; i < data.track.size(); ++i) {
    double z_pos = (position + i)/ z_max_ * (width()-10)+5;
    double radius =  data.track.at(i).first / r_max_ * (height()-10)/2;
    double energy = data.track.at(i).second * energy_scale_;

    switch(odd_even){
    case 1:
      XFillArc(display(), window(), context(data.sector), z_pos-energy, height()/2-radius-energy, energy*2, energy*2, 0, 360*64);
      break;
    case 0:
      XFillArc(display(), window(), context(data.sector), z_pos-(energy), height()/2+radius-(energy), energy*2, energy*2, 0, 360*64);
      break;
    }
  }
  XFlush(display());
  XUnlockDisplay(display());
#ifdef DEBUG
  std::cout << "[XDrawTracks::update] finished" << std::endl;
#endif // DEBUG
}


