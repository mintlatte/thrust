/*
 *  Copyright 2008-2012 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/detail/dispatchable.h>

namespace thrust
{
namespace detail
{


template<typename System, typename Allocator, typename Pointer, typename Size, typename T>
  inline void fill_construct_range(thrust::dispatchable<System> &system, Allocator &a, Pointer p, Size n, const T &value);


} // end detail
} // end thrust

#include <thrust/detail/allocator/fill_construct_range.inl>

