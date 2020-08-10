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
  __syscall_slong_t nsec = 1000000 * time;
  struct timespec timeout = {.tv_sec = 0, .tv_nsec = nsec};
  clock_nanosleep(CLOCK_MONOTONIC, 0 , &timeout, nullptr);
}

} // namespace curses
} // namespace jcu
