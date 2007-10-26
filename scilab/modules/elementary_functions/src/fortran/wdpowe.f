      subroutine wdpowe(vr,vi,p,rr,ri,ierr)
c!purpose
c     computes v^p with v complex and p real
c!calling sequence 
c     subroutine wdpowe(vr,vi,p,rr,ri,ierr)
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
c!

c     Copyright INRIA
      integer ierr
      double precision vr,vi,p,sr,si,rr,ri,infinity
c
      ierr=0
c
      if(dble(int(p)).eq.p) then 
         call wipowe(vr,vi,int(p),rr,ri,ierr)
      else
         if(abs(vr)+abs(vi).ne.0.0d+0) then
            call wlog(vr,vi,sr,si)
            sr=exp(sr*p)
            si=si*p
            rr=sr*cos(si)
            ri=sr*sin(si)
         else
            if(p.gt.0.0d+0) then
               rr=0.0d+0
               ri=0.0d+0
            elseif(p.lt.0.0d+0) then
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
