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

#include <thrust/detail/config.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/allocator/allocator_traits.h>
#include <thrust/detail/type_traits/pointer_traits.h>
#include <thrust/for_each.h>
#include <thrust/uninitialized_fill.h>
#include <memory>

namespace thrust
{
namespace detail
{
namespace allocator_traits_detail
{

// fill_construct_range has 2 cases:
// if Allocator has an effectful member function construct:
//   1. construct via the allocator
// else
//   2. construct via uninitialized_fill

template<typename Allocator, typename T, typename Arg1>
  struct has_effectful_member_construct2
    : has_member_construct2<Allocator,T,Arg1>
{};

// std::allocator::construct's only effect is to invoke placement new
template<typename U, typename T, typename Arg1>
  struct has_effectful_member_construct2<std::allocator<U>,T,Arg1>
    : thrust::detail::false_type
{};


template<typename Allocator, typename Arg1>
  struct construct2_via_allocator
{
  Allocator &a;
  Arg1 arg;

  construct2_via_allocator(Allocator &a, const Arg1 &arg)
    : a(a), arg(arg)
  {}

  template<typename T>
  inline __host__ __device__
  void operator()(T &x)
  {
    allocator_traits<Allocator>::construct(a, &x, arg);
  }
};


template<typename System, typename Allocator, typename Pointer, typename Size, typename T>
  typename enable_if<
    has_effectful_member_construct2<
      Allocator,
      typename pointer_element<Pointer>::type,
      T
    >::value
  >::type
    fill_construct_range(thrust::dispatchable<System> &system, Allocator &a, Pointer p, Size n, const T &value)
{
  thrust::for_each_n(system, p, n, construct2_via_allocator<Allocator,T>(a, value));
}


template<typename System, typename Allocator, typename Pointer, typename Size, typename T>
  typename disable_if<
    has_effectful_member_construct2<
      Allocator,
      typename pointer_element<Pointer>::type,
      T
    >::value
  >::type
    fill_construct_range(thrust::dispatchable<System> &system, Allocator &, Pointer p, Size n, const T &value)
{
  thrust::uninitialized_fill_n(system, p, n, value);
}


} // end allocator_traits_detail


template<typename System, typename Alloc, typename Pointer, typename Size, typename T>
  void fill_construct_range(thrust::dispatchable<System> &system, Alloc &a, Pointer p, Size n, const T &value)
{
  return allocator_traits_detail::fill_construct_range(system,a,p,n,value);
}


} // end detail
} // end thrust

