c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1996 - INRIA - Serge STEER
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
