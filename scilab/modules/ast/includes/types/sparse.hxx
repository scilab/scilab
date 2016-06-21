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

#ifndef __SPARSE_HH__
#define __SPARSE_HH__

//#include <Eigen/Sparse>
#include <complex>
#include "double.hxx"
#include "bool.hxx"
#include "keepForSparse.hxx"

#define SPARSE_CONST

namespace Eigen
{
template<typename _Scalar, int _Flags, typename _Index>  class SparseMatrix;
}

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
struct EXTERN_AST Sparse : GenericType
{
    virtual ~Sparse();
    /* @param src: Double matrix to copy into a new sparse matrix
    **/
    Sparse(Double SPARSE_CONST& src);
    /* @param src : Double matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
    **/
    Sparse(Double SPARSE_CONST& src, Double SPARSE_CONST& idx);
    /* @param src : Double matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
       @param dims : Double matrix containing the dimensions of the new matrix
    **/
    Sparse(Double SPARSE_CONST& src, Double SPARSE_CONST& idx, Double SPARSE_CONST& dims);
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
    Sparse(Double SPARSE_CONST& xadj, Double SPARSE_CONST& adjncy, Double SPARSE_CONST& src, std::size_t r, std::size_t c);

    //constructor to create a sparse from value extract to another ( save / load operation typically)
    Sparse(int rows, int cols, int nonzeros, int* inner, int* outer, double* real, double* img);

    bool isSparse()
    {
        return true;
    }
    void finalize();

    /*data management member function defined for compatibility with the Double API*/
    Sparse* set(int _iRows, int _iCols, double _dblReal, bool _bFinalize = true);
    Sparse* set(int _iIndex, double _dblReal, bool _bFinalize = true)
    {
        return set(_iIndex % m_iRows, _iIndex / m_iRows, _dblReal, _bFinalize);
    }

    Sparse* set(int _iRows, int _iCols, std::complex<double> v, bool _bFinalize = true);
    Sparse* set(int _iIndex, std::complex<double> v, bool _bFinalize = true)
    {
        return set(_iIndex % m_iRows, _iIndex / m_iRows, v, _bFinalize);
    }
    /*
      set non zero values to 1.
    **/
    bool one_set();
    /* get real value at coords (r,c)
    **/
    double getReal(int r, int c) const;
    double getReal(int _iIndex) const
    {
        return getReal(_iIndex % m_iRows, _iIndex / m_iRows);
    }

    double* get();
    double get(int r, int c) const;
    double get(int _iIndex) const
    {
        return get(_iIndex % m_iRows, _iIndex / m_iRows);
    }

    std::complex<double>* getImg();
    std::complex<double> getImg(int r, int c) const;
    std::complex<double> getImg(int _iIndex) const
    {
        return getImg(_iIndex % m_iRows, _iIndex / m_iRows);
    }

    /* return true if matrix contains complex numbers, false otherwise.
    **/
    bool isComplex() const;
    // overload of GenericType methode.
    bool isComplex()
    {
        // force const to call isComplex const method.
        const Sparse* sp = this;
        return sp->isComplex();
    }

    inline bool isScalar()
    {
        return (getRows() == 1 && getCols() == 1);
    }
    /* clear all the values of the matrix to 0. (or 0.+0.i if complex)
    **/
    bool zero_set();

    /*
      Config management and GenericType methods overrides
    */
    void whoAmI() SPARSE_CONST;
    bool isExtract() const;
    Sparse* clone(void);
    bool toString(std::wostringstream& ostr);

    /* post condition: dimensions are at least _iNewRows, _iNewCols
       preserving existing data.
       If dimensions where already >=, this is a no-op.

       @param _iNewRows new minimum nb of rows
       @param _iNewCols new minimum nb of cols
       @return true upon succes, false otherwise.
     */
    Sparse* resize(int _iNewRows, int _iNewCols);
    /* post condition: new total size must be equal to the old size.
                       Two dimensions maximum.

       @param _iNewRows new nb of rows
       @param _iNewCols new nb of cols
       @param _piNewDims new nb of dimension
       @param _iNewDims new size for each dimension
       @return true upon succes, false otherwise.
    */
    Sparse* reshape(int* _piNewDims, int _iNewDims);
    Sparse* reshape(int _iNewRows, int _iNewCols);
    /*
      insert _iSeqCount elements from _poSource at coords given by _piSeqCoord (max in _piMaxDim).
      coords are considered 1D if _bAsVector, 2D otherwise.
      @param _iSeqCount nb of elts to insert
      @param _piSeqCoord dest coords
      @param _poSource src
      @param  _bAsVector if _piSeqCoord contains 1D coords.
     */
    Sparse* insert(typed_list* _pArgs, InternalType* _pSource);

    GenericType* remove(typed_list* _pArgs);

    GenericType* insertNew(typed_list* _pArgs);

    /* append _poSource from coords _iRows, _iCols
       @param _iRows row to append from
       @param _iCols col to append from
       @param _poSource src data to append
     */
    Sparse* append(int r, int c, types::Sparse SPARSE_CONST* src);

    /*
      extract a submatrix
      @param _iSeqCount nb of elts to extract
      @param _piSeqCoord src coords
      @param _piMaxDim max coords
      @param _piDimSize size of the extracted matrix
      @param  _bAsVector if _piSeqCoord contains 1D coords.

     */
    GenericType* extract(typed_list* _pArgs);
    Sparse* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) SPARSE_CONST;
    virtual bool invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e);
    virtual bool isInvokable() const;
    virtual bool hasInvokeOption() const;
    virtual int getInvokeNbIn();
    virtual int getInvokeNbOut();

    /*
       change the sign (inplace).
     */
    void opposite();

    /*
      compares with an other value for equality (same nb of elts, with same values)
      TODO: should it handle other types ?
     */
    bool operator==(const InternalType& it) SPARSE_CONST;
    /*
      compares with an other value for inequality (same nb of elts, with same values)
      TODO: should it handle other types ?
     */
    bool operator!=(const InternalType& it) SPARSE_CONST
    {
        return !(*this == it);
    }


    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring         getTypeStr() SPARSE_CONST {return std::wstring(L"sparse");}
    /* return type as short string ( s, i, ce, l, ... ), as in overloading macros*/
    virtual std::wstring         getShortTypeStr() SPARSE_CONST {return std::wstring(L"sp");}

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
    Sparse* substract(Sparse const& o) const;

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
    Sparse* dotMultiply(Sparse SPARSE_CONST& o) const;

    /* create a new matrix containing the result of an ./
      @param o sparse matrix to ./
      @return ptr to the new matrix, 0 in case of failure
    */
    Sparse* dotDivide(Sparse SPARSE_CONST& o) const;

    bool neg(InternalType *& out);

    bool transpose(InternalType *& out);
    bool adjoint(InternalType *& out);
    int newCholLLT(Sparse** permut, Sparse** factor) const;

    /** create a new sparse matrix containing the non zero values set to 1.
       equivalent but faster than calling one_set() on a new copy of the
       current matrix.
       @return ptr to the new matrix, 0 in case of failure
     */
    Sparse* newOnes() const;

    Sparse* newReal() const;
    /** @return the nb of non zero values.
     */
    std::size_t nonZeros() const;

    /* @param i row of the current sparse matrix
       @return the nb of non zero values in row i
     */
    std::size_t nonZeros(std::size_t i) const;

    int* getNbItemByRow(int* _piNbItemByRows);
    int* getColPos(int* _piColPos);
    int* getInnerPtr(int* count);
    int* getOuterPtr(int* count);


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
    SparseBool* newLessThan(Sparse &o);

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
    SparseBool* newLessOrEqual(Sparse &o);

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
    SparseBool* newEqualTo(Sparse &o);

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
    int* outputRowCol(int* out)const;

    /**
       @param dest Double to be filled with values from the current sparse matrix.
     */
    void fill(Double& dest, int r = 0, int c = 0) SPARSE_CONST;


    inline ScilabType  getType(void) SPARSE_CONST
    {
        return ScilabSparse;
    }

    inline ScilabId    getId(void) SPARSE_CONST
    {
        if (isComplex())
        {
            return IdSparseComplex;
        }
        return IdSparse;
    }



    SparseBool* newLesserThan(Sparse const&o);

    typedef Eigen::SparseMatrix<double, 0x1, int>                   RealSparse_t;
    typedef Eigen::SparseMatrix<std::complex<double>, 0x1, int>     CplxSparse_t;
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
    void create(int rows, int cols, Double SPARSE_CONST& src, DestIter o, std::size_t n);
    void create2(int rows, int cols, Double SPARSE_CONST& src, Double SPARSE_CONST& idx);

    /** utility function used by insert functions conceptually : sp[destTrav]= src[srcTrav]
        @param src : data source
        @param SrcTrav : iterator over the data source
        @param n : nb of elements to copy
        @param sp : sparse destination matrix
        @param destTrav : iterator over the data sink (i.e. sp)
     */
    template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
    static bool copyToSparse(Src SPARSE_CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav);

    Sparse* insert(typed_list* _pArgs, Sparse* _pSource);
};

template<typename T>
void neg(const int r, const int c, const T * const in, Eigen::SparseMatrix<bool, 1, int> * const out);


/*
  Implement sparse boolean matrix
 */
struct EXTERN_AST SparseBool : GenericType
{
    virtual ~SparseBool();
    /* @param src: Bool matrix to copy into a new sparse matrix
    **/
    SparseBool(Bool SPARSE_CONST& src);
    /* @param src : Bool matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
    **/
    SparseBool(Bool SPARSE_CONST& src, Double SPARSE_CONST& idx);
    /* @param src : Bool matrix to copy into a new sparse matrix
       @param idx : Double matrix to use as indexes to get values from the src
       @param dims : Double matrix containing the dimensions of the new matrix
    **/
    SparseBool(Bool SPARSE_CONST& src, Double SPARSE_CONST& idx, Double SPARSE_CONST& dims);
    /*
       @param rows : nb of rows of the new matrix
       @param rows : nb of columns of the new matrix
    */
    SparseBool(int rows, int cols);

    SparseBool(SparseBool const& o);

    //constructor to create a sparse from value extract to another ( save / load operation typically)
    SparseBool(int rows, int cols, int trues, int* inner, int* outer);

    bool isSparseBool()
    {
        return true;
    }
    void finalize();

    bool toString(std::wostringstream& ostr);

    /* Config management and GenericType methods overrides */
    SparseBool* clone(void);

    SparseBool* resize(int _iNewRows, int _iNewCols);
    SparseBool* reshape(int* _piNewDims, int _iNewDims);
    SparseBool* reshape(int _iNewRows, int _iNewCols);
    SparseBool* insert(typed_list* _pArgs, InternalType* _pSource);
    SparseBool* append(int _iRows, int _iCols, SparseBool SPARSE_CONST* _poSource);

    GenericType* remove(typed_list* _pArgs);
    GenericType* insertNew(typed_list* _pArgs);
    GenericType* extract(typed_list* _pArgs);

    SparseBool* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector) SPARSE_CONST;

    virtual bool invoke(typed_list & in, optional_list &/*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e);
    virtual bool isInvokable() const;
    virtual bool hasInvokeOption() const;
    virtual int getInvokeNbIn();
    virtual int getInvokeNbOut();

    bool transpose(InternalType *& out);

    /** @return the nb of non zero values.
     */
    std::size_t nbTrue() const;
    /* @param i row of the current sparse matrix
       @return the nb of non zero values in row i
     */
    std::size_t nbTrue(std::size_t i) const;

    void setTrue(bool finalize = true);
    void setFalse(bool finalize = true);

    int* getNbItemByRow(int* _piNbItemByRows);
    int* getColPos(int* _piColPos);
    int* getInnerPtr(int* count);
    int* getOuterPtr(int* count);
    /**
       output 1-base column numbers of the non zero elements
       @param out : ptr used as an output iterator over double values
       @return past-the-end output iterator after ouput is done
     */

    int* outputRowCol(int* out)const;

    bool operator==(const InternalType& it) SPARSE_CONST;
    bool operator!=(const InternalType& it) SPARSE_CONST;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr() SPARSE_CONST {return std::wstring(L"boolean sparse");}
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr() SPARSE_CONST {return std::wstring(L"spb");}

    inline ScilabType getType(void) SPARSE_CONST
    {
        return ScilabSparseBool;
    }

    inline ScilabId getId(void) SPARSE_CONST
    {
        return IdSparseBool;
    }

    inline bool isScalar()
    {
        return (getRows() == 1 && getCols() == 1);
    }

    bool isTrue()
    {
        if (static_cast<int>(nbTrue()) == m_iSize)
        {
            return true;
        }
        return false;
    }

    bool neg(InternalType *& out)
    {
        SparseBool * _out = new SparseBool(getRows(), getCols());
        types::neg(getRows(), getCols(), matrixBool, _out->matrixBool);
        _out->finalize();
        out = _out;
        return true;
    }

    void whoAmI() SPARSE_CONST;

    bool* get();
    bool get(int r, int c) SPARSE_CONST;
    bool get(int _iIndex) SPARSE_CONST
    {
        return get(_iIndex % m_iRows, _iIndex / m_iRows);
    }

    SparseBool* set(int r, int c, bool b, bool _bFinalize = true) SPARSE_CONST;
    SparseBool* set(int _iIndex, bool b, bool _bFinalize = true) SPARSE_CONST
    {
        return set(_iIndex % m_iRows, _iIndex / m_iRows, b, _bFinalize);
    }

    void fill(Bool& dest, int r = 0, int c = 0) SPARSE_CONST;

    Sparse* newOnes() const;
    SparseBool* newNotEqualTo(SparseBool const&o) const;
    SparseBool* newEqualTo(SparseBool& o);

    SparseBool* newLogicalOr(SparseBool const&o) const;
    SparseBool* newLogicalAnd(SparseBool const&o) const;

    typedef Eigen::SparseMatrix<bool, 0x1, int> BoolSparse_t;
    SparseBool(BoolSparse_t* o);
    BoolSparse_t* matrixBool;

private:
    void create2(int rows, int cols, Bool SPARSE_CONST& src, Double SPARSE_CONST& idx);
    SparseBool* insert(typed_list* _pArgs, SparseBool* _pSource);
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
