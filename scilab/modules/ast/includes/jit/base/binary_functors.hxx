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

#ifndef __JIT_BINARY_FUNCTORS_HXX__
#define __JIT_BINARY_FUNCTORS_HXX__

#include "addition.hxx"
#include "subtraction.hxx"
#include "product.hxx"
#include "exponentiation.hxx"
#include "ldivision.hxx"
#include "rdivision.hxx"
#include "equal.hxx"
#include "not_equal.hxx"
#include "greater_or_eq.hxx"
#include "greater_than.hxx"
#include "lower_or_eq.hxx"
#include "lower_than.hxx"
#include "and.hxx"
#include "or.hxx"
#include "tools.hxx"

namespace jit
{
    namespace functors
    {
	template<typename T, typename U, typename V, typename Functor>
	struct Binary
	{
	    
	    template<typename W, typename X, typename Y>
	    inline void operator()(jit::vect::WrapVecIn<W> && x, jit::vect::WrapVecIn<X> && y, jit::vect::WrapOut<Y> && o, const int64_t i)
		{
		    o[i] = Functor()(x[i], y[i]);
		}
	};

	template<typename T, typename U, typename V>
	using add = Binary<T, U, V, jit::op::Add<T, U, V>>;

	template<typename T, typename U, typename V>
	using sub = Binary<T, U, V, jit::op::Sub<T, U, V>>;

	template<typename T, typename U, typename V>
	using dottimes = Binary<T, U, V, jit::op::Prod<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotpower = Binary<T, U, V, jit::op::Pow<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotldiv = Binary<T, U, V, jit::op::Ldiv<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotrdiv = Binary<T, U, V, jit::op::Rdiv<T, U, V>>;

	template<typename T, typename U, typename V>
	using neq = Binary<T, U, V, jit::op::Neq<T, U, V>>;

	template<typename T, typename U, typename V>
	using eq = Binary<T, U, V, jit::op::Eq<T, U, V>>;

	template<typename T, typename U, typename V>
	using gt = Binary<T, U, V, jit::op::Gt<T, U, V>>;

	template<typename T, typename U, typename V>
	using ge = Binary<T, U, V, jit::op::Ge<T, U, V>>;

	template<typename T, typename U, typename V>
	using lt = Binary<T, U, V, jit::op::Lt<T, U, V>>;

	template<typename T, typename U, typename V>
	using le = Binary<T, U, V, jit::op::Le<T, U, V>>;

	template<typename T, typename U, typename V>
	using And = Binary<T, U, V, jit::op::And<T, U, V>>;

	template<typename T, typename U, typename V>
	using Or = Binary<T, U, V, jit::op::Or<T, U, V>>;
	
    } // namespace functors

} // namespace jit

#endif // __JIT_BINARY_FUNCTORS_HXX__
