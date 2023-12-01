#ifndef UTILS_FLAGS_H_
#define UTILS_FLAGS_H_

// Because openfst is a dynamic library compiled with gflags/glog, we must use
// the gflags/glog from openfst to avoid them linked both statically and
// dynamically into the executable.
#include <gflags/gflags.h>

#endif  // UTILS_FLAGS_H_