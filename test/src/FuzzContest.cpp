/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Engine/Trace/Trace.hpp"
#include "Engine/Contest/Solvers/OLCTriangle.hpp"

#include <random>

template<typename Generator>
static Angle
RandomEarthAngle(Generator &gen)
{
  return Angle::Radians(std::generate_canonical<double, 10>(gen));
}

template<typename Generator>
static void
FuzzTracePoint(Trace &trace, unsigned &t, Generator &gen)
{
  TracePoint pt(GeoPoint(RandomEarthAngle(gen), RandomEarthAngle(gen)),
                t++, 1000, 0, 0);
  trace.push_back(pt);
}

static void
FuzzTrace(Trace &trace, unsigned &t, unsigned n = 1024)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  for (unsigned i = 0; i < n; ++i)
    FuzzTracePoint(trace, t, gen);
}

int main(int argc, char **argv)
{
  Trace trace;
  unsigned t = 1;

  if (false) {
    FuzzTrace(trace, t);

    OLCTriangle triangle(trace, true, false);
    triangle.SetIncremental(false);
    triangle.Reset();
    triangle.Solve(true);
  } else {
    OLCTriangle triangle(trace, true, false);
    triangle.SetIncremental(true);
    triangle.Reset();

    while (true) {
      FuzzTrace(trace, t, 5);
      triangle.Solve(false);
    }
  }

  return 0;
}
