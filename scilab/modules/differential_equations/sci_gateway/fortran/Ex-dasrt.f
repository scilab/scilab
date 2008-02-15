c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     Examples for dasrt 
c     ----------------------
c
c
      subroutine gr1 (neq, t, y, ng, groot, rpar, ipar)
      INTEGER neq, ng,ipar(*)
      DOUBLE PRECISION t, y(*), groot(*),rpar(*)
      groot(1) = ((2.0D0*LOG(y(1)) + 8.0D0)/t - 5.0D0)*y(1)
      groot(2) = LOG(y(1)) - 2.2491D0
      RETURN
      END

      subroutine gr2 (neq, t, y, ng, groot, rpar, ipar)
      INTEGER neq, ng, ipar(*)
      DOUBLE PRECISION t, y, groot,rpar(*)
      DIMENSION y(*), groot(*)
      groot(1) = y(1)
      RETURN
      END

