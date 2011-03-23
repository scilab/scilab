c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine simple(n,d,s)
c
      double precision d(*)
      real s(*),rmax,slamch
c
c      slamch return wrong value under MacOSX
c      slamch('o') <=> HUGE(ZERO)
c      http://www.netlib.org/lapack/util/slamch.f
       rmax=HUGE(0.0E+0)
c
      do 10 i=1,n
      if(abs(d(i)).gt.rmax) then
        s(i)=real(sign(dble(rmax),d(i)))
      else
        s(i)=real(d(i))
      endif
   10 continue
      return
      end
