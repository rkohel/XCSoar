/*
Copyright_License {

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

// Written by Curtis Olson, started September 1997.
//
// Copyright (C) 1997  Curtis L. Olson  - http://www.flightgear.org/~curt
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.

/**
 * Implements a simple linear least squares best fit routine.
 * @file leastsqs.h
 */

#ifndef _LEASTSQS_H
#define _LEASTSQS_H

#include "Util/TrivialArray.hxx"

#include <type_traits>

/**
 * A solver for least squares problems
 *
 * Classical least squares fit:
 *
 * \f[
 *     y = m * x + b
 * \f]
 *
 * the least squares error:
 *
 * \f[
 *     \frac{(y_i - \hat{y}_i)^2}{n}
 * \f]
 *
 * the maximum least squares error:
 *
 * \f[
 *     (y_i - \hat{y}_i)^2
 * \f]
 */
class LeastSquares
{
  double sum_xi, sum_yi, sum_xi_2, sum_xi_yi;

  unsigned sum_n;

  /**
  * \f[
  *     m = \frac{n * \sum_0^{i-1} (x_i*y_i) - \sum_0^{i-1} x_i* \sum_0^{i-1} y_i}
  *              {n*\sum_0^{i-1} x_i^2 - (\sum_0^{i-1} x_i)^2}
  * \f]
  */
  double m;
  /**
  * \f[
  *     b = \frac{\sum_0^{i-1} y_i}{n} - b_1 * \frac{\sum_0^{i-1} x_i}{n}
  * \f]
  */
  double b;
  double sum_error;

  double rms_error;
  double max_error;
  double sum_weights;
  double y_max;
  double y_min;
  double x_min;
  double x_max;

  double y_ave;

  struct Slot {
    double x, y;

#ifdef LEASTSQS_WEIGHT_STORE
    double weight;
#endif

    Slot() = default;

    constexpr
    Slot(double _x, double _y, double _weight)
      :x(_x), y(_y)
#ifdef LEASTSQS_WEIGHT_STORE
      , weight(_weight)
#endif
    {}
  };

  TrivialArray<Slot, 1000> slots;

public:
  bool IsEmpty() const {
    return sum_n == 0;
  }

  bool HasResult() const {
    return sum_n >= 2;
  }

  unsigned GetCount() const {
    return sum_n;
  }

  /**
   * Reset the LeastSquares calculator.
   */
  void Reset();

  double GetGradient() const {
    return m;
  }

  double GetMinX() const {
    return x_min;
  }

  double GetMaxX() const {
    return x_max;
  }

  double GetMiddleX() const {
    return (x_min + x_max) / 2.;
  }

  double GetMinY() const {
    return y_min;
  }

  double GetMaxY() const {
    return y_max;
  }

  double GetAverageY() const {
    return y_ave;
  }

  double GetYAt(double x) const {
    return x * m + b;
  }

  double GetYAtMinX() const {
    return GetYAt(GetMinX());
  }

  double GetYAtMaxX() const {
    return GetYAt(GetMaxX());
  }

  const TrivialArray<Slot, 1000> &GetSlots() const {
    return slots;
  }

  /**
   * Add a new data point to the values and calculate least squares
   * average (assumes x = sum_n + 1).
   *
   * @param y y-Value of the new data point
   */
  void Update(double y);

  /**
   * Add a new data point to the values and calculate least squares
   * average.
   *
   * @param x x-Value of the new data point
   * @param y y-Value of the new data point
   * @param weight Weight of the new data point (optional)
   */
  void Update(double x, double y, double weight=1);

private:
  /**
   * Calculate the least squares average.
   */
  void Compute();

  /**
   * Calculates the LeastSquaresError.
   */
  void UpdateError();

  /**
   * Add a new data point to the values.
   *
   * @param x x-Value of the new data point
   * @param y y-Value of the new data point
   * @param weight Weight of the new data point (optional)
   */
  void Add(double x, double y, double weight=1);
};

static_assert(std::is_trivial<LeastSquares>::value, "type is not trivial");

#endif // _LEASTSQS_H
