#ifndef XDRAWAVERAGES_H
#define XDRAWAVERAGES_H

#include <vector>
#include <deque>
#include <memory>

#include <X11/Xlib.h>

#include "XWindowObject.h"

#define PI 3.14159265

class Data;

class XDrawAverages : public XWindowObject
{
 public:
  XDrawAverages(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts = 8);

  void update(const Data& data) override;
  void average_over(unsigned int num){ average_over_ = num;}

 private:
  unsigned int n_sectors_;
  unsigned int average_over_ = 100;
  std::vector<double> sums_;
  std::vector< std::deque<double> > values_;
};

#endif  // XDRAWAVERAGES_H
