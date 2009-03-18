/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* @TODO : Add explanation about this stuff */


static void init(){}
static void fini(){}
static void preinit(){}
typedef void (*fp)();
fp __init_array_start[]={init};
fp __init_array_end[]={(init)+1};
fp __fini_array_start[]={fini};
fp __fini_array_end[]={fini+1};

fp __preinit_array_start[]={preinit};
fp __preinit_array_end[]={preinit+1};
