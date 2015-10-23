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

#ifndef __BASE_VECTORIZATION_HXX__
#define __BASE_VECTORIZATION_HXX__

namespace jit
{

namespace vect
{

template<bool as_usual, typename Functor, typename T, typename U>
struct __Vect_Helper;

template<typename Functor, typename T, typename U>
struct __Vect_Helper<true, Functor, T, U>
{
    inline void operator()(const int64_t size, WrapVecIn<T> && x, WrapOut<U> && o)
    {
        o.init(size);
        for (int64_t i = 0; i < size; ++i)
        {
            Functor()(std::move(x), std::move(o), i);
        }
    }
};

template<typename Functor, typename T>
struct __Vect_Helper<false, Functor, T, jit::cpx_t>
{
    inline void operator()(const int64_t size, WrapVecIn<T> && x, WrapOut<jit::cpx_t> && o)
    {
        o.initRe(size);
        for (int64_t i = 0; i < size; ++i)
        {
            Functor()(std::move(x), std::move(o), i, size);
        }
    }
};

template<typename Functor, typename T, typename U>
inline void unary(const int64_t size, WrapVecIn<T> && x, WrapOut<U> && o)
{
    __Vect_Helper<Functor::as_usual, Functor, T, U>()(size, std::move(x), std::move(o));
}

template<typename Functor, typename T, typename U, typename V>
inline void binary(const int64_t size, WrapVecIn<T> && x, WrapVecIn<U> && y, WrapOut<V> && o)
{
    o.init(size);
    for (int64_t i = 0; i < size; ++i)
    {
        Functor()(std::move(x), std::move(y), std::move(o), i);
    }
}

template<bool B, typename Functor, typename T, typename U>
inline int32_t breakOn(const int64_t size, WrapVecIn<T> && x, WrapVecIn<U> && y)
{
    for (int64_t i = 0; i < size; ++i)
    {
        if (Functor()(std::move(x), std::move(y), i) == B)
        {
            return B;
        }
    }
    return !B;
}

/*	template<typename F, typename T>
	inline void unary_cpx(const int64_t size, WrapVecIn<T> && x, WrapOut<std::complex<double>> && o)
	{
	    WrapPtr<std::complex<double>> _o = getPtr(o, size);
	    for (int64_t i = 0; i < size; ++i)
	    {
		if (F::USECPX(x[i]))
		{
		    if (!_o.im)
		    {
			*o.im = new double[size]();
			_o.im = *o.im;
		    }
		    _o[i] = F::CPX(x[i]);
		}
		else
		{
		    _o[i] = F::RE(x[i]);
		}
	    }
	}

	template<typename F, typename T, typename U, typename V>
        inline void binary(const int64_t size, WrapVecIn<T> && x, WrapVecIn<U> && y, WrapOut<V> && o)
        {
	    WrapPtr<V> _o = getPtr(o, size);
            for (int64_t i = 0; i < size; ++i)
            {
                _o[i] = F()(x[i], y[i]);
            }
        }
*/

// use it with frexp
/*template<typename T, typename U, typename V, V (F)(T, U *), typename W = V>
inline void vectorize(const T * x, const int64_t x_r, const int64_t x_c, V * o1, W * o2)
{
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        U t;
        o1[i] = (V)F(x[i], &t);
        o2[i] = (W)t;
    }
}

// use it with rand()
template<typename T, T (F)(), typename U = T>
inline void vectorize(const int64_t x_r, const int64_t x_c, U ** o)
{
const int64_t size = x_r * x_c;
    U * _o = getPtr(o, size);
    for (int64_t i = 0; i < x_r * x_c; ++i)
    {
        o[i] = (T)F();
    }
}*/

} // namespace vect

} // namespace jit

#endif // __BASE_VECTORISATION_HXX__
