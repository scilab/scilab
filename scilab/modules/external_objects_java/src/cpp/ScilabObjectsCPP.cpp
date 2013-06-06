/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* ------------------------------------------------------------------------- */
#include <string.h>
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "OptionsHelper.hxx"
#include "ScilabObjects.hxx"
#include "ScilabClassLoader.hxx"
#include "ScilabJavaClass.hxx"
//#include "ScilabJavaObjectHelper.hxx"
#include "ScilabJavaArray.hxx"
#include "ScilabJavaCompiler.hxx"
#include "GiwsException.hxx"
#include "NoMoreScilabMemoryException.hxx"
//#include "wrap.hpp"
#include "wrapwithcast.hpp"
//#include "unwrap.hpp"
#include "WrapAsDirectBufferTemplate.hpp"
/* ------------------------------------------------------------------------- */
using namespace org_scilab_modules_external_objects_java;
/* ------------------------------------------------------------------------- */
/*
 * Syntax: wrapForType(type, Type)
 * type: the Scilab data type
 * Type: used for the function name
 */
wrapForType(double, Double)
wrapForType(int, Int)
wrapForType(byte, Byte)
wrapForType(short, Short)
wrapForType(unsigned short, Char)

#ifdef __SCILAB_INT64__
wrapForType(long long, Long)
#endif
/* ------------------------------------------------------------------------- */
/*
 * Syntax: wrapWithCastForType(type, castType, Type)
 * type: the Scilab data type
 * castType: datas are copied and casted (into castType)
 * Type: used for the function name
 */
wrapWithCastForType(unsigned int, long long, UInt)
wrapWithCastForType(unsigned char, short, UByte)
wrapWithCastForType(unsigned short, int, UShort)
wrapWithCastForType(int, bool, Boolean)
wrapWithCastForType(double, float, Float)
/* ------------------------------------------------------------------------- */
/*
 * Syntax: unwrapForType(type,ScilabType,JNIType,Type)
 * type: used for the function name (unwrap##type(...))
 * ScilabType: the type in Scilab
 * JNIType: the type in JNI
 * Type: used for the functions wrapper
 */
unwrapForType(double, double, jdouble, Double)
unwrapForType(int, int, jint, Int)
unwrapForType(boolean, int, jboolean, Boolean)
unwrapForType(byte, char, jbyte, Byte)
unwrapForType(short, short, jshort, Short)
unwrapForType(char, unsigned short, jchar, Char)
unwrapForType(long, long, jlong, Long)
unwrapForType(float, double, jfloat, Float)
/* ------------------------------------------------------------------------- */

wrapIntoDirectBuffer(Double, double)
wrapIntoDirectBuffer(Int, int)
wrapIntoDirectBuffer(Char, char)
wrapIntoDirectBuffer(Short, short)
wrapIntoDirectBuffer(Long, long)
wrapIntoDirectBuffer(Byte, byte)
wrapIntoDirectBuffer(Float, float)
/* ------------------------------------------------------------------------- */
void unwrapstring(int id, int pos, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::unwrapString(vm, id, pos);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
void unwraprowstring(int id, int pos, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::unwrapRowString(vm, id, pos);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
void unwrapmatstring(int id, int pos, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::unwrapMatString(vm, id, pos);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
void initscilabjavaobject(char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObject::initScilabJavaObject(vm);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
void garbagecollect(char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObject::garbageCollect(vm);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }
}
/* ------------------------------------------------------------------------- */
int createjavaarray(char *className, int* dims, int len, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaArray::newInstance(vm, className, dims, len);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int loadjavaclass(char *className, char allowReload, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabClassLoader::loadJavaClass(vm, className, getAllowReload() == 0 ? false : true);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
char* getrepresentation(int id, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            char *rep = ScilabJavaObject::getRepresentation(vm, id);
            char *returnedRep = NULL;
            if (rep)
            {
                returnedRep = strdup(rep);
            }
            return returnedRep;
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return NULL;
        }
    }

    return NULL;
}
/* ------------------------------------------------------------------------- */
int isvalidjavaobject(int id)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        bool b = ScilabJavaObject::isValidJavaObject(vm, id);
        return b ? 1 : 0;
    }

    return 0;
}
/* ------------------------------------------------------------------------- */
int newinstance(int id, int *args, int argsSize, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaClass::newInstance(vm, id, args, argsSize);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int invoke(int id, char *methodName, int *args, int argsSize, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::invoke(vm, id, methodName, args, argsSize);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
void setfield(int id, char *fieldName, int idarg, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::setField(vm, id, fieldName, idarg);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return;
        }
    }

    return;
}
/* ------------------------------------------------------------------------- */
int getfield(int id, char *fieldName, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::getField(vm, id, fieldName);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int getfieldtype(int id, char *fieldName, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::getFieldType(vm, id, fieldName);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int getarrayelement(int id, int *index, int length, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::getArrayElement(vm, id, index, length);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
void setarrayelement(int id, int *index, int length, int idArg, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObject::setArrayElement(vm, id, index, length, idArg);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }
}
/* ------------------------------------------------------------------------- */
int javacast(int id, char *objName, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::javaCast(vm, id, objName);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int javacastwithid(int id, int classId, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::javaCast(vm, id, classId);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
            return -1;
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
void removescilabjavaobject(int id)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        ScilabJavaObject::removeScilabJavaObject(vm, id);
    }
}
/* ------------------------------------------------------------------------- */
void getaccessiblemethods(int id, int pos, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::getMethodResult(vm, "getAccessibleMethods", id, pos);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
void getaccessiblefields(int id, int pos, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::getMethodResult(vm, "getAccessibleFields", id, pos);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
}
/* ------------------------------------------------------------------------- */
char* getclassname(int id, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return strdup(ScilabJavaObject::getClassName(vm, id));
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
        catch (org_scilab_modules_external_objects_java::NoMoreScilabMemoryException e)
        {
            *errmsg = strdup(e.what());
        }
    }
    return NULL;
}
/* ------------------------------------------------------------------------- */
int wrapSingleString(char *x)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        return ScilabJavaObject::wrapString(vm, x);
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int wrapRowString(char **x, int len)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        return ScilabJavaObject::wrapString(vm, x, len);
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int wrapMatString(char **x, int r, int c)
{
    JavaVM *vm = getScilabJavaVM();
    if (vm)
    {
        if (getMethodOfConv())
        {
            char ***xx = new char**[r];
            int i, j, k;
            for (i = 0; i < r; i++)
            {
                xx[i] = new char*[c];
                for (j = 0; j < c; j++)
                {
                    int len = strlen(x[j * r + i]) + 1;
                    xx[i][j] = new char[len];
                    memcpy(xx[i][j], x[j * r + i], len);
                }
            }
            k = ScilabJavaObject::wrapString(vm, xx, r, c);
            for (i = 0; i < r; i++)
            {
                for (j = 0; j < c; j++)
                {
                    delete [] xx[i][j];
                }
                delete [] xx[i];
            }
            delete [] xx;
            return k;
        }
        else
        {
            char ***xx = new char**[c];
            int i;
            xx[0] = x;
            for (i = 1; i < c; xx[i] = xx[i++ - 1] + r)
            {
                ;
            }
            i = ScilabJavaObject::wrapString(vm, xx, c, r);
            delete [] xx;
            return i;
        }
    }
    return -1;
}
/* ------------------------------------------------------------------------- */
int isunwrappable(int id, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaObject::isUnwrappable(vm, id);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
int compilecode(char *className, char **code, int size, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            return ScilabJavaCompiler::compileCode(vm, className, code, size);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }

    return -1;
}
/* ------------------------------------------------------------------------- */
void releasedirectbuffer(void **ref, int* id, int len, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObjectHelper::releaseDirectBuffer(vm, ref, id, len);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }
}
/* ------------------------------------------------------------------------- */
void enabletrace(char *filename, char **errmsg)
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        try
        {
            ScilabJavaObject::enableTrace(vm, filename);
        }
        catch (GiwsException::JniException e)
        {
            *errmsg = strdup(e.getJavaDescription().c_str());
        }
    }
}
/* ------------------------------------------------------------------------- */
void disabletrace()
{
    JavaVM *vm = getScilabJavaVM ();
    if (vm)
    {
        ScilabJavaObject::disableTrace(vm);
    }
}
/* ------------------------------------------------------------------------- */
