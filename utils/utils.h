#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <cstdint>
#include <limits>
#include <vector>

namespace mtasr {

#define MTASR_DISALLOW_COPY_AND_ASSIGN(Type) \
  Type(const Type&) = delete;                \
  Type& operator=(const Type&) = delete;

const float kFloatMax = std::numeric_limits<float>::max();
// kSpaceSymbol in UTF-8 is: ▁
const char kSpaceSymbol[] = "\xe2\x96\x81";

// Return the sum of two probabilities in log scale
float LogAdd(float x, float y);

template <typename T>
void TopK(const std::vector<T>& data, int32_t k, std::vector<T>* values,
          std::vector<int>* indices);

}  // namespace mtasr

#endif  // UTILS_UTILS_H_