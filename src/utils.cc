/**
 * @file	utils.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "utils.h"

namespace jcu {
namespace curses {

void mySleepMs(int time) {
  long nsec = 1000000 * time;
  struct timespec timeout = {0};
  timeout.tv_sec = time / 1000;
  timeout.tv_nsec = (time % 1000) * 1000000;
  clock_nanosleep(CLOCK_MONOTONIC, 0 , &timeout, nullptr);
}

} // namespace curses
} // namespace jcu
