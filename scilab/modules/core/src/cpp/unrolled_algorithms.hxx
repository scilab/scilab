/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*-----------------------------------------------------------------------------------*/
#ifndef UNROLLED_ALGORITHMS_HXX
#define UNROLLED_ALGORITHMS_HXX

namespace
{
/*
 * generic template unrolling for small vectors
 */

/**
 * C++ does not allow function template partial specialization so eq_n<> delegates to work to a eq_n_t<> struct
 * which implements the function in operator().
 *
 * @param i1 : iterator to the beginning of the first sequence
 * @param i2 : iterator to the beginning of the second sequence
 * @return true iff *i1 == *i2 &&...&& *(i1+N-1) == *(i2+N-1)
 */
template<typename It1, typename It2, int N> struct eq_n_t
{
    bool operator()(It1 i1, It2 i2) const /* generic recursive implementation */
    {
        return (*i1 == *i2) && eq_n_t < It1, It2, N - 1 > ()(++i1, ++i2) ;
    }
};
template<typename It1, typename It2> struct eq_n_t<It1, It2, 0>
{
    bool operator()(It1 i1, It2 i2) const /* partial specialization for N=0 */
    {
        return true ;
    }
};
/**
 * Compares two sequences for equality when the size in know at compile time.
 * Uses template metaprogramming to ensure loop unrolling.
 *
 * @param i1 : iterator to the beginning of the first sequence
 * @param i2 : iterator to the beginning of the second sequence
 * @return true iff *i1 == *i2 &&...&& *(i1+N-1) == *(i2+N-1)
 */
template<int N, typename It1, typename It2> bool eq_n(It1 i1, It2 i2)
{
    return eq_n_t<It1, It2, N>()(i1, i2);
}

/**
 * ensure most efficient inlining irrespectively of optimization compiler options for ids of nsiz=6 ints
 *
 * we make all the (4)  combinations of (const or not) arguments.
 *
 * @param i1 : int* beginning of the first int[6]
 * @param i2 : int* beginning of the second int[6]
 * @return true iff *i1 == *i2 &&...&& *(i1+5) == *(i2+5)
 */
template<> bool eq_n<6, int const*, int const*>(int const* id1, int const* id2)
{
    return (id1[0] == id2[0]) && (id1[1] == id2[1]) && (id1[2] == id2[2]) && (id1[3] == id2[3]) && (id1[4] == id2[4]) && (id1[5] == id2[5]);
}

/**
 * See above.
 */
template<> bool eq_n<6, int const*, int*>(int const* id1, int* id2)
{
    return (id1[0] == id2[0]) && (id1[1] == id2[1]) && (id1[2] == id2[2]) && (id1[3] == id2[3]) && (id1[4] == id2[4]) && (id1[5] == id2[5]);
}

/**
 * See above.
 */
template<> bool eq_n<6, int*, int const*>(int* id1, int const* id2)
{
    return (id1[0] == id2[0]) && (id1[1] == id2[1]) && (id1[2] == id2[2]) && (id1[3] == id2[3]) && (id1[4] == id2[4]) && (id1[5] == id2[5]);
}


/**
 * See above.
 */
template<> bool eq_n<6, int*, int*>(int* id1, int* id2)
{
    return (id1[0] == id2[0]) && (id1[1] == id2[1]) && (id1[2] == id2[2]) && (id1[3] == id2[3]) && (id1[4] == id2[4]) && (id1[5] == id2[5]);
}

/**
 * C++ does not allow function template partial specialization so less_n<> delegates to work to a less_n_t<> struct
 * which implements the function in operator().
 *
 * @param i1 : iterator to the beginning of the first sequence
 * @param i2 : iterator to the beginning of the second sequence
 * @return true iff [i1...i1+N[ < [i2...i2+N[ in lexicographical order
 */

template<typename It1, typename It2, int N> struct less_n_t
{
    bool operator()(It1 i1, It2 i2) const /*/ generic recursive case */
    {
        return (*i1 < *i2) ? true : ( (*i1 > *i2) ? false : less_n_t < It1, It2, N - 1 > ()(++i1, ++i2));
    }
};

template<typename It1, typename It2> struct less_n_t<It1, It2, 0>
{
    bool operator()(It1 i1, It2 i2) const /* special case for empty sequences */
    {
        return false ;
    }
};

/**
 * Compares two sequences for lexicographical order when the size in know at compile time.
 * Uses template metaprogramming to ensure loop unrolling.
 *
 * @param i1 : iterator to the beginning of the first sequence
 * @param i2 : iterator to the beginning of the second sequence
 * @return true iff [i1...i1+N[ < [i2...i2+N[ in lexicographical order
 */
template<int N, typename It1, typename It2> bool less_n(It1 i1, It2 i2)
{
    return less_n_t<It1, It2, N>()(i1, i2);
}

/**
 * C++ does not allow function template partial specialization so copy_n<> delegates the work to a copy_n_t<> struct
 * which implements the function in operator().
 *
 * @param i1 : iterator to the beginning of the first sequence
 * @param i2 : iterator to the beginning of the second sequence
 * @return i1+N
 */

template<typename In, typename Out, int N> struct copy_n_t
{
    Out operator()(In in, Out out) const /* generic case for N>0 */
    {
        *out = *in;
        return copy_n_t < In, Out, N - 1 > ()(++in, ++out);
    }
};

template<typename In, typename Out> struct copy_n_t<In, Out, 0>
{
    Out operator()(In, Out out) const /* special case for empty sequences */
    {
        return out;
    }
};

/**
 * Copy one sequence to another when the size in know at compile time.
 * Uses template metaprogramming to ensure loop unrolling.
 *
 * @param i1 : iterator to the beginning of the input sequence
 * @param i2 : iterator to the beginning of the output sequence
 * @return in+N
 */
template<int N, typename In, typename Out> Out copy_n(In in, Out out)
{
    return copy_n_t<In, Out, N>()(in, out);
}

}
#endif
/*-----------------------------------------------------------------------------------*/
