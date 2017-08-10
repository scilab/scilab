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
      subroutine wwpowe(vr,vi,pr,pi,rr,ri,ierr)
c!purpose
c     computes v^p with v complex and p complex
c!calling sequence 
c     subroutine wdpowe(vr,vi,p,rr,ri,ierr)
c     integer ierr
c     double precision vr,vi,pr,pi,rr,ri
c     vr   : real part of v
c     vi   : imaginary part of v
c     pr   : real part of p
c     pi   : imaginary part of p
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
      double precision vr,vi,pr,pi,sr,si,rr,ri,infinity
c
      ierr=0
c     
      if(pi.eq.0.0d+0) then
         call wdpowe(vr,vi,pr,rr,ri,ierr)
      else
         if(abs(vr)+abs(vi).ne.0.0d+0) then
            call wlog(vr,vi,sr,si)
            call wmul(sr,si,pr,pi,sr,si)
            sr=exp(sr)
            rr=sr*cos(si)
            ri=sr*sin(si)
         else
            ri=0.0d0
            rr=infinity(ri)
            ierr=2
         endif
      endif
c     
      return
      end
