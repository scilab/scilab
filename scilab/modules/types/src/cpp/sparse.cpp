/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include <math.h>
#include "sparse.hxx"
#include <complex>
#include <iterator>
#include <algorithm>
#include "types.hxx"
#include "tostring_common.hxx"
#include "double.hxx"
#include "matrixiterator.hxx"
#include "types_substraction.hxx"
#include "types_addition.hxx"
#include "types_multiplication.hxx"
#include "formatmode.h"

#include "sparseOp.hxx"
extern "C"
{
#include "elem_common.h"
}
void dbg()
{
    volatile int a = 1;
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
        int iWidth      = 0;
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

        const typename Eigen::internal::traits<T>::Index* pInner = m.innerIndexPtr();
        const typename Eigen::internal::traits<T>::Index* pOuter = m.outerIndexPtr();

        int iRow = 0;
        int iCol = 0;

        for (size_t j = 0 ; j < m.rows() ; j++)
        {
            iRow = j;
            for (size_t i = 0 ; i < m.nonZeros() ; i++)
            {
                if (pInner[i] == j)
                {
                    //good row
                    for (size_t k = 0 ; k < m.outerSize() + 1; k++)
                    {
                        if (pOuter[k] > i)
                        {
                            iCol = k;
                            break;
                        }
                    }

                    ostr << L"(";
                    addUnsignedIntValue<unsigned long long>(&ostr, iRow + 1, iWidthRows);
                    ostr << L",";
                    addUnsignedIntValue<unsigned long long>(&ostr, iCol, iWidthCols);
                    ostr << L")\t" << p(m.valuePtr()[i]) << std::endl;
                }
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
        for (typename Eigen::internal::traits<T>::Index j = 0; j < s.outerSize(); ++j)
        {
            for (typename T::InnerIterator it(s, j); it; ++it)
            {
                it.valueRef() = v;
            }
        }
        return true;
    }



    template<typename Src, typename Sp>
    void doAppend(Src CONST& src, int r, int c, Sp& dest)
    {
        typedef typename Eigen::internal::traits<Sp>::Scalar data_t;
        mycopy_n(makeMatrixIterator<data_t>(src, makeNonZerosIterator(src)), nonZeros(src)
            , makeMatrixIterator<data_t>(dest, makeTranslatedIterator(makeNonZerosIterator(src), Coords2D(r, c))));
    }

    // TODO : awaiting ggael's response to bug for [sp, sp]
    template<typename Scalar1, typename Scalar2>
    void doAppend(Eigen::SparseMatrix<Scalar1> CONST& src, int r, int c, Eigen::SparseMatrix<Scalar2>& dest)
    {
        typedef typename Eigen::SparseMatrix<Scalar1>::InnerIterator srcIt_t;
        typedef Eigen::SparseMatrix<Scalar2> dest_t;
        for (std::size_t k = 0; k != src.outerSize(); ++k)
        {
            for (srcIt_t it(src, k); it; ++it)
            {
                dest.insert( it.row() + r, it.col() + c) =  it.value();
            }
        }
    }
    /*
    Sp is an Eigen::SparseMatrix
    */
    template<typename Sp, typename M>
    void cwiseInPlaceProduct(Sp& sp, M CONST& m)
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
    }

    Sparse::Sparse( Sparse const& src) : GenericType(src)
        , matrixReal(src.matrixReal ? new RealSparse_t(*src.matrixReal) : 0)
        , matrixCplx(src.matrixCplx ? new CplxSparse_t(*src.matrixCplx) : 0)

    {
        m_iDims = 2;
        m_piDims[0] = const_cast<Sparse*>(&src)->getRows();
        m_piDims[1] = const_cast<Sparse*>(&src)->getCols();
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
    }

    Sparse::Sparse(Double CONST& src)
    {
        create(src.getRows(), src.getCols(), src, RowWiseFullIterator(src.getRows(), src.getCols()), src.getSize());
    }

    Sparse::Sparse(Double CONST& src, Double CONST& idx)
    {
        double CONST* const endOfRow(idx.getReal() + idx.getRows());
        create( static_cast<int>(*std::max_element(idx.getReal(), endOfRow))
            , static_cast<int>(*std::max_element(endOfRow, endOfRow + idx.getRows()))
            , src, makeIteratorFromVar(idx), idx.getSize() / 2 );
    }

    Sparse::Sparse(Double CONST& src, Double CONST& idx, Double CONST& dims)
    {
        create(static_cast<int>(dims.getReal(0, 0))
            , static_cast<int>(dims.getReal(0, 1))
            , src, makeIteratorFromVar(idx), idx.getSize() / 2);
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
    }

    Sparse::Sparse(Double CONST& xadj, Double CONST& adjncy, Double CONST& src, std::size_t r, std::size_t c)
    {
        Adjacency a(xadj.getReal(), adjncy.getReal());
        create(static_cast<int>(r), static_cast<int>(c), src, makeIteratorFromVar(a), src.getSize());
    }

    template<typename DestIter>
    void Sparse::create(int rows, int cols, Double CONST& src, DestIter o, std::size_t n)
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
            matrixCplx =  new CplxSparse_t( rows, cols);
            matrixCplx->reserve(n);
            mycopy_n(makeMatrixIterator<std::complex<double> >(src, RowWiseFullIterator(src.getRows(), src.getCols())), n, makeMatrixIterator<std::complex<double> >(*matrixCplx, o));
        }
        else
        {
            matrixReal = new RealSparse_t(rows, cols);
            matrixReal->reserve(n);
            matrixCplx =  0;
            mycopy_n(makeMatrixIterator<double >(src,  RowWiseFullIterator(src.getRows(), src.getCols())), n
                , makeMatrixIterator<double>(*matrixReal, o));
        }
        finalize();
    }

    void Sparse::fill(Double& dest, int r, int c) CONST
    {
        Sparse& cthis(const_cast<Sparse&>(*this));
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

    bool Sparse::set(int _iRows, int _iCols, std::complex<double> v)
    {
        if (_iRows >= getRows() || _iCols >= getCols())
        {
            return false;
        }

        if (matrixReal)
        {
            double val = matrixReal->coeff(_iRows, _iCols);
            if (val == 0)
            {
                matrixReal->insert(_iRows, _iCols) = v.real();
            }
            else
            {
                matrixReal->coeffRef(_iRows, _iCols) = v.real();
            }
        }
        else
        {
            std::complex<double> val = matrixCplx->coeff(_iRows, _iCols);
            if (val == std::complex<double>(0, 0))
            {
                matrixCplx->insert(_iRows, _iCols) = v;
            }
            else
            {
                matrixCplx->coeffRef(_iRows, _iCols) = v;
            }
        }

        finalize();
        return true;
    }

    bool Sparse::set(int _iRows, int _iCols, double _dblReal)
    {
        if (_iRows >= getRows() || _iCols >= getCols())
        {
            return false;
        }

        if (matrixReal)
        {
            double val = matrixReal->coeff(_iRows, _iCols);
            if (val == 0)
            {
                matrixReal->insert(_iRows, _iCols) = _dblReal;
            }
            else
            {
                matrixReal->coeffRef(_iRows, _iCols) = _dblReal;
            }
        }
        else
        {
            std::complex<double> val = matrixCplx->coeff(_iRows, _iCols);
            if (val == std::complex<double>(0, 0))
            {
                matrixCplx->insert(_iRows, _iCols) = std::complex<double>(_dblReal, 0);
            }
            else
            {
                matrixCplx->coeffRef(_iRows, _iCols) = std::complex<double>(_dblReal, 0);
            }
        }

        finalize();
        return true;
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

    bool Sparse::isComplex() const
    {
        return static_cast<bool>(matrixCplx != NULL);
    }

    // TODO: should have both a bounds checking and a non-checking interface to elt access
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

    void Sparse::whoAmI() CONST
    {
        std::cout << "types::Sparse";
    }

    Sparse* Sparse::clone(void) const
    {
        return new Sparse(*this);
    }

    GenericType::RealType Sparse::getType(void) CONST
    {
        return RealSparse;
    }

    bool Sparse::zero_set()
    {
        bool res = false;
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
    bool Sparse::toString(std::wostringstream& ostr) const
    {
        int iPrecision = getFormatSize();
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

    bool Sparse::resize(int _iNewRows, int _iNewCols)
    {
        if (_iNewRows <= getRows() && _iNewCols <= getCols())
        {
            //nothing to do: hence we do NOT fail
            return true;
        }

        bool res = false;
        try
        {
            if (matrixReal)
            {
                RealSparse_t *newReal = new RealSparse_t(_iNewRows, _iNewCols);

                //item count
                size_t iNonZeros = nonZeros();

                //coords
                double* pRows = new double[iNonZeros * 2];
                outputRowCol(pRows);
                double* pCols = pRows + iNonZeros;

                //values
                double* pNonZeroR = new double[iNonZeros];
                double* pNonZeroI = new double[iNonZeros];
                outputValues(pNonZeroR, pNonZeroI);

                for (size_t i = 0 ; i < iNonZeros ; i++)
                {
                    newReal->insert((int)pRows[i] - 1, (int)pCols[i] - 1) = pNonZeroR[i];
                }

                delete matrixReal;
                matrixReal = newReal;
            }
            else
            {
                CplxSparse_t *newCplx = new CplxSparse_t(_iNewRows, _iNewCols);

                //item count
                size_t iNonZeros = nonZeros();

                //coords
                double* pRows = new double[iNonZeros * 2];
                outputRowCol(pRows);
                double* pCols = pRows + iNonZeros;

                //values
                double* pNonZeroR = new double[iNonZeros];
                double* pNonZeroI = new double[iNonZeros];
                outputValues(pNonZeroR, pNonZeroI);

                for (size_t i = 0 ; i < iNonZeros ; i++)
                {
                    newCplx->insert((int)pRows[i] - 1, (int)pCols[i] - 1) = std::complex<double>(pNonZeroR[i], pNonZeroI[i]);
                }

                delete matrixCplx;
                matrixCplx = newCplx;
            }

            m_iRows = _iNewRows;
            m_iCols = _iNewCols;
            m_iSize = _iNewRows * _iNewCols;
            res = true;
        }
        catch (...)
        {
            res = false;
        }
        return res;
    }
    // TODO decide if a complex matrix with 0 imag can be == to a real matrix
    // not true for dense (cf double.cpp)
    bool Sparse::operator==(const InternalType& it) CONST
    {
        Sparse* otherSparse = const_cast<Sparse*>(dynamic_cast<Sparse const*>(&it));/* types::GenericType is not const-correct :( */
        Sparse& cthis (const_cast<Sparse&>(*this));

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

    InternalType* Sparse::insertNew(typed_list* _pArgs, InternalType* _pSource)
    {
        typed_list pArg;
        InternalType *pOut  = NULL;
        Sparse* pSource = _pSource->getAs<Sparse>();

        int iDims           = (int)_pArgs->size();
        int* piMaxDim       = new int[iDims];
        int* piCountDim     = new int[iDims];
        bool bComplex       = pSource->isComplex();
        bool bUndefine      = false;

        //evaluate each argument and replace by appropriate value and compute the count of combinations
        int iSeqCount = checkIndexesArguments(NULL, _pArgs, &pArg, piMaxDim, piCountDim);
        if(iSeqCount == 0)
        {
            return createEmptyDouble();
        }

        if(iSeqCount < 0)
        {
            iSeqCount = -iSeqCount;
            bUndefine = true;
        }

        if(bUndefine)
        {//manage : and $ in creation by insertion
            int iSource         = 0;
            int *piSourceDims   = pSource->getDimsArray();

            for(int i = 0 ; i < iDims ; i++)
            {
                if(pArg[i] == NULL)
                {//undefine value
                    if(pSource->isScalar())
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
        for(int i = (iDims - 1) ; i >= 2 ; i--)
        {
            if(piMaxDim[i] == 1)
            {
                iDims--;
                pArg.pop_back();
            }
            else
            {
                break;
            }
        }

        if(checkArgValidity(pArg) == false)
        {//contain bad index, like <= 0, ...
            return NULL;
        }

        if(iDims == 1)
        {
            if(pSource->getCols() == 1)
            {
                pOut = new Sparse(piCountDim[0], 1, bComplex);
            }
            else
            {//rows == 1
                pOut = new Sparse(1, piCountDim[0], bComplex);
            }
        }
        else
        {
            pOut = new Sparse(piMaxDim[0], piMaxDim[1], bComplex);
            //pOut = pSource->createEmpty(iDims, piMaxDim, bComplex);
        }

        //fill with null item
        Sparse* pSpOut = pOut->getAs<Sparse>();

        //insert values in new matrix
        InternalType* pOut2 = pSpOut->insert(&pArg, pSource);
        if(pOut != pOut2)
        {
            delete pOut;
        }

        return pOut2;
    }

    Sparse* Sparse::insert(typed_list* _pArgs, InternalType* _pSource)
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
        Double* pSource = _pSource->getAs<Double>();

        //evaluate each argument and replace by appropriate value and compute the count of combinations
        int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
        if (iSeqCount == 0)
        {
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
                //out of range
                return NULL;
            }
        }
        else
        {
            if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
            {
                bNeedToResize = true;
                iNewRows = Max(getRows(), piMaxDim[0]);
                iNewCols = Max(getCols(), piMaxDim[1]);
            }
        }

        //check number of insertion
        if (pSource->isScalar() == false && pSource->getSize() != iSeqCount)
        {
            return NULL;
        }

        //now you are sure to be able to insert values
        if (bNeedToResize)
        {
            if (resize(iNewRows, iNewCols) == false)
            {
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
                        set(iRow, iCol, std::complex<double>(pSource->get(0), pSource->getImg(0)));
                    }
                    else
                    {
                        set(iRow, iCol, pSource->get(0));
                    }
                }
                else
                {
                    if (pSource->isComplex())
                    {
                        set(iRow, iCol, std::complex<double>(pSource->get(i), pSource->getImg(i)));
                    }
                    else
                    {
                        set(iRow, iCol, pSource->get(i));
                    }
                }
            }
        }
        else
        {
            double* pIdxRow = pArg[0]->getAs<Double>()->get();
            double* pIdxCol = pArg[1]->getAs<Double>()->get();
            for (int i = 0 ; i < iSeqCount ; i++)
            {
                if (pSource->isScalar())
                {
                    if (pSource->isComplex())
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, std::complex<double>(pSource->get(0), pSource->getImg(0)));
                    }
                    else
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, pSource->get(0));
                    }
                }
                else
                {
                    if (pSource->isComplex())
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, std::complex<double>(pSource->get(i), pSource->getImg(i)));
                    }
                    else
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, pSource->get(i));
                    }
                }
            }
        }

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
                //out of range
                return NULL;
            }
        }
        else
        {
            if (piMaxDim[0] > getRows() || piMaxDim[1] > getCols())
            {
                bNeedToResize = true;
                iNewRows = Max(getRows(), piMaxDim[0]);
                iNewCols = Max(getCols(), piMaxDim[1]);
            }
        }

        //check number of insertion
        if (_pSource->isScalar() == false && _pSource->getSize() != iSeqCount)
        {
            return NULL;
        }

        //now you are sure to be able to insert values
        if (bNeedToResize)
        {
            if (resize(iNewRows, iNewCols) == false)
            {
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
            int idx = 0;
            for (int i = 0 ; i < iSeqCount ; i++)
            {
                int iRow = static_cast<int>(pIdx[i] - 1) % getRows();
                int iCol = static_cast<int>(pIdx[i] - 1) / getRows();

                if (_pSource->isScalar())
                {
                    if (_pSource->isComplex())
                    {
                        set(iRow, iCol, _pSource->getImg(0, 0));
                    }
                    else
                    {
                        set(iRow, iCol, _pSource->get(0, 0));
                    }
                }
                else
                {
                    int iRowOrig = idx % _pSource->getRows();
                    int iColOrig = idx / _pSource->getRows();
                    if (_pSource->isComplex())
                    {
                        set(iRow, iCol, _pSource->getImg(iRowOrig, iColOrig));
                    }
                    else
                    {
                        set(iRow, iCol, _pSource->get(iRowOrig, iColOrig));
                    }
                }
                idx++;
            }
        }
        else
        {
            double* pIdxRow = pArg[0]->getAs<Double>()->get();
            double* pIdxCol = pArg[1]->getAs<Double>()->get();
            int idx = 0;
            for (int i = 0 ; i < iSeqCount ; i++)
            {
                if (_pSource->isScalar())
                {
                    if (_pSource->isComplex())
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, _pSource->getImg(0, 0));
                    }
                    else
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, _pSource->get(0, 0));
                    }
                }
                else
                {
                    int iRowOrig = idx % _pSource->getRows();
                    int iColOrig = idx / _pSource->getRows();
                    if (_pSource->isComplex())
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, _pSource->getImg(iRowOrig, iColOrig));
                    }
                    else
                    {
                        set((int)pIdxRow[i % 2] - 1, (int)pIdxCol[i / 2] - 1, _pSource->get(iRowOrig, iColOrig));
                    }
                }
                idx++;
            }
        }

        return this;
    }

    bool Sparse::append(int r, int c, types::Sparse CONST* src)
    {
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

        return true; // realloc is meaningless for sparse matrices
    }

    /*
    * create a new Sparse of dims according to resSize and fill it from currentSparse (along coords)
    */
    InternalType* Sparse::extract(typed_list* _pArgs)
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
            if (_pArgs->size() == 0)
            {
                //a()
                return this;
            }
            else
            {
                //a([])
                return Double::Empty();
            }
        }

        if (iDims < 2)
        {
            if (piMaxDim[0] <= getSize())
            {
                int iNewRows = Max(pArg[0]->getAs<Double>()->getRows(), pArg[0]->getAs<Double>()->getCols());
                int iNewCols = 1;

                pOut = new Sparse(iNewRows, iNewCols, isComplex());
                double* pIdx = pArg[0]->getAs<Double>()->get();
                for (int i = 0 ; i < iSeqCount ; i++)
                {
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
                            pOut->set(iRowWrite, iColWrite, dbl);
                        }
                    }
                    else
                    {
                        double dbl = get(iRowRead, iColRead);
                        if (dbl != 0)
                        {
                            //only non zero values
                            pOut->set(iRowWrite, iColWrite, dbl);
                        }
                    }
                }
            }
            else
            {
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
                        if (isComplex())
                        {
                            std::complex<double> dbl = getImg((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                            if (dbl.real() != 0 || dbl.imag() != 0)
                            {
                                //only non zero values
                                pOut->set(iRow, iCol, dbl);
                            }
                        }
                        else
                        {
                            double dbl = get((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                            if (dbl != 0)
                            {
                                //only non zero values
                                pOut->set(iRow, iCol, dbl);
                            }
                        }
                        iPos++;
                    }
                }
            }
            else
            {
                return NULL;
            }
        }

        return pOut;
    }

    Sparse* Sparse::extract(int nbCoords, int CONST* coords, int CONST* maxCoords, int CONST* resSize, bool asVector) CONST
    {
        if ( (asVector && maxCoords[0] > getSize()) ||
            (asVector == false && maxCoords[0] > getRows()) ||
            (asVector == false && maxCoords[1] > getCols()))
        {
            return 0;
        }

        bool const cplx(isComplex());
        Sparse* pSp (0);
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
            pSp = 0;
        }
        return pSp;
    }
    /*
    coords are Scilab 1-based
    extract std::make_pair(coords, asVector), rowIter
    */
    template<typename Src, typename SrcTraversal, typename Sz, typename DestTraversal>
    bool Sparse::copyToSparse(Src CONST& src, SrcTraversal srcTrav, Sz n, Sparse& sp, DestTraversal destTrav)
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

    // GenericType because we might return a Double* for scalar operand
    GenericType* Sparse::substract(Sparse const& o) const
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
            realSp = new RealSparse_t(*matrixReal * *(o.matrixReal));
        }
        else if (isComplex() == false && o.isComplex() == true)
        {
            cplxSp = new CplxSparse_t(matrixReal->cast<std::complex<double> >() * *(o.matrixCplx));
        }
        else if (isComplex() == true && o.isComplex() == false)
        {
            cplxSp = new CplxSparse_t(*matrixCplx * o.matrixReal->cast<std::complex<double> >());
        }
        else if (isComplex() == true && o.isComplex() == true)
        {
            cplxSp = new CplxSparse_t(*matrixCplx * *(o.matrixCplx));
        }

        return new Sparse(realSp, cplxSp);
    }

    Sparse* Sparse::dotMultiply(Sparse CONST& o) const
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

    Sparse* Sparse::newTransposed() const
    {
        return new Sparse( matrixReal ? new RealSparse_t(matrixReal->adjoint()) : 0
            , matrixCplx ? new CplxSparse_t(matrixCplx->adjoint()) : 0);
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

    namespace
    {
        template<typename S> struct GetReal: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.value();
            }
        };
        template<> struct GetReal< Eigen::SparseMatrix<std::complex<double > > >
        : std::unary_function<Eigen::SparseMatrix<std::complex<double > > ::InnerIterator, double>
        {
            double operator()( Eigen::SparseMatrix<std::complex<double > > ::InnerIterator it) const
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
        template<typename S> struct GetRow: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.row() + 1;
            }
        };
        template<typename S> struct GetCol: std::unary_function<typename S::InnerIterator, double>
        {
            double operator()(typename S::InnerIterator it) const
            {
                return it.col() + 1;
            }
        };

        template<typename S, typename Out, typename F> Out sparseTransform(S& s, Out o, F f)
        {
            for (std::size_t k(0); k < s.outerSize(); ++k)
            {
                for (typename S::InnerIterator it(s, k); it; ++it, ++o)
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

    double* Sparse::outputRowCol(double* out)const
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

    SparseBool* Sparse::newLessThan(Sparse const&o) const
    {
        return cwiseOp<std::less>(*this, o);
    }

    SparseBool* Sparse::newGreaterThan(Sparse const&o) const
    {
        return cwiseOp<std::greater>(*this, o);
    }

    SparseBool* Sparse::newNotEqualTo(Sparse const&o) const
    {
        return cwiseOp<std::not_equal_to>(*this, o);
    }

    SparseBool* Sparse::newLessOrEqual(Sparse const&o) const
    {
        return cwiseOp<std::less_equal>(*this, o);
    }

    SparseBool* Sparse::newGreaterOrEqual(Sparse const&o) const
    {
        return cwiseOp<std::greater_equal>(*this, o);
    }

    SparseBool* Sparse::newEqualTo(Sparse const&o) const
    {
        return cwiseOp<std::equal_to>(*this, o);
    }

    //    SparseBool* SparseBool::new

    SparseBool::SparseBool(Bool CONST& src)
    {
        create(src.getRows(), src.getCols(), src, RowWiseFullIterator(src.getRows(), src.getCols()), src.getSize());
    }
    /* @param src : Bool matrix to copy into a new sparse matrix
    @param idx : Double matrix to use as indexes to get values from the src
    **/
    SparseBool::SparseBool(Bool CONST& src, Double CONST& idx)
    {
        double CONST* const endOfRow(idx.getReal() + idx.getRows());
        create( static_cast<int>(*std::max_element(idx.getReal(), endOfRow))
            , static_cast<int>(*std::max_element(endOfRow, endOfRow + idx.getRows()))
            , src, makeIteratorFromVar(idx), idx.getSize() / 2 );
    }

    /* @param src : Bool matrix to copy into a new sparse matrix
    @param idx : Double matrix to use as indexes to get values from the src
    @param dims : Double matrix containing the dimensions of the new matrix
    **/
    SparseBool::SparseBool(Bool CONST& src, Double CONST& idx, Double CONST& dims)
    {
        create((int)dims.getReal(0, 0) , (int)dims.getReal(0, 1) , src, makeIteratorFromVar(idx), (int)idx.getSize() / 2);
    }

    SparseBool::SparseBool(int _iRows, int _iCols) : matrixBool(new BoolSparse_t(_iRows, _iCols))
    {
        m_iRows = _iRows;
        m_iCols = _iCols;
        m_iSize = _iRows * _iCols;
        m_iDims = 2;
        m_piDims[0] = _iRows;
        m_piDims[1] = _iCols;
    }

    SparseBool::SparseBool(SparseBool const& src) : GenericType(src),  matrixBool(new BoolSparse_t(*src.matrixBool))
    {
        m_iDims = 2;
        m_piDims[0] = const_cast<SparseBool*>(&src)->getRows();
        m_piDims[1] = const_cast<SparseBool*>(&src)->getCols();
    }

    SparseBool::SparseBool(BoolSparse_t* src) : matrixBool(src)
    {
        m_iRows = src->rows();
        m_iCols = src->cols();
        m_iSize = m_iRows * m_iCols;
    }

    template<typename DestIter>
    void SparseBool::create(int rows, int cols, Bool CONST& src, DestIter o, std::size_t n)
    {
        m_iCols = cols;
        m_iRows = rows;
        m_iSize = cols * rows;
        m_iDims = 2;
        m_piDims[0] = m_iRows;
        m_piDims[1] = m_iCols;

        matrixBool = new BoolSparse_t(rows, cols);

        matrixBool->reserve(n);
        mycopy_n(makeMatrixIterator<int>(src,  RowWiseFullIterator(src.getRows(), src.getCols())), n
            , makeMatrixIterator<bool>(*matrixBool, o));
        finalize();
    }


    bool SparseBool::toString(std::wostringstream& ostr) const
    {
        ostr << ::toString(*matrixBool, 0);
        return true;
    }

    void SparseBool::whoAmI() CONST
    {
        std::cout << "types::SparseBool";
    }

    SparseBool* SparseBool::clone(void) const
    {
        return new SparseBool(*this);
    }

    bool SparseBool::resize(int _iNewRows, int _iNewCols)
    {
        matrixBool->resize(_iNewRows, _iNewCols);
        return true;
    }

    bool SparseBool::insert(int nbCoords, int CONST* coords, int CONST * maxCoords, GenericType CONST* src, bool asVector)
    {
        int iNewRows = Max(maxCoords[0], getRows());
        int iNewCols = Max(maxCoords[1], getCols());
        int iPos     = 0;
        int row      = 0;
        int col      = 0;
        bool res(false);
        // invalid index < 1
        if (*std::min_element(coords, coords + (asVector ? 1 : 2)* nbCoords) < 1)
        {
            return false;
        }
        switch (src->getType())
        {
        case InternalType::RealBool :
            {
                res = false;
                if ( asVector )
                {
                    mycopy_n(makeMatrixIterator<bool>(*src->getAs<Bool>(), RowWiseFullIterator(src->getRows(), src->getCols()))
                        , nbCoords
                        , makeMatrixIterator<bool>(*matrixBool,  Coords<true>(coords, getRows())));
                }
                else
                {
                    mycopy_n(makeMatrixIterator<bool>(*src->getAs<Bool>(), RowWiseFullIterator(src->getRows(), src->getCols()))
                        , nbCoords
                        , makeMatrixIterator<bool>(*matrixBool,  Coords<false>(coords)));
                }
                res = true;
                finalize();
                break;
            }
        case InternalType::RealSparseBool :
            {
                res = false;
                if ( asVector )
                {
                    mycopy_n(makeMatrixIterator<bool>(*src->getAs<SparseBool>(), RowWiseFullIterator(src->getRows(), src->getCols()))
                        , nbCoords
                        , makeMatrixIterator<bool>(*matrixBool,  Coords<true>(coords, getRows())));
                }
                else
                {
                    mycopy_n(makeMatrixIterator<bool>(*src->getAs<SparseBool>(), RowWiseFullIterator(src->getRows(), src->getCols()))
                        , nbCoords
                        , makeMatrixIterator<bool>(*matrixBool,  Coords<false>(coords)));
                }
                res = true;
                finalize();
                break;
            }
        default :
            {
                res = false;
            }
        }
        return res;
    }

    bool SparseBool::append(int r, int c, SparseBool CONST* src)
    {
        doAppend(*src, r, c, *matrixBool);
        finalize();
        return true;
    }

    SparseBool* SparseBool::insert_new(int nbCoords, int CONST* coords, int CONST* maxCoords, GenericType CONST* src, bool asVector)
    {
        SparseBool* pSp;
        if (asVector)
        {
            if (src->getCols() == 1)
            {
                pSp = new SparseBool(maxCoords[0], 1);
            }
            else
            {
                pSp = (src->getRows() == 1) ? new SparseBool(1, maxCoords[0]) : 0;
            }
        }
        else
        {
            pSp =  new SparseBool(maxCoords[0], maxCoords[1]);
        }
        if ( pSp && !pSp->insert(nbCoords, coords, maxCoords, src, asVector) )
        {
            delete pSp;
            pSp = 0;
        }
        return pSp;
    }

    SparseBool* SparseBool::extract(int nbCoords, int CONST* coords, int CONST* maxCoords, int CONST* resSize, bool asVector) CONST
    {
        if ( (asVector && maxCoords[0] > getSize()) ||
            (asVector == false && maxCoords[0] > getRows()) ||
            (asVector == false && maxCoords[1] > getCols()))
        {
            return 0;
        }

        SparseBool* pSp (0);
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
    InternalType* SparseBool::extract(typed_list* _pArgs)
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
            if (_pArgs->size() == 0)
            {
                //a()
                return this;
            }
            else
            {
                //a([])
                return Double::Empty();
            }
        }

        if (iDims < 2)
        {
            // Check that we stay inside the input size.
            if (piMaxDim[0] <= getSize())
            {
                int iNewRows = Max(pArg[0]->getAs<Double>()->getRows(), pArg[0]->getAs<Double>()->getCols());
                int iNewCols = 1;

                pOut = new SparseBool(iNewRows, iNewCols);
                double* pIdx = pArg[0]->getAs<Double>()->get();
                // Write in output all elements extract from input.
                for (int i = 0 ; i < iSeqCount ; i++)
                {
                    int iRowRead = static_cast<int>(pIdx[i] - 1) % getRows();
                    int iColRead = static_cast<int>(pIdx[i] - 1) / getRows();

                    int iRowWrite = static_cast<int>(i) % iNewRows;
                    int iColWrite = static_cast<int>(i) / iNewRows;

                    bool bValue = get(iRowRead, iColRead);
                    if (bValue)
                    {
                        //only non zero values
                        pOut->set(iRowWrite, iColWrite, true);
                    }
                }
            }
            else
            {
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
                        bool bValue = get((int)pIdxRow[iRow] - 1, (int)pIdxCol[iCol] - 1);
                        if (bValue)
                        {
                            //only non zero values
                            pOut->set(iRow, iCol, true);
                        }
                        iPos++;
                    }
                }
            }
            else
            {
                return NULL;
            }
        }

        return pOut;
    }

    SparseBool* SparseBool::newTransposed() const
    {
        return new SparseBool(new BoolSparse_t(matrixBool->adjoint()));
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


    double* SparseBool::outputRowCol(double* out)const
    {
        return sparseTransform(*matrixBool, sparseTransform(*matrixBool, out, GetRow<BoolSparse_t>()), GetCol<BoolSparse_t>());
    }

    bool SparseBool::operator==(const InternalType& it) CONST
    {
        SparseBool* otherSparse = const_cast<SparseBool*>(dynamic_cast<SparseBool const*>(&it));/* types::GenericType is not const-correct :( */
        return (otherSparse
            && (otherSparse->getRows() != getRows())
            && (otherSparse->getCols() != getCols())
            && equal(*matrixBool, *otherSparse->matrixBool));
    }

    bool SparseBool::operator!=(const InternalType& it) CONST
    {
        return !(*this == it);
    }

    void SparseBool::finalize()
    {
        matrixBool->prune(&keepForSparse<bool>);
        matrixBool->finalize();
    }

    GenericType::RealType SparseBool::getType(void) CONST
    {
        return InternalType::RealSparseBool;
    }

    bool SparseBool::get(int r, int c) CONST
    {
        return matrixBool->coeff(r, c);
    }

    bool SparseBool::set(int _iRows, int _iCols, bool _bVal) CONST
    {
        bool val = matrixBool->coeff(_iRows, _iCols);
        if (val)
        {
            matrixBool->coeffRef(_iRows, _iCols) = _bVal;
        }
        else
        {
            matrixBool->insert(_iRows, _iCols) = _bVal;
        }

        finalize();
        return true;
    }

    void SparseBool::fill(Bool& dest, int r, int c) CONST
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

    SparseBool* SparseBool::newEqualTo(SparseBool const&o) const
    {
        return cwiseOp<std::equal_to>(*this, o);
    }

    SparseBool* SparseBool::newLogicalOr(SparseBool const&o) const
    {
        return cwiseOp<std::logical_or>(*this, o);
    }

    SparseBool* SparseBool::newLogicalAnd(SparseBool const&o) const
    {
        return cwiseOp<std::logical_and>(*this, o);
    }
}
