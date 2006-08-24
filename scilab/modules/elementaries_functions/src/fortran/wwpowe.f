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
c!
c     Copyright INRIA
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
