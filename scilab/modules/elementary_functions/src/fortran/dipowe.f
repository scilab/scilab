c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1996 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
      subroutine dipowe(v,p,r,ierr)
c!purpose
c     computes r=v^p where v double precision and b integer
c!calling sequence
c     subroutine dipowe(v,p,r,ierr)
c     integer p ,ierr
c     double precision v,r
c
c     ierr : error flag
c            ierr=0 if ok
c            ierr=1 if 0**0
c            ierr=2 if  0**k with k<0
c
      integer p ,ierr
      double precision v,r,infinity
c
      ierr=0
c
      if(p.eq.1) then
         r=v
      elseif(p.eq.0) then
c     .  v^0
c         if(v.ne.0.0d+0) then
            r=1.0d+0
c         else

c            ierr=1
c         endif
      else if(p.lt.0) then
         if(v.ne.0.0d+0) then
            r=v**p
         else
            r=0.0d0
            r=infinity(r)
            ierr=2
         endif
      else
         r=v**p
      endif
c     
      return
      end
