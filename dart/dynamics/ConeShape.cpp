/*
 * Copyright (c) 2011-2019, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/master/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "dart/dynamics/ConeShape.hpp"

#include <cmath>

#include "dart/dynamics/CylinderShape.hpp"
#include "dart/dynamics/SphereShape.hpp"
#include "dart/math/Helpers.hpp"

namespace dart {
namespace dynamics {

//==============================================================================
ConeShape::ConeShape(s_t radius, s_t height)
  : Shape(CONE), mRadius(radius), mHeight(height)
{
  assert(0.0 < radius);
  assert(0.0 < height);
}

//==============================================================================
const std::string& ConeShape::getType() const
{
  return getStaticType();
}

//==============================================================================
const std::string& ConeShape::getStaticType()
{
  static const std::string type("ConeShape");
  return type;
}

//==============================================================================
s_t ConeShape::getRadius() const
{
  return mRadius;
}

//==============================================================================
void ConeShape::setRadius(s_t radius)
{
  assert(0.0 < radius);
  mRadius = radius;
  mIsBoundingBoxDirty = true;
  mIsVolumeDirty = true;

  incrementVersion();
}

//==============================================================================
s_t ConeShape::getHeight() const
{
  return mHeight;
}

//==============================================================================
void ConeShape::setHeight(s_t height)
{
  assert(0.0 < height);
  mHeight = height;
  mIsBoundingBoxDirty = true;
  mIsVolumeDirty = true;

  incrementVersion();
}

//==============================================================================
s_t ConeShape::computeVolume(s_t radius, s_t height)
{
  return (1.0 / 3.0) * math::constantsd::pi() * pow(radius, 2) * height;
}

//==============================================================================
Eigen::Matrix3s ConeShape::computeInertia(s_t radius, s_t height, s_t mass)
{
  // Reference: https://en.wikipedia.org/wiki/List_of_moments_of_inertia

  const auto radius2 = radius * radius;
  const auto height2 = height * height;

  const auto Ixx = (3.0 / 20.0) * mass * (radius2 + (2.0 / 3.0) * height2);
  const auto Izz = (3.0 / 10.0) * mass * radius2;

  return Eigen::Vector3s(Ixx, Ixx, Izz).asDiagonal();
}

//==============================================================================
void ConeShape::updateBoundingBox() const
{
  const Eigen::Vector3s corner(mRadius, mRadius, mRadius + 0.5 * mHeight);

  mBoundingBox.setMin(-corner);
  mBoundingBox.setMax(corner);

  mIsBoundingBoxDirty = false;
}

//==============================================================================
void ConeShape::updateVolume() const
{
  mVolume = computeVolume(mRadius, mHeight);
  mIsVolumeDirty = false;
}

//==============================================================================
Eigen::Matrix3s ConeShape::computeInertia(s_t mass) const
{
  return computeInertia(mRadius, mHeight, mass);
}

} // namespace dynamics
} // namespace dart
