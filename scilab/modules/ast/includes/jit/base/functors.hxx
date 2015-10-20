/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __JIT_FUNCTORS_HXX__
#define __JIT_FUNCTORS_HXX__

#include "elem_functions.hxx"
#include "opposite.hxx"
#include "negation.hxx"
#include "tools.hxx"

namespace jit
{
namespace functors
{
template<typename T, typename U, typename F>
struct Unary
{

    const static bool as_usual = true;

    template<typename V, typename W>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
    {
        o[i] = F()(x[i]);
    }
};

template<typename USECPX, typename CPX, typename RE>
struct Unary_cpx
{
    const static bool as_usual = false;

    template<typename V>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
    {
        const double xi = (double)x[i];
        if (USECPX()(xi))
        {
            o.initIm(size);
            o[i] = CPX()(xi);
        }
        else
        {
            o[i] = RE()(xi);
        }
    }
};

template<typename T, typename U>
using opp = Unary<T, U, jit::op::Opposite<T, U>>;

template<typename T, typename U>
using neg = Unary<T, U, jit::op::Negation<T, U>>;
    
template<typename T, typename U>
using sin = Unary<T, U, jit::elem::sin<T, U>>;

template<typename T, typename U>
using cos = Unary<T, U, jit::elem::cos<T, U>>;

template<typename T, typename U>
using tan = Unary<T, U, jit::elem::tan<T, U>>;

template<typename T, typename U>
using tanh = Unary<T, U, jit::elem::tanh<T, U>>;

template<typename T, typename U>
using sinh = Unary<T, U, jit::elem::sinh<T, U>>;

template<typename T, typename U>
using cosh = Unary<T, U, jit::elem::cosh<T, U>>;

template<typename T, typename U>
using atan = Unary<T, U, jit::elem::atan<T, U>>;

template<typename T, typename U>
using abs = Unary<T, U, jit::elem::abs<T, U>>;

template<typename T, typename U>
using exp = Unary<T, U, jit::elem::exp<T, U>>;

template<typename T, typename U>
using sign = Unary<T, U, jit::elem::sign<T, U>>;

template<typename T, typename U>
using gamma = Unary<T, U, jit::elem::tgamma<T, U>>;

template<typename T, typename U>
using gammaln = Unary<T, U, jit::elem::lgamma<T, U>>;

template<typename T, typename U>
using ceil = Unary<T, U, jit::elem::ceil<T, U>>;

template<typename T, typename U>
using floor = Unary<T, U, jit::elem::floor<T, U>>;

template<typename T, typename U>
using round = Unary<T, U, jit::elem::round<T, U>>;

template<typename T, typename U>
using trunc = Unary<T, U, jit::elem::trunc<T, U>>;

template<typename T, typename U>
using fix = Unary<T, U, jit::elem::trunc<T, U>>;

template<typename T, typename U>
using int8 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using int16 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using int32 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using int64 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using uint8 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using uint16 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using uint32 = Unary<T, U, jit::elem::cast<T, U>>;

template<typename T, typename U>
using uint64 = Unary<T, U, jit::elem::cast<T, U>>;

//template<typename T, typename U>
//using erfi = Unary<T, U, jit::elem::erfi<T, U>>;
//
//template<typename T, typename U>
//using erfcx = Unary<T, U, jit::elem::erfcx<T, U>>;
//
//template<typename T, typename U>
//using erf = Unary<T, U, jit::elem::erf<T, U>>;
//
//template<typename T, typename U>
//using erfc = Unary<T, U, jit::elem::erfc<T, U>>;

template<typename T, typename U>
using real = Unary<T, U, jit::elem::real<T, U>>;

template<typename T, typename U>
using imag = Unary<T, U, jit::elem::imag<T, U>>;

template<typename T, typename U>
struct log
{
    typedef jit::functors::Unary<T, U, jit::elem::log<T, U>> F;
    const static bool as_usual = F::as_usual;

    template<typename V, typename W>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
    {
        F()(std::move(x), std::move(o), i);
    }
};

template<>
struct log<double, jit::cpx_t>
{
    typedef jit::functors::Unary_cpx<jit::elem::is_strict_negative<double>, jit::elem::log_neg<double, std::complex<double>>, jit::elem::log<double, double>> F;
    const static bool as_usual = F::as_usual;

    template<typename V>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
    {
        F()(std::move(x), std::move(o), i, size);
    }
};

template<typename T, typename U>
struct log2
{
    typedef jit::functors::Unary<T, U, jit::elem::log2<T, U>> F;
    const static bool as_usual = F::as_usual;

    template<typename V, typename W>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
    {
        F()(std::move(x), std::move(o), i);
    }
};

template<>
struct log2<double, jit::cpx_t>
{
    typedef jit::functors::Unary_cpx<jit::elem::is_strict_negative<double>, jit::elem::log2_neg<double, std::complex<double>>, jit::elem::log2<double, double>> F;
    const static bool as_usual = F::as_usual;

    template<typename V>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
    {
        F()(std::move(x), std::move(o), i, size);
    }
};

template<typename T, typename U>
struct log10
{
    typedef jit::functors::Unary<T, U, jit::elem::log10<T, U>> F;
    const static bool as_usual = F::as_usual;

    template<typename V, typename W>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
    {
        F()(std::move(x), std::move(o), i);
    }
};

template<>
struct log10<double, jit::cpx_t>
{
    typedef jit::functors::Unary_cpx<jit::elem::is_strict_negative<double>, jit::elem::log10_neg<double, std::complex<double>>, jit::elem::log10<double, double>> F;
    const static bool as_usual = F::as_usual;

    template<typename V>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
    {
        F()(std::move(x), std::move(o), i, size);
    }
};


template<typename T, typename U>
struct sqrt
{
    typedef jit::functors::Unary<T, U, jit::elem::sqrt<T, U>> F;
    const static bool as_usual = F::as_usual;

    template<typename V, typename W>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<W> && o, const int64_t i)
    {
        F()(std::move(x), std::move(o), i);
    }
};

template<>
struct sqrt<double, jit::cpx_t>
{
    typedef jit::functors::Unary_cpx<jit::elem::is_strict_negative<double>, jit::elem::sqrt_neg<double, std::complex<double>>, jit::elem::sqrt<double, double>> F;
    const static bool as_usual = F::as_usual;

    template<typename V>
    inline void operator()(jit::vect::WrapVecIn<V> && x, jit::vect::WrapOut<jit::cpx_t> && o, const int64_t i, const int64_t size)
    {
        F()(std::move(x), std::move(o), i, size);
    }
};

} // namespace functors

} // namespace jit

#endif // __JIT_FUNCTORS_HXX__
