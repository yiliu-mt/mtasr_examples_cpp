#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_

// Because openfst is a dynamic library compiled with gflags/glog, we must use
// the gflags/glog from openfst to avoid them linked both statically and
// dynamically into the executable.
#include <glog/logging.h>

#endif  // UTILS_LOG_H_
