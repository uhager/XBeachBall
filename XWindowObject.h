#ifndef XWINDOWOBJECT_H
#define XWINDOWOBJECT_H

#include <memory>
#include <vector>
#include <string>

#include "Data.h"

#include "XDisplayBase.h"

class XWindowObject
{
 public:
  XWindowObject(std::string name, std::shared_ptr<XDisplayBase> display_base, unsigned int width, unsigned int height, unsigned int n_contexts);
  ~XWindowObject();
  virtual void update(const Data& data);
  unsigned int width() {return width_;}
  unsigned int height() { return height_;}


 protected:
  GC context(unsigned int i) {return contexts_.at(i);}
  Display* display(){ return display_base->xDisplay;}
  Window window() { return window_;}
  
 private:
  void open_window(std::string name, unsigned int n_contexts);

  std::shared_ptr<XDisplayBase> display_base;
  std::vector<GC> contexts_;
  unsigned int height_;
  unsigned int width_;
  Window window_;
  XWindowAttributes window_attributes_;
};

#endif  //  XWINDOWOBJECT_H
