#ifndef DATA_H
#define DATA_H

#include <mutex>

struct Data
{
  int sector = 0;
  double energy = 0.0;
  bool has_data = false;
};


#endif  // DATA_H
