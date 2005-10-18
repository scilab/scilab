      subroutine mpddpowe(v,p,rr,ri,ierr,iscmpl)
      include 'mpstack.h'
c!purpose
c     computes v^p with p and v double preision
c!calling sequence
c     subroutine ddpowe(v,p,rr,ri,ierr,iscmpl)
c     integer ierr
c     double precision v,p,rr,ri
c
c     rr   : result's real part
c     ri   : result's imaginary part
c     ierr : error flag
c            ierr=0 if ok
c            ierr=1 if 0**0
c            ierr=2 if  0**k with k<0
c     iscmpl :
c            iscmpl=0 if result is real
c            iscmpl=1 if result is complex
c!origin
c Serge Steer INRIA 1996
c!
c     Copyright INRIA
      integer ierr
      double precision v,p,sr,si,rr,ri
c     
      ierr=0
      iscmpl=0
c     
      call mpdipowe(v,p,rr,ierr)
      ri=PCZERO
c     
      return
      end
