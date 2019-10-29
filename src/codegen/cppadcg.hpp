//
// Copyright (c) 2018-2019 CNRS INRIA
//

#ifndef __pinocchio_codegen_ccpadcg_hpp__
#define __pinocchio_codegen_ccpadcg_hpp__

#include "pinocchio/math/fwd.hpp"

#include <cmath>
#include <cppad/cg/support/cppadcg_eigen.hpp>

#include "pinocchio/autodiff/cppad.hpp"

#ifndef PINOCCHIO_WITH_CXX11_SUPPORT
   #error CppADCodeGen requires C++11 or more
#endif

namespace boost
{
  namespace math
  {
    namespace constants
    {
      namespace detail
      {
        template<typename Scalar>
        struct constant_pi< CppAD::cg::CG<Scalar> > : constant_pi<Scalar> {};
      }
    }
  }
}

namespace Eigen
{
  namespace internal
  {
    // Specialization of Eigen::internal::cast_impl for CppAD input types
    template<typename Scalar>
    struct cast_impl<CppAD::cg::CG<Scalar>,Scalar>
    {
#if EIGEN_VERSION_AT_LEAST(3,2,90)
      EIGEN_DEVICE_FUNC
#endif
      static inline Scalar run(const CppAD::cg::CG<Scalar> & x)
      {
        return x.getValue();
      }
    };
  }
} // namespace Eigen

namespace pinocchio
{
  template<typename Scalar>
  struct TaylorSeriesExpansion< CppAD::cg::CG<Scalar> > : TaylorSeriesExpansion<Scalar>
  {
    typedef TaylorSeriesExpansion<Scalar> Base;
    using Base::precision;
  };
} // namespace pinocchio

#endif // #ifndef __pinocchio_codegen_ccpadcg_hpp__
