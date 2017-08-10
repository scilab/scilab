/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include <cstring>
#include <cstdio>
#include <vector>

#include "internal.hxx"
#include "EOType.hxx"
#include "mlist.hxx"
#include "ScilabObjects.hxx"
#include "context.hxx"
#include "function.hxx"
#include "gatewaystruct.hxx"
extern "C" {
#include "api_scilab.h"
    extern int C2F(varfunptr)(int *, int *, int *);
}

namespace org_modules_external_objects
{
bool ScilabObjects::isInit = false;
const char * ScilabObjects::_EOBJ[] = {"_EObj", "_EnvId", "_id"};
const char * ScilabObjects::_ECLASS[] = {"_EClass", "_EnvId", "_id"};
const char * ScilabObjects::_EVOID[] = {"_EVoid", "_EnvId", "_id"};
const wchar_t * ScilabObjects::_INVOKE_ = L"!!_invoke_";
const wchar_t pwstEClass[] = {L"_EClass"};
const wchar_t pwstEObj[] = {L"_EObj"};
const wchar_t pwstEVoid[] = {L"_EVoid"};

void ScilabObjects::initialization(ScilabAbstractEnvironment & env, void * pvApiCtx)
{
    if (!isInit)
    {
        isInit = true;
    }
}

int ScilabObjects::createNamedEnvironmentObject(int type, const char * name, int id, const int envId, void * pvApiCtx)
{
    const char ** fields;
    int * mlistaddr = 0;
    SciErr err;

    if (envId < 0)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid environment"));
    }

    switch (type)
    {
        case EXTERNAL_OBJECT:
            fields = static_cast<const char **>(_EOBJ);
            break;
        case EXTERNAL_CLASS:
            fields = static_cast<const char **>(_ECLASS);
            break;
        case EXTERNAL_VOID:
            fields = static_cast<const char **>(_EVOID);
            break;
        default :
            fields = static_cast<const char **>(_EOBJ);
            break;
    }

    err = createNamedMList(pvApiCtx, name, FIELDS_LENGTH, &mlistaddr);
    if (err.iErr)
    {
        if (err.iErr == API_ERROR_INVALID_NAME)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable name: %s"), name);
        }

        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfStringInNamedList(pvApiCtx, name, mlistaddr, 1, 1, FIELDS_LENGTH, fields);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfInteger32InNamedList(pvApiCtx, name, mlistaddr, EXTERNAL_ENV_ID_POSITION, 1, 1, &envId);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfInteger32InNamedList(pvApiCtx, name, mlistaddr, EXTERNAL_OBJ_ID_POSITION, 1, 1, &id);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    return 1;
}

void ScilabObjects::createEnvironmentObjectAtPos(int type, int pos, int id, const int envId, void * pvApiCtx)
{
    const char ** fields = 0;
    int * mlistaddr = 0;
    SciErr err;

    if (envId < 0)
    {
        throw ScilabAbstractEnvironmentException("Invalid environment");
    }

    switch (type)
    {
        case EXTERNAL_OBJECT:
            fields = static_cast<const char **>(_EOBJ);
            break;
        case EXTERNAL_CLASS:
            fields = static_cast<const char **>(_ECLASS);
            break;
        case EXTERNAL_VOID:
            fields = static_cast<const char **>(_EVOID);
            break;
        default :
            fields = static_cast<const char **>(_EOBJ);
            break;
    }

    err = createMList(pvApiCtx, pos, FIELDS_LENGTH, &mlistaddr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, FIELDS_LENGTH, fields);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, EXTERNAL_ENV_ID_POSITION, 1, 1, &envId);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, EXTERNAL_OBJ_ID_POSITION, 1, 1, &id);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Cannot allocate memory"));
    }
}

void ScilabObjects::copyInvocationMacroToStack(int pos, const int envId, bool isNew, void * pvApiCtx)
{
    EOType* invoke = new EOType(envId, isNew);
    types::GatewayStruct* str = (types::GatewayStruct*)pvApiCtx;
    //assign function as return value
    str->m_pOut[pos - str->m_iIn - 1 /*0*/] = invoke;
}

void ScilabObjects::removeTemporaryVars(const int envId, int * tmpvar)
{
    if (tmpvar && *tmpvar)
    {
        ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
        env.removeobject(tmpvar + 1, *tmpvar);
        *tmpvar = 0;
    }
}

void ScilabObjects::removeVar(int * addr, void * pvApiCtx)
{
    SciErr err;
    int type, row, col, * id;

    err = getVarType(pvApiCtx, addr, &type);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (type == sci_mlist && (isExternalObjOrClass(addr, pvApiCtx)))
    {
        err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        int envId = getEnvironmentId(addr, pvApiCtx);
        ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);

        env.removeobject(*id);
    }
    else if (type == sci_strings)
    {
        char * varName = 0;
        if (getAllocatedSingleString(pvApiCtx, addr, &varName))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        err = getVarAddressFromName(pvApiCtx, varName, &addr);
        if (err.iErr)
        {
            freeAllocatedSingleString(varName);
            return;
        }

        err = getVarType(pvApiCtx, addr, &type);
        if (err.iErr)
        {
            freeAllocatedSingleString(varName);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }

        if (type == sci_mlist && isExternalObjOrClass(addr, pvApiCtx))
        {
            err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
            if (err.iErr)
            {
                freeAllocatedSingleString(varName);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            int envId = getEnvironmentId(addr, pvApiCtx);
            ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);

            env.removeobject(*id);
            deleteNamedVariable(pvApiCtx, varName);
        }
        freeAllocatedSingleString(varName);
    }
}

bool ScilabObjects::unwrap(int idObj, int pos, const int envId, void * pvApiCtx)
{
    if (idObj == 0)
    {
        // id == 0 <==> null object plugged on empty matrix
        if (createEmptyMatrix(pvApiCtx, pos))
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create data"));
        }
        return true;
    }

    VariableType type = Nothing;
    ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);
    const ScilabAbstractEnvironmentWrapper & wrapper = env.getWrapper();

    type = env.isunwrappable(idObj);
    switch (type)
    {
        case Nothing:
            return false;
        case Null:
            if (createEmptyMatrix(pvApiCtx, pos))
            {
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot create data"));
            }
            break;
        case SingleDouble:
            wrapper.unwrapdouble(idObj, ScilabDoubleStackAllocator(pvApiCtx, pos));
            break;
        case RowDouble:
            wrapper.unwraprowdouble(idObj, ScilabDoubleStackAllocator(pvApiCtx, pos));
            break;
        case MatDouble:
            wrapper.unwrapmatdouble(idObj, ScilabDoubleStackAllocator(pvApiCtx, pos));
            break;
        case SingleString:
            wrapper.unwrapstring(idObj, ScilabStringStackAllocator(pvApiCtx, pos));
            break;
        case RowString:
            wrapper.unwraprowstring(idObj, ScilabStringStackAllocator(pvApiCtx, pos));
            break;
        case MatString:
            wrapper.unwrapmatstring(idObj, ScilabStringStackAllocator(pvApiCtx, pos));
            break;
        case SingleBoolean:
            wrapper.unwrapboolean(idObj, ScilabBooleanStackAllocator(pvApiCtx, pos));
            break;
        case RowBoolean:
            wrapper.unwraprowboolean(idObj, ScilabBooleanStackAllocator(pvApiCtx, pos));
            break;
        case MatBoolean:
            wrapper.unwrapmatboolean(idObj, ScilabBooleanStackAllocator(pvApiCtx, pos));
            break;
        case SingleChar:
            wrapper.unwrapchar(idObj, ScilabCharStackAllocator(pvApiCtx, pos));
            break;
        case RowChar:
            wrapper.unwraprowchar(idObj, ScilabCharStackAllocator(pvApiCtx, pos));
            break;
        case MatChar:
            wrapper.unwrapmatchar(idObj, ScilabCharStackAllocator(pvApiCtx, pos));
            break;
        case SingleUChar:
            wrapper.unwrapuchar(idObj, ScilabUCharStackAllocator(pvApiCtx, pos));
            break;
        case RowUChar:
            wrapper.unwraprowuchar(idObj, ScilabUCharStackAllocator(pvApiCtx, pos));
            break;
        case MatUChar:
            wrapper.unwrapmatuchar(idObj, ScilabUCharStackAllocator(pvApiCtx, pos));
            break;
        case SingleShort:
            wrapper.unwrapshort(idObj, ScilabShortStackAllocator(pvApiCtx, pos));
            break;
        case RowShort:
            wrapper.unwraprowshort(idObj, ScilabShortStackAllocator(pvApiCtx, pos));
            break;
        case MatShort:
            wrapper.unwrapmatshort(idObj, ScilabShortStackAllocator(pvApiCtx, pos));
            break;
        case SingleUShort:
            wrapper.unwrapushort(idObj, ScilabUShortStackAllocator(pvApiCtx, pos));
            break;
        case RowUShort:
            wrapper.unwraprowushort(idObj, ScilabUShortStackAllocator(pvApiCtx, pos));
            break;
        case MatUShort:
            wrapper.unwrapmatushort(idObj, ScilabUShortStackAllocator(pvApiCtx, pos));
            break;
        case SingleInt:
            wrapper.unwrapint(idObj, ScilabIntStackAllocator(pvApiCtx, pos));
            break;
        case RowInt:
            wrapper.unwraprowint(idObj, ScilabIntStackAllocator(pvApiCtx, pos));
            break;
        case MatInt:
            wrapper.unwrapmatint(idObj, ScilabIntStackAllocator(pvApiCtx, pos));
            break;
        case SingleUInt:
            wrapper.unwrapuint(idObj, ScilabUIntStackAllocator(pvApiCtx, pos));
            break;
        case RowUInt:
            wrapper.unwraprowuint(idObj, ScilabUIntStackAllocator(pvApiCtx, pos));
            break;
        case MatUInt:
            wrapper.unwrapmatuint(idObj, ScilabUIntStackAllocator(pvApiCtx, pos));
            break;
        case SingleLong:
            wrapper.unwraplong(idObj, ScilabLongStackAllocator(pvApiCtx, pos));
            break;
        case RowLong:
            wrapper.unwraprowlong(idObj, ScilabLongStackAllocator(pvApiCtx, pos));
            break;
        case MatLong:
            wrapper.unwrapmatlong(idObj, ScilabLongStackAllocator(pvApiCtx, pos));
            break;
        case SingleULong:
            wrapper.unwrapulong(idObj, ScilabULongStackAllocator(pvApiCtx, pos));
            break;
        case RowULong:
            wrapper.unwraprowulong(idObj, ScilabULongStackAllocator(pvApiCtx, pos));
            break;
        case MatULong:
            wrapper.unwrapmatulong(idObj, ScilabULongStackAllocator(pvApiCtx, pos));
            break;
        case SingleFloat:
            wrapper.unwrapfloat(idObj, ScilabFloatStackAllocator(pvApiCtx, pos));
            break;
        case RowFloat:
            wrapper.unwraprowfloat(idObj, ScilabFloatStackAllocator(pvApiCtx, pos));
            break;
        case MatFloat:
            wrapper.unwrapmatfloat(idObj, ScilabFloatStackAllocator(pvApiCtx, pos));
            break;
        case SingleComplex:
            wrapper.unwrapcomplex(idObj, ScilabComplexStackAllocator(pvApiCtx, pos));
            break;
        case RowComplex:
            wrapper.unwraprowcomplex(idObj, ScilabComplexStackAllocator(pvApiCtx, pos));
            break;
        case MatComplex:
            wrapper.unwrapmatcomplex(idObj, ScilabComplexStackAllocator(pvApiCtx, pos));
            break;
        default:
            return false;
    }

    return true;
}

int ScilabObjects::getEnvironmentId(int * addr, void * pvApiCtx)
{
    SciErr err;
    int row = 0, col = 0;
    int * envId = 0;

    err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_ENV_ID_POSITION, &row, &col, &envId);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    return *envId;
}

int ScilabObjects::getExternalId(int * addr, void * pvApiCtx)
{
    SciErr err;
    int row = 0, col = 0;
    int * id = 0;

    err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    return *id;
}

int ScilabObjects::getArgumentId(int * addr, int * tmpvars, const bool isRef, const bool isClass, const int envId, void * pvApiCtx)
{
    SciErr err;
    int typ, row = 0, col = 0, returnId;
    const ScilabAbstractEnvironmentWrapper & wrapper = ScilabEnvironments::getEnvironment(envId).getWrapper();

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        removeTemporaryVars(envId, tmpvars);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
    }

    if (isClass && typ != sci_mlist)
    {
        removeTemporaryVars(envId, tmpvars);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("External Class expected"));
    }

    switch (typ)
    {
        case sci_matrix :
        {
            double * mat = 0;

            if (isVarComplex(pvApiCtx, addr))
            {
                double * imag = 0;
                err = getComplexMatrixOfDouble(pvApiCtx, addr, &row, &col, &mat, &imag);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                returnId = wrap(row, col, mat, imag, wrapper, isRef);
            }
            else
            {
                err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &mat);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                returnId = wrap<double>(row, col, mat, wrapper, isRef);
            }

            tmpvars[++tmpvars[0]] = returnId;

            return returnId;
        }
        case sci_poly :
        {
            /* '$+1' should be handled to ease insertion/extraction */
            int nameLen = 5;
            char name[5];

            err = getPolyVariableName(pvApiCtx, addr, name, &nameLen);
            if (err.iErr)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            if (name[0] == '$' && nameLen == 1)
            {
                err = getMatrixOfPoly(pvApiCtx, addr, &row, &col, NULL, NULL);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                if (row * col != 1)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                int coefs;
                err = getMatrixOfPoly(pvApiCtx, addr, &row, &col, &coefs, NULL);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                // should be std::dynarray
                std::vector<double> mat = std::vector<double>(row * col * coefs);
                double* pMat = mat.data();
                err = getMatrixOfPoly(pvApiCtx, addr, &row, &col, &coefs, &pMat);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                returnId = wrapper.wrapPoly(row * col * coefs, mat.data());
            }
            else
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }
            return returnId;
        }
        case sci_ints :
        {
            int prec = 0;
            void * ints = 0;

            err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
            if (err.iErr)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            switch (prec)
            {
                case SCI_INT8 :
                    err = getMatrixOfInteger8(pvApiCtx, addr, &row, &col, (char**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<char>(row, col, static_cast<char *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_UINT8 :
                    err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, &row, &col, (unsigned char**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<unsigned char>(row, col, static_cast<unsigned char *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_INT16 :
                    err = getMatrixOfInteger16(pvApiCtx, addr, &row, &col, (short**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<short>(row, col, static_cast<short *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_UINT16 :
                    err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, &row, &col, (unsigned short**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<unsigned short>(row, col, static_cast<unsigned short *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_INT32 :
                    err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, (int**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<int>(row, col, static_cast<int *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_UINT32 :
                    err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, &row, &col, (unsigned int**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<unsigned int>(row, col, static_cast<unsigned int *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;

#ifdef __SCILAB_INT64__
                case SCI_INT64 :
                    err = getMatrixOfInteger64(pvApiCtx, addr, &row, &col, (long long**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<long long>(row, col, static_cast<long long *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
                case SCI_UINT64 :
                    err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, &row, &col, (unsigned long long**)(&ints));
                    if (err.iErr)
                    {
                        removeTemporaryVars(envId, tmpvars);
                        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                    }

                    returnId = wrap<unsigned long long>(row, col, static_cast<unsigned long long *>(ints), wrapper, isRef);
                    tmpvars[++tmpvars[0]] = returnId;
                    return returnId;
#endif
            }

            // invalid int code : should never be called
            removeTemporaryVars(envId, tmpvars);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
        }
        case sci_strings :
        {
            char ** matS = NULL;
            if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &matS))
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            returnId = wrap<char *>(row, col, matS, wrapper, isRef);
            freeAllocatedMatrixOfString(row, col, matS);
            tmpvars[++tmpvars[0]] = returnId;

            return returnId;
        }
        case sci_boolean :
        {
            int * matB;

            err = getMatrixOfBoolean(pvApiCtx, addr, &row, &col, &matB);
            if (err.iErr)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            returnId = wrapBool(row, col, matB, wrapper, isRef);
            tmpvars[++tmpvars[0]] = returnId;

            return returnId;
        }
        case sci_list :
        {
            int length;

            err = getListItemNumber(pvApiCtx, addr, &length);
            if (err.iErr)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            // empty list
            if (length <= 0)
            {
                return 0;
            }

            // should be std::dynarray
            std::vector<int> childrenIds = std::vector<int>(length + 1);

            // loop all over the items
            for (int i = 0; i < length; i++)
            {
                int* pvItem;

                err = getListItemAddress(pvApiCtx, addr, i + 1, &pvItem);
                if (err.iErr)
                {
                    removeTemporaryVars(envId, childrenIds.data());
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
                }

                getArgumentId(pvItem, childrenIds.data(), false, false, envId, pvApiCtx);
            }

            returnId = wrapper.wrapList(length, childrenIds.data());
            tmpvars[++tmpvars[0]] = returnId;

            return returnId;
        }
        case sci_mlist :
        {
            int * id = 0;
            int type = getMListType(addr, pvApiCtx);
            int eId = getEnvironmentId(addr, pvApiCtx);

            if (type == EXTERNAL_INVALID)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("External object expected"));
            }

            err = getMatrixOfInteger32InList(pvApiCtx, addr, EXTERNAL_OBJ_ID_POSITION, &row, &col, &id);
            if (err.iErr)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid variable: cannot retrieve the data"));
            }

            if (eId != envId && id != 0)
            {
                removeTemporaryVars(envId, tmpvars);
                throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Incompatible External object"));
            }

            if (isClass)
            {
                if (type == EXTERNAL_CLASS)
                {
                    return *id;
                }
                else
                {
                    removeTemporaryVars(envId, tmpvars);
                    throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("External Class expected"));
                }
            }

            if (type == EXTERNAL_OBJECT || type == EXTERNAL_CLASS)
            {
                return *id;
            }
            else if (type == EXTERNAL_VOID)
            {
                return -1;
            }

            break;
        }
        default :
        {
            removeTemporaryVars(envId, tmpvars);
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Unable to wrap. Unmanaged datatype (%d) ?"), typ);
        }
    }

    return -1;
}

int ScilabObjects::getMListType(int * mlist, void * pvApiCtx)
{
    types::InternalType* pVar = (types::InternalType*) mlist;

    //if (mlist[0] == 0)
    //{
    //    return EXTERNAL_VOID;
    //}

    if (!pVar->isMList())
    {
        return EXTERNAL_INVALID;
    }

    types::MList* pMlist = pVar->getAs<types::MList>();
    if (pMlist->getSize() != 3)
    {
        return EXTERNAL_INVALID;
    }

    types::String* pStrFieldNames = pMlist->getFieldNames();
    if (pStrFieldNames->getSize() != 3)
    {
        // first field is not a matrix 1x3 of strings
        return EXTERNAL_INVALID;
    }

    wchar_t* pwstMlistType = pStrFieldNames->get(0);
    if (wcslen(pwstMlistType) == strlen("_EClass") && wcscmp(pwstMlistType, L"_EClass") == 0)
    {
        return EXTERNAL_CLASS;
    }

    if (wcslen(pwstMlistType) == strlen("_EObj") && wcscmp(pwstMlistType, L"_EObj") == 0)
    {
        return EXTERNAL_OBJECT;
    }

    if (wcslen(pwstMlistType) == strlen("_EVoid") && wcscmp(pwstMlistType, L"_EVoid") == 0)
    {
        return EXTERNAL_VOID;
    }

    return EXTERNAL_INVALID;
}

bool ScilabObjects::isValidExternal(int * mlist, void * pvApiCtx)
{
    int type = getMListType(mlist, pvApiCtx);
    return type == EXTERNAL_OBJECT || type == EXTERNAL_CLASS;
}

bool ScilabObjects::isExternalObj(int * mlist, void * pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == EXTERNAL_OBJECT;
}

bool ScilabObjects::isExternalClass(int * mlist, void * pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == EXTERNAL_CLASS;
}

bool ScilabObjects::isExternalObjOrClass(int * mlist, void * pvApiCtx)
{
    int type = getMListType(mlist, pvApiCtx);
    return type == EXTERNAL_OBJECT || type == EXTERNAL_CLASS;
}

bool ScilabObjects::isExternalVoid(int * mlist, void * pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == EXTERNAL_VOID;
}

char * ScilabObjects::getSingleString(int pos, void * pvApiCtx)
{
    SciErr err;
    int * addr = 0;
    char * str = 0;

    err = getVarAddressFromPosition(pvApiCtx, pos, &addr);
    if (err.iErr)
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid String"));
    }

    if (!isStringType(pvApiCtx, addr))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("A single string expected"));
    }

    if (!isScalar(pvApiCtx, addr))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("A single String expected"));
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &str))
    {
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid String"));
    }

    return str;
}

int ScilabObjects::isPositiveIntegerAtAddress(int * addr, void * pvApiCtx)
{
    SciErr err;
    int typ = 0, row, col, prec;

    err = getVarDimension(pvApiCtx, addr, &row, &col);
    if (err.iErr)
    {
        return -1;
    }

    if (row != 1 || col != 1)
    {
        return -1;
    }

    err = getVarType(pvApiCtx, addr, &typ);
    if (err.iErr)
    {
        return -1;
    }

    if (typ == sci_ints)
    {
        err = getMatrixOfIntegerPrecision(pvApiCtx, addr, &prec);
        if (err.iErr)
        {
            return -1;
        }
        switch (prec)
        {
            case SCI_INT8:
            {
                char * cvalue = 0;
                err = getMatrixOfInteger8(pvApiCtx, addr, &row, &col, &cvalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*cvalue);
            }
            break;
            case SCI_UINT8:
            {
                unsigned char * ucvalue = 0;
                err = getMatrixOfUnsignedInteger8(pvApiCtx, addr, &row, &col, &ucvalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*ucvalue);
            }
            break;
            case SCI_INT16:
            {
                short * svalue = 0;
                err = getMatrixOfInteger16(pvApiCtx, addr, &row, &col, &svalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*svalue);
            }
            break;
            case SCI_UINT16:
            {
                unsigned short * usvalue = 0;
                err = getMatrixOfUnsignedInteger16(pvApiCtx, addr, &row, &col, &usvalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*usvalue);
            }
            break;
            case SCI_INT32:
            {
                int * ivalue = 0;
                err = getMatrixOfInteger32(pvApiCtx, addr, &row, &col, &ivalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*ivalue);
            }
            break;
            case SCI_UINT32:
            {
                unsigned int * uivalue = 0;
                err = getMatrixOfUnsignedInteger32(pvApiCtx, addr, &row, &col, &uivalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*uivalue);
            }
            break;
#ifdef __SCILAB_INT64__
            case SCI_INT64:
            {
                long long * llvalue = 0;
                err = getMatrixOfInteger64(pvApiCtx, addr, &row, &col, &llvalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*llvalue);
            }
            break;
            case SCI_UINT64:
            {
                unsigned long long * ullvalue = 0;
                err = getMatrixOfUnsignedInteger64(pvApiCtx, addr, &row, &col, &ullvalue);
                if (err.iErr)
                {
                    return -1;
                }
                return (int)(*ullvalue);
            }
            break;
#endif
            default:
                return -1;
        }
    }
    else if (typ == sci_matrix)
    {
        double * dvalue = 0;

        if (isVarComplex(pvApiCtx, addr))
        {
            return -1;
        }

        err = getMatrixOfDouble(pvApiCtx, addr, &row, &col, &dvalue);
        if (err.iErr)
        {
            return -1;
        }

        if (*dvalue - (double)(int)(*dvalue) == 0.0)
        {
            return (int)(*dvalue);
        }
    }

    return -1;
}
}
