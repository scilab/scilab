/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __RETURNTYPE_H__
#define __RETURNTYPE_H__

enum _ReturnType_ {
    jni_string,
    jni_string_vector,
    jni_double,
    jni_double_vector,
    jni_int,
    jni_int_vector,
    jni_bool,
    jni_bool_vector
};

#endif /* !__RETURNTYPE_H__ */
