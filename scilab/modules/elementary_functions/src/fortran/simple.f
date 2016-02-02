c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
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
