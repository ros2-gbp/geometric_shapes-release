// Copyright 2019 Open Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the Open Robotics nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/* Author: Martin Pecka */

#include <geometric_shapes/aabb.h>

#include <fcl/geometry/shape/utility.h>

void bodies::AABB::extendWithTransformedBox(const Eigen::Isometry3d& transform, const Eigen::Vector3d& box)
{
  // Method adapted from FCL src/shape/geometric_shapes_utility.cpp#computeBV<AABB, Box>(...) (BSD-licensed code):
  // https://github.com/flexible-collision-library/fcl/blob/fcl-0.4/src/shape/geometric_shapes_utility.cpp#L292
  // We don't call their code because it would need creating temporary objects, and their method is in floats in FCL 0.5
  //
  // Here's a nice explanation why it works: https://zeuxcg.org/2010/10/17/aabb-from-obb-with-component-wise-abs/
  const Eigen::Matrix3d& r = transform.rotation();
  const Eigen::Vector3d& t = transform.translation();

  double x_range = 0.5 * (fabs(r(0, 0) * box[0]) + fabs(r(0, 1) * box[1]) + fabs(r(0, 2) * box[2]));
  double y_range = 0.5 * (fabs(r(1, 0) * box[0]) + fabs(r(1, 1) * box[1]) + fabs(r(1, 2) * box[2]));
  double z_range = 0.5 * (fabs(r(2, 0) * box[0]) + fabs(r(2, 1) * box[1]) + fabs(r(2, 2) * box[2]));

  fcl::AABBd aabb;
  fcl::computeBV(fcl::Boxd(box), transform, aabb);
  extend(aabb.min_);
  extend(aabb.max_);
}
