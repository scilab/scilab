/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
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

#include <sstream>
#include <math.h>
#include <Eigen/Sparse>
#include <complex>
#include <iterator>
#include <algorithm>

#include <Eigen/Core>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>

#include "sparse.hxx"
#include "types.hxx"
#include "tostring_common.hxx"
#include "double.hxx"
#include "matrixiterator.hxx"
#include "types_subtraction.hxx"
#include "types_addition.hxx"
#include "types_multiplication.hxx"
#include "configvariable.hxx"
#include "scilabWrite.hxx"
#include "exp.hxx"
#include "types_tools.hxx"

#include "sparseOp.hxx"

extern "C"
{
#include "elem_common.h"
}
namespace
{

/* used for debuging output
*/
template<typename Os, typename In, typename Sz> Os& writeData(wchar_t const* title, In beg, Sz n, Os& os)
{
    os << title;
    /* TODO: use tostring_common (with a kind of std::boolalpha for boolean output)
    */
    mycopy_n(beg, n, std::ostream_iterator<typename std::iterator_traits<In>::value_type, char>(os, L" "));
    os << std::endl;
    return os;
}

struct Printer
{
    Printer (int precision) : p(precision)
    {
    }
    template<typename T>
    std::wstring emptyName( /* */) const
    {
        return L" zero";
    }

    template<typename T>
    std::wstring operator()(T const& t) const
    {
        //never call ?
        std::wostringstream ostr;
        ostr.precision(p);
        ostr << t;
        return ostr.str();
    }
    int p;
};

template<>
std::wstring Printer::operator()(bool const& b) const
{
    if (b)
    {
        return L"T";
    }
    else
    {
        return L"F";
    }
}

template<>
std::wstring Printer::operator()(double const& d) const
{
    std::wostringstream ostr;
    DoubleFormat df;
    getDoubleFormat(d, &df);
    addDoubleValue(&ostr, d, &df);
    return ostr.str();
}

template<>
std::wstring Printer::operator()(std::complex<double > const& c) const
{
    std::wostringstream ostr;
    int iLen = 0;
    DoubleFormat dfR, dfI;
    getComplexFormat(c.real(), c.imag(), &iLen, &dfR, &dfI);
    addDoubleComplexValue(&ostr, c.real(), c.imag(), iLen, &dfR, &dfI);
    return ostr.str();
}

template<>
std::wstring Printer::emptyName<bool>() const
{
    return L"False";
}


template<typename T> std::wstring toString(T const& m, int precision)
{
    std::wostringstream ostr;

    int iWidthRows  = 0;
    int iWidthCols  = 0;
    getSignedIntFormat(m.rows(), &iWidthRows);
    getSignedIntFormat(m.cols(), &iWidthCols);

    ostr << L"(" ;
    addUnsignedIntValue<unsigned long long>(&ostr, m.rows(), iWidthRows);
    ostr << ",";
    addUnsignedIntValue<unsigned long long>(&ostr, m.cols(), iWidthCols);
    ostr << L")";

    Printer p(precision);
    if (!m.nonZeros())
    {
        ostr << ( p.emptyName<typename Eigen::internal::traits<T>::Scalar>());
    }
    ostr << L" sparse matrix\n\n";

    auto * pIColPos      = m.innerIndexPtr();
    auto * pINbItemByRow = m.outerIndexPtr();

    int iPos = 0;

    for (size_t j = 1 ; j < m.rows() + 1 ; j++)
    {
        for (size_t i = pINbItemByRow[j - 1] ; i < pINbItemByRow[j] ; i++)
        {
            ostr << L"(";
            addUnsignedIntValue<unsigned long long>(&ostr, (int)j, iWidthRows);
            ostr << L",";
            addUnsignedIntValue<unsigned long long>(&ostr, pIColPos[iPos] + 1, iWidthCols);
            ostr << L")\t" << p(m.valuePtr()[iPos]) << std::endl;

            iPos++;
        }
    }

    return ostr.str();
}

/** utility function to compare two Eigen::Sparse matrices to equality
*/
template<typename T> bool equal(T const& s1, T const& s2)
{
    bool res(true);
    // only compares elts when both inner iterators are "defined", so we assert that we compared all the non zero values
    // i.e. the inner iterators where defined for the same values
    std::size_t nbElts(0);

    for (int k = 0; res && k != s1.outerSize(); ++k)
    {
        for (typename T::InnerIterator it1(s1, k), it2(s2, k); res && it1 && it2 ; ++it1, ++it2, ++nbElts)
        {
            res = (it1.value() == it2.value()
                   && it1.row() == it2.row()
                   && it1.col() == it2.col());
        }
    }
    return res && (nbElts == s1.nonZeros()) && (nbElts == s2.nonZeros());
}
/**
utility function to set non zero values of an Eigen::Sparse matrix to a fixed values
@param s : sparse matrix to modify
@param v : value to set (default to 1.)
*/
template<typename T> bool setNonZero(T& s, typename Eigen::internal::traits<T>::Scalar v = 1.)
{
    for (auto j = 0; j < s.outerSize(); ++j)
    {
        for (typename T::InnerIterator it(s, j); it; ++it)
        {
            it.valueRef() = v;
        }
    }
    return true;
}



template<typename Src, typename Sp>
void doAppend(Src SPARSE_CONST& src, int r, int c, Sp& dest)
{
    typedef typename Eigen::internal::traits<Sp>::Scalar data_t;
    mycopy_n(makeMatrixIterator<data_t>(src, makeNonZerosIterator(src)), nonZeros(src)
             , makeMatrixIterator<data_t>(dest, makeTranslatedIterator(makeNonZerosIterator(src), Coords2D(r, c))));
}

template<typename Scalar1, typename Scalar2>
void doAppend(Eigen::SparseMatrix<Scalar1, Eigen::RowMajor> SPARSE_CONST& src, int r, int c, Eigen::SparseMatrix<Scalar2, Eigen::RowMajor>& dest)
{
    typedef typename Eigen::SparseMatrix<Scalar1, Eigen::RowMajor>::InnerIterator srcIt_t;
    for (std::size_t k = 0; k != src.outerSize(); ++k)
    {
        for (srcIt_t it(src, (int)k); it; ++it)
        {
            dest.insert( it.row() + r, it.col() + c) =  it.value();
        }
    }
}
/*
Sp is an Eigen::SparseMatrix
*/
template<typename Sp, typename M>
void cwiseInPlaceProduct(Sp& sp, M SPARSE_CONST& m)
{
    // should be a transform_n() over makeNonZerosIterator(src)
    for (std::size_t k = 0; k != sp.outerSize(); ++k)
    {
        for (typename Sp::InnerIterator it(sp, k); it; ++it)
        {
            it.valueRef() *= get<typename Eigen::internal::traits<Sp>::Scalar >(m, it.row(), it.col());
        }
    }

}
}
namespace types
{

template<typename T, typename Arg>
T* create_new(Arg const& a)
{
    return 0;
}

template<>
Double* create_new(double const& d)
{
    Double* res(new Double(1, 1, false));
    res->set(0, 0, d);
    return res;
}

template<>
Double* create_new(std::complex<double>const& c)
{
    Double* res(new Double(1, 1, true));
    res->set(0, 0, c.real());
    res->setImg(0, 0, c.imag());
    return res;
}

template<>
Double* create_new(Sparse const& s)
{
    Sparse& cs(const_cast<Sparse&>(s)); // inherited member functions are not const-correct
    Double* res(new Double(cs.getRows(), cs.getCols(), cs.isComplex()));
    const_cast<Sparse&>(s).fill(*res);
    return res;
}


Sparse::~Sparse()
{
    delete matrixReal;
    delete matrixCplx;
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

Sparse::Sparse(Sparse const& src)
    : matrixReal(src.matrixReal ? new RealSparse_t(*src.matrixReal) : 0)
    , matrixCplx(src.matrixCplx ? new CplxSparse_t(*src.matrixCplx) : 0)

{
    m_iRows = const_cast<Sparse*>(&src)->getRows();
    m_iCols = const_cast<Sparse*>(&src)->getCols();
    m_iSize = m_iRows * m_iCols;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(int _iRows, int _iCols, bool cplx)
    : matrixReal(cplx ? 0 : new RealSparse_t(_iRows, _iCols))
    , matrixCplx(cplx ? new CplxSparse_t(_iRows, _iCols) : 0)
{
    m_iRows = _iRows;
    m_iCols = _iCols;
    m_iSize = _iRows * _iCols;
    m_iDims = 2;
    m_piDims[0] = _iRows;
    m_piDims[1] = _iCols;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(Double SPARSE_CONST& src)
{
    //compute idx
    int size = src.getSize();
    int row = src.getRows();
    Double* idx = new Double(src.getSize(), 2);
    double* p = idx->get();
    for (int i = 0; i < size; ++i)
    {
        p[i]        = (double)(i % row) + 1;
        p[i + size] = (double)(i / row) + 1;
    }
    create2(src.getRows(), src.getCols(), src, *idx);
    idx->killMe();
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(Double SPARSE_CONST& src, Double SPARSE_CONST& idx)
{
    int idxrow = idx.getRows();
    int rows = static_cast<int>(*std::max_element(idx.get(), idx.get() + idxrow));
    int cols = static_cast<int>(*std::max_element(idx.get() + idxrow, idx.get() + idxrow * 2));

    create2(rows, cols, src, idx);
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

Sparse::Sparse(Double SPARSE_CONST& src, Double SPARSE_CONST& idx, Double SPARSE_CONST& dims)
{
    create2(static_cast<int>(dims.get(0)), static_cast<int>(dims.get(1)), src, idx);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(RealSparse_t* realSp, CplxSparse_t* cplxSp):  matrixReal(realSp), matrixCplx(cplxSp)
{
    if (realSp)
    {
        m_iCols = realSp->cols();
        m_iRows = realSp->rows();
    }
    else
    {
        m_iCols = cplxSp->cols();
        m_iRows = cplxSp->rows();
    }
    m_iSize = m_iCols * m_iRows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;

    finalize();
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(Double SPARSE_CONST& xadj, Double SPARSE_CONST& adjncy, Double SPARSE_CONST& src, std::size_t r, std::size_t c)
{
    Adjacency a(xadj.get(), adjncy.get());
    create(static_cast<int>(r), static_cast<int>(c), src, makeIteratorFromVar(a), src.getSize());
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Sparse::Sparse(int rows, int cols, int nonzeros, int* inner, int* outer, double* real, double* img)
{
    int* out = nullptr;
    int* in = nullptr;

    if (img)
    {
        matrixCplx = new CplxSparse_t(rows, cols);
        matrixCplx->reserve((int)nonzeros);
        out = matrixCplx->outerIndexPtr();
        in = matrixCplx->innerIndexPtr();
        matrixReal = nullptr;
    }
    else
    {
        matrixReal = new RealSparse_t(rows, cols);
        matrixReal->reserve((int)nonzeros);
        out = matrixReal->outerIndexPtr();
        in = matrixReal->innerIndexPtr();
        matrixCplx = nullptr;
    }

    //update outerIndexPtr
    memcpy(out, outer, sizeof(int) * (rows + 1));
    //update innerIndexPtr
    memcpy(in, inner, sizeof(int) * nonzeros);

    if (img)
    {
        std::complex<double>* data = matrixCplx->valuePtr();
        for (int i = 0; i < nonzeros; ++i)
        {
            data[i] = std::complex<double>(real[i], img[i]);
        }
    }
    else
    {
        double* data = matrixReal->valuePtr();
        for (int i = 0; i < nonzeros; ++i)
        {
            data[i] = real[i];
        }

    }

    m_iCols = cols;
    m_iRows = rows;
    m_iSize = cols * rows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;

    matrixCplx ? matrixCplx->resizeNonZeros(nonzeros) : matrixReal->resizeNonZeros(nonzeros);
    //finalize();
}

template<typename DestIter>
void Sparse::create(int rows, int cols, Double SPARSE_CONST& src, DestIter o, std::size_t n)
{
    m_iCols = cols;
    m_iRows = rows;
    m_iSize = cols * rows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;

    if (src.isComplex())
    {
        matrixReal = 0;
        matrixCplx = new CplxSparse_t(rows, cols);
        matrixCplx->reserve((int)n);
        mycopy_n(makeMatrixIterator<std::complex<double> >(src, RowWiseFullIterator(src.getRows(), src.getCols())), n, makeMatrixIterator<std::complex<double> >(*matrixCplx, o));
    }
    else
    {
        matrixReal = new RealSparse_t(rows, cols);
        matrixReal->reserve((int)n);
        matrixCplx = 0;
        mycopy_n(makeMatrixIterator<double >(src, RowWiseFullIterator(src.getRows(), src.getCols())), n
                 , makeMatrixIterator<double>(*matrixReal, o));
    }
    finalize();
}

void Sparse::create2(int rows, int cols, Double SPARSE_CONST& src, Double SPARSE_CONST& idx)
{
    int nnz = src.getSize();
    double* i = idx.get();
    double* j = i + idx.getRows();
    double* valR = src.get();

    if (src.isComplex())
    {
        matrixReal = 0;

        typedef Eigen::Triplet<std::complex<double> > T;
        std::vector<T> tripletList;
        tripletList.reserve((int)nnz);

        double* valI = src.getImg();

        for (int k = 0; k < nnz; ++k)
        {
            tripletList.push_back(T(static_cast<int>(i[k]) - 1, static_cast<int>(j[k]) - 1, std::complex<double>(valR[k], valI[k])));
        }

        matrixCplx = new CplxSparse_t(rows, cols);
        matrixCplx->setFromTriplets(tripletList.begin(), tripletList.end());
        m_iRows = matrixCplx->rows();
        m_iCols = matrixCplx->cols();
    }
    else
    {
        matrixCplx = 0;

        typedef Eigen::Triplet<double> T;
        std::vector<T> tripletList;
        tripletList.reserve((int)nnz);

        for (int k = 0; k < nnz; ++k)
        {
            tripletList.push_back(T(static_cast<int>(i[k]) - 1, static_cast<int>(j[k]) - 1, valR[k]));
        }

        matrixReal = new RealSparse_t(rows, cols);
        matrixReal->setFromTriplets(tripletList.begin(), tripletList.end());

        m_iRows = matrixReal->rows();
        m_iCols = matrixReal->cols();
    }

    m_iSize = m_iCols * m_iRows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;
    finalize();
}

void Sparse::fill(Double& dest, int r, int c) SPARSE_CONST
{
    Sparse & cthis(const_cast<Sparse&>(*this));
    if (isComplex())
    {
        mycopy_n(makeMatrixIterator<std::complex<double> >(*matrixCplx, RowWiseFullIterator(cthis.getRows(), cthis.getCols())), cthis.getSize()
        , makeMatrixIterator<std::complex<double> >(dest, RowWiseFullIterator(dest.getRows(), dest.getCols(), r, c)));
    }
    else
    {
        mycopy_n( makeMatrixIterator<double>(*matrixReal,  RowWiseFullIterator(cthis.getRows(), cthis.getCols())), cthis.getSize()
        , makeMatrixIterator<double >(dest, RowWiseFullIterator(dest.getRows(), dest.getCols(), r, c)));
    }
}

Sparse* Sparse::set(int _iRows, int _iCols, std::complex<double> v, bool _bFinalize)
{
    if (_iRows >= getRows() || _iCols >= getCols())
    {
        return NULL;
    }

    typedef Sparse* (Sparse::*set_t)(int, int, std::complex<double>, bool);
    Sparse* pIT = checkRef(this, (set_t)&Sparse::set, _iRows, _iCols, v, _bFinalize);
    if (pIT != this)
    {
        return pIT;
    }

    if (matrixReal)
    {
        matrixReal->coeffRef(_iRows, _iCols) = v.real();
    }
    else
    {
        matrixCplx->coeffRef(_iRows, _iCols) = v;
    }

    if (_bFinalize)
    {
        finalize();
    }
    return this;
}

Sparse* Sparse::set(int _iRows, int _iCols, double _dblReal, bool _bFinalize)
{
    if (_iRows >= getRows() || _iCols >= getCols())
    {
        return NULL;
    }

    typedef Sparse* (Sparse::*set_t)(int, int, double, bool);
    Sparse* pIT = checkRef(this, (set_t)&Sparse::set, _iRows, _iCols, _dblReal, _bFinalize);
    if (pIT != this)
    {
        return pIT;
    }

    if (matrixReal)
    {
        matrixReal->coeffRef(_iRows, _iCols) = _dblReal;
    }
    else
    {
        matrixCplx->coeffRef(_iRows, _iCols) = std::complex<double>(_dblReal, 0);
    }


    if (_bFinalize)
    {
        finalize();
    }

    return this;
}

void Sparse::finalize()
{
    if (isComplex())
    {
        matrixCplx->prune(&keepForSparse<std::complex<double> >);
        matrixCplx->finalize();
    }
    else
    {
        matrixReal->prune(&keepForSparse<double>);
        matrixReal->finalize();
    }

}

bool Sparse::neg(InternalType *& out)
{
    SparseBool * _out = new SparseBool(getRows(), getCols());
    types::neg(getRows(), getCols(), matrixReal, _out->matrixBool);
    out = _out;

    return true;
}


bool Sparse::isComplex() const
{
    return static_cast<bool>(matrixCplx != NULL);
}

// TODO: should have both a bounds checking and a non-checking interface to elt access
double* Sparse::get()
{
    if (isComplex() == false)
    {
        return matrixReal->valuePtr();
    }

    return nullptr;
}

double  Sparse::get(int _iRows, int _iCols) const
{
    return getReal(_iRows, _iCols);
}

double Sparse::getReal(int _iRows, int _iCols) const
{
    double res = 0;
    if (matrixReal)
    {
        res = matrixReal->coeff(_iRows, _iCols);
    }
    else
    {
        res = matrixCplx->coeff(_iRows, _iCols).real();
    }
    return res;
}

std::complex<double>* Sparse::getImg()
{
    if (isComplex())
    {
        return matrixCplx->valuePtr();
    }

    return nullptr;
}

std::complex<double> Sparse::getImg(int _iRows, int _iCols) const
{
    std::complex<double> res;
    if (matrixCplx)
    {
        res = matrixCplx->coeff(_iRows, _iCols);
    }
    else
    {
        res = std::complex<double>(matrixReal->coeff(_iRows, _iCols), 0.);
    }

    return res;
}

void Sparse::whoAmI() SPARSE_CONST
{
    std::cout << "types::Sparse";
}

Sparse* Sparse::clone(void)
{
    return new Sparse(*this);
}

bool Sparse::zero_set()
{
    if (matrixReal)
    {
        matrixReal->setZero();
    }
    else
    {
        matrixCplx->setZero();
    }

    return true;
}

// TODO: handle precision and line length
bool Sparse::toString(std::wostringstream& ostr)
{
    int iPrecision = ConfigVariable::getFormatSize();
    std::wstring res;
    if (matrixReal)
    {
        res = ::toString(*matrixReal, iPrecision);
    }
    else
    {
        res = ::toString(*matrixCplx, iPrecision);
    }

    ostr << res;
    return true;
}

Sparse* Sparse::resize(int _iNewRows, int _iNewCols)
{
    typedef Sparse* (Sparse::*resize_t)(int, int);
    Sparse* pIT = checkRef(this, (resize_t)&Sparse::resize, _iNewRows, _iNewCols);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iNewRows <= getRows() && _iNewCols <= getCols())
    {
        //nothing to do: hence we do NOT fail
        return this;
    }

    Sparse* res = NULL;
    try
    {
        if (matrixReal)
        {
            //item count
            size_t iNonZeros = nonZeros();
            RealSparse_t *newReal = new RealSparse_t(_iNewRows, _iNewCols);
            newReal->reserve((int)iNonZeros);


            //coords
            int* pRows = new int[iNonZeros * 2];
            outputRowCol(pRows);
            int* pCols = pRows + iNonZeros;

            //values
            double* pNonZeroR = new double[iNonZeros];
            double* pNonZeroI = new double[iNonZeros];
            outputValues(pNonZeroR, pNonZeroI);

            typedef Eigen::Triplet<double> triplet;
            std::vector<triplet> tripletList;

            for (size_t i = 0 ; i < iNonZeros ; i++)
            {
                tripletList.push_back(triplet((int)pRows[i] - 1, (int)pCols[i] - 1, pNonZeroR[i]));
            }

            newReal->setFromTriplets(tripletList.begin(), tripletList.end());

            delete matrixReal;
            matrixReal = newReal;
            delete[] pRows;
            delete[] pNonZeroR;
            delete[] pNonZeroI;
        }
        else
        {
            //item count
            size_t iNonZeros = nonZeros();
            CplxSparse_t *newCplx = new CplxSparse_t(_iNewRows, _iNewCols);
            newCplx->reserve((int)iNonZeros);

            //coords
            int* pRows = new int[iNonZeros * 2];
            outputRowCol(pRows);
            int* pCols = pRows + iNonZeros;

            //values
            double* pNonZeroR = new double[iNonZeros];
            double* pNonZeroI = new double[iNonZeros];
            outputValues(pNonZeroR, pNonZeroI);

            typedef Eigen::Triplet<std::complex<double> > triplet;
            std::vector<triplet> tripletList;

            for (size_t i = 0 ; i < iNonZeros ; i++)
            {
                tripletList.push_back(triplet((int)pRows[i] - 1, (int)pCols[i] - 1, std::complex<double>(pNonZeroR[i], pNonZeroI[i])));
            }

            newCplx->setFromTriplets(tripletList.begin(), tripletList.end());


            delete matrixCplx;
            matrixCplx = newCplx;
            delete[] pRows;
            delete[] pNonZeroR;
            delete[] pNonZeroI;
        }

        m_iRows = _iNewRows;
        m_iCols = _iNewCols;
        m_iSize = _iNewRows * _iNewCols;
        m_piDims[0] = m_iRows;
        m_piDims[1] = m_iCols;

        res = this;
    }
    catch (...)
    {
        res = NULL;
    }
    return res;
}
// TODO decide if a complex matrix with 0 imag can be == to a real matrix
// not true for dense (cf double.cpp)
bool Sparse::operator==(const InternalType& it) SPARSE_CONST
{
    Sparse* otherSparse = const_cast<Sparse*>(dynamic_cast<Sparse const*>(&it));/* types::GenericType is not const-correct :( */
    Sparse & cthis (const_cast<Sparse&>(*this));

    if (otherSparse == NULL)
    {
        return false;
    }

    if (otherSparse->getRows() != cthis.getRows())
    {
        return false;
    }

    if (otherSparse->getCols() != cthis.getCols())
    {
        return false;
    }

    if (otherSparse->isComplex() != isComplex())
    {
        return false;
    }

    if (isComplex())
    {
        return equal(*matrixCplx, *otherSparse->matrixCplx);
    }
    else
    {
        return equal(*matrixReal, *otherSparse->matrixReal);
    }
}

bool Sparse::one_set()
{
    if (isComplex())
    {
        return setNonZero(*matrixCplx);
    }
    else
    {
        return setNonZero(*matrixReal);
    }
}

void Sparse::toComplex()
{
    if (!isComplex())
    {
        try
        {
            matrixCplx = new CplxSparse_t(matrixReal->cast<std::complex<double> >());
            delete matrixReal;
            matrixReal = NULL;
        }
        catch (...)
        {
            delete matrixCplx;
            matrixCplx = NULL;
            throw;
        }
    }
}

GenericType* Sparse::insertNew(typed_list* _pArgs)
{
    typed_list pArg;
    Sparse *pOut  = NULL;

    int iDims           = (int)_pArgs->size();
    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];
    bool bComplex       = isComplex();
    bool bUndefine      = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmptyDouble();
    }

    if (iSeqCount < 0)
    {
        iSeqCount = -iSeqCount;
        bUndefine = true;
    }

    if (bUndefine)
    {
        //manage : and $ in creation by insertion
        int iSource         = 0;
        int *piSourceDims   = getDimsArray();

        for (int i = 0 ; i < iDims ; i++)
        {
            if (pArg[i] == NULL)
            {
                //undefine value
                if (isScalar())
                {
                    piMaxDim[i]     = 1;
                    piCountDim[i]   = 1;
                }
                else
                {
                    piMaxDim[i]     = piSourceDims[iSource];
                    piCountDim[i]   = piSourceDims[iSource];
                }
                iSource++;
                //replace pArg value by the new one
                pArg[i] = createDoubleVector(piMaxDim[i]);
            }
            //else
            //{
            //    piMaxDim[i] = piCountDim[i];
            //}
        }
    }

    //remove last dimension at size 1
    //remove last dimension if are == 1
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piMaxDim[i] == 1)
        {
            iDims--;
            pArg.pop_back();
        }
        else
        {
            break;
        }
    }

    if (checkArgValidity(pArg) == false)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //contain bad index, like <= 0, ...
        return NULL;
    }

    if (iDims == 1)
    {
        if (getCols() == 1)
        {
            pOut = new Sparse(piCountDim[0], 1, bComplex);
        }
        else
        {
            //rows == 1
            pOut = new Sparse(1, piCountDim[0], bComplex);
        }
    }
    else
    {
        pOut = new Sparse(piMaxDim[0], piMaxDim[1], bComplex);
        //pOut = createEmpty(iDims, piMaxDim, bComplex);
    }

    //insert values in new matrix
    Sparse* pOut2 = pOut->insert(&pArg, this);
    if (pOut != pOut2)
    {
        delete pOut;
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut2;
}

Sparse* Sparse::insert(typed_list* _pArgs, InternalType* _pSource)
{
    typedef Sparse* (Sparse::*insert_t)(typed_list*, InternalType*);
    Sparse* pIT = checkRef(this, (insert_t)&Sparse::insert, _pArgs, _pSource);
    if (pIT != this)
    {
        return pIT;
    }

    if (_pSource->isSparse())
    {
        return insert(_pArgs, _pSource->getAs<Sparse>());
    }

    bool bNeedToResize  = false;
    int iDims           = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //on case of resize
    int iNewRows    = 0;
    int iNewCols    = 0;
    Double* pSource = _pSource->getAs<Double>();

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    if (iDims < 2)
    {
        //see as vector
        if (getRows() == 1 || getCols() == 1)
        {
            //vector or scalar
            if (getSize() < piMaxDim[0])
            {
                bNeedToResize = true;

                //need to enlarge sparse dimensions
                if (getCols() == 1 || getSize() == 0)
                {
                    //column vector
                    iNewRows    = piMaxDim[0];
                    iNewCols    = 1;
                }
                else if (getRows() == 1)
                {
                    //row vector
                    iNewRows    = 1;
                    iNewCols    = piMaxDim[0];
                }
            }
        }
        else if (getSize() < piMaxDim[0])
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            //out of range
            return NULL;
        }
    }
    else
    {
        if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
        {
            bNeedToResize = true;
            iNewRows = std::max(getRows(), piMaxDim[0]);
            iNewCols = std::max(getCols(), piMaxDim[1]);
        }
    }

    //check number of insertion
    if (pSource->isScalar() == false && pSource->getSize() != iSeqCount)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //now you are sure to be able to insert values
    if (bNeedToResize)
    {
        if (resize(iNewRows, iNewCols) == NULL)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    //update complexity
    if (pSource->isComplex() && isComplex() == false)
    {
        toComplex();
    }


    if (iDims == 1)
    {
        double* pIdx = pArg[0]->getAs<Double>()->get();
        for (int i = 0 ; i < iSeqCount ; i++)
        {
            int iRow = static_cast<int>(pIdx[i] - 1) % getRows();
            int iCol = static_cast<int>(pIdx[i] - 1) / getRows();
            if (pSource->isScalar())
            {
                if (pSource->isComplex())
                {
                    set(iRow, iCol, std::complex<double>(pSource->get(0), pSource->getImg(0)), false);
                }
                else
                {
                    set(iRow, iCol, pSource->get(0), false);
                }
            }
            else
            {
                if (pSource->isComplex())
                {
                    set(iRow, iCol, std::complex<double>(pSource->get(i), pSource->getImg(i)), false);
                }
                else
                {
                    set(iRow, iCol, pSource->get(i), false);
                }
            }
        }
    }
    else
    {
        double* pIdxRow = pArg[0]->getAs<Double>()->get();
        int iRowSize    = pArg[0]->getAs<Double>()->getSize();
        double* pIdxCol = pArg[1]->getAs<Double>()->get();

        for (int i = 0 ; i < iSeqCount ; i++)
        {
            if (pSource->isScalar())
            {
                if (pSource->isComplex())
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, std::complex<double>(pSource->get(0), pSource->getImg(0)), false);
                }
                else
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, pSource->get(0), false);
                }
            }
            else
            {
                int iRowOrig = i % pSource->getRows();
                int iColOrig = i / pSource->getRows();

                if (pSource->isComplex())
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, std::complex<double>(pSource->get(iRowOrig, iColOrig), pSource->getImg(iRowOrig, iColOrig)), false);
                }
                else
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, pSource->get(iRowOrig, iColOrig), false);
                }
            }
        }
    }

    finalize();

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return this;
}

Sparse* Sparse::insert(typed_list* _pArgs, Sparse* _pSource)
{
    bool bNeedToResize  = false;
    int iDims           = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //on case of resize
    int iNewRows    = 0;
    int iNewCols    = 0;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    if (iDims < 2)
    {
        //see as vector
        if (getRows() == 1 || getCols() == 1)
        {
            //vector or scalar
            bNeedToResize = true;
            if (getSize() < piMaxDim[0])
            {
                //need to enlarge sparse dimensions
                if (getCols() == 1 || getSize() == 0)
                {
                    //column vector
                    iNewRows    = piMaxDim[0];
                    iNewCols    = 1;
                }
                else if (getRows() == 1)
                {
                    //row vector
                    iNewRows    = 1;
                    iNewCols    = piMaxDim[0];
                }
            }
        }
        else if (getSize() < piMaxDim[0])
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            //out of range
            return NULL;
        }
    }
    else
    {
        if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
        {
            bNeedToResize = true;
            iNewRows = std::max(getRows(), piMaxDim[0]);
            iNewCols = std::max(getCols(), piMaxDim[1]);
        }
    }

    //check number of insertion
    if (_pSource->isScalar() == false && _pSource->getSize() != iSeqCount)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //now you are sure to be able to insert values
    if (bNeedToResize)
    {
        if (resize(iNewRows, iNewCols) == NULL)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    //update complexity
    if (_pSource->isComplex() && isComplex() == false)
    {
        toComplex();
    }

    if (iDims == 1)
    {
        double* pIdx = pArg[0]->getAs<Double>()->get();
        for (int i = 0 ; i < iSeqCount ; i++)
        {
            int iRow = static_cast<int>(pIdx[i] - 1) % getRows();
            int iCol = static_cast<int>(pIdx[i] - 1) / getRows();

            if (_pSource->isScalar())
            {
                if (_pSource->isComplex())
                {
                    set(iRow, iCol, _pSource->getImg(0, 0), false);
                }
                else
                {
                    set(iRow, iCol, _pSource->get(0, 0), false);
                }
            }
            else
            {
                int iRowOrig = i % _pSource->getRows();
                int iColOrig = i / _pSource->getRows();
                if (_pSource->isComplex())
                {
                    set(iRow, iCol, _pSource->getImg(iRowOrig, iColOrig), false);
                }
                else
                {
                    set(iRow, iCol, _pSource->get(iRowOrig, iColOrig), false);
                }
            }
        }
    }
    else
    {
        double* pIdxRow = pArg[0]->getAs<Double>()->get();
        int iRowSize    = pArg[0]->getAs<Double>()->getSize();
        double* pIdxCol = pArg[1]->getAs<Double>()->get();

        for (int i = 0 ; i < iSeqCount ; i++)
        {
            if (_pSource->isScalar())
            {
                if (_pSource->isComplex())
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->getImg(0, 0), false);
                }
                else
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->get(0, 0), false);
                }
            }
            else
            {
                int iRowOrig = i % _pSource->getRows();
                int iColOrig = i / _pSource->getRows();
                if (_pSource->isComplex())
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->getImg(iRowOrig, iColOrig), false);
                }
                else
                {
                    set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->get(iRowOrig, iColOrig), false);
                }
            }
        }
    }

    finalize();

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return this;
}

GenericType* Sparse::remove(typed_list* _pArgs)
{
    Sparse* pOut = NULL;
    int iDims = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    bool* pbFull = new bool[iDims];
    //coord must represent all values on a dimension
    for (int i = 0 ; i < iDims ; i++)
    {
        pbFull[i]       = false;
        int iDimToCheck = getVarMaxDim(i, iDims);
        int iIndexSize  = pArg[i]->getAs<GenericType>()->getSize();

        //we can have index more than once
        if (iIndexSize >= iDimToCheck)
        {
            //size is good, now check datas
            double* pIndexes = getDoubleArrayFromDouble(pArg[i]);
            for (int j = 0 ; j < iDimToCheck ; j++)
            {
                bool bFind = false;
                for (int k = 0 ; k < iIndexSize ; k++)
                {
                    if ((int)pIndexes[k] == j + 1)
                    {
                        bFind = true;
                        break;
                    }
                }
                pbFull[i]  = bFind;
            }
        }
    }

    //only one dims can be not full/entire
    bool bNotEntire = false;
    int iNotEntire  = 0;
    bool bTooMuchNotEntire = false;
    for (int i = 0 ; i < iDims ; i++)
    {
        if (pbFull[i] == false)
        {
            if (bNotEntire == false)
            {
                bNotEntire = true;
                iNotEntire = i;
            }
            else
            {
                bTooMuchNotEntire = true;
                break;
            }
        }
    }

    delete[] pbFull;

    if (bTooMuchNotEntire == true)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //find index to keep
    int iNotEntireSize          = pArg[iNotEntire]->getAs<GenericType>()->getSize();
    double* piNotEntireIndex    = getDoubleArrayFromDouble(pArg[iNotEntire]);
    int iKeepSize               = getVarMaxDim(iNotEntire, iDims);
    bool* pbKeep                = new bool[iKeepSize];

    //fill pbKeep with true value
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        pbKeep[i] = true;
    }

    for (int i = 0 ; i < iNotEntireSize ; i++)
    {
        int idx = (int)piNotEntireIndex[i] - 1;

        //don't care of value out of bounds
        if (idx < iKeepSize)
        {
            pbKeep[idx] = false;
        }
    }

    int iNewDimSize = 0;
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        if (pbKeep[i] == true)
        {
            iNewDimSize++;
        }
    }
    delete[] pbKeep;

    int* piNewDims = new int[iDims];
    for (int i = 0 ; i < iDims ; i++)
    {
        if (i == iNotEntire)
        {
            piNewDims[i] = iNewDimSize;
        }
        else
        {
            piNewDims[i] = getVarMaxDim(i, iDims);
        }
    }

    //remove last dimension if are == 1
    int iOrigDims = iDims;
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piNewDims[i] == 1)
        {
            iDims--;
        }
        else
        {
            break;
        }
    }

    if (iDims == 1)
    {
        if (iNewDimSize == 0)
        {
            delete[] piNewDims;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return new Sparse(0, 0);
        }
        else
        {
            //two cases, depends of original matrix/vector
            if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] != 1)
            {
                //special case for row vector
                pOut = new Sparse(1, iNewDimSize, isComplex());
                //in this case we have to care of 2nd dimension
                //iNotEntire = 1;
            }
            else
            {
                pOut = new Sparse(iNewDimSize, 1, isComplex());
            }
        }
    }
    else
    {
        pOut = new Sparse(piNewDims[0], piNewDims[0], isComplex());
    }

    delete[] piNewDims;
    //find a way to copy existing data to new variable ...
    int iNewPos = 0;
    int* piIndexes = new int[iOrigDims];
    int* piViewDims = new int[iOrigDims];
    for (int i = 0 ; i < iOrigDims ; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iOrigDims);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        bool bByPass = false;
        getIndexesWithDims(i, piIndexes, piViewDims, iOrigDims);

        //check if piIndexes use removed indexes
        for (int j = 0 ; j < iNotEntireSize ; j++)
        {
            if ((piNotEntireIndex[j] - 1) == piIndexes[iNotEntire])
            {
                //by pass this value
                bByPass = true;
                break;
            }
        }

        if (bByPass == false)
        {
            //compute new index
            if (isComplex())
            {
                pOut->set(iNewPos, getImg(i));
            }
            else
            {
                pOut->set(iNewPos, get(i));
            }
            iNewPos++;
        }
    }

    //free allocated data
    for (int i = 0 ; i < iDims ; i++)
    {
        if (pArg[i] != (*_pArgs)[i])
        {
            delete pArg[i];
        }
    }

    delete[] piIndexes;
    delete[] piViewDims;

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut;
}

Sparse* Sparse::append(int r, int c, types::Sparse SPARSE_CONST* src)
{
    Sparse* pIT = checkRef(this, &Sparse::append, r, c, src);
    if (pIT != this)
    {
        return pIT;
    }

    //        std::wcerr << L"to a sparse of size"<<getRows() << L","<<getCols() << L" should append @"<<r << L","<<c<< "a sparse:"<< src->toString(32,80)<<std::endl;
    if (src->isComplex())
    {
        toComplex();
    }
    if (isComplex())
    {
        if (src->isComplex())
        {
            doAppend(*(src->matrixCplx), r, c, *matrixCplx);
        }
        else
        {
            doAppend(*(src->matrixReal), r, c, *matrixCplx);
        }
    }
    else
    {
        doAppend(*(src->matrixReal), r, c, *matrixReal);
    }

    finalize();

    return this; // realloc is meaningless for sparse matrices
}

/*
* create a new Sparse of dims according to resSize and fill it from currentSparse (along coords)
*/
GenericType* Sparse::extract(typed_list* _pArgs)
{
    Sparse* pOut        = NULL;
    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        if (_pArgs->size() == 0)
        {
            //a()
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return this;
        }
        else
        {
            //a([])
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return Double::Empty();
        }
    }

    if (iDims < 2)
    {
        if (piMaxDim[0] <= getSize())
        {
            int iNewRows = 0;
            int iNewCols = 0;

            if (getRows() == 1 && getCols() != 1 && (*_pArgs)[0]->isColon() == false)
            {
                //special case for row vector
                iNewRows = 1;
                iNewCols = piCountDim[0];
            }
            else
            {
                iNewRows = piCountDim[0];
                iNewCols = 1;
            }

            pOut = new Sparse(iNewRows, iNewCols, isComplex());
            double* pIdx = pArg[0]->getAs<Double>()->get();
            for (int i = 0 ; i < iSeqCount ; i++)
            {
                if (pIdx[i] < 1)
                {
                    delete pOut;
                    pOut = NULL;
                    delete[] piMaxDim;
                    delete[] piCountDim;
                    cleanIndexesArguments(_pArgs, &pArg);
                    return NULL;
                }
                int iRowRead = static_cast<int>(pIdx[i] - 1) % getRows();
                int iColRead = static_cast<int>(pIdx[i] - 1) / getRows();

                int iRowWrite = static_cast<int>(i) % iNewRows;
                int iColWrite = static_cast<int>(i) / iNewRows;
                if (isComplex())
                {
                    std::complex<double> dbl = getImg(iRowRead, iColRead);
                    if (dbl.real() != 0 || dbl.imag() != 0)
                    {
                        //only non zero values
                        pOut->set(iRowWrite, iColWrite, dbl, false);
                    }
                }
                else
                {
                    double dbl = get(iRowRead, iColRead);
                    if (dbl != 0)
                    {
                        //only non zero values
                        pOut->set(iRowWrite, iColWrite, dbl, false);
                    }
                }
            }
        }
        else
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }
    else
    {
        if (piMaxDim[0] <= getRows() && piMaxDim[1] <= getCols())
        {
            double* pIdxRow = pArg[0]->getAs<Double>()->get();
            double* pIdxCol = pArg[1]->getAs<Double>()->get();

            int iNewRows = pArg[0]->getAs<Double>()->getSize();
            int iNewCols = pArg[1]->getAs<Double>()->getSize();

            pOut = new Sparse(iNewRows, iNewCols, isComplex());

            int iPos = 0;
            for (int iRow = 0 ; iRow < iNewRows ; iRow++)
            {
                for (int iCol = 0 ; iCol < iNewCols ; iCol++)
                {
                    if ((pIdxRow[iRow] < 1) || (pIdxCol[iCol] < 1))
                    {
                        delete pOut;
                        pOut = NULL;
                        delete[] piMaxDim;
                        delete[] piCountDim;
                        //free pArg content
                        cleanIndexesArguments(_pArgs, &pArg);
                        return NULL;
                    }
                    if (isComplex())
                    {
                        std::complex<double> dbl = getImg((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                        if (dbl.real() != 0 || dbl.imag() != 0)
                        {
                            //only non zero values
                            pOut->set(iRow, iCol, dbl, false);
                        }
                    }
                    else
                    {
                        double dbl = get((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                        if (dbl != 0)
                        {
                            //only non zero values
                            pOut->set(iRow, iCol, dbl, false);
                        }
                    }
                    iPos++;
                }
            }
        }
        else
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    pOut->finalize();

    delete[] piMaxDim;
    delete[] piCountDim;
    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut;
}

Sparse* Sparse::extract(int nbCoords, int SPARSE_CONST* coords, int SPARSE_CONST* maxCoords, int SPARSE_CONST* resSize, bool asVector) SPARSE_CONST
{
    if ( (asVector && maxCoords[0] > getSize()) ||
    (asVector == false && maxCoords[0] > getRows()) ||
    (asVector == false && maxCoords[1] > getCols()))
    {
        return 0;
    }

    bool const cplx(isComplex());
    Sparse * pSp (0);
    if (asVector)
    {
        pSp = (getRows() == 1) ?  new Sparse(1, resSize[0], cplx) : new Sparse(resSize[0], 1, cplx);
    }
    else
    {
        pSp = new Sparse(resSize[0], resSize[1], cplx);
    }
    //        std::cerr<<"extracted sparse:"<<pSp->getRows()<<", "<<pSp->getCols()<<"seqCount="<<nbCoords<<"maxDim="<<maxCoords[0] <<","<< maxCoords[1]<<std::endl;
    if (! (asVector
    ? copyToSparse(*this,  Coords<true>(coords, getRows()), nbCoords
    , *pSp, RowWiseFullIterator(pSp->getRows(), pSp->getCols()))
    : copyToSparse(*this,  Coords<false>(coords), nbCoords
    , *pSp, RowWiseFullIterator(pSp->getRows(), pSp->getCols()))))
    {
        delete pSp;
        pSp = NULL;
    }
    return pSp;
}

bool Sparse::invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
    }
    else
    {
        InternalType * _out = extract(&in);
        if (!_out)
        {
            std::wostringstream os;
            os << _W("Invalid index.\n");
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }
        out.push_back(_out);
    }

    return true;
}


bool Sparse::isInvokable() const
{
    return true;
}

bool Sparse::hasInvokeOption() const
{
    return false;
}

int Sparse::getInvokeNbIn()
{
    return -1;
}

int Sparse::getInvokeNbOut()
{
    return 1;
}

/*
coords are Scilab 1-based
extract std::make_pair(coords, asVector), rowIter
*/
template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
bool Sparse::copyToSparse(Src SPARSE_CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav)
{
    if (!(src.isComplex() || sp.isComplex()))
    {
        mycopy_n(makeMatrixIterator<double>(src, srcTrav), n
                 , makeMatrixIterator<double>(*sp.matrixReal, destTrav));
    }
    else
    {
        sp.toComplex();
        mycopy_n(makeMatrixIterator<std::complex<double> >(src, srcTrav), n
                 , makeMatrixIterator<std::complex<double> >(*sp.matrixCplx, destTrav));
    }

    sp.finalize();
    return true;
}

// GenericType because we might return a Double* for scalar operand
Sparse* Sparse::add(Sparse const& o) const
{
    RealSparse_t* realSp(0);
    CplxSparse_t* cplxSp(0);
    if (isComplex() == false && o.isComplex() == false)
    {
        //R + R -> R
        realSp = new RealSparse_t(*matrixReal + * (o.matrixReal));
    }
    else if (isComplex() == false && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >() + * (o.matrixCplx));
    }
    else if (isComplex() == true && o.isComplex() == false)
    {
        cplxSp = new CplxSparse_t(*matrixCplx + o.matrixReal->cast<std::complex<double> >());
    }
    else if (isComplex() == true && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(*matrixCplx + * (o.matrixCplx));
    }

    return new Sparse(realSp, cplxSp);
}

Sparse* Sparse::substract(Sparse const& o) const
{
    RealSparse_t* realSp(0);
    CplxSparse_t* cplxSp(0);
    if (isComplex() == false && o.isComplex() == false)
    {
        //R - R -> R
        realSp = new RealSparse_t(*matrixReal - * (o.matrixReal));
    }
    else if (isComplex() == false && o.isComplex() == true)
    {
        //R - C -> C
        cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >() - * (o.matrixCplx));
    }
    else if (isComplex() == true && o.isComplex() == false)
    {
        //C - R -> C
        cplxSp = new CplxSparse_t(*matrixCplx - o.matrixReal->cast<std::complex<double> >());
    }
    else if (isComplex() == true && o.isComplex() == true)
    {
        //C - C -> C
        cplxSp = new CplxSparse_t(*matrixCplx - * (o.matrixCplx));
    }

    return new Sparse(realSp, cplxSp);
}

Sparse* Sparse::multiply(double s) const
{
    return new Sparse( isComplex() ? 0 : new RealSparse_t((*matrixReal)*s)
                       , isComplex() ? new CplxSparse_t((*matrixCplx)*s) : 0);
}

Sparse* Sparse::multiply(std::complex<double> s) const
{
    return new Sparse( 0
                       , isComplex() ? new CplxSparse_t((*matrixCplx) * s) : new CplxSparse_t((*matrixReal) * s));
}

Sparse* Sparse::multiply(Sparse const& o) const
{
    RealSparse_t* realSp(0);
    CplxSparse_t* cplxSp(0);

    if (isComplex() == false && o.isComplex() == false)
    {
        realSp = new RealSparse_t(*matrixReal **(o.matrixReal));
    }
    else if (isComplex() == false && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >() **(o.matrixCplx));
    }
    else if (isComplex() == true && o.isComplex() == false)
    {
        cplxSp = new CplxSparse_t(*matrixCplx * o.matrixReal->cast<std::complex<double> >());
    }
    else if (isComplex() == true && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(*matrixCplx **(o.matrixCplx));
    }

    return new Sparse(realSp, cplxSp);
}

Sparse* Sparse::dotMultiply(Sparse SPARSE_CONST& o) const
{
    RealSparse_t* realSp(0);
    CplxSparse_t* cplxSp(0);
    if (isComplex() == false && o.isComplex() == false)
    {
        realSp = new RealSparse_t(matrixReal->cwiseProduct(*(o.matrixReal)));
    }
    else if (isComplex() == false && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >().cwiseProduct( *(o.matrixCplx)));
    }
    else if (isComplex() == true && o.isComplex() == false)
    {
        cplxSp = new CplxSparse_t(matrixCplx->cwiseProduct(o.matrixReal->cast<std::complex<double> >()));
    }
    else if (isComplex() == true && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixCplx->cwiseProduct(*(o.matrixCplx)));
    }

    return new Sparse(realSp, cplxSp);
}

Sparse* Sparse::dotDivide(Sparse SPARSE_CONST& o) const
{
    RealSparse_t* realSp(0);
    CplxSparse_t* cplxSp(0);
    if (isComplex() == false && o.isComplex() == false)
    {
        realSp = new RealSparse_t(matrixReal->cwiseQuotient(*(o.matrixReal)));
    }
    else if (isComplex() == false && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >().cwiseQuotient( *(o.matrixCplx)));
    }
    else if (isComplex() == true && o.isComplex() == false)
    {
        cplxSp = new CplxSparse_t(matrixCplx->cwiseQuotient(o.matrixReal->cast<std::complex<double> >()));
    }
    else if (isComplex() == true && o.isComplex() == true)
    {
        cplxSp = new CplxSparse_t(matrixCplx->cwiseQuotient(*(o.matrixCplx)));
    }

    return new Sparse(realSp, cplxSp);
}

int Sparse::newCholLLT(Sparse** _SpPermut, Sparse** _SpFactor) const
{
    typedef Eigen::SparseMatrix<double, Eigen::ColMajor> RealSparseCol_t;
    RealSparseCol_t spColMajor = RealSparseCol_t((const RealSparse_t&) * matrixReal);

    // Constructs and performs the LLT factorization of sparse
    Eigen::SimplicialLLT<RealSparseCol_t> pLLT(spColMajor);
    int iInfo = pLLT.info();
    if (iInfo != Eigen::Success)
    {
        *_SpFactor = NULL;
        *_SpPermut = NULL;
        return iInfo;
    }

    // Get the lower matrix of factorization.
    // The new RealSparse_t will be setted in Sparse without copy.
    *_SpFactor = new Sparse(new RealSparse_t(pLLT.matrixL()), NULL);

    // Get the permutation matrix.
    Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic, int> p = pLLT.permutationP();
    *_SpPermut = new Sparse(p.rows(), p.cols());
    for (int i = 0; i < p.rows(); i++)
    {
        (*_SpPermut)->set(i, p.indices()[i], 1, false);
    }

    (*_SpPermut)->finalize();

    return iInfo;
}

bool Sparse::transpose(InternalType *& out)
{
    out = new Sparse(matrixReal ? new RealSparse_t(matrixReal->transpose()) : 0, matrixCplx ? new CplxSparse_t(matrixCplx->transpose()) : 0);
    return true;
}

bool Sparse::adjoint(InternalType *& out)
{
    out = new Sparse(matrixReal ? new RealSparse_t(matrixReal->adjoint()) : 0, matrixCplx ? new CplxSparse_t(matrixCplx->adjoint()) : 0);
    return true;
}

struct BoolCast
{
    BoolCast(std::complex<double> const& c): b(c.real() || c.imag()) {}
    operator bool () const
    {
        return b;
    }
    operator double() const
    {
        return b ? 1. : 0.;
    }
    bool b;
};
Sparse* Sparse::newOnes() const
{
    // result is never cplx
    return new Sparse( matrixReal
                       ? new RealSparse_t(matrixReal->cast<bool>().cast<double>())
                       : new RealSparse_t(matrixCplx->cast<BoolCast>().cast<double>())
                       , 0);
}

struct RealCast
{
    RealCast(std::complex<double> const& c): b(c.real()) {}
    operator bool () const
    {
        return b != 0;
    }
    operator double() const
    {
        return b;
    }
    double b;
};
Sparse* Sparse::newReal() const
{
    return new Sparse( matrixReal
                       ? matrixReal
                       : new RealSparse_t(matrixCplx->cast<RealCast>().cast<double>())
                       , 0);
}

std::size_t Sparse::nonZeros() const
{
    if (isComplex())
    {
        return matrixCplx->nonZeros();
    }
    else
    {
        return matrixReal->nonZeros();
    }
}
std::size_t Sparse::nonZeros(std::size_t r) const
{
    std::size_t res;
    if (matrixReal)
    {
        int* piIndex = matrixReal->outerIndexPtr();
        res = piIndex[r + 1] - piIndex[r];
    }
    else
    {
        int* piIndex = matrixCplx->outerIndexPtr();
        res = piIndex[r + 1] - piIndex[r];
    }

    return res;
}

int* Sparse::getNbItemByRow(int* _piNbItemByRows)
{
    int* piNbItemByCols = new int[getRows() + 1];
    if (isComplex())
    {
        mycopy_n(matrixCplx->outerIndexPtr(), getRows() + 1, piNbItemByCols);
    }
    else
    {
        mycopy_n(matrixReal->outerIndexPtr(), getRows() + 1, piNbItemByCols);
    }

    for (int i = 0 ; i < getRows() ; i++)
    {
        _piNbItemByRows[i] = piNbItemByCols[i + 1] - piNbItemByCols[i];
    }

    delete[] piNbItemByCols;
    return _piNbItemByRows;
}

int* Sparse::getColPos(int* _piColPos)
{
    if (isComplex())
    {
        mycopy_n(matrixCplx->innerIndexPtr(), nonZeros(), _piColPos);
    }
    else
    {
        mycopy_n(matrixReal->innerIndexPtr(), nonZeros(), _piColPos);
    }

    for (size_t i = 0; i < nonZeros(); i++)
    {
        _piColPos[i]++;
    }

    return _piColPos;
}

int* Sparse::getInnerPtr(int* count)
{
    int* ret = nullptr;
    if (isComplex())
    {
        ret = matrixCplx->innerIndexPtr();
        *count = matrixCplx->innerSize();
    }
    else
    {
        ret = matrixReal->innerIndexPtr();
        *count = matrixReal->innerSize();
    }

    return ret;
}

int* Sparse::getOuterPtr(int* count)
{
    int* ret = nullptr;
    if (isComplex())
    {
        ret = matrixCplx->outerIndexPtr();
        *count = matrixCplx->outerSize();
    }
    else
    {
        ret = matrixReal->outerIndexPtr();
        *count = matrixReal->outerSize();
    }

    return ret;
}

namespace
{
template<typename S> struct GetReal: std::unary_function<typename S::InnerIterator, double>
{
    double operator()(typename S::InnerIterator it) const
    {
        return it.value();
    }
};
template<> struct GetReal< Eigen::SparseMatrix<std::complex<double >, Eigen::RowMajor > >
        : std::unary_function<Sparse::CplxSparse_t::InnerIterator, double>
{
    double operator()( Sparse::CplxSparse_t::InnerIterator it) const
    {
        return it.value().real();
    }
};
template<typename S> struct GetImag: std::unary_function<typename S::InnerIterator, double>
{
    double operator()(typename S::InnerIterator it) const
    {
        return it.value().imag();
    }
};
template<typename S> struct GetRow: std::unary_function<typename S::InnerIterator, int>
{
    int operator()(typename S::InnerIterator it) const
    {
        return it.row() + 1;
    }
};
template<typename S> struct GetCol: std::unary_function<typename S::InnerIterator, int>
{
    int operator()(typename S::InnerIterator it) const
    {
        return it.col() + 1;
    }
};

template<typename S, typename Out, typename F> Out sparseTransform(S& s, Out o, F f)
{
    for (std::size_t k(0); k < s.outerSize(); ++k)
    {
        for (typename S::InnerIterator it(s, (int)k); it; ++it, ++o)
        {
            *o = f(it);
        }
    }
    return o;
}
}

std::pair<double*, double*> Sparse::outputValues(double* outReal, double* outImag)const
{
    return matrixReal
           ? std::make_pair(sparseTransform(*matrixReal, outReal, GetReal<RealSparse_t>()), outImag)
           : std::make_pair(sparseTransform(*matrixCplx, outReal, GetReal<CplxSparse_t>())
                            , sparseTransform(*matrixCplx, outImag, GetImag<CplxSparse_t>()));
}

int* Sparse::outputRowCol(int* out)const
{
    return matrixReal
           ? sparseTransform(*matrixReal, sparseTransform(*matrixReal, out, GetRow<RealSparse_t>()), GetCol<RealSparse_t>())
           : sparseTransform(*matrixCplx, sparseTransform(*matrixCplx, out, GetRow<CplxSparse_t>()), GetCol<CplxSparse_t>());
}
double* Sparse::outputCols(double* out) const
{
    if (isComplex())
    {
        mycopy_n(matrixCplx->innerIndexPtr(), nonZeros(), out);
    }
    else
    {
        mycopy_n(matrixReal->innerIndexPtr(), nonZeros(), out);
    }

    return std::transform(out, out, out, std::bind2nd(std::plus<double>(), 1));

}

void Sparse::opposite(void)
{
    if (isComplex())
    {
        std::complex<double>* data = matrixCplx->valuePtr();
        std::transform(data, data + matrixCplx->nonZeros(), data, std::negate<std::complex<double> >());
    }
    else
    {
        double* data = matrixReal->valuePtr();
        std::transform(data, data + matrixReal->nonZeros(), data, std::negate<double>());
    }
}

SparseBool* Sparse::newLessThan(Sparse &o)
{
    //only real values !

    //return cwiseOp<std::less>(*this, o);
    int rowL = getRows();
    int colL = getCols();

    int rowR = o.getRows();
    int colR = o.getCols();
    int row = std::max(rowL, rowR);
    int col = std::max(colL, colR);

    //create a boolean sparse matrix with dims of sparses
    types::SparseBool* ret = new types::SparseBool(row, col);
    if (isScalar() && o.isScalar())
    {
        double l = get(0, 0);
        double r = o.get(0, 0);
        ret->set(0, 0, l < r, false);
    }
    else if (isScalar())
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());

        //compare all items of R with R[0]
        double l = get(0, 0);
        if (l < 0)
        {
            //set true
            ret->setTrue(false);
        }

        for (int i = 0; i < nnzR; ++i)
        {
            double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l < r, false);
        }
    }
    else if (o.isScalar())
    {
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());

        double r = o.get(0, 0);
        if (r > 0)
        {
            ret->setTrue(true);
        }

        for (int i = 0; i < nnzL; ++i)
        {
            double l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l < r, false);
        }
    }
    else
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());
        //set all values to %t
        ret->setFalse(false);

        for (int i = 0; i < nnzL; ++i)
        {
            double l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l < 0, false);
        }
        ret->finalize();

        //set _pR[i] == _pL[i] for each _pR values
        for (int i = 0; i < nnzR; ++i)
        {
            //get l and r following non zeros value of R
            double l = get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            //set value following non zeros value of R
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l < r, false);
        }
    }

    ret->finalize();
    return ret;
}

SparseBool* Sparse::newNotEqualTo(Sparse const&o) const
{
    return cwiseOp<std::not_equal_to>(*this, o);
}

SparseBool* Sparse::newLessOrEqual(Sparse &o)
{
    //only real values !

    //return cwiseOp<std::less>(*this, o);
    int rowL = getRows();
    int colL = getCols();

    int rowR = o.getRows();
    int colR = o.getCols();
    int row = std::max(rowL, rowR);
    int col = std::max(colL, colR);

    //create a boolean sparse matrix with dims of sparses
    types::SparseBool* ret = new types::SparseBool(row, col);
    if (isScalar() && o.isScalar())
    {
        double l = get(0, 0);
        double r = o.get(0, 0);
        ret->set(0, 0, l <= r, false);
    }
    else if (isScalar())
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());

        //compare all items of R with R[0]
        double l = get(0, 0);
        if (l <= 0)
        {
            //set true
            ret->setTrue(false);
        }

        for (int i = 0; i < nnzR; ++i)
        {
            double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l <= r, false);
        }
    }
    else if (o.isScalar())
    {
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());

        double r = o.get(0, 0);
        if (r > 0)
        {
            ret->setTrue(true);
        }

        for (int i = 0; i < nnzL; ++i)
        {
            double l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l <= r, false);
        }
    }
    else
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());
        //set all values to %t
        ret->setTrue(false);

        for (int i = 0; i < nnzL; ++i)
        {
            double l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l <= 0, false);
        }
        ret->finalize();

        //set _pR[i] == _pL[i] for each _pR values
        for (int i = 0; i < nnzR; ++i)
        {
            //get l and r following non zeros value of R
            double l = get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            //set value following non zeros value of R
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l <= r, false);
        }
    }

    ret->finalize();
    return ret;
}

SparseBool* Sparse::newEqualTo(Sparse &o)
{
    int rowL = getRows();
    int colL = getCols();

    int rowR = o.getRows();
    int colR = o.getCols();
    int row = std::max(rowL, rowR);
    int col = std::max(colL, colR);

    //create a boolean sparse matrix with dims of sparses
    types::SparseBool* ret = new types::SparseBool(row, col);
    if (isScalar() && o.isScalar())
    {
        if (isComplex() || o.isComplex())
        {
            std::complex<double> l = getImg(0, 0);
            std::complex<double> r = o.getImg(0, 0);
            ret->set(0, 0, l == r, false);
        }
        else
        {
            double l = get(0, 0);
            double r = o.get(0, 0);
            ret->set(0, 0, l == r, false);
        }
    }
    else if (isScalar())
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());

        //compare all items of R with R[0]
        if (isComplex() || o.isComplex())
        {
            std::complex<double> l = getImg(0, 0);
            for (int i = 0; i < nnzR; ++i)
            {
                std::complex<double> r = o.getImg(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
            }
        }
        else
        {
            double l = get(0, 0);
            for (int i = 0; i < nnzR; ++i)
            {
                double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
            }
        }
    }
    else if (o.isScalar())
    {
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());

        if (isComplex() || o.isComplex())
        {
            std::complex<double> r = o.getImg(0, 0);
            for (int i = 0; i < nnzL; ++i)
            {
                std::complex<double> l = getImg(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
                ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l == r, false);
            }
        }
        else
        {
            double r = get(0, 0);
            for (int i = 0; i < nnzL; ++i)
            {
                double l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
                ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l == r, false);
            }
        }
    }
    else
    {
        int nnzR = static_cast<int>(o.nonZeros());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());
        int nnzL = static_cast<int>(nonZeros());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());
        //set all values to %t
        ret->setTrue(false);
        //set %f in each pL values
        for (int i = 0; i < nnzL; ++i)
        {
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, false, false);
        }
        ret->finalize();

        //set _pR[i] == _pL[i] for each _pR values
        if (isComplex() || o.isComplex())
        {
            for (int i = 0; i < nnzR; ++i)
            {
                //get l and r following non zeros value of R
                std::complex<double> l = getImg(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                std::complex<double> r = o.getImg(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                //set value following non zeros value of R
                ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
            }
        }
        else
        {
            for (int i = 0; i < nnzR; ++i)
            {
                //get l and r following non zeros value of R
                double l = get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                double r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
                //set value following non zeros value of R
                ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
            }
        }
    }

    ret->finalize();
    return ret;
}

Sparse* Sparse::reshape(int* _piDims, int _iDims)
{
    Sparse* pSp = NULL;
    int iCols = 1;

    if (_iDims == 2)
    {
        iCols = _piDims[1];
    }

    if (_iDims <= 2)
    {
        pSp = reshape(_piDims[0], iCols);
    }

    return pSp;
}

Sparse* Sparse::reshape(int _iNewRows, int _iNewCols)
{
    typedef Sparse* (Sparse::*reshape_t)(int, int);
    Sparse* pIT = checkRef(this, (reshape_t)&Sparse::reshape, _iNewRows, _iNewCols);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iNewRows * _iNewCols != getRows() * getCols())
    {
        return NULL;
    }

    Sparse* res = NULL;
    try
    {
        if (matrixReal)
        {
            //item count
            size_t iNonZeros = nonZeros();
            RealSparse_t *newReal = new RealSparse_t(_iNewRows, _iNewCols);
            newReal->reserve((int)iNonZeros);

            //coords
            int* pRows = new int[iNonZeros * 2];
            outputRowCol(pRows);
            int* pCols = pRows + iNonZeros;

            //values
            double* pNonZeroR = new double[iNonZeros];
            double* pNonZeroI = new double[iNonZeros];
            outputValues(pNonZeroR, pNonZeroI);

            typedef Eigen::Triplet<double> triplet;
            std::vector<triplet> tripletList;

            for (size_t i = 0 ; i < iNonZeros ; i++)
            {
                int iCurrentPos = ((int)pCols[i] - 1) * getRows() + ((int)pRows[i] - 1);
                tripletList.push_back(triplet((int)(iCurrentPos % _iNewRows), (int)(iCurrentPos / _iNewRows), pNonZeroR[i]));
            }

            newReal->setFromTriplets(tripletList.begin(), tripletList.end());

            delete matrixReal;
            matrixReal = newReal;
            delete[] pRows;
            delete[] pNonZeroR;
            delete[] pNonZeroI;
        }
        else
        {
            //item count
            size_t iNonZeros = nonZeros();
            CplxSparse_t *newCplx = new CplxSparse_t(_iNewRows, _iNewCols);
            newCplx->reserve((int)iNonZeros);

            //coords
            int* pRows = new int[iNonZeros * 2];
            outputRowCol(pRows);
            int* pCols = pRows + iNonZeros;

            //values
            double* pNonZeroR = new double[iNonZeros];
            double* pNonZeroI = new double[iNonZeros];
            outputValues(pNonZeroR, pNonZeroI);

            typedef Eigen::Triplet<std::complex<double> > triplet;
            std::vector<triplet> tripletList;

            for (size_t i = 0 ; i < iNonZeros ; i++)
            {
                int iCurrentPos = ((int)pCols[i] - 1) * getRows() + ((int)pRows[i] - 1);
                tripletList.push_back(triplet((int)(iCurrentPos % _iNewRows), (int)(iCurrentPos / _iNewRows), std::complex<double>(pNonZeroR[i], pNonZeroI[i])));
            }

            newCplx->setFromTriplets(tripletList.begin(), tripletList.end());

            delete matrixCplx;
            matrixCplx = newCplx;
            delete[] pRows;
            delete[] pNonZeroR;
            delete[] pNonZeroI;
        }

        m_iRows = _iNewRows;
        m_iCols = _iNewCols;
        m_iSize = _iNewRows * _iNewCols;

        m_iDims = 2;
        m_piDims[0] = m_iRows;
        m_piDims[1] = m_iCols;

        finalize();

        res = this;
    }
    catch (...)
    {
        res = NULL;
    }
    return res;
}

//    SparseBool* SparseBool::new

SparseBool::SparseBool(Bool SPARSE_CONST& src)
{
    //compute idx
    int size = src.getSize();
    int row = src.getRows();
    Double* idx = new Double(src.getSize(), 2);
    double* p = idx->get();
    for (int i = 0; i < size; ++i)
    {
        p[i] = (double)(i % row) + 1;
        p[i + size] = (double)(i / row) + 1;
    }
    create2(src.getRows(), src.getCols(), src, *idx);
    idx->killMe();
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}
/* @param src : Bool matrix to copy into a new sparse matrix
@param idx : Double matrix to use as indexes to get values from the src
**/
SparseBool::SparseBool(Bool SPARSE_CONST& src, Double SPARSE_CONST& idx)
{
    int idxrow = idx.getRows();
    int rows = static_cast<int>(*std::max_element(idx.get(), idx.get() + idxrow));
    int cols = static_cast<int>(*std::max_element(idx.get() + idxrow, idx.get() + idxrow * 2));
    create2(rows, cols, src, idx);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

/* @param src : Bool matrix to copy into a new sparse matrix
@param idx : Double matrix to use as indexes to get values from the src
@param dims : Double matrix containing the dimensions of the new matrix
**/
SparseBool::SparseBool(Bool SPARSE_CONST& src, Double SPARSE_CONST& idx, Double SPARSE_CONST& dims)
{
    create2(static_cast<int>(dims.get(0)), static_cast<int>(dims.get(1)), src, idx);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SparseBool::SparseBool(int _iRows, int _iCols) : matrixBool(new BoolSparse_t(_iRows, _iCols))
{
    m_iRows = _iRows;
    m_iCols = _iCols;
    m_iSize = _iRows * _iCols;
    m_iDims = 2;
    m_piDims[0] = _iRows;
    m_piDims[1] = _iCols;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SparseBool::SparseBool(SparseBool const& src) : matrixBool(new BoolSparse_t(*src.matrixBool))
{
    m_iDims = 2;
    m_iRows = const_cast<SparseBool*>(&src)->getRows();
    m_iCols = const_cast<SparseBool*>(&src)->getCols();
    m_iSize = m_iRows * m_iCols;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SparseBool::SparseBool(BoolSparse_t* src) : matrixBool(src)
{
    m_iRows = src->rows();
    m_iCols = src->cols();
    m_iSize = m_iRows * m_iCols;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SparseBool::SparseBool(int rows, int cols, int trues, int* inner, int* outer)
{
    int* out = nullptr;
    int* in = nullptr;

    matrixBool = new BoolSparse_t(rows, cols);
    matrixBool->reserve((int)trues);
    out = matrixBool->outerIndexPtr();
    in = matrixBool->innerIndexPtr();

    //update outerIndexPtr
    memcpy(out, outer, sizeof(int) * (rows + 1));
    //update innerIndexPtr
    memcpy(in, inner, sizeof(int) * trues);

    bool* data = matrixBool->valuePtr();
    for (int i = 0; i < trues; ++i)
    {
        data[i] = true;
    }

    m_iCols = cols;
    m_iRows = rows;
    m_iSize = cols * rows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;

    matrixBool->resizeNonZeros(trues);
}

void SparseBool::create2(int rows, int cols, Bool SPARSE_CONST& src, Double SPARSE_CONST& idx)
{
    int nnz = src.getSize();
    double* i = idx.get();
    double* j = i + idx.getRows();
    int* val = src.get();

    typedef Eigen::Triplet<bool> T;
    std::vector<T> tripletList;
    tripletList.reserve((int)nnz);

    for (int k = 0; k < nnz; ++k)
    {
        tripletList.push_back(T(static_cast<int>(i[k]) - 1, static_cast<int>(j[k]) - 1, val[k] == 1));
    }

    matrixBool = new BoolSparse_t(rows, cols);
    matrixBool->setFromTriplets(tripletList.begin(), tripletList.end());

    m_iRows = matrixBool->rows();
    m_iCols = matrixBool->cols();
    m_iSize = cols * rows;
    m_iDims = 2;
    m_piDims[0] = m_iRows;
    m_piDims[1] = m_iCols;
    finalize();
}

SparseBool::~SparseBool()
{
    delete matrixBool;
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

bool SparseBool::toString(std::wostringstream& ostr)
{
    ostr << ::toString(*matrixBool, 0);
    return true;
}

void SparseBool::whoAmI() SPARSE_CONST
{
    std::cout << "types::SparseBool";
}

SparseBool* SparseBool::clone(void)
{
    return new SparseBool(*this);
}

SparseBool* SparseBool::resize(int _iNewRows, int _iNewCols)
{
    typedef SparseBool* (SparseBool::*resize_t)(int, int);
    SparseBool* pIT = checkRef(this, (resize_t)&SparseBool::resize, _iNewRows, _iNewCols);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iNewRows <= getRows() && _iNewCols <= getCols())
    {
        //nothing to do: hence we do NOT fail
        return this;
    }

    SparseBool* res = NULL;
    try
    {
        //item count
        size_t iNonZeros = nbTrue();

        BoolSparse_t *newBool = new BoolSparse_t(_iNewRows, _iNewCols);
        newBool->reserve((int)iNonZeros);

        //coords
        int* pRows = new int[iNonZeros * 2];
        outputRowCol(pRows);
        int* pCols = pRows + iNonZeros;

        typedef Eigen::Triplet<bool> triplet;
        std::vector<triplet> tripletList;

        for (size_t i = 0 ; i < iNonZeros ; i++)
        {
            tripletList.push_back(triplet((int)pRows[i] - 1, (int)pCols[i] - 1, true));
        }

        newBool->setFromTriplets(tripletList.begin(), tripletList.end());

        delete matrixBool;
        matrixBool = newBool;
        delete[] pRows;

        m_iRows = _iNewRows;
        m_iCols = _iNewCols;
        m_iSize = _iNewRows * _iNewCols;
        m_piDims[0] = m_iRows;
        m_piDims[1] = m_iCols;

        res = this;
    }
    catch (...)
    {
        res = NULL;
    }
    return res;
}

SparseBool* SparseBool::insert(typed_list* _pArgs, SparseBool* _pSource)
{
    bool bNeedToResize  = false;
    int iDims           = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //on case of resize
    int iNewRows    = 0;
    int iNewCols    = 0;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    if (iDims < 2)
    {
        //see as vector
        if (getRows() == 1 || getCols() == 1)
        {
            //vector or scalar
            if (getSize() < piMaxDim[0])
            {
                bNeedToResize = true;

                //need to enlarge sparse dimensions
                if (getCols() == 1 || getSize() == 0)
                {
                    //column vector
                    iNewRows    = piMaxDim[0];
                    iNewCols    = 1;
                }
                else if (getRows() == 1)
                {
                    //row vector
                    iNewRows    = 1;
                    iNewCols    = piMaxDim[0];
                }
            }
        }
        else if (getSize() < piMaxDim[0])
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            //out of range
            return NULL;
        }
    }
    else
    {
        if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
        {
            bNeedToResize = true;
            iNewRows = std::max(getRows(), piMaxDim[0]);
            iNewCols = std::max(getCols(), piMaxDim[1]);
        }
    }

    //check number of insertion
    if (_pSource->isScalar() == false && _pSource->getSize() != iSeqCount)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //now you are sure to be able to insert values
    if (bNeedToResize)
    {
        if (resize(iNewRows, iNewCols) == NULL)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    if (iDims == 1)
    {
        double* pIdx = pArg[0]->getAs<Double>()->get();
        for (int i = 0 ; i < iSeqCount ; i++)
        {
            int iRow = static_cast<int>(pIdx[i] - 1) % getRows();
            int iCol = static_cast<int>(pIdx[i] - 1) / getRows();

            if (_pSource->isScalar())
            {
                set(iRow, iCol, _pSource->get(0, 0), false);
            }
            else
            {
                int iRowOrig = i % _pSource->getRows();
                int iColOrig = i / _pSource->getRows();
                set(iRow, iCol, _pSource->get(iRowOrig, iColOrig), false);
            }
        }
    }
    else
    {
        double* pIdxRow = pArg[0]->getAs<Double>()->get();
        int iRowSize    = pArg[0]->getAs<Double>()->getSize();
        double* pIdxCol = pArg[1]->getAs<Double>()->get();

        for (int i = 0 ; i < iSeqCount ; i++)
        {
            if (_pSource->isScalar())
            {
                set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->get(0, 0), false);
            }
            else
            {
                int iRowOrig = i % _pSource->getRows();
                int iColOrig = i / _pSource->getRows();
                set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, _pSource->get(iRowOrig, iColOrig), false);
            }
        }
    }

    finalize();

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return this;
}

SparseBool* SparseBool::insert(typed_list* _pArgs, InternalType* _pSource)
{
    typedef SparseBool* (SparseBool::*insert_t)(typed_list*, InternalType*);
    SparseBool* pIT = checkRef(this, (insert_t)&SparseBool::insert, _pArgs, _pSource);
    if (pIT != this)
    {
        return pIT;
    }

    if (_pSource->isSparseBool())
    {
        return insert(_pArgs, _pSource->getAs<SparseBool>());
    }

    bool bNeedToResize  = false;
    int iDims           = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //on case of resize
    int iNewRows    = 0;
    int iNewCols    = 0;
    Bool* pSource = _pSource->getAs<Bool>();

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    if (iDims < 2)
    {
        //see as vector
        if (getRows() == 1 || getCols() == 1)
        {
            //vector or scalar
            bNeedToResize = true;
            if (getSize() < piMaxDim[0])
            {
                //need to enlarge sparse dimensions
                if (getCols() == 1 || getSize() == 0)
                {
                    //column vector
                    iNewRows    = piMaxDim[0];
                    iNewCols    = 1;
                }
                else if (getRows() == 1)
                {
                    //row vector
                    iNewRows    = 1;
                    iNewCols    = piMaxDim[0];
                }
            }
        }
        else if (getSize() < piMaxDim[0])
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            //out of range
            return NULL;
        }
    }
    else
    {
        if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
        {
            bNeedToResize = true;
            iNewRows = std::max(getRows(), piMaxDim[0]);
            iNewCols = std::max(getCols(), piMaxDim[1]);
        }
    }

    //check number of insertion
    if (pSource->isScalar() == false && pSource->getSize() != iSeqCount)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //now you are sure to be able to insert values
    if (bNeedToResize)
    {
        if (resize(iNewRows, iNewCols) == NULL)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    if (iDims == 1)
    {
        double* pIdx = pArg[0]->getAs<Double>()->get();
        for (int i = 0 ; i < iSeqCount ; i++)
        {
            int iRow = static_cast<int>(pIdx[i] - 1) % getRows();
            int iCol = static_cast<int>(pIdx[i] - 1) / getRows();
            if (pSource->isScalar())
            {
                set(iRow, iCol, pSource->get(0) != 0, false);
            }
            else
            {
                set(iRow, iCol, pSource->get(i) != 0, false);
            }
        }
    }
    else
    {
        double* pIdxRow = pArg[0]->getAs<Double>()->get();
        int iRowSize    = pArg[0]->getAs<Double>()->getSize();
        double* pIdxCol = pArg[1]->getAs<Double>()->get();

        for (int i = 0 ; i < iSeqCount ; i++)
        {
            if (pSource->isScalar())
            {
                set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, pSource->get(0) != 0, false);
            }
            else
            {
                int iRowOrig = i % pSource->getRows();
                int iColOrig = i / pSource->getRows();

                set((int)pIdxRow[i % iRowSize] - 1, (int)pIdxCol[i / iRowSize] - 1, pSource->get(iRowOrig, iColOrig) != 0, false);
            }
        }
    }

    finalize();

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);
    return this;
}

GenericType* SparseBool::remove(typed_list* _pArgs)
{
    SparseBool* pOut = NULL;
    int iDims = (int)_pArgs->size();
    if (iDims > 2)
    {
        //sparse are only in 2 dims
        return NULL;
    }

    typed_list pArg;

    int piMaxDim[2];
    int piCountDim[2];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return this;
    }

    bool* pbFull = new bool[iDims];
    //coord must represent all values on a dimension
    for (int i = 0 ; i < iDims ; i++)
    {
        pbFull[i]       = false;
        int iDimToCheck = getVarMaxDim(i, iDims);
        int iIndexSize  = pArg[i]->getAs<GenericType>()->getSize();

        //we can have index more than once
        if (iIndexSize >= iDimToCheck)
        {
            //size is good, now check datas
            double* pIndexes = getDoubleArrayFromDouble(pArg[i]);
            for (int j = 0 ; j < iDimToCheck ; j++)
            {
                bool bFind = false;
                for (int k = 0 ; k < iIndexSize ; k++)
                {
                    if ((int)pIndexes[k] == j + 1)
                    {
                        bFind = true;
                        break;
                    }
                }
                pbFull[i]  = bFind;
            }
        }
    }

    //only one dims can be not full/entire
    bool bNotEntire = false;
    int iNotEntire  = 0;
    bool bTooMuchNotEntire = false;
    for (int i = 0 ; i < iDims ; i++)
    {
        if (pbFull[i] == false)
        {
            if (bNotEntire == false)
            {
                bNotEntire = true;
                iNotEntire = i;
            }
            else
            {
                bTooMuchNotEntire = true;
                break;
            }
        }
    }

    delete[] pbFull;

    if (bTooMuchNotEntire == true)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    //find index to keep
    int iNotEntireSize          = pArg[iNotEntire]->getAs<GenericType>()->getSize();
    double* piNotEntireIndex    = getDoubleArrayFromDouble(pArg[iNotEntire]);
    int iKeepSize               = getVarMaxDim(iNotEntire, iDims);
    bool* pbKeep                = new bool[iKeepSize];

    //fill pbKeep with true value
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        pbKeep[i] = true;
    }

    for (int i = 0 ; i < iNotEntireSize ; i++)
    {
        int idx = (int)piNotEntireIndex[i] - 1;

        //don't care of value out of bounds
        if (idx < iKeepSize)
        {
            pbKeep[idx] = false;
        }
    }

    int iNewDimSize = 0;
    for (int i = 0 ; i < iKeepSize ; i++)
    {
        if (pbKeep[i] == true)
        {
            iNewDimSize++;
        }
    }
    delete[] pbKeep;

    int* piNewDims = new int[iDims];
    for (int i = 0 ; i < iDims ; i++)
    {
        if (i == iNotEntire)
        {
            piNewDims[i] = iNewDimSize;
        }
        else
        {
            piNewDims[i] = getVarMaxDim(i, iDims);
        }
    }

    //remove last dimension if are == 1
    int iOrigDims = iDims;
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piNewDims[i] == 1)
        {
            iDims--;
        }
        else
        {
            break;
        }
    }

    if (iDims == 1)
    {
        if (iNewDimSize == 0)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return new SparseBool(0, 0);
        }
        else
        {
            //two cases, depends of original matrix/vector
            if ((*_pArgs)[0]->isColon() == false && m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] != 1)
            {
                //special case for row vector
                pOut = new SparseBool(1, iNewDimSize);
                //in this case we have to care of 2nd dimension
                //iNotEntire = 1;
            }
            else
            {
                pOut = new SparseBool(iNewDimSize, 1);
            }
        }
    }
    else
    {
        pOut = new SparseBool(piNewDims[0], piNewDims[0]);
    }

    delete[] piNewDims;
    //find a way to copy existing data to new variable ...
    int iNewPos = 0;
    int* piIndexes = new int[iOrigDims];
    int* piViewDims = new int[iOrigDims];
    for (int i = 0 ; i < iOrigDims ; i++)
    {
        piViewDims[i] = getVarMaxDim(i, iOrigDims);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        bool bByPass = false;
        getIndexesWithDims(i, piIndexes, piViewDims, iOrigDims);

        //check if piIndexes use removed indexes
        for (int j = 0 ; j < iNotEntireSize ; j++)
        {
            if ((piNotEntireIndex[j] - 1) == piIndexes[iNotEntire])
            {
                //by pass this value
                bByPass = true;
                break;
            }
        }

        if (bByPass == false)
        {
            //compute new index
            pOut->set(iNewPos, get(i));
            iNewPos++;
        }
    }

    //free allocated data
    for (int i = 0 ; i < iDims ; i++)
    {
        if (pArg[i] != (*_pArgs)[i])
        {
            delete pArg[i];
        }
    }

    delete[] piIndexes;
    delete[] piViewDims;

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut;
}

SparseBool* SparseBool::append(int r, int c, SparseBool SPARSE_CONST* src)
{
    SparseBool* pIT = checkRef(this, &SparseBool::append, r, c, src);
    if (pIT != this)
    {
        return pIT;
    }

    doAppend(*src, r, c, *matrixBool);
    finalize();
    return this;
}

GenericType* SparseBool::insertNew(typed_list* _pArgs)
{
    typed_list pArg;
    SparseBool *pOut  = NULL;

    int iDims           = (int)_pArgs->size();
    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];
    bool bUndefine      = false;

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return createEmptyDouble();
    }

    if (iSeqCount < 0)
    {
        iSeqCount = -iSeqCount;
        bUndefine = true;
    }

    if (bUndefine)
    {
        //manage : and $ in creation by insertion
        int iSource         = 0;
        int *piSourceDims   = getDimsArray();

        for (int i = 0 ; i < iDims ; i++)
        {
            if (pArg[i] == NULL)
            {
                //undefine value
                if (isScalar())
                {
                    piMaxDim[i]     = 1;
                    piCountDim[i]   = 1;
                }
                else
                {
                    piMaxDim[i]     = piSourceDims[iSource];
                    piCountDim[i]   = piSourceDims[iSource];
                }
                iSource++;
                //replace pArg value by the new one
                pArg[i] = createDoubleVector(piMaxDim[i]);
            }
            //else
            //{
            //    piMaxDim[i] = piCountDim[i];
            //}
        }
    }

    //remove last dimension at size 1
    //remove last dimension if are == 1
    for (int i = (iDims - 1) ; i >= 2 ; i--)
    {
        if (piMaxDim[i] == 1)
        {
            iDims--;
            pArg.pop_back();
        }
        else
        {
            break;
        }
    }

    if (checkArgValidity(pArg) == false)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //contain bad index, like <= 0, ...
        return NULL;
    }

    if (iDims == 1)
    {
        if (getCols() == 1)
        {
            pOut = new SparseBool(piCountDim[0], 1);
        }
        else
        {
            //rows == 1
            pOut = new SparseBool(1, piCountDim[0]);
        }
    }
    else
    {
        pOut = new SparseBool(piMaxDim[0], piMaxDim[1]);
    }

    //insert values in new matrix
    SparseBool* pOut2 = pOut->insert(&pArg, this);
    if (pOut != pOut2)
    {
        delete pOut;
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut2;
}

SparseBool* SparseBool::extract(int nbCoords, int SPARSE_CONST* coords, int SPARSE_CONST* maxCoords, int SPARSE_CONST* resSize, bool asVector) SPARSE_CONST
{
    if ( (asVector && maxCoords[0] > getSize()) ||
    (asVector == false && maxCoords[0] > getRows()) ||
    (asVector == false && maxCoords[1] > getCols()))
    {
        return 0;
    }

    SparseBool * pSp (0);
    if (asVector)
    {
        pSp = (getRows() == 1) ?  new SparseBool(1, resSize[0]) : new SparseBool(resSize[0], 1);
        mycopy_n(makeMatrixIterator<bool>(*this,  Coords<true>(coords, getRows())), nbCoords
        , makeMatrixIterator<bool>(*(pSp->matrixBool), RowWiseFullIterator(pSp->getRows(), pSp->getCols())));
    }
    else
    {
        pSp = new SparseBool(resSize[0], resSize[1]);
        mycopy_n(makeMatrixIterator<bool>(*this,  Coords<false>(coords, getRows())), nbCoords
        , makeMatrixIterator<bool>(*(pSp->matrixBool), RowWiseFullIterator(pSp->getRows(), pSp->getCols())));

    }
    return pSp;
}

/*
* create a new SparseBool of dims according to resSize and fill it from currentSparseBool (along coords)
*/
GenericType* SparseBool::extract(typed_list* _pArgs)
{
    SparseBool* pOut    = NULL;
    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        if (_pArgs->size() == 0)
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //a()
            return this;
        }
        else
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //a([])
            return Double::Empty();
        }
    }

    if (iDims < 2)
    {
        // Check that we stay inside the input size.
        if (piMaxDim[0] <= getSize())
        {
            int iNewRows = 0;
            int iNewCols = 0;

            if (getRows() == 1 && getCols() != 1 && (*_pArgs)[0]->isColon() == false)
            {
                //special case for row vector
                iNewRows = 1;
                iNewCols = piCountDim[0];
            }
            else
            {
                iNewRows = piCountDim[0];
                iNewCols = 1;
            }

            pOut = new SparseBool(iNewRows, iNewCols);
            double* pIdx = pArg[0]->getAs<Double>()->get();
            // Write in output all elements extract from input.
            for (int i = 0 ; i < iSeqCount ; i++)
            {
                if (pIdx[i] < 1)
                {
                    delete pOut;
                    pOut = NULL;
                    break;
                }
                int iRowRead = static_cast<int>(pIdx[i] - 1) % getRows();
                int iColRead = static_cast<int>(pIdx[i] - 1) / getRows();

                int iRowWrite = static_cast<int>(i) % iNewRows;
                int iColWrite = static_cast<int>(i) / iNewRows;

                bool bValue = get(iRowRead, iColRead);
                if (bValue)
                {
                    //only non zero values
                    pOut->set(iRowWrite, iColWrite, true, false);
                }
            }
        }
        else
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }
    else
    {
        // Check that we stay inside the input size.
        if (piMaxDim[0] <= getRows() && piMaxDim[1] <= getCols())
        {
            double* pIdxRow = pArg[0]->getAs<Double>()->get();
            double* pIdxCol = pArg[1]->getAs<Double>()->get();

            int iNewRows = pArg[0]->getAs<Double>()->getSize();
            int iNewCols = pArg[1]->getAs<Double>()->getSize();

            pOut = new SparseBool(iNewRows, iNewCols);

            int iPos = 0;
            // Write in output all elements extract from input.
            for (int iRow = 0 ; iRow < iNewRows ; iRow++)
            {
                for (int iCol = 0 ; iCol < iNewCols ; iCol++)
                {
                    if ((pIdxRow[iRow] < 1) || (pIdxCol[iCol] < 1))
                    {
                        delete pOut;
                        pOut = NULL;
                        delete[] piMaxDim;
                        delete[] piCountDim;
                        //free pArg content
                        cleanIndexesArguments(_pArgs, &pArg);
                        return NULL;
                    }
                    bool bValue = get((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                    if (bValue)
                    {
                        //only non zero values
                        pOut->set(iRow, iCol, true, false);
                    }
                    iPos++;
                }
            }
        }
        else
        {
            delete[] piMaxDim;
            delete[] piCountDim;
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            return NULL;
        }
    }

    finalize();

    delete[] piMaxDim;
    delete[] piCountDim;
    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return pOut;
}

bool SparseBool::invoke(typed_list & in, optional_list &/*opt*/, int /*_iRetCount*/, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
    }
    else
    {
        InternalType * _out = extract(&in);
        if (!_out)
        {
            std::wostringstream os;
            os << _W("Invalid index.\n");
            throw ast::InternalError(os.str(), 999, e.getLocation());
        }
        out.push_back(_out);
    }

    return true;
}

bool SparseBool::isInvokable() const
{
    return true;
}

bool SparseBool::hasInvokeOption() const
{
    return false;
}

int SparseBool::getInvokeNbIn()
{
    return -1;
}

int SparseBool::getInvokeNbOut()
{
    return 1;
}

std::size_t SparseBool::nbTrue() const
{
    return  matrixBool->nonZeros() ;
}
std::size_t SparseBool::nbTrue(std::size_t r) const
{
    int* piIndex = matrixBool->outerIndexPtr();
    return piIndex[r + 1] - piIndex[r];
}


void SparseBool::setTrue(bool finalize)
{
    int rows = getRows();
    int cols = getCols();

    typedef Eigen::Triplet<bool> triplet;
    std::vector<triplet> tripletList;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            tripletList.push_back(triplet(i, j, true));
        }
    }

    matrixBool->setFromTriplets(tripletList.begin(), tripletList.end());

    if (finalize)
    {
        matrixBool->finalize();
    }
}

void SparseBool::setFalse(bool finalize)
{
    int rows = getRows();
    int cols = getCols();

    typedef Eigen::Triplet<bool> triplet;
    std::vector<triplet> tripletList;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            tripletList.push_back(triplet(i, j, false));
        }
    }

    matrixBool->setFromTriplets(tripletList.begin(), tripletList.end());

    if (finalize)
    {
        matrixBool->finalize();
    }
}

int* SparseBool::getNbItemByRow(int* _piNbItemByRows)
{
    int* piNbItemByRows = new int[getRows() + 1];
    mycopy_n(matrixBool->outerIndexPtr(), getRows() + 1, piNbItemByRows);

    for (int i = 0 ; i < getRows() ; i++)
    {
        _piNbItemByRows[i] = piNbItemByRows[i + 1] - piNbItemByRows[i];
    }

    delete[] piNbItemByRows;
    return _piNbItemByRows;
}

int* SparseBool::getColPos(int* _piColPos)
{
    mycopy_n(matrixBool->innerIndexPtr(), nbTrue(), _piColPos);
    for (size_t i = 0; i < nbTrue(); i++)
    {
        _piColPos[i]++;
    }

    return _piColPos;
}

int* SparseBool::outputRowCol(int* out)const
{
    return sparseTransform(*matrixBool, sparseTransform(*matrixBool, out, GetRow<BoolSparse_t>()), GetCol<BoolSparse_t>());
}

int* SparseBool::getInnerPtr(int* count)
{
    *count = matrixBool->innerSize();
    return matrixBool->innerIndexPtr();
}

int* SparseBool::getOuterPtr(int* count)
{
    *count = matrixBool->outerSize();
    return matrixBool->outerIndexPtr();
}


bool SparseBool::operator==(const InternalType& it) SPARSE_CONST
{
    SparseBool* otherSparse = const_cast<SparseBool*>(dynamic_cast<SparseBool const*>(&it));/* types::GenericType is not const-correct :( */
    return (otherSparse
    && (otherSparse->getRows() == getRows())
    && (otherSparse->getCols() == getCols())
    && equal(*matrixBool, *otherSparse->matrixBool));
}

bool SparseBool::operator!=(const InternalType& it) SPARSE_CONST
{
    return !(*this == it);
}

void SparseBool::finalize()
{
    matrixBool->prune(&keepForSparse<bool>);
    matrixBool->finalize();
}

bool SparseBool::get(int r, int c) SPARSE_CONST
{
    return matrixBool->coeff(r, c);
}

SparseBool* SparseBool::set(int _iRows, int _iCols, bool _bVal, bool _bFinalize) SPARSE_CONST
{
    typedef SparseBool* (SparseBool::*set_t)(int, int, bool, bool);
    SparseBool* pIT = checkRef(this, (set_t)&SparseBool::set, _iRows, _iCols, _bVal, _bFinalize);
    if (pIT != this)
    {
        return pIT;
    }

    matrixBool->coeffRef(_iRows, _iCols) = _bVal;

    if (_bFinalize)
    {
        finalize();
    }

    return this;
}

void SparseBool::fill(Bool& dest, int r, int c) SPARSE_CONST
{
    mycopy_n(makeMatrixIterator<bool >(*matrixBool, RowWiseFullIterator(getRows(), getCols())), getSize()
    , makeMatrixIterator<bool >(dest, RowWiseFullIterator(dest.getRows(), dest.getCols(), r, c)));
}

Sparse* SparseBool::newOnes() const
{
    return new Sparse(new types::Sparse::RealSparse_t(matrixBool->cast<double>()), 0);
}

SparseBool* SparseBool::newNotEqualTo(SparseBool const&o) const
{
    return cwiseOp<std::not_equal_to>(*this, o);
}

SparseBool* SparseBool::newEqualTo(SparseBool& o)
{
    int rowL = getRows();
    int colL = getCols();

    int rowR = o.getRows();
    int colR = o.getCols();
    int row = std::max(rowL, rowR);
    int col = std::max(colL, colR);

    //create a boolean sparse matrix with dims of sparses
    types::SparseBool* ret = new types::SparseBool(row, col);

    if (isScalar() && o.isScalar())
    {
        bool l = get(0, 0);
        bool r = o.get(0, 0);
        ret->set(0, 0, l == r, false);
    }
    else if (isScalar())
    {
        int nnzR = static_cast<int>(o.nbTrue());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());

        //compare all items of R with R[0]
        bool l = get(0, 0);
        for (int i = 0; i < nnzR; ++i)
        {
            bool r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
        }
    }
    else if (o.isScalar())
    {
        int nnzL = static_cast<int>(nbTrue());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());

        bool r = get(0, 0);
        for (int i = 0; i < nnzL; ++i)
        {
            bool l = get(rowcolL[i] - 1, rowcolL[i + nnzL] - 1);
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, l == r, false);
        }
    }
    else
    {
        int nnzR = static_cast<int>(o.nbTrue());
        std::vector<int> rowcolR(nnzR * 2, 0);
        o.outputRowCol(rowcolR.data());
        int nnzL = static_cast<int>(nbTrue());
        std::vector<int> rowcolL(nnzL * 2, 0);
        outputRowCol(rowcolL.data());
        //set all values to %t
        ret->setTrue(false);
        //set %f in each pL values
        for (int i = 0; i < nnzL; ++i)
        {
            ret->set(rowcolL[i] - 1, rowcolL[i + nnzL] - 1, false, false);
        }
        ret->finalize();

        //set _pR[i] == _pL[i] for each _pR values
        for (int i = 0; i < nnzR; ++i)
        {
            //get l and r following non zeros value of R
            bool l = get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            bool r = o.get(rowcolR[i] - 1, rowcolR[i + nnzR] - 1);
            //set value following non zeros value of R
            ret->set(rowcolR[i] - 1, rowcolR[i + nnzR] - 1, l == r, false);
        }
    }

    ret->finalize();
    return ret;
}

SparseBool* SparseBool::newLogicalOr(SparseBool const&o) const
{
    return cwiseOp<std::logical_or>(*this, o);
}

SparseBool* SparseBool::newLogicalAnd(SparseBool const&o) const
{
    return cwiseOp<std::logical_and>(*this, o);
}

SparseBool* SparseBool::reshape(int* _piDims, int _iDims)
{
    SparseBool* pSpBool = NULL;
    int iCols = 1;

    if (_iDims == 2)
    {
        iCols = _piDims[1];
    }

    if (_iDims <= 2)
    {
        pSpBool = reshape(_piDims[0], iCols);
    }

    return pSpBool;
}

SparseBool* SparseBool::reshape(int _iNewRows, int _iNewCols)
{
    typedef SparseBool* (SparseBool::*reshape_t)(int, int);
    SparseBool* pIT = checkRef(this, (reshape_t)&SparseBool::reshape, _iNewRows, _iNewCols);
    if (pIT != this)
    {
        return pIT;
    }

    if (_iNewRows * _iNewCols != getRows() * getCols())
    {
        return NULL;
    }

    SparseBool* res = NULL;
    try
    {
        //item count
        size_t iNonZeros = matrixBool->nonZeros();
        BoolSparse_t *newBool = new BoolSparse_t(_iNewRows, _iNewCols);
        newBool->reserve((int)iNonZeros);

        //coords
        int* pRows = new int[iNonZeros * 2];
        outputRowCol(pRows);
        int* pCols = pRows + iNonZeros;

        typedef Eigen::Triplet<bool> triplet;
        std::vector<triplet> tripletList;

        for (size_t i = 0 ; i < iNonZeros ; i++)
        {
            int iCurrentPos = ((int)pCols[i] - 1) * getRows() + ((int)pRows[i] - 1);
            tripletList.push_back(triplet((int)(iCurrentPos % _iNewRows), (int)(iCurrentPos / _iNewRows), true));
        }

        newBool->setFromTriplets(tripletList.begin(), tripletList.end());

        delete matrixBool;
        matrixBool = newBool;
        delete[] pRows;

        m_iRows = _iNewRows;
        m_iCols = _iNewCols;
        m_iSize = _iNewRows * _iNewCols;

        m_iDims = 2;
        m_piDims[0] = m_iRows;
        m_piDims[1] = m_iCols;

        finalize();

        res = this;
    }
    catch (...)
    {
        res = NULL;
    }
    return res;
}

bool SparseBool::transpose(InternalType *& out)
{
    out = new SparseBool(new BoolSparse_t(matrixBool->transpose()));
    return true;
}

template<typename T>
void neg(const int r, const int c, const T * const in, Eigen::SparseMatrix<bool, 1> * const out)
{
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            out->coeffRef(i, j) = !in->coeff(i, j);
        }
    }

    out->prune(&keepForSparse<bool>);
    out->finalize();
}


}
