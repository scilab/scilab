/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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
/*--------------------------------------------------------------------------*/
/*
 * Provide several wrappers for Scilab's matrix allocation
 * Mainly used in unwrap.hpp
 */
/*--------------------------------------------------------------------------*/
#ifndef __JIMS__WRAPUNWRAP_H__
#define __JIMS__WRAPUNWRAP_H__

#ifdef _MSC_VER
//#include <Windows.h>
#endif

#define __JIMS_getmethod__(Kind, Type) static const char * get##Kind##MethodName() \
    {                                                                   \
        return "unwrap" #Kind #Type;                                    \
    };

#define __JIMS_getsignature__(Kind, Signature) static const char * get##Kind##MethodSignature() \
    {                                                                   \
        return "(I)" Signature;                                         \
    };

#define __JIMS_getvariableptr__() static T get##Kind##MethodSignature() \
    {                                                                   \
        return "(I)" Signature;                                         \
    };

/*
 * Structs defined to make easier the unwrapping
 */
#define __JIMS_wrapToUnwrapObject__(JType, Type, type, Signature) struct __JIMS__Scilab##JType##__ { \
        static SciErr allocMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, void **_pdblReal) \
            {                                                           \
                return allocMatrixOf##Type(_pvCtx, _iVar, _iRows, _iCols, (type**) _pdblReal); \
            };                                                          \
        static type getSingleVar(JNIEnv * curEnv, jclass cls, jmethodID id, int javaID) \
            {                                                           \
                return static_cast<type>(curEnv->CallStatic##JType##Method(cls, id, javaID)); \
            };                                                          \
        __JIMS_getmethod__(,JType)                                      \
        __JIMS_getsignature__(,#Signature)                              \
        __JIMS_getmethod__(Row,JType)                                   \
        __JIMS_getsignature__(Row,"[" #Signature)                       \
        __JIMS_getmethod__(Mat,JType)                                   \
        __JIMS_getsignature__(Mat,"[[" #Signature)                      \
    };

__JIMS_wrapToUnwrapObject__(Double, Double, double, D)
__JIMS_wrapToUnwrapObject__(Boolean, Boolean, int, Z)
__JIMS_wrapToUnwrapObject__(Byte, Integer8, char, B)
__JIMS_wrapToUnwrapObject__(Short, Integer16, short, S)
__JIMS_wrapToUnwrapObject__(Char, UnsignedInteger16, unsigned short, C)
__JIMS_wrapToUnwrapObject__(Int, Integer32, int, I)
__JIMS_wrapToUnwrapObject__(Float, Double, double, F)

#ifdef __SCILAB_INT64__
__JIMS_wrapToUnwrapObject__(Long, Integer64, long, J)
#else
__JIMS_wrapToUnwrapObject__(Long, UnsignedInteger32, unsigned int, J)
#endif

/*
 * Structs defined to make easier the wrapping
 */
#define __JIMS_wrapToWrapObject__(Type,type) struct __JIMS__ScilabAnOtherWrapper##Type##__ { \
        static int wrap(JavaVM *vm, type x)                             \
            {                                                           \
                return org_scilab_modules_external_objects_java::ScilabJavaObject::wrap##Type(vm, x); \
            };                                                          \
        static int wrap(JavaVM *vm, type *x, int len)                   \
            {                                                           \
                return org_scilab_modules_external_objects_java::ScilabJavaObject::wrap##Type(vm, x, len); \
            };                                                          \
        static int wrap(JavaVM *vm, type **xx, int r, int c)            \
            {                                                           \
                return org_scilab_modules_external_objects_java::ScilabJavaObject::wrap##Type(vm, xx, r, c); \
            };                                                          \
    };

__JIMS_wrapToWrapObject__(Double, double)
__JIMS_wrapToWrapObject__(Int, int)
__JIMS_wrapToWrapObject__(Byte, byte)
__JIMS_wrapToWrapObject__(Short, short)
__JIMS_wrapToWrapObject__(Char, unsigned short)
__JIMS_wrapToWrapObject__(UInt, long long)
__JIMS_wrapToWrapObject__(UByte, short)
__JIMS_wrapToWrapObject__(UShort, int)
__JIMS_wrapToWrapObject__(Boolean, bool)
__JIMS_wrapToWrapObject__(Float, float)
__JIMS_wrapToWrapObject__(Long, long long)

#endif //__JIMS__WRAPUNWRAP_H__
/*--------------------------------------------------------------------------*/
