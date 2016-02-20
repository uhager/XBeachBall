#ifndef XDrawTracks_h
#define XDrawTracks_h

#include "XWindowObject.h"


class XDisplayBase;

class XDrawTracks  : public XWindowObject
{
    public:
  XDrawTracks(std::string name, std::shared_ptr<XDisplayBase> disp_base, unsigned int width, unsigned int height, unsigned int n_contexts = 8);
  void update(const Data& data) override;

private:
  unsigned int n_sectors_;
  double energy_scale_ = 1;
  int z_max_ = 64;
  int r_max_ = 32;
  GC lineContext[2];
};

#endif

