/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#include "FlatTriangleFan.hpp"

#include <assert.h>

void
FlatTriangleFan::CalcBoundingBox()
{
  assert(!vs.empty());

  auto it = vs.begin(), end = vs.end();
  bounding_box = FlatBoundingBox(*it);
  for (++it; it != end; ++it)
    bounding_box.Expand(*it);
}

void
FlatTriangleFan::AddPoint(FlatGeoPoint p)
{
  // avoid duplicates
  if (!vs.empty() && p == vs.back())
    return;

  vs.push_back(p);
}

bool
FlatTriangleFan::IsInside(FlatGeoPoint p) const
{
  if (!bounding_box.IsInside(p))
    return false;

  bool inside = false;
  for (auto i = vs.begin(), j = std::prev(vs.end()), end = vs.end();
       i != end; j = i++) {
    if ((i->y > p.y) == (j->y > p.y))
      continue;

    const FlatGeoPoint ji = *j - *i;
    const FlatGeoPoint pi = p - *i;

    if (0 < ji.x * pi.y / ji.y - pi.x)
      inside = !inside;
  }

  return inside;
}
