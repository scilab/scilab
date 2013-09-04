c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1996 - INRIA - Serge STEER
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine dwpowe(v,pr,pi,rr,ri,ierr)
c!purose
c     computes v^p with v double precision and p complex
c!calling sequence
c     subroutine dwpowe(v,pr,pi,rr,ri,ierr)
c     integer ierr
c     double precision v,pr,pi,rr,ri
c
c     pr   : exponent real part
c     pi   : exponent imaginary part
c     rr   : result's real part
c     ri   : result's imaginary part
c     ierr : error flag
c            ierr=0 if ok
c            ierr=1 if 0**0
c            ierr=2 if  0**k with k<0
c!origin
c Serge Steer INRIA 1996
c
      integer ierr
      double precision v,pr,pi,sr,si,rr,ri,infinity
c     
      ierr=0
c     
      if(pi.eq.0.0d+0) then
c     p real
         call ddpowe(v,pr,rr,ri,ierr,iscmpl)
      else
         if(v.ne.0.0d+0) then
            call wlog(v,0.0d0,sr,si)
            call wmul(sr,si,pr,pi,sr,si)
            sr=exp(sr)
            rr=sr*cos(si)
            ri=sr*sin(si)
         else
            if(pr.gt.0.0d+0) then
               rr=0.0d+0
               ri=0.0d+0
            elseif(pr.lt.0.0d+0) then
               ri=0.0d+0
               rr=infinity(ri)
               ierr=2
            else
               rr=1.0d+0
               ri=0.0d+0
            endif
         endif
      endif
c     
      return
      end
