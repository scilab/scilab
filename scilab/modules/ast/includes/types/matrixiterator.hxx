/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bernard HUGUENEY
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
#ifndef MATRIXITERATORS_HXX
#define MATRIXITERATORS_HXX

#include <complex>
#include <utility>
#include <iterator>

#include <Eigen/Sparse>
#include "double.hxx"
#include "sparse.hxx"


/*
  In order to reuse code for the various Matrix Classes, we need some uniform API to access elements.
  We cannot use runtime polymorphism (with dynamic dispatching) because of the runtime cost so we have to
  use compile-time polymorphism with templates.

  The provided free function templates get<>() and set<>() provide such an uniform API.

  In order to perform element-wise operations on Matrices (copy, partial or total assignments, etc.),
  we provide an iterator. To enable reading (with get<>()) or writiting (with set<>()) we provide an Accessor<> proxy.

  As is it common to iterate over a sub-matrix according to indices given by a Scilab variable (Double) we provide
  an iterator created from such a variable (IteratorFromVar).
 */
template<typename T>
struct UndefinedAccessorForType {};

/**
   This free function overloads perform read access into a 2D container, using 0-based indices.
   @param s the 2D structure used to fetch a value of type V.
   @param r : the row (0 based)
   @param c : the column (0 based)
   @return : the value of type V at row r and column c of structure s
*/

template<typename V, typename S> V get(S SPARSE_CONST&, int, int)
{
    return UndefinedAccessorForType<S>();
}

template<> double get(types::Double SPARSE_CONST& d, int r, int c)
{
    return d.getReal(r, c);
}
template<> std::complex<double> get(types::Double SPARSE_CONST& d, int r, int c)
{
    return std::complex<double>(d.getReal(r, c), d.getImg(r, c));
}

template<> bool get(types::Bool SPARSE_CONST& d, int r, int c)
{
    return d.get(r, c) == 1;
}
template<> int get(types::Bool SPARSE_CONST& d, int r, int c)
{
    return d.get(r, c);
}
template<> bool get(types::SparseBool SPARSE_CONST& d, int r, int c)
{
    return d.get(r, c);
}
template<> int get(types::SparseBool SPARSE_CONST& d, int r, int c)
{
    return d.get(r, c);
}

template<> double get(types::Sparse SPARSE_CONST& s, int r, int c)
{
    return s.getReal(r, c);
}
template<> std::complex<double> get(types::Sparse SPARSE_CONST& s, int r, int c)
{
    return s.get(r, c);
}

template<> double get(types::Sparse::RealSparse_t SPARSE_CONST&s, int r, int c)
{
    return s.coeff(r, c);
}
template<> std::complex<double> get(types::Sparse::RealSparse_t SPARSE_CONST&s, int r, int c)
{
    return std::complex<double>(s.coeff(r, c), 0.);
}

template<> bool get(types::SparseBool::BoolSparse_t SPARSE_CONST& d, int r, int c)
{
    return d.coeff(r, c);
}

template<> double get(types::Sparse::CplxSparse_t SPARSE_CONST&s, int r, int c)
{
    return s.coeff(r, c).real();
}
template<> std::complex<double> get(types::Sparse::CplxSparse_t SPARSE_CONST&s, int r, int c)
{
    return s.coeff(r, c);
}


/**
   This free function overloads perform write access into a 2D container, using 0-based indices.
   @param s the 2D structure used to fetch a value of type V.
   @param r : the row (0 based)
   @param c : the column (0 based)
   @param v : the value of type V to set at row r and column c of structure s
   @return : true iff everything went ok (should throw otherwise anyway).
*/


template<typename S, typename V> bool set(S &, int, int, V)
{
    return UndefinedAccessorForType<S>();
}

template<> bool set(types::Double & d, int r, int c, double v)
{
    return d.set(r, c, v);
}
template<> bool set(types::Double & d, int r, int c, std::complex<double> v)
{
    return d.set(r, c, v.real()) && d.setImg(r, c, v.imag());
}

template<> bool set(types::Sparse & s, int r, int c, double v)
{
    return s.set(r, c, v);
}
template<> bool set(types::Sparse & s, int r, int c, std::complex<double> v)
{
    return s.set(r, c, v);
}
template<> bool set(types::Bool & d, int r, int c, bool v)
{
    return d.set(r, c, v);
}
template<> bool set(types::SparseBool & d, int r, int c, bool v)
{
    return d.set(r, c, v);
}
template<> bool set(types::Bool & d, int r, int c, int v)
{
    return d.set(r, c, v);
}
template<> bool set(types::SparseBool & d, int r, int c, int v)
{
    return d.set(r, c, v != 0);
}

template<> bool set(types::Sparse::RealSparse_t& s, int r, int c, double v)
{
    if (v != 0.)
    {
        s.insert(r, c) = v;
    }
    return true;
}

template<> bool set(types::Sparse::RealSparse_t& s, int r, int c, std::complex<double> v)
{
    if ( v.real() != 0.)
    {
        s.insert(r, c) = v.real();
    }
    return  true;
}
// should we make this a compile error ?
template<> bool set(types::Sparse::CplxSparse_t& s, int r, int c, double v)
{
    if (v != 0.)
    {
        s.insert(r, c) = std::complex<double>(v);
    }
    return true;
}

namespace
{
std::complex<double> const cplxZero(0., 0.);
}
template<> bool set(types::Sparse::CplxSparse_t& s, int r, int c, std::complex<double> v)
{
    if (v != cplxZero)
    {
        s.insert(r, c) = v;
    }
    return true;
}

template<> bool set(types::SparseBool::BoolSparse_t& s, int r, int c, bool v)
{
    if (v)
    {
        s.insert(r, c) = v;
    }
    return true;
}




template<typename S> inline int rows(S SPARSE_CONST&s)
{
    return s.rows();
}
template<typename S> inline int cols(S SPARSE_CONST&s)
{
    return s.cols();
}

template<> inline int rows(types::Double SPARSE_CONST&d)
{
    return d.getRows();
}
template<> inline int cols(types::Double SPARSE_CONST&d)
{
    return d.getCols();
}
template<> inline int rows(types::Sparse SPARSE_CONST&s)
{
    return s.getRows();
}
template<> inline int cols(types::Sparse SPARSE_CONST&s)
{
    return s.getCols();
}
template<> inline int rows(types::Bool SPARSE_CONST&s)
{
    return s.getRows();
}
template<> inline int cols(types::Bool SPARSE_CONST&s)
{
    return s.getCols();
}
template<> inline int rows(types::SparseBool SPARSE_CONST&s)
{
    return s.getRows();
}
template<> inline int cols(types::SparseBool SPARSE_CONST&s)
{
    return s.getCols();
}



/**
  These free function overloads handle nb of rows size queries for 2D containers
   wrapping the corresponding member function.
   @param s : 2D structure to query
   @return : nb of rows
*/
template<typename S> inline int rows(S SPARSE_CONST&s);
template<> inline int rows(types::Double SPARSE_CONST&d);

/**
  These free function overloads handle nb of cols size queries for 2D containers
   wrapping the corresponding member function.
   @param s : 2D structure to query
   @return : nb of cols
*/
template<typename S> inline int cols(S SPARSE_CONST&s);
template<> inline int cols(types::Double SPARSE_CONST&d);

/* this proxy struct provides read and write access (using set and get)
   with the usual operators (operator*() and operator=() )*/
template<typename S, typename V> struct Accessor
{
    /**
       @param s_ : 2D structure to access
       @param r_ : row to access
       @param c_ ; column to access
    */
    Accessor(S& s_, int r_, int c_): s(s_), r(r_), c(c_) {}
    /**
       read accessor as a casting operator
       @return : value of s at (r,c)
     */
    operator V() SPARSE_CONST
    {
        //        std::cerr<<"reading "<<get<S,V>(s, r, c)<<" @("<<r<<","<<c<<")\n";
        return ::get<V>(s, r, c);
    }
    /**
       write accessor as an assignment operator
       @param v : value to set at (r,c) in s.
    */
    template<typename Sa, typename Va>
    Accessor& operator=(Accessor<Sa, Va> const& a)
    {
        //        std::cerr<<"writing "<<( Va(const_cast<Accessor<Sa, Va>&>(a)))<<" @("<<r<<","<<c<<")\n";
        //        Va tmp=const_cast<Accessor<Sa, Va>&>(a);
        //        ::set<S,V>(s, r, c, tmp);
        ::set<S, V>(s, r, c, Va(const_cast<Accessor<Sa, Va>&>(a)));
        return *this;
    }

    Accessor& operator=(Accessor const& a)
    {
        //        std::cerr<<"writing "<<( V(const_cast<Accessor&>(a)))<<" @("<<r<<","<<c<<")\n";
        ::set<S, V>(s, r, c, V(const_cast<Accessor&>(a)));
        return *this;
    }
    Accessor& operator=(V const& v)
    {
        //        std::cerr<<"writing "<<v<<" @("<<r<<","<<c<<")\n";
        ::set<S, V>(s, r, c, v);
        return *this;
    }
private:
    S& s;
    int r, c;
};

/* convenient typedef for pairs of (row, column) int values used as 2D coords */
typedef std::pair<int, int> Coords2D;
/* convenient typedef for iterator over pairs of (row, column) int values used as 2D coords */
typedef std::iterator<std::forward_iterator_tag, Coords2D > Coords2DIterator;
/**
   Iterator over coords making a full row-wise traversal wrapping around when reaching
   the end of the 2D container.
 */
struct RowWiseFullIterator : Coords2DIterator
{
    /**
       @param cMax : size of the 2D structure
     */
    RowWiseFullIterator(Coords2D cMax): c(0, 0), cMax(cMax)
    {
    }
    /**
       @param cMax : size of the 2D structure
       @param cInit : starting coords of the traversal.
     */
    RowWiseFullIterator(Coords2D cMax, Coords2D cInit): c(cInit), cMax(cMax)
    {
    }
    /**
       @param rm : nb of rows of the 2D structure
       @param cm : nb of column of the 2D structure
     */
    RowWiseFullIterator(int rm, int cm): c(0, 0), cMax(rm, cm)
    {
    }
    /**
       @param rm : nb of rows of the 2D structure
       @param cm : nb of column of the 2D structure
       @param rInit : starting row of the traversal
       @param cInit : starting column of the traversal
     */
    RowWiseFullIterator(int rm, int cm, int rInit, int cInit): c(rInit, cInit), cMax(rm, cm)
    {
    }
    RowWiseFullIterator& operator++()
    {
        if (++c.first == cMax.first)
        {
            c.first = 0;
            if (++c.second == cMax.second)
            {
                /* wrap around */
                c.first = c.second = 0;
            }
        }
        return *this;
    }
    RowWiseFullIterator operator++(int)
    {
        RowWiseFullIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    std::pair<int, int> operator*() const
    {
        return c;
    }
private:
    Coords2D c;
    Coords2D const cMax;
};

/**
   Iterator over coords making a row-wise traversal of non zero elements of an Eigen Sparse Matrix
 */
template<typename Sp>
struct RowWiseSparseIterator : Coords2DIterator
{
    /**
       @param sp: sparse matrix for non zero elements traversal
     */
    RowWiseSparseIterator(Sp const& sp): sp(sp), outerIdx(0), innerIt(sp, 0)
    {
    }
    RowWiseSparseIterator& operator++()
    {
        ++innerIt;
        if (!innerIt)
        {
            if (++outerIdx >= sp.outerSize())
            {
                outerIdx = 0;
            }
            new (&innerIt) typename Sp::InnerIterator(sp, outerIdx);// innerIt= typename Sp::InnerIterator(sp, outerIdx) when Eigen will be fixed
        }
        return *this;
    }
    RowWiseSparseIterator operator++(int)
    {
        RowWiseFullIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    std::pair<int, int> operator*() const
    {
        //        std::cerr<<"sparse it r="<<innerIt.row()<<" c="<<innerIt.col()<<std::endl;
        return std::pair<int, int>(innerIt.row(), innerIt.col());
    }
private:
    Sp const& sp;
    typename Eigen::internal::traits<Sp>::Index outerIdx;
    typename Sp::InnerIterator innerIt;
};

/**
   translate an iterator
 */
template<typename C2DIter>
struct TranslatedIterator : Coords2DIterator
{
    /**
       @param C2DIter: translation as a vector of (rows, cols)
       @param tr: translation as a vector of (rows, cols)
     */
    TranslatedIterator(C2DIter const& c2dIter, Coords2D tr): it(c2dIter), tr(tr)
    {
    }
    TranslatedIterator& operator++()
    {
        ++it;
        return *this;
    }
    TranslatedIterator operator++(int)
    {
        TranslatedIterator tmp(*this);
        ++(*this);
        return tmp;
    }
    std::pair<int, int> operator*() const
    {
        std::pair<int, int>res(*it);
        res.first += tr.first;
        res.second += tr.second;
        //        std::cerr<<"translated it r="<< res.first<<" c="<<res.second<<std::endl;

        return res;
    }
private:
    C2DIter it;
    Coords2D const tr;
};

/**
 * Template for iterator over 2D coords from an int*.
 * Could handle wrap around with a length arg (i.e. to recycle values instead of raising
 * "error 15 Submatrix incorrectly defined."
 */
template<bool AsVector = false> struct Coords : Coords2DIterator
{
    Coords(int SPARSE_CONST* coords, int unused = 0): coords(coords), unused(unused)
    {
    }

    Coords& operator++()
    {
        coords += 2;
        return *this;
    }

    Coords& operator++(int)
    {
        Coords tmp(*this);
        ++(*this);
        return tmp;
    }

    Coords2D operator*()const
    {
        return Coords2D(coords[0] - 1, coords[1] - 1);
    }

private:
    int const* coords;
    int unused;
};
/**
   explicit specialization for 2D from 1D int* sequences
   (The 2D strcture is considered as a vector)
 */
template<> struct Coords<true> : Coords2DIterator
{
    Coords(int SPARSE_CONST* coords, int rMax): coords(coords), rMax(rMax)
    {
    }

    Coords& operator++()
    {
        ++coords;
        return *this;
    }

    Coords operator++(int)
    {
        Coords tmp(*this);
        ++(*this);
        return tmp;
    }

    Coords2D operator*()const
    {
        return Coords2D((coords[0] - 1) % rMax, (coords[0] - 1) / rMax);
    }

private:
    int const* coords;
    int const rMax;
};
/* This 'iterator' class allows traverses the 2D containers, either
Rowwisefull traversal
or with 2D coords from another matrix
or with 1D coords from another vector (1x) matrix
to respect Double insert() API, we take int* and a bool
*/
template<typename S, typename V, typename Iter>
struct MatrixIterator : std::iterator<std::forward_iterator_tag, V>
{
    MatrixIterator(S& s_, Iter i_): s(s_), i(i_)
    {
    }
    MatrixIterator& operator++()
    {
        ++i;
        return *this;
    }
    MatrixIterator operator++(int)
    {
        MatrixIterator tmp(*this);
        ++i;
        return tmp;
    }
    Accessor<S, V> operator*()
    {
        return Accessor<S, V>(s, (*i).first, (*i).second);
    }
private:
    S& s;
    Iter i;
};
template<typename V, typename S, typename Iter>
MatrixIterator<S, V, Iter> makeMatrixIterator(S& s, Iter i)
{
    return MatrixIterator<S, V, Iter>(s, i);
}

template<typename S> struct IteratorFromVar;

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s);

struct Adjacency
{
    Adjacency(double const* x, double const*a): xadj(x), adjncy(a) {}
    double const* xadj;
    double const* adjncy;
};

template<typename In, typename Sz, typename Out>
Out mycopy_n(In i, Sz n, Out o)
{
    for (; n; --n, ++i, ++o)
    {
        *o = *i;
    }
    return o;
}

template<typename T> std::size_t nonZeros(T SPARSE_CONST& t)
{
    return t.getSize();
}
template<> std::size_t nonZeros(types::Sparse SPARSE_CONST& sp)
{
    return sp.nonZeros();
}
template<typename Scalar, int Options, typename Index> std::size_t nonZeros(Eigen::SparseMatrix<Scalar, Options, Index> SPARSE_CONST& sp)
{
    return sp.nonZeros();
}


/* Default for dense matrix Scilab matrix types
 */
template<typename D> RowWiseFullIterator makeNonZerosIterator(D SPARSE_CONST& d)
{
    return RowWiseFullIterator(d.getRows(), d.getCols());
}
template<typename Scalar, int Options, typename Index> RowWiseSparseIterator<Eigen::SparseMatrix<Scalar, Options, Index> > makeNonZerosIterator(Eigen::SparseMatrix<Scalar, Options, Index> SPARSE_CONST& sp)
{
    return RowWiseSparseIterator<Eigen::SparseMatrix<Scalar, Options, Index> >(sp);
}
template<typename Iter> TranslatedIterator<Iter> makeTranslatedIterator(Iter const& it, Coords2D const& tr)
{
    return TranslatedIterator<Iter>(it, tr);
}



template<typename S> struct IteratorFromVar { };

template<> struct IteratorFromVar<types::Double> : Coords2DIterator
{
    IteratorFromVar(types::Double& d_): d(d_), r(0)
    {
        // check dimension ?
    }

    IteratorFromVar& operator++()
    {
        ++r;
        return *this;
    }
    IteratorFromVar operator++(int)
    {
        IteratorFromVar tmp(*this);
        ++r;
        return tmp;
    }
    Coords2D operator*()
    {
        return std::pair<int, int>(static_cast<int>(d.getReal(r, 0) - 1), static_cast<int>(d.getReal(r, 1) - 1));
    }
private:
    types::Double& d;
    int r;
};

/*
  iterator from adjacency matrices :
 */
template<> struct IteratorFromVar<Adjacency> : Coords2DIterator
{
    IteratorFromVar(Adjacency& a): xadj(a.xadj), adjncy(a.adjncy), c(1), nb(1)
    {
        update();
    }

    IteratorFromVar& operator++()
    {
        ++nb;
        update();
        ++adjncy;
        return *this;
    }
    IteratorFromVar operator++(int)
    {
        IteratorFromVar tmp(*this);
        ++nb;
        update();
        ++adjncy;
        return tmp;
    }
    std::pair<int, int> operator*()
    {
        return std::pair<int, int>(static_cast<int>(*adjncy) - 1, c - 1);
    }
private:
    void update()
    {
        for (; xadj[1] <= nb; ++c, ++xadj)
        {
        }
    }
    double const* xadj;
    double const* adjncy;
    int c;
    std::size_t nb;
};

template<typename S> IteratorFromVar<S> makeIteratorFromVar(S& s)
{
    return IteratorFromVar<S>(s);
}

#endif
