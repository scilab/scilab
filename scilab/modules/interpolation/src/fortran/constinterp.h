*     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*     Copyright (C) INRIA
*     
*     This file must be used under the terms of the CeCILL.
*     This source file is licensed as described in the file COPYING, which
*     you should have received as part of this distribution.  The terms
*     are also available at
*     http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

*     various constant used in somespline.f
      integer    NOT_A_KNOT,   NATURAL,   CLAMPED,   PERIODIC
      parameter (NOT_A_KNOT=0, NATURAL=1, CLAMPED=2, PERIODIC=3)
      integer    FAST,   FAST_PERIODIC,   MONOTONE,   BY_ZERO
      parameter (FAST=4, FAST_PERIODIC=5, MONOTONE=6, BY_ZERO=7)
      integer    C0,   LINEAR,    BY_NAN
      parameter (C0=8, LINEAR=9,  BY_NAN=10)
