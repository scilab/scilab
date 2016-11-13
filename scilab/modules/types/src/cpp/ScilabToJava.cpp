/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#include "ScilabToJava.hxx"
#ifdef _MSC_VER
#pragma warning(disable: 4800)
#endif

namespace org_modules_types
{

int ScilabToJava::refreshId = -1;

void ScilabToJava::sendAllListenedVariables()
{
    if (refreshId == -1)
    {
        refreshId = ScilabVariablesRefresh::getScilabVariablesRefreshId(getScilabJavaVM());
    }

    char ** vars = ScilabVariables::getAllListenedVariables(getScilabJavaVM());
    while (*vars)
    {
        sendVariable(std::string(*vars), true, refreshId);
        delete[] *vars;
        vars++;
    }
    delete[] vars;
}

bool ScilabToJava::sendVariableAsReference(const std::string & name, int handlerId)
{
    int * addr = 0;
    SciErr err;

    if (!isNamedVarExist(0, name.c_str()))
    {
        return false;
    }

    err = getVarAddressFromName(0, name.c_str(), &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return false;
    }

    std::vector<int> indexes;
    return sendVariable(name, indexes, addr, false, true, handlerId, 0);
}

bool ScilabToJava::sendVariable(const std::string & name, bool swaped, int handlerId)
{
    int * addr = 0;
    SciErr err;

    if (!isNamedVarExist(0, name.c_str()))
    {
        return false;
    }

    err = getVarAddressFromName(0, name.c_str(), &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return false;
    }

    std::vector<int> indexes;
    return sendVariable(name, indexes, addr, swaped, false, handlerId, 0);
}

bool ScilabToJava::sendVariable(const std::string & name, int * addr, bool swaped, int handlerId, void * pvApiCtx)
{
    std::vector<int> indexes;
    return sendVariable(name, indexes, addr, swaped, false, handlerId, pvApiCtx);
}

bool ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, int * addr, bool swaped, bool byref, int handlerId, void * pvApiCtx)
{
    SciErr err;

    int type = 0;
    int row = 0;
    int col = 0;
    int prec = 0;

    // Double
    double * real = 0;
    double * img = 0;

    // Integers
    char * integers8 = 0;
    unsigned char * uintegers8 = 0;
    short * integers16 = 0;
    unsigned short * uintegers16 = 0;
    int * integers32 = 0;
    unsigned int * uintegers32 = 0;

    // Boolean
    int * booleans = 0;

#ifdef __SCILAB_INT64__
    long long * integers64 = 0;
    unsigned long long * uintegers64 = 0;
#endif

    // Strings
    char ** strings = 0;

    // Sparse
    int nbItem;
    int * nbItemRow = 0;
    int * colPos = 0;

    // Polynomial
    double ** re = 0;
    double ** im = 0;
    int rc;
    int * nbCoeffs = 0;
    char varName[5];
    int varNameLen = 0;

    // Lists
    char listtype = 0;
    int nbItems = 0;

    err = getVarType(pvApiCtx, addr, &type);
    if (err.iErr)
    {
        printError(&err, 0);
        return false;
    }

    switch (type)
    {
        case sci_matrix :
            if (isVarComplex(pvApiCtx, addr))
            {
                err = getComplexMatrixOfDouble(pvApiCtx, addr, &row, &col, &real, &img);
                if (err.iErr)
                {
                    printError(&err, 0);
                    return false;
                }

                sendVariable<double>(name, indexes, row, col, real, img, swaped, byref, handlerId);
            }
            else
            {
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &real);
                if (err.iErr)
                {
                    printError(&err, 0);
                    return false;
                }

                sendVariable<double>(name, indexes, row, col, real, swaped, byref, handlerId);
            }
            break;
        case sci_ints :
            err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
            if (err.iErr)
            {
                printError(&err, 0);
                return false;
            }

            switch (prec)
            {
                case SCI_INT8 :
                    err = getMatrixOfInteger8(pvApiCtx, addr, &row, &col, &integers8);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendVariable<byte>(name, indexes, row, col, (byte *)integers8, swaped, byref, handlerId);
                    break;
                case SCI_UINT8 :
                    err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, &row, &col, &uintegers8);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendUnsignedVariable<byte>(name, indexes, row, col, (byte *)uintegers8, swaped, byref, handlerId);
                    break;
                case SCI_INT16 :
                    err = getMatrixOfInteger16(pvApiCtx, addr, &row, &col, &integers16);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendVariable<short>(name, indexes, row, col, integers16, swaped, byref, handlerId);
                    break;
                case SCI_UINT16 :
                    err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, &row, &col, &uintegers16);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendUnsignedVariable<short>(name, indexes, row, col, (short *)uintegers16, swaped, byref, handlerId);
                    break;
                case SCI_INT32 :
                    err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &integers32);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendVariable<int>(name, indexes, row, col, integers32, swaped, byref, handlerId);
                    break;
                case SCI_UINT32 :
                    err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, &row, &col, &uintegers32);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendUnsignedVariable<int>(name, indexes, row, col, (int *)uintegers32, swaped, byref, handlerId);
                    break;

#ifdef __SCILAB_INT64__
                case SCI_INT64 :
                    err = getMatrixOfInteger64(pvApiCtx, addr, &row, &col, &integers64);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendVariable<long long>(name, indexes, row, col, integers64, swaped, byref, handlerId);
                    break;
                case SCI_UINT64 :
                    err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, &row, &col, &uintegers64);
                    if (err.iErr)
                    {
                        printError(&err, 0);
                        return false;
                    }

                    sendUnsignedVariable<long long>(name, indexes, row, col, (long long *)uintegers64, swaped, byref, handlerId);
                    break;
#endif
                default :
                    return false;
            }
            break;
        case sci_strings :
            if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &strings))
            {
                return false;
            }

            sendStringVariable(name, indexes, row, col, strings, swaped, false, handlerId);
            freeAllocatedMatrixOfString(row, col, strings);
            break;
        case sci_boolean :
            err = getMatrixOfBoolean(pvApiCtx, addr, &row, &col, &booleans);
            if (err.iErr)
            {
                printError(&err, 0);
                return false;
            }

            sendConvertedBooleanVariable(name, indexes, row, col, booleans, swaped, byref, handlerId);
            break;
        case sci_sparse :
            if (isVarComplex(pvApiCtx, addr))
            {
                err = getComplexSparseMatrix(pvApiCtx, addr, &row, &col, &nbItem, &nbItemRow, &colPos, &real, &img);
                if (err.iErr)
                {
                    printError(&err, 0);
                    return 0;
                }

                sendVariable<double>(name, indexes, nbItem, nbItemRow, colPos, row, col, real, img, false, handlerId);
            }
            else
            {
                err = getSparseMatrix(pvApiCtx, addr, &row, &col, &nbItem, &nbItemRow, &colPos, &real);
                if (err.iErr)
                {
                    printError(&err, 0);
                    return 0;
                }

                sendVariable<double>(name, indexes, nbItem, nbItemRow, colPos, row, col, real, false, handlerId);
            }
            break;
        case sci_boolean_sparse :
            err = getBooleanSparseMatrix(pvApiCtx, addr, &row, &col, &nbItem, &nbItemRow, &colPos);
            if (err.iErr)
            {
                printError(&err, 0);
                return 0;
            }

            sendBooleanSparseVariable(name, indexes, nbItem, nbItemRow, colPos, row, col, false, handlerId);
            delete[] integers8;
            break;
        case sci_poly :
            err = getPolyVariableName(pvApiCtx, addr, varName, &varNameLen);
            if (err.iErr)
            {
                printError(&err, 0);
                return false;
            }

            if (isVarComplex(pvApiCtx, addr))
            {
                getComplexMatrixOfPoly(pvApiCtx, addr, &row, &col, 0, 0, 0);
                rc = row * col;
                nbCoeffs = new int[rc];

                getComplexMatrixOfPoly(pvApiCtx, addr, &row, &col, nbCoeffs, 0, 0);
                re = new double*[rc];
                for (int i = 0; i < rc; i++)
                {
                    re[i] = new double[nbCoeffs[i]];
                }

                im = new double*[rc];
                for (int i = 0; i < rc; i++)
                {
                    im[i] = new double[nbCoeffs[i]];
                }
                err = getComplexMatrixOfPoly(pvApiCtx, addr, &row, &col, nbCoeffs, re, im);
                if (err.iErr)
                {
                    for (int i = 0; i < rc; i++)
                    {
                        delete[] im[i];
                        delete[] re[i];
                    }
                    delete[] im;
                    delete[] re;
                    delete[] nbCoeffs;
                    printError(&err, 0);
                    return false;
                }

                sendVariable<double>(name, indexes, varName, row, col, nbCoeffs, re, im, swaped, false, handlerId);
                for (int i = 0; i < rc; i++)
                {
                    delete[] im[i];
                }
                delete[] im;
            }
            else
            {
                getMatrixOfPoly(pvApiCtx, addr, &row, &col, 0, 0);
                rc = row * col;
                nbCoeffs = new int[rc];

                getMatrixOfPoly(pvApiCtx, addr, &row, &col, nbCoeffs, 0);
                re = new double*[rc];
                for (int i = 0; i < rc; i++)
                {
                    re[i] = new double[nbCoeffs[i]];
                }

                err = getMatrixOfPoly(pvApiCtx, addr, &row, &col, nbCoeffs, re);
                if (err.iErr)
                {
                    for (int i = 0; i < rc; i++)
                    {
                        delete[] re[i];
                    }
                    delete[] re;
                    delete[] nbCoeffs;
                    printError(&err, 0);
                    return false;
                }

                sendVariable<double>(name, indexes, varName, row, col, nbCoeffs, re, swaped, false, handlerId);
            }
            for (int i = 0; i < rc; i++)
            {
                delete[] re[i];
            }
            delete[] re;
            delete[] nbCoeffs;

            break;
        case sci_list :
            listtype = 'l';
            break;
        case sci_mlist :
            listtype = 'm';
            break;
        case sci_tlist :
            listtype = 't';
            break;
        default :
            return false;
    }

    if (listtype)
    {
        err = getListItemNumber(pvApiCtx, addr, &nbItems);
        if (err.iErr)
        {
            printError(&err, 0);
            return false;
        }

        sendVariable(name, nbItems, indexes, listtype, false, handlerId);
        bool b = sendItems(name, nbItems, indexes, addr, swaped, byref, handlerId, pvApiCtx);
        closeList(indexes, handlerId);
        return b;
    }

    return true;
}

inline bool ScilabToJava::sendItems(const std::string & name, const int nbItems, std::vector<int> & indexes, int * addr, bool swaped, bool byref, int handlerId, void * pvApiCtx)
{
    int * itemAddr = 0;
    SciErr err;

    for (int i = 1; i <= nbItems; i++)
    {
        err = getListItemAddress(pvApiCtx, addr, i, &itemAddr);
        if (err.iErr)
        {
            printError(&err, 0);
            return false;
        }

        indexes.push_back(i);

        if (!sendVariable(name, indexes, itemAddr, swaped, byref, handlerId, pvApiCtx))
        {
            return false;
        }

        indexes.pop_back();
    }

    return true;
}

inline int * ScilabToJava::getIndexesPointer(std::vector<int> & indexes)
{
    if (indexes.size() == 0)
    {
        return 0;
    }

    return &(indexes[0]);
}

// Lists
// byref is useless
inline void ScilabToJava::sendVariable(const std::string & name, const int nbItems, std::vector<int> & indexes, char type, bool byref, int handlerId)
{
    ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), nbItems, getIndexesPointer(indexes), (int)indexes.size(), type, handlerId);
}

inline void ScilabToJava::closeList(std::vector<int> & indexes, int handlerId)
{
    ScilabVariables::closeList(getScilabJavaVM(), getIndexesPointer(indexes), (int)indexes.size(), handlerId);
}

// Sparse
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, int nbItem, int * nbItemRow, int * colPos, int row, int col, T * data, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos, nbItem, data, nbItem, handlerId);
    }
    else
    {
        int * colPos_ = new int[nbItem];
        for (int i = 0; i < nbItem; i++)
        {
            colPos_[i] = colPos[i] - 1;
        }
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos_, nbItem, data, nbItem, handlerId);
        delete[] colPos_;
    }
}

// Double, ...
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, int row, int col, T * data, bool swaped, bool byRef, int handlerId)
{
    if (byRef)
    {
        ScilabVariables::sendDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), data, row * col, row, col, handlerId);
    }
    else
    {
        T ** addr = getMatrix<T>(row, col, data, swaped);
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), addr, row, col, swaped, handlerId);
        deleteMatrix<T>(addr, swaped);
    }
}

// String
inline void ScilabToJava::sendStringVariable(const std::string & name, std::vector<int> & indexes, int row, int col, char ** data, bool swaped, bool byRef, int handlerId)
{
    char *** addr = getMatrix<char *>(row, col, data, swaped);
    ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), addr, row, col, swaped, handlerId);
    deleteMatrix<char *>(addr, swaped);
}

// Boolean sparse
inline void ScilabToJava::sendBooleanSparseVariable(const std::string & name, std::vector<int> & indexes, int nbItem, int * nbItemRow, int * colPos, int row, int col, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos, nbItem, handlerId);
    }
    else
    {
        int * colPos_ = new int[nbItem];
        for (int i = 0; i < nbItem; i++)
        {
            colPos_[i] = colPos[i] - 1;
        }
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos_, nbItem, handlerId);
        delete[] colPos_;
    }
}

// uint* matrix with a bigger storage
// TODO : change the Java wrapping
template<typename T, typename U>
inline void ScilabToJava::sendUnsignedVariableWithCast(const std::string & name, std::vector<int> & indexes, int row, int col, U * data, bool swaped, int handlerId)
{
    T ** addr = getConvertedMatrix<T, U>(row, col, data, swaped);
    ScilabVariables::sendUnsignedData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), addr, row, col, swaped, handlerId);
    deleteMatrix<T>(addr, swaped);
}

// uint*
template<typename T>
inline void ScilabToJava::sendUnsignedVariable(const std::string & name, std::vector<int> & indexes, int row, int col, T * data, bool swaped, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendUnsignedDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), data, row * col, row, col, handlerId);
    }
    else
    {
        T ** addr = getMatrix<T>(row, col, data, swaped);
        ScilabVariables::sendUnsignedData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), addr, row, col, swaped, handlerId);
        deleteMatrix<T>(addr, swaped);
    }
}

// Boolean
inline void ScilabToJava::sendConvertedBooleanVariable(const std::string & name, std::vector<int> & indexes, int row, int col, int * data, bool swaped, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendBooleanDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), data, row * col, row, col, handlerId);
    }
    else
    {
        bool ** addr = getConvertedMatrix<bool, int>(row, col, data, swaped);
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), addr, row, col, swaped, handlerId);
        deleteMatrix<bool>(addr, swaped);
    }
}

// Complex sparse
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, int nbItem, int * nbItemRow, int * colPos, int row, int col, T * real, T * img, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos, nbItem, real, nbItem, img, nbItem, handlerId);
    }
    else
    {
        int * colPos_ = new int[nbItem];
        for (int i = 0; i < nbItem; i++)
        {
            colPos_[i] = colPos[i] - 1;
        }
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), row, col, nbItem, nbItemRow, row, colPos_, nbItem, real, nbItem, img, nbItem, handlerId);
        delete[] colPos_;
    }
}

// Complex
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, int row, int col, T * real, T * img, bool swaped, bool byref, int handlerId)
{
    if (byref)
    {
        ScilabVariables::sendDataAsBuffer(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), real, row * col, img, row * col, row, col, handlerId);
    }
    else
    {
        T ** re = getMatrix<T>(row, col, real, swaped);
        T ** im = getMatrix<T>(row, col, img, swaped);
        ScilabVariables::sendData(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), re, row, col, im, row, col, swaped, handlerId);
        deleteMatrix<T>(re, swaped);
        deleteMatrix<T>(im, swaped);
    }
}

// Polynomial
// byref is useless
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, const char * varName, int row, int col, int * nbcoeff, T ** data, bool swaped, bool byref, int handlerId)
{
    T *** addr = getMatrix<T*>(row, col, data, swaped);
    int ** nbc = getMatrix<int>(row, col, nbcoeff, swaped);
    ScilabPolynomialToJava::sendPolynomial(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), (char *)varName, addr, row, col, nbc, swaped, handlerId);
    deleteMatrix<T*>(addr, swaped);
    deleteMatrix<int>(nbc, swaped);
}

// Complex polynomial
// byref is useless
template<typename T>
inline void ScilabToJava::sendVariable(const std::string & name, std::vector<int> & indexes, const char * varName, int row, int col, int * nbcoeff, T ** real, T ** img, bool swaped, bool byref, int handlerId)
{
    T *** re = getMatrix<T*>(row, col, real, swaped);
    T *** im = getMatrix<T*>(row, col, img, swaped);
    int ** nbc = getMatrix<int>(row, col, nbcoeff, swaped);
    ScilabPolynomialToJava::sendPolynomial(getScilabJavaVM(), (char *)name.c_str(), getIndexesPointer(indexes), (int)indexes.size(), (char *)varName, re, im, row, col, nbc, swaped, handlerId);
    deleteMatrix<T*>(re, swaped);
    deleteMatrix<T*>(im, swaped);
    deleteMatrix<int>(nbc, swaped);
}

template<typename T>
inline T ** ScilabToJava::getMatrix(int row, int col, T * data, bool swaped)
{
    T ** addr = 0;

    if (row && col)
    {
        if (swaped)
        {
            T * d = new T[row * col];
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    d[i * col + j] = data[j * row + i];
                }
            }
            addr = convertMatrix<T>(row, col, d);
        }
        else
        {
            addr = convertMatrix<T>(col, row, data);
        }
    }

    return addr;
}

template<typename T, typename U>
inline T ** ScilabToJava::getConvertedMatrix(int row, int col, U * data, bool swaped)
{
    T ** addr = 0;

    if (row && col)
    {
        int rc = row * col;
        T * d = new T[rc];
        if (swaped)
        {
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    d[i * col + j] = static_cast<T>(data[j * row + i]);
                }
            }
            addr = convertMatrix<T>(row, col, d);
        }
        else
        {
            for (int i = 0; i < rc; i++)
            {
                d[i] = static_cast<T>(data[i]);
            }
            addr = convertMatrix<T>(col, row, d);
        }
    }

    return addr;
}

template<typename T>
inline T ** ScilabToJava::convertMatrix(int row, int col, T * data)
{
    T ** addr = 0;

    if (row && col)
    {
        addr = new T*[row];
        *addr = data;
        for (int i = 1; i < row; i++)
        {
            addr[i] = addr[i - 1] + col;
        }
    }

    return addr;
}

template<typename T>
inline void ScilabToJava::deleteMatrix(T ** data, bool swaped)
{
    if (data)
    {
        if (swaped && *data)
        {
            delete[] *data;
        }
        delete[] data;
    }
}
}

void getScilabVariable(const char * variableName, int swapRowCol, int handlerId)
{
    org_modules_types::ScilabToJava::sendVariable(std::string(variableName), swapRowCol != 0, handlerId);
}

void getScilabVariableAsReference(const char * variableName, int handlerId)
{
    org_modules_types::ScilabToJava::sendVariableAsReference(std::string(variableName), handlerId);
}
