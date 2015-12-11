c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1996 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine wipowe(vr,vi,p,rr,ri,ierr)
c!purpose
c     computes v^p with v complex and p integer
c!calling sequence 
c     subroutine wipowe(vr,vi,p,rr,ri,ierr)
c     integer ierr
c     double precision vr,vi,p,rr,ri
c     vr   : real part of v
c     vi   : imaginary part of v
c     rr   : result's real part
c     ri   : result's imaginary part
c     ierr : error flag
c            ierr=0 if ok
c            ierr=1 if 0**0
c            ierr=2 if  0**k with k<0
c!origin
c Serge Steer INRIA 1996
c
      integer p,ierr
      double precision vr,vi,xr,xi,rr,ri,infinity
c
      ierr=0
c
      if(p.eq.0) then
         rr=1.0d+0
         ri=0.0d+0
      elseif(p.lt.0) then
         if(abs(vr)+abs(vi).ne.0.0d+0) then
            call wdiv(1.0d+0,0.0d+0,vr,vi,rr,ri)
            xr=rr
            xi=ri
            do 10 k=2,abs(p)
               call wmul(xr,xi,rr,ri,rr,ri)
 10         continue
         else
            ri=0.0d0
            rr=infinity(ri)
            ierr=2
         endif
      else
         rr=vr
         ri=vi
         xr=rr
         xi=ri
         do 20 k=2,abs(p)
            call wmul(xr,xi,rr,ri,rr,ri)
 20      continue
      endif
      return
      end
