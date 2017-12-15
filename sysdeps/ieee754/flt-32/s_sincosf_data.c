/* Shared data between sinf, cosf and sincosf.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "math_config.h"

const struct sincosf_data __sincosf_data = {
  .pio2_table = {
    0 * M_PI_2,
    1 * M_PI_2,
    2 * M_PI_2,
    3 * M_PI_2,
    4 * M_PI_2,
    5 * M_PI_2
  },
  .invpio4_table = {
    0x0p+0,
    0x1.45f306cp+0,
    0x1.c9c882ap-28,
    0x1.4fe13a8p-58,
    0x1.f47d4dp-85,
    0x1.bb81b6cp-112,
    0x1.4acc9ep-142,
    0x1.0e4107cp-169
  },
  .ones = { 1.0, -1.0 }
};
