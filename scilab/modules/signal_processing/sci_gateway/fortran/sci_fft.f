c     =======================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     =======================================
      subroutine scifft(fname)
c     implicit undefined (a-z)
      include 'stack.h'
      character*(*) fname
      double precision adelp,adels,fact,zzi(16),zzr(16),zpi(16),zpr(16)
      double precision alpha,beta,u,y,eps,eps1,v
      double precision kappa,lambda,mu,nu
      integer ordre,lw,lt,li,lr,lo,lf,lg
      integer i,top2
      logical vect,arma
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      if(rhs.ne.1.and.rhs.ne.2.and.rhs.ne.4) then
         call error(39)
         return
      endif
      ierr=0
      vect=.false.
      lbot=lstk(bot)
      top2=top-rhs+1
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         call funnam(ids(1,pt+1),'fft',iadr(lstk(top2)))
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      mn=m*n
      it=istk(il+3)
      lr=sadr(il+4)
      vect=m.eq.1.or.n.eq.1


      if(rhs.ge.4) goto 22
c
c fft 
c
      if(mn.le.1) then
         top=top-rhs+1
         return
      endif

      if(rhs.eq.2) then
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=2
            call error(53)
            return
         endif
         l1=sadr(il1+4)
         isn=nint(stk(l1))
         if(isn.ne.1.and.isn.ne.-1) then
            err=2
            call error(36)
            return
         endif
         top=top-1
      else
c     .  fft(a) is equivalent to fft(a,-1)
         isn=-1
      endif
 
      li=lr+mn
      libre=li+mn
      lw=lbot-libre-1
      err=libre-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(it.eq.0) then
c     .  create imaginary part and set it to zero
         call dset(mn,0.0d+0,stk(li),1)
         istk(il+3)=1
      endif

      if(vect) then
c     .  fft of a vector
         mn2=2**int(log(dble(mn)+0.5d0)/log(2.0d+0))
         if(mn2.eq.mn) then
            if(mn.le.2**15) then
               call fft842(isn,mn,stk(lr),stk(li),ierr)
c     .        ierr should be always 0
            else
               call dfft2(stk(lr),stk(li),1,mn,1,isn,ierr,stk(libre),lw)
            endif
         else
            call dfft2(stk(lr),stk(li),1,mn,1,isn,ierr,stk(libre),lw)
         endif
      else
c     .  2 dimensionnal fft
cxx         call dfft2(stk(lr),stk(li),n,m,1,isn,ierr,stk(libre),lw)
cxx         if(ierr.lt.0) goto 21
cxx         call dfft2(stk(lr),stk(li),1,n,m,isn,ierr,stk(libre),lw)



         m2=2**int(log(dble(m)+0.5d0)/log(2.0d+0))
         n2=2**int(log(dble(n)+0.5d0)/log(2.0d+0))
c     .  first call ()
c     .  If m2 is a power of two and less than m<2^15 use fft842 for efficiency 
         if(m2.eq.m) then
            if(m.le.2**15) then
               do 25 i=1,n
                  call fft842(isn,m,stk(lr+m*(i-1)),stk(li+m*(i-1)),
     $                 ierr)
 25            continue
            else
               call dfft2(stk(lr),stk(li),n,m,1,isn,ierr,stk(libre),lw)
               if(ierr.lt.0) goto 21
            endif
         else
            call dfft2(stk(lr),stk(li),n,m,1,isn,ierr,stk(libre),lw)
            if(ierr.lt.0) goto 21
         endif
c     .  second call ()
         if(2*n.le.lw) then
            if(n2.eq.n) then
               if(n.le.2**15) then
                  ltempr=libre
                  ltempi=libre+n
                  do 26 i=1,m
c     .              compute the fft on each line of the matrix
                     call dcopy(n,stk(lr+(i-1)),m,stk(ltempr),1)
                     call dcopy(n,stk(li+(i-1)),m,stk(ltempi),1)
                     call fft842(isn,n,stk(ltempr),stk(ltempi),ierr)
                     if(ierr.lt.0) goto 21
                     call dcopy(n,stk(ltempr),1,stk(lr+(i-1)),m)
                     call dcopy(n,stk(ltempi),1,stk(li+(i-1)),m)
 26               continue
               else
                  call dfft2(stk(lr),stk(li),1,n,m,isn,ierr,stk(libre)
     $                 ,lw)
                  if(ierr.lt.0) goto 21
               endif
            else
               call dfft2(stk(lr),stk(li),1,n,m,isn,ierr,stk(libre),lw)
               if(ierr.lt.0) goto 21
            endif
         else
            call dfft2(stk(lr),stk(li),1,n,m,isn,ierr,stk(libre),lw)
            if(ierr.lt.0) goto 21
         endif 


      endif
 21   if(ierr.lt.0) then
         buf='fft fails by lack of memory'
         call error(999)
         return
      endif
      lstk(top+1)=li+mn
      return

c     n dimensional fft
 22   continue

      il1=iadr(lstk(top2+1))
      if(istk(il1).ne.1) then
         err=2
         call error(53)
         return
      endif
      l1=sadr(il1+4)
      isn=nint(stk(l1))
      if(isn.ne.1.and.isn.ne.-1) then
         err=2
         call error(36)
         return
      endif

      ilinc=iadr(lstk(top))
      if (istk(ilinc).ne.1) then  
         err=4
         call error(53)
         return
      endif
      linc=sadr(ilinc+4)
      nspn=int(stk(linc))
      if (nspn.eq.0) then  
         err=4
         call error(36)
         return
      endif
      top=top-1
      iln=iadr(lstk(top))
      ln=sadr(iln+4)
      n=int(stk(ln))
      li=lr+mn
      err=li-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         err=li+mn-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn,0.0d+0,stk(li),1)
         istk(il+3)=1
      endif
      libre=li+mn
      lw=lbot-libre-1
      nseg=mn/n/nspn
      call dfft2(stk(lr),stk(li),nseg,n,nspn,isn,ierr,stk(libre),lw)
      top=top2
      lstk(top+1)=li+mn
      return
      end
c     ========================================
