#ifndef DATA_H
#define DATA_H

#include <mutex>
#include <vector>

struct Data
{
  unsigned int sector = 0;
  double energy = 0.0;
  unsigned int position = 0;
  std::vector<std::pair<double,double>> track;
};


#endif  // DATA_H
