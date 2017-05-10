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

std::chrono::time_point<std::chrono::high_resolution_clock> start;
std::chrono::time_point<std::chrono::high_resolution_clock> stop;
size_t bytes;

void start_timing() { start = std::chrono::high_resolution_clock::now(); }
void stop_timing() { stop = std::chrono::high_resolution_clock::now(); }

int size_list[] = {1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19,
                   1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, 1 << 25, 1 << 26};
size_t buffer_size = 1 << 28;

void BM_memcpy_readwritecache(int iters, int size) {
  unsigned char * buf1 = new unsigned char [size];
  unsigned char * buf2 = new unsigned char [size];

  memset (buf1, 0xa5, size); memset (buf2, 0x5a, size);

  start_timing();
  for (int i = 0; i < iters; ++i) {
    memcpy(buf2, buf1, size);
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

void BM_memcpy_nocache(int iters, int size) {
  unsigned char * buf1 = new unsigned char [buffer_size];
  unsigned char * buf2 = new unsigned char [buffer_size];

  memset (buf1, 0xa5, buffer_size); memset (buf2, 0x5a, buffer_size);

  size_t offset = 0;
  start_timing();
  for (int i = 0; i < iters; ++i) {
    memcpy(buf2 + offset, buf1 + offset, size);
    offset += std::max(4097, size + 1);
    if (offset >= buffer_size - size) offset = 0;
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

void BM_memcpy_readcache(int iters, int size) {
  unsigned char * buf1 = new unsigned char [size];
  unsigned char * buf2 = new unsigned char [buffer_size];

  memset (buf1, 0xa5, size); memset (buf2, 0x5a, buffer_size);

  size_t offset = 0;
  start_timing();
  for (int i = 0; i < iters; ++i) {
    memcpy(buf2 + offset, buf1, size);
    offset += std::max(4097, size + 1);
    if (offset >= buffer_size - size) offset = 0;
  }
  stop_timing();

  delete[] buf1; delete[] buf2;
}

double do_timing(std::function<void(int, int)> &fn, int size) {
  int iters = 2; double time = 0;
  while (time < 500) {
    iters *= 3;
    fn(iters, size);
    time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    bytes = (2UL * iters * size);
  }
  return time;
}

std::map<std::string, std::function<void(int, int)>> schemes =
  {{"Read and Write Cache", BM_memcpy_readwritecache},
   {"No Cache", BM_memcpy_nocache},
   {"Read Cache", BM_memcpy_readcache}};

int main(void) {
  std::cout << "      Size (bytes) Time (msec) BW (Gbytes/sec)" << std::endl;
  for (auto scheme : schemes) {
    std::cout << scheme.first << std::endl;
    for (auto size : size_list) {
      int time = do_timing(scheme.second, size);
      printf("%12d %10d %10.2f\n", size, time, (bytes * 1000L / time) / 1e9);
    }
    std::cout << "----------------\n";
  }
}