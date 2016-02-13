#ifndef BeachBall_h
#define BeachBall_h

#include <vector>
#include <memory>

#include <X11/Xlib.h>

#include "XDisplayBase.h"

#define PI 3.14159265

class XDisplayBase;


class XBeachBall {
 public:
  XBeachBall(std::shared_ptr<XDisplayBase> display_base, unsigned int step = 1);
  ~XBeachBall();
  void increase_sector(unsigned int sector);


 private:
  void check_size(int counts, int width);
  void open_window();
  void zero_sectors();

  std::shared_ptr<XDisplayBase> display_base;
  unsigned int step_size;
  Window ball_window;
  std::vector<GC> sector_context;
  XWindowAttributes ball_window_attributes;
  std::vector<int> sector_count;

};

#endif
