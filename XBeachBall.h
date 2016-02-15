#ifndef BeachBall_h
#define BeachBall_h

#include <vector>
#include <memory>

#include <X11/Xlib.h>

#include "XWindowObject.h"
#include "Data.h"

#define PI 3.14159265

class XDisplayBase;


class XBeachBall : public XWindowObject
{
 public:
  XBeachBall(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts = 8);
  void update(const Data& data) override;
  void set_step_size(unsigned int step) {step_size_ = step;}

 private:
  void check_size(unsigned int counts);
  void zero_sectors();

  unsigned int n_sectors_;
  unsigned int step_size_ = 1;
  std::vector<unsigned int> sector_count;

};

#endif
