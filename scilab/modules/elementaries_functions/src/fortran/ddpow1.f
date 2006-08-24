      subroutine ddpow1(n,v,iv,p,ip,rr,ri,ir,ierr,iscmpl)
c!purpose
c     computes V^P with V and P real vectors
c!calling sequence
c     subroutine ddpow1(n,v,iv,p,ip,rr,ri,ir,ierr,iscmpl)
c     integer n,iv,ip,ir,ierr,iscmpl
c     double precision v(*),p(*),rr(*),ri(*)
c
c     n    : number of elements of V and P vectors
c     v    : array containing V elements V(i)=v(1+(i-1)*iv)
c     iv   : increment between two V elements in v (may be 0)
c     p    : array containing P elements P(i)=p(1+(i-1)*ip)
c     ip   : increment between two P elements in p (may be 0)
c     rr   : array containing real part of the results vector R:
c            real(R(i))=rr(1+(i-1)*ir)
c     ri   : array containing imaginary part of the results vector R:
c            imag(R(i))=ri(1+(i-1)*ir)
c     ir   : increment between two R elements in rr and ri
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
      integer n,iv,ierr,ierr1
      double precision v(*),p(*),rr(*),ri(*)
c
      ierr=0
      iscmpl=0
c

      ii=1
      iip=1
      iir=1
      do 20 i=1,n
         call ddpowe(v(ii),p(iip),rr(iir),ri(iir),ierr1,isc)
         ierr=max(ierr,ierr1)
c         if(ierr.ne.0) return
         iscmpl=max(iscmpl,isc)
         ii=ii+iv
         iip=iip+ip
         iir=iir+ir
 20   continue
c
      return
      end
