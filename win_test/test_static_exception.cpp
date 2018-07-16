#include <thread>
#include <cstdlib>
#include <string>
#include <exception>
#include <malloc.h>

#define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)

namespace __cxxabiv1
{
  class __cxa_refcounted_exception
  {
  };
  class __cxa_dependent_exception
  {
  };
}

#include <exception_memory_pool.hpp>

void main()
{
  uint32_t number_of_processors = std::stoul(std::getenv("NUMBER_OF_PROCESSORS"));
  uint32_t  tmp = 0;
}