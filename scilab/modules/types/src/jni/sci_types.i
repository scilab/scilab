/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
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
