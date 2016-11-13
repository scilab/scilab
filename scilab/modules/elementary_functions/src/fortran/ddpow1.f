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
c
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
