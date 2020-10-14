#include <stdint.h>
#include "cpucycles.h"

#if defined(__x86_64__)

// XXX call also cpuid?
// cf http://www.intel.com/content/www/us/en/embedded/training/ia-32-ia-64-benchmark-code-execution-paper.html
int64_t cpucycles(void) {
  unsigned int hi, lo;

  __asm__ volatile("rdtsc\n\t" : "=a"(lo), "=d"(hi));
  return ((int64_t)lo) | (((int64_t)hi) << 32);
}

#elif defined(__aarch64__)

/**
 * Prettify the asm macro.
 */
#define asm		__asm__ volatile

int64_t cpucycles(void) {
#if defined(__GNUC__)
	uint32_t r = 0;
#if  defined __aarch64__
	asm("mrs %0, pmccntr_el0":"=r"(r));
#elif defined(__ARM_ARCH_7A__)
	asm("mrc p15, 0, %0, c9, c13, 0":"=r"(r));
#else
#error Unsupported architecture/compiler!
#endif
	return r;
#endif
}

#else

/* This is not very precise..., but still portable. */

#include <time.h>

unsigned long cpucycles(void) {
	return clock();
}

#endif
