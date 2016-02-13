#include <iostream>
#include <memory>
#include <unistd.h>

#include "XDisplayBase.h"
#include "XBeachBall.h"


int main()
{
  std::cout << "Welcome to BeachBall" << std::endl;
  std::shared_ptr<XDisplayBase>display_base = std::make_shared<XDisplayBase>();
  XBeachBall beach_ball(display_base, 2);
  for (unsigned int i = 0 ; i < 200 ; ++i ) {
    beach_ball.increase_sector(i%8);
  }
  sleep(2);
}
