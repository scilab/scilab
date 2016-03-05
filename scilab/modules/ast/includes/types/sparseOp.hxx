/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard Hugueney
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

#ifndef __SPARSEOP_HH__
#define __SPARSEOP_HH__

#include "sparse.hxx"
#include "keepForSparse.hxx"


/*
 * Logical operators implementation for types::Sparse and types::SparseBool
 *
 * Many operations result in sparse matrices with a lot of true elements
 * they are slow and take a lot of memory to implement.
 *
 * Mixed mode operators between real and complex values are commented out
 * until Eigen::Sparse provides a .coeff() member function to cast<> retults.
 *
 * complex ordering is selected through conditional compilation to show
 * how SEP 21 could be implemented.
 *
 * pruning [c|sh]ould be tied to relative precision as defined in elem_common.h
 *
 */

namespace std
{
/* std namespace does not implement complex ordering: we use the lexicographical order   */
typedef double (*cplxFun_t)(std::complex<double> const&);
#ifndef SCI_CPLX_ORDER_SEP21
/* on abs() and arg() according to current practise */
cplxFun_t const mostSignificant (&std::abs);
cplxFun_t const leastSignificant(&std::arg);
#else
/*  on real and imaginary parts as per SEP 21 recommandation.  */
cplxFun_t const mostSignificant (&std::real);
cplxFun_t const leastSignificant(&std::imag);
#endif

template<> struct less<std::complex<double> >: std::binary_function<std::complex<double>, std::complex<double>, bool>
{
    bool operator()(std::complex<double> const& op1, std::complex<double> const& op2) const
    {
        if (mostSignificant(op1) < mostSignificant(op2))
        {
            return true;
        }
        if (mostSignificant(op2) < mostSignificant(op1))
        {
            return false;
        }
        return leastSignificant(op1) < leastSignificant(op2);
    }
};
template<> struct greater<std::complex<double> >: std::binary_function<std::complex<double>, std::complex<double>, bool>
{
    bool operator()(std::complex<double> const& op1, std::complex<double> const& op2) const
    {
        if (mostSignificant(op1) > mostSignificant(op2))
        {
            return true;
        }
        if (mostSignificant(op2) > mostSignificant(op1))
        {
            return false;
        }
        return leastSignificant(op1) > leastSignificant(op2);
    }
};

template<> struct less_equal<std::complex<double> >: std::binary_function<std::complex<double>, std::complex<double>, bool>
{
    bool operator()(std::complex<double> const& op1, std::complex<double> const& op2) const
    {
        return !std::greater<std::complex<double> >()(op2, op1);
    }
};
template<> struct greater_equal<std::complex<double> >: std::binary_function<std::complex<double>, std::complex<double>, bool>
{
    bool operator()(std::complex<double> const& op1, std::complex<double> const& op2) const
    {
        return !std::less<std::complex<double> >()(op2, op1);
    }
};
}

namespace
{
/*
 * Operations on sparse matrices can require full traversal of the sparse args if the default (0 or false) value
 * of one arg does not set the result to the default result value.
 * For example, '==' does require full traversal because "0. == 0" is true (and the default SparseBool value is false)
 * However, '!=' does not require it because "0. != 0." is false so the default value of the sparse result is good.
 * we dispatch according the operators thanks to the OperatorTraits struct (as is usually done for example to
 * dispatch amongst std:: algorithms implementations according to the iterator_category in std::iterator_traits<>
 */
struct FullTraversal {};
struct UnionTraversal {};

struct WithFullTraversal
{
    typedef FullTraversal traversal;
};

struct WithUnionTraversal
{
    typedef UnionTraversal traversal;
};

template<typename T> struct OperatorTraits : WithFullTraversal
{
};


template<typename Scalar> struct OperatorTraits<std::less<Scalar> > : WithUnionTraversal
{
};
template<typename Scalar> struct OperatorTraits<std::greater<Scalar> > : WithUnionTraversal
{
};
template<typename Scalar> struct OperatorTraits<std::not_equal_to<Scalar> > : WithUnionTraversal
{
};
template<typename Scalar> struct OperatorTraits<std::less_equal<Scalar> > : WithFullTraversal
{
};
template<typename Scalar> struct OperatorTraits<std::greater_equal<Scalar> > : WithFullTraversal
{
};
template<typename Scalar> struct OperatorTraits<std::equal_to<Scalar> > : WithFullTraversal
{
};

// would be even better with intersectionTraversal if Eigen exposed such traversal
template<typename Scalar> struct OperatorTraits<std::logical_and<Scalar> > : WithUnionTraversal
{
};

template<typename Scalar> struct OperatorTraits<std::logical_or<Scalar> > : WithUnionTraversal
{
};

/*
 * Special case when the second operand is a Scalar
 * @param op1 Eigen first operand
 * @param op2 scalar second operand
 * @param op binary operation to perform
 * @return ptr to the new Eigen::Sparse result containing the cwise binary op with the scalar.
 */
template<typename Sp1, typename Scalar2, typename Op>
Eigen::SparseMatrix<typename Op::result_type, Eigen::RowMajor>* scalarOp(Eigen::EigenBase<Sp1> const& op1, Scalar2 op2, Op op)
{
    typedef typename Eigen::internal::traits<Sp1>::Scalar Scalar1;
    typedef typename Op::result_type result_scalar;
    typedef Eigen::SparseMatrix<result_scalar, Eigen::RowMajor> result_t;
    result_t* res;
    if (op(Scalar1(), op2) == result_scalar())
    {
        res = new result_t(op1.derived().unaryExpr(std::bind2nd(op, op2)));
        res->prune(&keepForSparse<result_scalar>);
    }
    else
    {
        /* it is not possible to  perform full traversal (including virtual 0 elts) of the sparse
         matrix hence the need for a dense tmp :( */
        // TODO remove dense temp when Eigen provides sparse full traversal API
        Eigen::Matrix<Scalar1, Eigen::Dynamic, Eigen::Dynamic> tmp(op1);
        res = new result_t(tmp.unaryExpr(std::bind2nd(op, op2)).sparseView());
    }
    return res;
}

/*
 * Special case when the first operand is a Scalar
 * @param op1 scalar first operand
 * @param op2 Eigen second operand
 * @param op binary operation to perform
 * @return ptr to the new Eigen::Sparse result containing the cwise binary op with the scalar.
 */
template<typename Scalar1, typename Sp2, typename Op>
Eigen::SparseMatrix<typename Op::result_type, Eigen::RowMajor>* scalarOp(Scalar1 op1, Eigen::EigenBase<Sp2> const& op2, Op op)
{
    typedef typename Eigen::internal::traits<Sp2>::Scalar Scalar2;
    typedef typename Op::result_type result_scalar;
    typedef Eigen::SparseMatrix<result_scalar, Eigen::RowMajor> result_t;
    result_t* res;
    if (op(op1, Scalar2()) == result_scalar())
    {
        res = new result_t(op2.derived().unaryExpr(std::bind1st(op, op1)));
        res->prune(&keepForSparse<result_scalar>);
    }
    else
    {
        // TODO remove dense temp when Eigen provides sparse full traversal API
        Eigen::Matrix<Scalar2, Eigen::Dynamic, Eigen::Dynamic> tmp(op2);
        res = new result_t(tmp.unaryExpr(std::bind1st(op, op1)).sparseView());
    }
    return res;
}
/*
 * Common case between two Eigen matrices. Performs the dispatch to scalar specialized versions
 * or according to the traversal.
 *
 * @param op1 Eigen first operand
 * @param op2 Eigen second operand
 * @param op binary operation to perform
 * @return ptr to the new Eigen::Sparse result containing the cwise binary op with the two matrices.
 */
template< typename Sp1, typename Sp2 , typename Op>
Eigen::SparseMatrix<typename Op::result_type, Eigen::RowMajor>* cwiseOp(Eigen::EigenBase<Sp1> const& op1, Eigen::EigenBase<Sp2> const& op2, Op op)
{
    if (op1.rows() == 1 && op1.cols() == 1)
    {
        return scalarOp(op1.derived().coeff(0, 0), op2, op);
    }
    if (op2.rows() == 1 && op2.cols() == 1)
    {
        return scalarOp(op1, op2.derived().coeff(0, 0), op);
    }
    return cwiseOp(op1, op2, op, typename OperatorTraits<Op>::traversal());
}
/*
 * Performs the cwise binary operator between two non scalar matrices, with full traversal.
 * SLOW and using memory, but you asked for it by using inapropriate operators.
 *
 * @param op1 Eigen first operand
 * @param op2 Eigen second operand
 * @param op binary operation to perform
 * @return ptr to the new Eigen::Sparse result containing the cwise binary op between the two matrices
 */
template< typename Sp1, typename Sp2 , typename Op>
Eigen::SparseMatrix<typename Op::result_type, Eigen::RowMajor>* cwiseOp(Eigen::EigenBase<Sp1> const& op1, Eigen::EigenBase<Sp2> const& op2, Op op, FullTraversal /*unused*/)
{
    typedef typename Op::result_type result_scalar;
    typedef Eigen::SparseMatrix<result_scalar, Eigen::RowMajor> result_t;
    typedef typename Eigen::internal::traits<Sp1>::Scalar Scalar1;
    typedef typename Eigen::internal::traits<Sp2>::Scalar Scalar2;
    // TODO remove dense temp when Eigen provides sparse full traversal API
    Eigen::Matrix<Scalar1, Eigen::Dynamic, Eigen::Dynamic> tmp1(op1);
    Eigen::Matrix<Scalar2, Eigen::Dynamic, Eigen::Dynamic> tmp2(op2);

    return new result_t(tmp1.binaryExpr(tmp2, op).sparseView());
}
/*
 * Performs the cwise binary operator between two non scalar matrices, with full traversal.
 * SLOW and using memory, but you asked for it by using inapropriate operators.
 *
 * @param op1 Eigen first operand
 * @param op2 Eigen second operand
 * @param op binary operation to perform
 * @return ptr to the new Eigen::Sparse result containing the cwise binary op with the scalar.
 */
template< typename Sp1, typename Sp2 , typename Op>
Eigen::SparseMatrix<typename Op::result_type, Eigen::RowMajor>* cwiseOp(Eigen::EigenBase<Sp1> const& op1, Eigen::EigenBase<Sp2> const& op2, Op op, UnionTraversal /*unused*/)
{
    typedef typename Op::result_type result_scalar;
    typedef Eigen::SparseMatrix<result_scalar, Eigen::RowMajor> result_t;
    result_t* res(new result_t(op1.derived().binaryExpr(op2.derived(), op)));
    res->prune(&keepForSparse<result_scalar>);
    return res;
}

/*
 * Performs the cwise binary operator between two types::Sparse arguments
 * dispatching according to whether they are complex or not.
 *
 * @param op1 types::Sparse first operand
 * @param op2 types::Sparse second operand
 * @param op binary operation to perform
 * @return ptr to the new types::SparseBool result containing the cwise binary op with the scalar.
 */
template<template <typename ElementType> class Op>
types::SparseBool* cwiseOp(types::Sparse const& op1, types::Sparse const& op2)
{
    types::SparseBool::BoolSparse_t* res(0);
    if (op1.isComplex())
    {
        if (op2.isComplex())
        {
            res = cwiseOp(*op1.matrixCplx, *op2.matrixCplx, Op<std::complex<double> >());
        }
        else
        {
            types::Sparse temp(op2);
            temp.toComplex();
            res = cwiseOp(*op1.matrixCplx, *temp.matrixCplx, Op<std::complex<double> >());
        }
    }
    else
    {

        if (op2.isComplex())
        {
            types::Sparse temp(op1);
            temp.toComplex();
            res = cwiseOp(*temp.matrixCplx, *op2.matrixCplx, Op<std::complex<double> >());
        }
        else
        {
            res = cwiseOp(*op1.matrixReal, *op2.matrixReal, Op<double>());
        }
    }
    return new types::SparseBool(res);
}

template<template <typename ElementType> class Op>
types::SparseBool* cwiseOp(types::SparseBool const& op1, types::SparseBool const& op2)
{
    return new types::SparseBool(cwiseOp(*op1.matrixBool, *op2.matrixBool, Op<bool>()));
}

}
#endif
