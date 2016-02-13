#ifndef BeachBall_h
#define BeachBall_h

#include <vector>
#include <memory>

#include <X11/Xlib.h>

#include "XDisplayBase.h"
#include "Data.h"

#define PI 3.14159265

class XDisplayBase;

class XBeachBall {
 public:
  XBeachBall(std::shared_ptr<XDisplayBase> display_base, unsigned int nsectors = 8, unsigned int step = 1);
  ~XBeachBall();
  void update(const Data& data);


 private:
  void check_size(int counts, int width);
  void open_window();
  void zero_sectors();

  std::shared_ptr<XDisplayBase> display_base;
  unsigned int n_sectors_;
  unsigned int step_size_;
  Window window_;
  std::vector<GC> sector_context;
  XWindowAttributes window_attributes_;
  std::vector<int> sector_count;

};

#endif
