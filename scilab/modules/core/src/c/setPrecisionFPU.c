/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
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

#if defined(linux) && defined(__i386__)
#include <fpu_control.h>
#include "setPrecisionFPU.h"
/*
 * This function is an standalone function which aims to see
 * if the bug 3443 ( http://bugzilla.scilab.org/show_bug.cgi?id=3443 )
 * is present.
 * As far as we know, this bug is only present under Linux 32 bits
 * Basically, the Java/JNI function JNI_CreateJavaVM which is changing the
 * Precision Control (PC) of the FPU.
  * Then, the goal of this function is:
 * - Check the bug exist
 * - If it does exist, change the PC of the FPU
 * - Check if the bug is actually fixed.
 * - If not fixed, you are screwed and we would like to the know your
 *   configuration
 *
 * Note that this function is not used because the first case is always true
 * (which is not the case in a stand alone code)
 *
 * For more information on the subject:
 * http://wiki.scilab.org/Scilab_precision
 * http://www.vinc17.org/research/extended.en.html
 */

/* Set the FPU to 53 bits... */
void setFPUToDouble(void)
{
    fpu_control_t _cw;
    _FPU_GETCW(_cw);
    _cw = (_cw & ~_FPU_EXTENDED) | _FPU_DOUBLE;
    _FPU_SETCW(_cw);
}


/* Set the FPU to 64 bits... It is the expected behaviour of a 32 bits Linux */
void setFPUToExtended(void)
{
    fpu_control_t _cw;
    //printf("Changing precision\n");
    _FPU_GETCW(_cw);
    _cw = (_cw & ~_FPU_DOUBLE) | _FPU_EXTENDED;
    _FPU_SETCW(_cw);
}

#endif
