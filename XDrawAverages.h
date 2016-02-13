#ifndef XDRAWAVERAGES_H
#define XDRAWAVERAGES_H

#include <vector>
#include <deque>
#include <memory>

#include <X11/Xlib.h>

#include "XDisplayBase.h"

#define PI 3.14159265

class XDisplayBase;
class Data;

class XDrawAverages {
 public:
  XDrawAverages(std::shared_ptr<XDisplayBase> display_base, unsigned int nsectors = 8, unsigned int ave_over = 100);
  ~XDrawAverages();
  void update(const Data& data);


 private:
  void open_window();

  std::shared_ptr<XDisplayBase> display_base;
  unsigned int n_sectors_;
  unsigned int average_over_;
  Window window_;
  std::vector<GC> sector_context;
  XWindowAttributes window_attributes_;
  std::vector<double> sums_;
  std::vector< std::deque<double> > values_;
};

#endif  // XDRAWAVERAGES_H
