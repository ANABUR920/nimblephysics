/*
 * Copyright (c) 2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
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

#ifndef DART_COMMON_DETAIL_EXTENSIBLE_H_
#define DART_COMMON_DETAIL_EXTENSIBLE_H_

#include "dart/common/Cloneable.h"
#include "dart/common/StlHelpers.h"

namespace dart {
namespace common {

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>::MakeCloneable()
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
template <typename ... Args>
MakeCloneable<Base, Mixin>::MakeCloneable(Args&&... args)
  : Mixin(std::forward<Args>(args)...)
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>::MakeCloneable(const Mixin& mixin)
  : Mixin(mixin)
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>::MakeCloneable(Mixin&& mixin)
  : Mixin(std::move(mixin))
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>::MakeCloneable(
    const MakeCloneable<Base, Mixin>& other)
  : Mixin(other)
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>::MakeCloneable(MakeCloneable<Base, Mixin>&& other)
  : Mixin(other)
{
  // Do nothing
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>& MakeCloneable<Base, Mixin>::operator=(
    const Mixin& mixin)
{
  static_cast<Mixin&>(*this) = mixin;
  return *this;
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>& MakeCloneable<Base, Mixin>::operator=(Mixin&& mixin)
{
  static_cast<Mixin&>(*this) = std::move(mixin);
  return *this;
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>& MakeCloneable<Base, Mixin>::operator=(
    const MakeCloneable& other)
{
  static_cast<Mixin&>(*this) = static_cast<const Mixin&>(other);
  return *this;
}

//==============================================================================
template <class Base, class Mixin>
MakeCloneable<Base, Mixin>& MakeCloneable<Base, Mixin>::operator=(
    MakeCloneable&& other)
{
  static_cast<Mixin&>(*this) = std::move(static_cast<Mixin&&>(other));
}

//==============================================================================
template <class Base, class Mixin>
std::unique_ptr<Base> MakeCloneable<Base, Mixin>::clone() const
{
  return common::make_unique<MakeCloneable<Base, Mixin>>(*this);
}

//==============================================================================
template <class Base, class Mixin>
void MakeCloneable<Base, Mixin>::copy(const Base& other)
{
  *this = static_cast<const MakeCloneable<Base, Mixin>&>(other);
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable()
  : mOwner(nullptr),
    mData(make_unique<Data>())
{
  // Do nothing
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable(
    OwnerT* owner)
  : mOwner(owner)
{
  // Do nothing
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
template <typename... Args>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable(
    OwnerT* owner, Args&&... args)
  : mOwner(owner)
{
  set(Data(std::forward<Args>(args)...));
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
template <typename... Args>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable(
    Args&&... args)
  : mOwner(nullptr),
    mData(make_unique<Data>(std::forward<Args>(args)...))
{
  // Do nothing
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable(
    const ProxyCloneable& other)
  : mOwner(nullptr),
    mData(nullptr)
{
  set(other);
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
ProxyCloneable<Base, OwnerT, DataT, setData, getData>::ProxyCloneable(
    ProxyCloneable&& other)
  : mOwner(nullptr)
{
  set(other);
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
auto ProxyCloneable<Base, OwnerT, DataT, setData, getData>::operator =(
    const Data& data) -> ProxyCloneable&
{
  set(data);
  return *this;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
auto ProxyCloneable<Base, OwnerT, DataT, setData, getData>::operator =(
    Data&& data) -> ProxyCloneable&
{
  set(data);
  return *this;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
auto ProxyCloneable<Base, OwnerT, DataT, setData, getData>::operator =(
    const ProxyCloneable& other) -> ProxyCloneable&
{
  set(other);
  return *this;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
auto ProxyCloneable<Base, OwnerT, DataT, setData, getData>::operator =(
    ProxyCloneable&& other) -> ProxyCloneable&
{
  set(other);
  return *this;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
void ProxyCloneable<Base, OwnerT, DataT, setData, getData>::set(
    const Data& data)
{
  if(mOwner)
  {
    (*setData)(mOwner, data);
    return;
  }

  mData = make_unique<Data>(data);
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
void ProxyCloneable<Base, OwnerT, DataT, setData, getData>::set(
    Data&& data)
{
  if(mOwner)
  {
    (*setData)(mOwner, data);
    return;
  }

  mData = make_unique<Data>(std::move(data));
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
void ProxyCloneable<Base, OwnerT, DataT, setData, getData>::set(
    const ProxyCloneable& other)
{
  set(other.get());
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
void ProxyCloneable<Base, OwnerT, DataT, setData, getData>::set(
    ProxyCloneable&& other)
{
  set(other.get());
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
auto ProxyCloneable<Base, OwnerT, DataT, setData, getData>::get() const -> Data
{
  if(mOwner)
    return (*getData)(mOwner);

  return *mData;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
OwnerT* ProxyCloneable<Base, OwnerT, DataT, setData, getData>::getOwner()
{
  return mOwner;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
const OwnerT* ProxyCloneable<Base, OwnerT, DataT, setData, getData>::getOwner() const
{
  return mOwner;
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
std::unique_ptr<Base> ProxyCloneable<
    Base, OwnerT, DataT, setData, getData>::clone() const
{
  return make_unique<ProxyCloneable>(get());
}

//==============================================================================
template <class Base, class OwnerT, class DataT,
          void (*setData)(OwnerT*, const DataT&),
          DataT (*getData)(const OwnerT*)>
void ProxyCloneable<Base, OwnerT, DataT, setData, getData>::copy(
    const Base& other)
{
  set(static_cast<const ProxyCloneable&>(other));
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>::CloneableMap(
    const CloneableMap& otherHolder)
{
  *this = otherHolder;
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>::CloneableMap(
    CloneableMap&& otherHolder)
{
  *this = std::move(otherHolder);
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>::CloneableMap(const MapType& otherMap)
{
  *this = otherMap;
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>::CloneableMap(MapType&& otherMap)
{
  *this = std::move(otherMap);
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>& CloneableMap<MapType>::operator=(
    const CloneableMap& otherHolder)
{
  *this = otherHolder.getMap();

  return *this;
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>& CloneableMap<MapType>::operator=(
    CloneableMap&& otherHolder)
{
  mMap = std::move(otherHolder.mMap);

  return *this;
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>& CloneableMap<MapType>::operator=(
    const MapType& otherMap)
{
  typename MapType::iterator receiver = mMap.begin();
  typename MapType::const_iterator sender = otherMap.begin();

  while( otherMap.end() != sender )
  {
    if( mMap.end() == receiver )
    {
      // If we've reached the end of this CloneableMapHolder's map, then we
      // should just add each entry
      mMap[sender->first] = sender->second->clone();
      ++sender;
    }
    else if( receiver->first == sender->first )
    {
      // We should copy the incoming object when possible so we can avoid the
      // memory allocation overhead of cloning.
      if(receiver->second)
        receiver->second->copy(*sender->second);
      else
        receiver->second = sender->second->clone();

      ++receiver;
      ++sender;
    }
    else if( receiver->first < sender->first )
    {
      // Clear this entry in the map, because it does not have an analog in the
      // map that we are copying
      receiver->second = nullptr;
      ++receiver;
    }
    else
    {
      mMap[sender->first] = sender->second->clone();
      ++sender;
    }
  }

  while( mMap.end() != receiver )
  {
    mMap.erase(receiver++);
  }

  return *this;
}

//==============================================================================
template <typename MapType>
CloneableMap<MapType>& CloneableMap<MapType>::operator=(
    MapType&& otherHolder)
{
  mMap = std::move(otherHolder);

  return *this;
}

//==============================================================================
template <typename MapType>
MapType& CloneableMap<MapType>::getMap()
{
  return mMap;
}

//==============================================================================
template <typename MapType>
const MapType& CloneableMap<MapType>::getMap() const
{
  return mMap;
}

//==============================================================================
template <typename T>
CloneableVector<T>::CloneableVector(const std::vector<T>& regularVector)
  : mVector(regularVector)
{
  // Do nothing
}

//==============================================================================
template <typename T>
CloneableVector<T>::CloneableVector(std::vector<T>&& regularVector)
{
  mVector = std::move(regularVector);
}

//==============================================================================
template <typename T>
std::unique_ptr< CloneableVector<T> > CloneableVector<T>::clone() const
{
  std::vector<T> clonedVector;
  clonedVector.reserve(mVector.size());

  for(const T& entry : mVector)
    clonedVector.push_back(entry->clone());

  return common::make_unique< CloneableVector<T> >( std::move(clonedVector) );
}

//==============================================================================
template <typename T>
void CloneableVector<T>::copy(const CloneableVector<T>& anotherVector)
{
  const std::vector<T>& other = anotherVector.getVector();
  mVector.resize(other.size());

  for(size_t i=0; i < other.size(); ++i)
  {
    if(mVector[i] && other[i])
      mVector[i]->copy(*other[i]);
    else if(other[i])
      mVector[i] = other[i]->clone();
    else
      mVector[i] = nullptr;
  }
}

//==============================================================================
template <typename T>
const std::vector<T>& CloneableVector<T>::getVector() const
{
  return mVector;
}

} // namespace common
} // namespace dart

#endif // DART_COMMON_DETAIL_EXTENSIBLE_H_