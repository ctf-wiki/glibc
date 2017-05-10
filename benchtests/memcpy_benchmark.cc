/* Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ======================================================================*/

#if !defined DO_STRING_INLINES
#undef __USE_STRING_INLINES
#endif

#include <string.h>
#include <chrono>
#include <iostream>
#include <functional>
#include <map>
#include <string>

#define TEST_MAIN
#define TEST_NAME "memcpy"
#define TIMEOUT (60 * 60)
#include "bench-string.h"

typedef char *(*proto_t) (char *, const char *, size_t);
IMPL (memcpy, 1)

std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::time_point<std::chrono::high_resolution_clock> stop;
size_t bytes;

#define MAX_ALIGN 128
int src_align, dest_align;

void start_timing() { start = std::chrono::high_resolution_clock::now(); }
void stop_timing() { stop = std::chrono::high_resolution_clock::now(); }

int size_list[] = {1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19,
                   1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26};
size_t buffer_size = 1 << 28;

void BM_memcpy_readwritecache(impl_t *impl, int iters, int size) {
  unsigned char * buf1 = new unsigned char [size];
  unsigned char * buf2 = new unsigned char [size];

  src_align  = ((uintptr_t) buf1) & (MAX_ALIGN - 1);
  dest_align  = ((uintptr_t) buf2) & (MAX_ALIGN - 1);

  memset (buf1, 0xa5, size); memset (buf2, 0x5a, size);

  start_timing();
  for (int i = 0; i < iters; ++i) {
    CALL(impl, buf2, buf1, size);
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

void BM_memcpy_nocache(impl_t *impl, int iters, int size) {
  unsigned char * buf1 = new unsigned char [buffer_size];
  unsigned char * buf2 = new unsigned char [buffer_size];

  src_align  = ((uintptr_t) buf1) & (MAX_ALIGN - 1);
  dest_align  = ((uintptr_t) buf2) & (MAX_ALIGN - 1);

  memset (buf1, 0xa5, buffer_size); memset (buf2, 0x5a, buffer_size);

  size_t offset = 0;
  start_timing();
  for (int i = 0; i < iters; ++i) {
    CALL(impl, buf2 + offset, buf1 + offset, size);
    offset += std::max(4097, size + 1);
    if (offset >= buffer_size - size) offset = 0;
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

void BM_memcpy_readcache(impl_t *impl, int iters, int size) {
  unsigned char * buf1 = new unsigned char [size];
  unsigned char * buf2 = new unsigned char [buffer_size];

  src_align  = ((uintptr_t) buf1) & (MAX_ALIGN - 1);
  dest_align  = ((uintptr_t) buf2) & (MAX_ALIGN - 1);

  memset (buf1, 0xa5, size); memset (buf2, 0x5a, buffer_size);

  size_t offset = 0;
  start_timing();
  for (int i = 0; i < iters; ++i) {
    CALL(impl, buf2 + offset, buf1, size);
    offset += std::max(4097, size + 1);
    if (offset >= buffer_size - size) offset = 0;
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

double do_timing(std::function<void(impl_t *, int, int)> &fn, impl_t *impl, int size) {
  int iters = 2; double time = 0;
  while (time < 500) {
    iters *= 3;
    fn(impl, iters, size);
    time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    bytes = (2UL * iters * size);
  }
  return time;
}

std::map<std::string, std::function<void(impl_t *, int, int)>> schemes =
  {{"Read and Write Cache", BM_memcpy_readwritecache},
   {"No Cache", BM_memcpy_nocache},
   {"Read Cache", BM_memcpy_readcache}};

void test() {
  std::cout << "Size(bytes) Alignment(src/dest) BW(Gbytes/sec)" << std::endl;
  bool first = true;
  FOR_EACH_IMPL (impl, 0)
    {
      if (!first)
	std::cout << " ";
      std::cout << impl->name;
      first = false;
    }
  std::cout << std::endl;
  for (auto scheme : schemes) {
    std::cout << scheme.first << std::endl;
    for (auto size : size_list) {
      first = true;
      FOR_EACH_IMPL (impl, 0)
	{
	  int time = do_timing(scheme.second, impl, size);
	  if (first)
	    {
	      first = false;
	      printf("%d %d/%-d %.2f",
		     size, src_align, dest_align,
		     (bytes * 1000L / time) / 1e9);
	    }
	  else
	    printf(" %.2f",
		   (bytes * 1000L / time) / 1e9);
	}
      printf ("\n");
    }
    std::cout << "----------------\n";
  }
}

int test_main(void) {
  test_init ();
  test ();
  return 0;
}
#include <support/test-driver.c>
