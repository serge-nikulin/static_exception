#include <stdio.h>
#include <omp.h>
#include <vector>
#include <atomic>
#ifdef _MSC_VER
#define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#define alligned_free _aligned_free

namespace __cxxabiv1
{
  class __cxa_dependent_exception
  {
    char ram[112];
  };
  class __cxa_refcounted_exception
  {
    char ram[128];
  };
}
#endif  // _MSC_VER
#include "exception_memory_pool.hpp"

static std::atomic<uint64_t> g_total_count;

static void test_function(uint32_t max_allocs_per_thread, uint32_t alloc_size)
{
  std::array<void *, EXCEPTION_MEMORY__CXX_POOL_SIZE> allocated_pointers;
  std::vector<uint8_t> expected_sig(alloc_size);

  for (;;) {
    for (uint32_t i = 0; i < max_allocs_per_thread; ++i) {
      allocated_pointers[i] = __cxa_allocate_exception(alloc_size);
      if (nullptr == allocated_pointers[i]) {
        printf("__cxa_allocate_exception failed\n");
        exit(1);
      } else {
        memset(allocated_pointers[i], i & 0xFF, alloc_size);
      }
    }

    for (uint32_t i = 0; i < max_allocs_per_thread; ++i) {
      memset(&expected_sig[0U], (i & 0xFF), alloc_size);
      if (memcmp(allocated_pointers[i], &expected_sig[0U], alloc_size) != 0) {
        printf("memcmp failed\n");
        exit(1);
      }
      __cxa_free_exception(allocated_pointers[i]);
    }
    uint64_t local_counter = std::atomic_fetch_add(&g_total_count, 1UL);
    if ((local_counter % 10000ULL) == 0ULL) {
      printf("local_counter: %uM\n", uint32_t((local_counter * max_allocs_per_thread) / 1000000UL));
    }
  }
}

int main()
{
  uint32_t max_allocs_per_thread = EXCEPTION_MEMORY__CXX_POOL_SIZE / omp_get_max_threads();
  printf("max_allocs_per_thread: %u\n", max_allocs_per_thread);

  const uint32_t alloc_size = EXCEPTION_MEMORY__CXX_MAX_EXCEPTION_SIZE - sizeof(__cxxabiv1::__cxa_refcounted_exception);
  printf("alloc_size: %u\n", alloc_size);

#pragma omp parallel for
  for (int64_t i = 0; i < omp_get_max_threads(); ++i) {
    test_function(max_allocs_per_thread, alloc_size);
  }

  return 0;
}