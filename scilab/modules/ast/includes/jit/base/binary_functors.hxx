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
	template<typename Functor>
	struct Binary
	{
	    
	    template<typename T, typename U, typename V>
	    inline void operator()(jit::vect::WrapVecIn<T> && x, jit::vect::WrapVecIn<U> && y, jit::vect::WrapOut<V> && o, const int64_t i)
		{
		    o[i] = Functor()(x[i], y[i]);
		}
	};

	template<typename T, typename U, typename V>
	using add = Binary<jit::op::Add<T, U, V>>;

	template<typename T, typename U, typename V>
	using sub = Binary<jit::op::Sub<T, U, V>>;

	template<typename T, typename U, typename V>
	using dottimes = Binary<jit::op::Prod<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotpower = Binary<jit::op::Pow<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotldiv = Binary<jit::op::Ldiv<T, U, V>>;

	template<typename T, typename U, typename V>
	using dotrdiv = Binary<jit::op::Rdiv<T, U, V>>;

	template<typename T, typename U, typename V>
	using neq = Binary<jit::op::Neq<T, U, V>>;

	template<typename T, typename U, typename V>
	using eq = Binary<jit::op::Eq<T, U, V>>;

	template<typename T, typename U, typename V>
	using gt = Binary<jit::op::Gt<T, U, V>>;

	template<typename T, typename U, typename V>
	using ge = Binary<jit::op::Ge<T, U, V>>;

	template<typename T, typename U, typename V>
	using lt = Binary<jit::op::Lt<T, U, V>>;

	template<typename T, typename U, typename V>
	using le = Binary<jit::op::Le<T, U, V>>;

	template<typename T, typename U, typename V>
	using And = Binary<jit::op::And<T, U, V>>;

	template<typename T, typename U, typename V>
	using Or = Binary<jit::op::Or<T, U, V>>;
	
    } // namespace functors

} // namespace jit

#endif // __JIT_BINARY_FUNCTORS_HXX__
