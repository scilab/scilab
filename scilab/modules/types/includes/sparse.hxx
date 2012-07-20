/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard Hugueney
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SPARSE_HH__
#define __SPARSE_HH__

#include <Eigen/Sparse>
#include <complex>
#include "double.hxx"
#include "bool.hxx"

#define CONST

#include "dynlib_types.h"


namespace types
{
/* Utility function to create a new var on the heap from another type
 */
template<typename Dest, typename Arg>
Dest* create_new(Arg const&);

struct SparseBool;

/**
   Sparse is a wrapper over Eigen sparse matrices templates for either double or std::complex<double> values.
 */
struct TYPES_IMPEXP Sparse : GenericType
{
    virtual ~Sparse();
    /* @param src: Double matrix to copy into a new sparse matrix
    **/
    Sparse(Double CONST& src);
    /* @param src : Double matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
    **/
    Sparse(Double CONST& src, Double CONST& idx);
    /* @param src : Double matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
       @param dims : Double matrix containing the dimensions of the new matrix
    **/
    Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims);
    /*
      @param rows : nb of rows of the new matrix
      @param rows : nb of columns of the new matrix
      @param cplx : if the new matrix contains complex numbers
    **/
    Sparse(int rows, int cols, bool cplx = false);
    Sparse(Sparse const& o);
    /* cf. adj2sp()
      @param xadj : adjacency matrix for the new matrix
      @param adjncy : adjacency matrix (row indexes) for the new matrix
      @param src : data for the new matrix
      @param r : nb of rows for the new matrix
      @param c : nb of columns for the new matrix
    **/
    Sparse(Double CONST& xadj, Double CONST& adjncy, Double CONST& src, std::size_t r, std::size_t c);


    bool isSparse()
    {
        return true;
    }
    void finalize();

    /*data management member function defined for compatibility with the Double API*/
    bool set(int _iRows, int _iCols, double _dblReal);
    bool set(int _iRows, int _iCols, std::complex<double> v);
    /*
      set non zero values to 1.
    **/
    bool one_set();
    /* get real value at coords (r,c)
    **/
    double getReal(int r, int c) const;
    double get(int r, int c) const;
    std::complex<double> getImg(int r, int c) const;

    /* return true if matrix contains complex numbers, false otherwise.
    **/
    bool isComplex() const;
    bool isScalar()
    {
        return (getRows() == 1 && getCols() == 1);
    }
    /* clear all the values of the matrix to 0. (or 0.+0.i if complex)
    **/
    bool zero_set();

    Sparse* getColumnValues(int _iPos)
    {
        return NULL;
    }

    /*
      Config management and GenericType methods overrides
    */
    void whoAmI() CONST;
    bool isExtract() const;
    Sparse* clone(void) const;
    Sparse* clone(void)
    {
        return const_cast<Sparse const*>(this)->clone();
    }
    bool toString(std::wostringstream& ostr) const;
    bool toString(std::wostringstream& ostr)
    {
        return const_cast<Sparse const*>(this)->toString(ostr);
    }

    /* post condition: dimensions are at least _iNewRows, _iNewCols
       preserving existing data.
       If dimensions where already >=, this is a no-op.

       @param _iNewRows new minimum nb of rows
       @param _iNewCols new minimum nb of cols
       @return true upon succes, false otherwise.
     */
    bool resize(int _iNewRows, int _iNewCols);
    /* post condition: new total size must be equal to the old size.
                       Two dimensions maximum.

       @param _iNewRows new nb of rows
       @param _iNewCols new nb of cols
       @param _piNewDims new nb of dimension
       @param _iNewDims new size for each dimension
       @return true upon succes, false otherwise.
    */
    bool reshape(int* _piNewDims, int _iNewDims);
    bool reshape(int _iNewRows, int _iNewCols);
    /*
      insert _iSeqCount elements from _poSource at coords given by _piSeqCoord (max in _piMaxDim).
      coords are considered 1D if _bAsVector, 2D otherwise.
      @param _iSeqCount nb of elts to insert
      @param _piSeqCoord dest coords
      @param _poSource src
      @param  _bAsVector if _piSeqCoord contains 1D coords.
     */
    Sparse* insert(typed_list* _pArgs, InternalType* _pSource);
    Sparse* insert(typed_list* _pArgs, Sparse* _pSource);

    static InternalType* insertNew(typed_list* _pArgs, InternalType* _pSource);

    /* append _poSource from coords _iRows, _iCols
       @param _iRows row to append from
       @param _iCols col to append from
       @param _poSource src data to append
     */
    bool append(int r, int c, types::Sparse CONST* src);

    /*
      extract a submatrix
      @param _iSeqCount nb of elts to extract
      @param _piSeqCoord src coords
      @param _piMaxDim max coords
      @param _piDimSize size of the extracted matrix
      @param  _bAsVector if _piSeqCoord contains 1D coords.

     */
    InternalType* extract(typed_list* _pArgs);
    Sparse* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) CONST;

    /*
       change the sign (inplace).
     */
    void opposite();

    /*
      compares with an other value for equality (same nb of elts, with same values)
      TODO: should it handle other types ?
     */
    bool operator==(const InternalType& it) CONST;
    /*
      compares with an other value for inequality (same nb of elts, with same values)
      TODO: should it handle other types ?
     */
    bool operator!=(const InternalType& it) CONST
    {
        return !(*this == it);
    }


    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring         getTypeStr() CONST {return std::wstring(L"sparse");}
    /* return type as short string ( s, i, ce, l, ... ), as in overloading macros*/
    virtual std::wstring         getShortTypeStr() CONST {return std::wstring(L"sp");}

    /* create a new sparse matrix containing the result of an addition
       @param o other matrix to add
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* add(Sparse const& o) const;

    /* create a new sparse matrix containing the result of an addition
       @param d scalar to add
       @return ptr to the new matrix, 0 in case of failure
     */
    Double* add(double d) const;

    /* create a new sparse matrix containing the result of an addition
       @param c complex  to add
       @return ptr to the new matrix, 0 in case of failure
     */
    Double* add(std::complex<double> c) const;




    /* create a new sparse matrix containing the result of a substraction
       @param o other matrix to substract
       @return ptr to the new matrix, 0 in case of failure
     */
    GenericType* substract(Sparse const& o) const;

    /* create a new sparse matrix containing the result of an subtraction
       @param d scalar to subtract
       @return ptr to the new matrix, 0 in case of failure
     */
    Double* substract(double d) const;

    /* create a new sparse matrix containing the result of an subtraction
       @param c scalar to subtract
       @return ptr to the new matrix, 0 in case of failure
     */
    Double* substract(std::complex<double> c) const;


    /* create a new sparse matrix containing the result of a multiplication
       @param o other matrix to substract
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* multiply(Sparse const& o) const;

    /* create a new sparse matrix containing the result of an multiplication
       @param s scalar to multiply by
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* multiply(double s) const;

    /* create a new sparse matrix containing the result of an multiplication
       @param c scalar to subtract
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* multiply(std::complex<double> c) const;

    /* create a new matrix containing the result of an .*
       @param o sparse matrix to .*
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* dotMultiply(Sparse CONST& o) const;

    /* create a new sparse matrix containing the result of a transposition
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* newTransposed() const;

    /** create a new sparse matrix containing the non zero values set to 1.
       equivalent but faster than calling one_set() on a new copy of the
       current matrix.
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* newOnes() const;

    /** @return the nb of non zero values.
     */
    std::size_t nonZeros() const;

    /* @param i row of the current sparse matrix
       @return the nb of non zero values in row i
     */
    std::size_t nonZeros(std::size_t i) const;


    /**
       "in-place" cast into a sparse matrix of comlpex values
     */
    void toComplex();

    /* coefficient wise relational operator < between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '<' between the elements of *this and those of o.
     */
    SparseBool* newLessThan(Sparse const&o) const;

    /* coefficient wise relational operator > between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '>' between the elements of *this and those of o.
     */
    SparseBool* newGreaterThan(Sparse const&o) const;

    /* coefficient wise relational operator != between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '!=' between the elements of *this and those of o.
     */
    SparseBool* newNotEqualTo(Sparse const&o) const;

    /* coefficient wise relational operator <= between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       Do not use this function is possible as the result will be dense because
       0. <= 0. is true, hence the result matrix will hold a non default value (i.e. true)
       for each pair of default values (0.) of the sparse arguments !

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '<=' between the elements of *this and those of o.
     */
    SparseBool* newLessOrEqual(Sparse const&o) const;

    /* coefficient wise relational operator >= between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       Do not use this function is possible as the result will be dense because
       0. >= 0. is true, hence the result matrix will hold a non default value (i.e. true)
       for each pair of default values (0.) of the sparse arguments !

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '>=' between the elements of *this and those of o.
     */
    SparseBool* newGreaterOrEqual(Sparse const&o) const;

    /* coefficient wise relational operator == between *this sparse matrix and an other.
       Matrices must have the same dimensions except if one of them is of size (1,1)
       (i.e. a scalar) : it is then treated as a constant matrix of thre required dimensions.

       Do not use this function is possible as the result will be dense because
       0. == 0. is true, hence the result matrix will hold a non default value (i.e. true)
       for each pair of default values (0.) of the sparse arguments !

       @param o other sparse matrix

       @return ptr to a new Sparse matrix where each element is the result of the logical operator
        '==' between the elements of *this and those of o.
     */
    SparseBool* newEqualTo(Sparse const&o) const;

    /**
       output 1-base column numbers of the non zero elements
       @param out : ptr used as an output iterator over double values
       @return past-the-end output iterator after ouput is done
     */
    double* outputCols(double* out) const;

    /**
       output real and imaginary values of the non zero elements
       @param outReal : ptr used as an output iterator over double values for real values
       @param outImag : ptr used as an output iterator over double values for imaginary values if any
       @return pair of past-the-end output iterators after ouput is done
     */
    std::pair<double*, double*> outputValues(double* outReal, double* outImag)const;

    /**
       ouput rows and afterwards columns of the non zero elements
       @param out : ptr used as an output iterator over double values
       @return past-the-end output iterators after ouput is done
     */
    double* outputRowCol(double* out)const;

    /**
       @param dest Double to be filled with values from the current sparse matrix.
     */
    void fill(Double& dest, int r = 0, int c = 0) CONST;


    RealType getType(void) CONST;


    SparseBool* newLesserThan(Sparse const&o);

    typedef Eigen::SparseMatrix<double >   RealSparse_t;
    typedef Eigen::SparseMatrix<std::complex<double > >    CplxSparse_t;
    /**
       One and only one of the args should be 0.
       @param realSp ptr to an Eigen sparse matrix of double values
       @param cplxSp ptr to an Eigen sparse matrix of std::complex<double> elements
     */
    Sparse(RealSparse_t* realSp, CplxSparse_t* cplxSp);

    RealSparse_t* matrixReal;
    CplxSparse_t* matrixCplx;

protected :
private :

    /** utility function used by constructors
        @param rows : nb of rows
        @param cols : nb of columns
        @param src : Double matrix data source
        @param : iterator (cf MatrixIterator.hxx) with indices
        @param n : nb of elements to copy from data source.
     */
    template<typename DestIter>
    void create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n);

    /** utility function used by insert functions conceptually : sp[destTrav]= src[srcTrav]
        @param src : data source
        @param SrcTrav : iterator over the data source
        @param n : nb of elements to copy
        @param sp : sparse destination matrix
        @param destTrav : iterator over the data sink (i.e. sp)
     */
    template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
    static bool copyToSparse(Src CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav);
};
/*
  Implement sparse boolean matrix
 */
struct TYPES_IMPEXP SparseBool : GenericType
{

    /* @param src: Bool matrix to copy into a new sparse matrix
    **/
    SparseBool(Bool CONST& src);
    /* @param src : Bool matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
    **/
    SparseBool(Bool CONST& src, Double CONST& idx);
    /* @param src : Bool matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
       @param dims : Double matrix containing the dimensions of the new matrix
    **/
    SparseBool(Bool CONST& src, Double CONST& idx, Double CONST& dims);
    /*
       @param rows : nb of rows of the new matrix
       @param rows : nb of columns of the new matrix
    */
    SparseBool(int rows, int cols);

    SparseBool(SparseBool const& o);

    bool isSparseBool()
    {
        return true;
    }
    void finalize();

    bool toString(std::wostringstream& ostr) const;
    bool toString(std::wostringstream& ostr)
    {
        return const_cast<SparseBool const*>(this)->toString(ostr);
    }

    /* Config management and GenericType methods overrides */
    SparseBool* clone(void) const;
    SparseBool* clone(void)
    {
        return const_cast<SparseBool const*>(this)->clone();
    }
    bool resize(int _iNewRows, int _iNewCols);

    bool reshape(int* _piNewDims, int _iNewDims);
    bool reshape(int _iNewRows, int _iNewCols);

    bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType CONST* _poSource, bool _bAsVector);

    bool append(int _iRows, int _iCols, SparseBool CONST* _poSource);

    static SparseBool* insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType CONST* _poSource, bool _bAsVector);
    SparseBool* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) CONST;
    InternalType* extract(typed_list* _pArgs);

    SparseBool* getColumnValues(int _iPos)
    {
        return NULL;
    }

    /* create a new sparse matrix containing the result of a transposition
       @return ptr to the new matrix, 0 in case of failure
     */
    SparseBool* newTransposed() const;

    /** @return the nb of non zero values.
     */
    std::size_t nbTrue() const;
    /* @param i row of the current sparse matrix
       @return the nb of non zero values in row i
     */
    std::size_t nbTrue(std::size_t i) const;

        int* getNbItemByRow();
        int* getColPos();
    /**
       output 1-base column numbers of the non zero elements
       @param out : ptr used as an output iterator over double values
       @return past-the-end output iterator after ouput is done
     */

    double* outputRowCol(double* out)const;

    bool operator==(const InternalType& it) CONST;
    bool operator!=(const InternalType& it) CONST;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr() CONST {return std::wstring(L"boolean sparse");}
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr() CONST {return std::wstring(L"spb");}

    RealType getType(void) CONST;

    void whoAmI() CONST;

    bool get(int r, int c) CONST;
    bool set(int r, int c, bool b) CONST;

    void fill(Bool& dest, int r = 0, int c = 0) CONST;

    Sparse* newOnes() const;
    SparseBool* newNotEqualTo(SparseBool const&o) const;
    SparseBool* newEqualTo(SparseBool const&o) const;

    SparseBool* newLogicalOr(SparseBool const&o) const;
    SparseBool* newLogicalAnd(SparseBool const&o) const;

    typedef Eigen::SparseMatrix<bool> BoolSparse_t;
    SparseBool(BoolSparse_t* o);
    BoolSparse_t* matrixBool;

private:
    template<typename DestIter>
    void create(int rows, int cols, Bool CONST& src, DestIter o, std::size_t n);

};
template<typename T>
struct SparseTraits
{
    typedef types::Sparse type;
};
template<>
struct SparseTraits<types::Bool>
{
    typedef types::SparseBool type;
};
template<>
struct SparseTraits<types::SparseBool>
{
    typedef types::SparseBool type;
};

}

#endif /* !__SPARSE_HH__ */
