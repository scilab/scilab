*     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*     Copyright (C) INRIA
*
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.

*     various constant used in somespline.f
      integer    NOT_A_KNOT,   NATURAL,   CLAMPED,   PERIODIC
      parameter (NOT_A_KNOT=0, NATURAL=1, CLAMPED=2, PERIODIC=3)
      integer    FAST,   FAST_PERIODIC,   MONOTONE,   BY_ZERO
      parameter (FAST=4, FAST_PERIODIC=5, MONOTONE=6, BY_ZERO=7)
      integer    C0,   LINEAR,    BY_NAN
      parameter (C0=8, LINEAR=9,  BY_NAN=10)
