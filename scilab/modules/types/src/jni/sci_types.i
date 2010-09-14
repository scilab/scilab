/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* sci_types.i */
/**
 * Windows: swig -java -package org.scilab.modules.types -outdir ../java/org/scilab/modules/types/ sci_types.i 
 * Other: Use the option --enable-build-swig to the configure
*/

%include "enums.swg"
%javaconst(1);
// Rename the enum datatype
%rename(ScilabTypeEnum) sci_types;
%rename(ScilabIntegerTypeEnum) sci_int_types;

%module types
%{
#include "../../../core/includes/sci_types.h"
%}
%include "../../../core/includes/sci_types.h"
