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

#ifndef __BASE_TOOLS_HXX__
#define __BASE_TOOLS_HXX__

#include <complex>

namespace jit
{
    
    using cpx_t = std::complex<double>;

    namespace vect
    {

	template<bool is_pointer, typename T>
	struct __WrapVecIn_Helper;

	template<typename T>
	struct __WrapVecIn_Helper<true, T>
	{
	    typedef typename std::remove_pointer<T>::type type;
	    
	    T x;
	    __WrapVecIn_Helper(T _x) : x(_x) { }
	    
	    inline type operator[](int64_t i) { return x[i]; }
	};
	
	template<typename T>
	struct __WrapVecIn_Helper<false, T>
	{
	    typedef T type;
	    
	    T x;
	    __WrapVecIn_Helper(T _x) : x(_x) { }
	    
	    inline type operator[](int64_t i) { return x; }
	};
	
	template<typename T>
	struct WrapVecIn
	{
	    typedef __WrapVecIn_Helper<std::is_pointer<T>::value, T> type;
	    
	    type x;
	    WrapVecIn(T _x) : x(_x) { }
	    
	    inline typename type::type operator[](int64_t i) { return x[i]; }
	};

	template<>
	struct WrapVecIn<cpx_t *>
	{
	    const double * re;
	    const double * im;
	    
	    WrapVecIn(const double * _re, const double * _im) : re(_re), im(_im) { }
	    
	    inline cpx_t operator[](int64_t i) { return cpx_t(re[i], im[i]); }
	};

	template<>
	struct WrapVecIn<cpx_t>
	{
	    const cpx_t z;
	    
	    WrapVecIn(const double _re, const double _im) : z(_re, _im) { }
	    
	    inline cpx_t operator[](int64_t i) { return z; }
	};

	template<>
	struct WrapVecIn<bool *>
	{
	    const int32_t * x;
	    
	    WrapVecIn(const int32_t * _x) : x(_x) { }
	    
	    inline bool operator[](int64_t i) { return x[i] != 0; }
	};

	template<>
	struct WrapVecIn<bool>
	{
	    const int32_t x;
	    
	    WrapVecIn(const int32_t _x) : x(_x) { }
	    
	    inline bool operator[](int64_t i) { return x != 0; }
	};

	template<typename T>
	struct WrapOut
	{
	    T ** x;
	    T * _x;
	    
	    WrapOut(T ** __x) : x(__x), _x(*__x) { }

	    inline void init(const int64_t size)
		{
		    if (!_x)
		    {
			_x = new T[size];
			*x = _x;
		    }
		}
	    
	    inline T & operator[](int64_t i) { return _x[i]; }
	};
	
	template<>
	struct WrapOut<cpx_t>
	{
	    struct __Proxy
	    {
		double * a;
		double * b;

		__Proxy(double * _a, double * _b) : a(_a), b(_b) { }

		inline void operator=(const cpx_t & z)
		    {
			*a = z.real();
			*b = z.imag();
		    }

		inline void operator=(const double x)
		    {
			*a = x;
		    }
	    };
	    
	    WrapOut<double> re;
	    WrapOut<double> im;
	    
	    WrapOut(double ** _re, double ** _im) : re(_re), im(_im) { }

	    inline void init(const int64_t size)
		{
		    initRe(size);
		    initIm(size);
		}

	    inline void initRe(const int64_t size)
		{
		    re.init(size);
		}

	    inline void initIm(const int64_t size)
		{
		    im.init(size);
		}

	    inline __Proxy operator[](int64_t i) { return __Proxy(re._x + i, im._x + i); }

	};

    } // namespace vect

} // namespace jit

#endif // __BASE_TOOLS_HXX__
