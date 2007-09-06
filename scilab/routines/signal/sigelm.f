      subroutine sigelm
c     Copyright INRIA
c ================================== ( Inria    ) =============
c     basic signal processing routines
      include '../stack.h'
c
      double precision adelp,adels,fact,zzi(16),zzr(16),zpi(16),zpr(16)
      double precision alpha,beta,u,y,eps,eps1,v
      double precision kappa,lambda,mu,nu
      integer ordre,lw,lt,li,lr,lo,lf,lg
      integer i,top2
      integer iadr,sadr
c
      logical vect,arma
c
c functions
c
c    0    1      2      3      4      5      6      7      8      9
c 0     ffir   fft   fiir   corr   rpem   amell  delip  remez  syredi
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' sigelm '//buf(1:4))
      endif
c
      lbot=lstk(bot)
      goto(10,20,30,40,50,60,70,80,90)fin
c
c     filtres a reponse impulsionnelle finie  c
 10   continue
c
      return
c
c fft : transformee de  fourier rapide
c ---
c
 20   continue
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
c     ========================================
c
 30   continue
c
c
      return
 40   continue
      call scicorr("corr")
      return
c
 50   continue
      arma=.false.
      top2=top-rhs+1
      il=iadr(lstk(top2))
      if(istk(il).ne.15) then
         err=1
         call error(56)
         return
      endif
      if(istk(il+1).ne.5) then
         err=1
         call error(89)
         return
      endif
      il7=il+7
      ilt=il7+istk(il+2)
      lt=sadr(ilt+4)
      lp=lt+istk(il+3)-istk(il+2)
      ilp=sadr(lp)-4
      ll=lp+istk(il+4)-istk(il+3)
      ill=sadr(ll)-4
      lphi=ll+istk(il+5)-istk(il+4)
      ilphi=sadr(lphi)-4
      lpsi=lphi+istk(il+6)-istk(il+5)
      ilpsi=sadr(ilphi)-4
      top2=top2+1
      ilu=iadr(lstk(top2))
      mnu=istk(ilu+1)*istk(ilu+2)
      if(mnu.le.1)  arma=.true.
      lu=sadr(ilu+4)
      top2=top2+1
      ily=iadr(lstk(top2))
      ly=sadr(ily+4)
      mny=istk(ily+1)*istk(ily+2)
      if(mnu.ne.mny) then
         call error(60)
         return
      endif
      if(top2.eq.top) then
         llam=lstk(top+1)
         stk(llam)=0.950d+0
         stk(llam+1)=0.990d+0
         stk(llam+2)=0.010d+0
         lk=llam+3
         stk(lk)=0.00d+0
         stk(lk+1)=0. 980d+0
         stk(lk+2)=0.020d+0
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      illam=iadr(lstk(top2))
      llam=sadr(illam+4)
      if(top2.eq.top) then
         lk=lstk(top+1)
         stk(lk)=0.00d+0
         stk(lk+1)=0.980d+0
         stk(lk+2)=0.020d+0
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      ilk=iadr(lstk(top2))
      lk=sadr(ilk+4)
      if(top2.eq.top) then
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      ilc=iadr(lstk(top2))
      lc=sadr(ilc+4)
      ilk=iadr(lstk(top2-1))
      lk=sadr(ilk+4)
      illam=iadr(lstk(top2-2))
      llam=sadr(illam+4)
 55   continue
      mnt=istk(ilt+1)*istk(ilt+2)
      ordre=mnt/3
      lf=lc+1+2*ordre
      lg=lf+mnt
      lsta=lg+mnt
      lwork=lsta+1+ordre
      lmax=lwork+1
      err=lmax-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lambda=stk(llam)
      alpha=stk(llam+1)
      beta=stk(llam+2)
      kappa=stk(lk)
      mu=stk(lk+1)
      nu=stk(lk+2)
      do 52 k=1,mny-1
         y=stk(ly+k)
         u=stk(lu+k-1)
         call rpem(stk(lt),stk(lp),ordre,u,y,lambda,
     1        kappa,stk(lc),ista,v,eps,eps1,mnt,
     2        stk(lphi),stk(lpsi),stk(lsta),stk(lwork),
     3        stk(lf),stk(lg),stk(ll))
         lambda=alpha*lambda+beta
         kappa=mu*kappa+nu
 52   continue
      if (lhs.eq.1) then
         top=top-rhs+1
         return
      endif
      if(lhs.eq.2) then
         top=top-rhs+2
         istk(ilu)=1
         istk(ilu+1)=1
         istk(ilu+2)=1
         istk(ilu+3)=0
         stk(lu)=v
         lstk(top+1)=lu+1
         return
      endif
      return
 60   continue
c
c     amell: calculation of the jacobi's elliptic function am(u,k)
      if(rhs.ne.2) then
         call error(39)
         return
      endif
c
      il=iadr(lstk(top))
      itv=istk(il)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l1=sadr(il+4)
      if(stk(l1).lt.0.or.stk(l1).gt.1)then
         err=rhs
         call error(36)
         return
      endif
      il=iadr(lstk(top-1))
      itv=istk(il)
      m=istk(il+1)
      n=istk(il+2)
      length=m*n
      it=istk(il+3)
      l=sadr(il+4)
      lw=lstk(top+1)
      err=lw+length-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      call amell(stk(l),stk(l1),stk(lw),length)
      call unsfdcopy(length,stk(lw),1,stk(l),1)
      top=top-1
      lstk(top+1)=l+length
      return
c
c delip
c -----
c
 70   continue
      if(rhs.ne.2.or.lhs.ne.1) then
         call error(39)
         return
      endif
      il1=iadr(lstk(top))
      itv1=istk(il1)
      if(itv1.ne.1) then
         err=2
         call error(53)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      if(m1*n1.ne.1) then
         err=2
         call error(89)
         return
      endif
      it1=istk(il1+3)
      l1=sadr(il1+4)
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      if(itv2.ne.1) then
         err=1
         call error(53)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      length=m2*n2
      it2=istk(il2+3)
      l2=sadr(il2+4)
      lw=lstk(top+1)
      err=lw+2*length-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      call delip(length,stk(lw),stk(lw+length),stk(l2),stk(l1))
      top=top-1
      istk(il2)=1
      istk(il2+1)=m2
      istk(il2+2)=n2
      istk(il2+3)=1
      call unsfdcopy(length,stk(lw),1,stk(l2),1)
      call unsfdcopy(length,stk(lw+length),1,stk(l2+length),1)
      lstk(top+1)=l2+2*length
      return
 80   continue
c
c     remez goes here
c
      il1=iadr(lstk(top))
      itv1=istk(il1)
      m1=istk(il1+1)
      n1=istk(il1+2)
      ngrid=m1*n1
      it1=istk(il1+3)
      l1=sadr(il1+4)
      call simple(ngrid,stk(l1),stk(l1))
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      call simple(ngrid,stk(l2),stk(l2))
      il3=iadr(lstk(top-2))
      itv3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      call simple(ngrid,stk(l3),stk(l3))
      il4=iadr(lstk(top-3))
      itv4=istk(il4)
      m4=istk(il4+1)
      n4=istk(il4+2)
      nc=m4*n4-2
      it4=istk(il4+3)
      l4=sadr(il4+4)
      ir4=il4+4
      call entier(nc+2,stk(l4),stk(l4))
      lw=lstk(top+1)
      err=lw+7*(nc+2)-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      lw1=lw+nc+2
      lw2=lw1+nc+2
      lw3=lw2+nc+2
      lw4=lw3+nc+2
      lw5=lw4+nc+2
      lw6=lw5+nc+2
      call remez(ngrid,nc,stk(l4),stk(lw1),stk(lw2),stk(lw3),
     *     stk(l3),stk(l2),stk(l1),stk(lw4),
     *     stk(lw5),stk(lw6),stk(lw))
      top=top-3
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nc+1
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(nc+1,stk(lw),1,stk(lo),1)
      lstk(top+1)=lo+nc+1
      return
 90   continue
c
c     syredi goes here
c
      il1=iadr(lstk(top))
      itv1=istk(il1)
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      adels=stk(l1)
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      adelp=stk(l2)
      il3=iadr(lstk(top-2))
      itv3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      il4=iadr(lstk(top-3))
      itv4=istk(il4)
      m4=istk(il4+1)
      n4=istk(il4+2)
      it4=istk(il4+3)
      l4=sadr(il4+4)
c     call entier(1,stk(l4),stk(l4))
      iapro=stk(l4)
      il5=iadr(lstk(top-4))
      itv5=istk(il5)
      m5=istk(il5+1)
      n5=istk(il5+2)
      it5=istk(il5+3)
      l5=sadr(il5+4)
c     call entier(1,stk(l5),stk(l5))
      ityp=stk(l5)
      lw1=lstk(top+1)
      lw2=lw1+32
      lw3=lw2+32
      lw4=lw3+32
      lw5=lw4+32
      lw6=lw5+32
      lw7=lw6+64
      lw8=lw7+64
      lw9=lw8+64
      lw10=lw9+64
      lw11=lw10+64
      lw12=lw11+64
      lw13=lw12+64
      lw14=lw13+64
      lw15=lw14+256
      lw16=lw15+256
      lw17=lw16+16
      lw18=lw17+64
      lw19=lw18+64
      err=lw19-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      maxdeg=64
      call syredi(maxdeg,ityp,iapro,stk(l3),adelp,adels,
     *     ndeg,nb,
     *     fact,stk(lw1),stk(lw2),stk(lw3),stk(lw4),stk(lw5),
     *     stk(lw6),stk(lw7),stk(lw8),stk(lw9),
     *     ierr,stk(lw10),stk(lw11),stk(lw12),stk(lw13),
     *     stk(lw14),stk(lw15),stk(lw16),stk(lw17),stk(lw18))
c     subroutine syredi(maxdeg,ityp,iapro,om,adelp,adels,
c     *                   ndeg,nb,
c     *                   fact,b2,b1,b0,c1,c0,
c     *                   zzr,zzi,zpr,zpi,
c     *                   ierr,spr,spi,pren,pimn,
c     *                   zm,sm,rom,nzero,nze)
c
c     nb=(maxdeg+1)/2
c     dimension b2,b1,b0,c1,c0 :nb
c     dimension zzr,zzi,zpr,zpi,spr,spi :maxdeg
c     dimension pren,pimn,nzero,nze :maxdeg
c     dimension zm,sm :maxdeg x 4
      if(ierr.eq.-7) then
         buf='specs=>invalid order filter'
         call error(9999)
         return
      endif
      if(ierr.eq.-9) then
         buf='specs=>too high order filter'
         call error(9999)
         return
      endif
      if(ierr.ne.0)then
         buf='error in built in fct syredi'
         call error(9999)
         return
      endif
      top=top-4
c     fact
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=1
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      stk(lo)=fact
      lstk(top+1)=lo+length

c     b2
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw1),1,stk(lo),1)
      lstk(top+1)=lo+length

c     b1
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw2),1,stk(lo),1)
      lstk(top+1)=lo+length

c     b0
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw3),1,stk(lo),1)
      lstk(top+1)=lo+length
c     c1
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw4),1,stk(lo),1)
      lstk(top+1)=lo+length
c     c0
      top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nb
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(length,stk(lw5),1,stk(lo),1)
      lstk(top+1)=lo+length

c     zeros
      j=0
      i=0
 1001 if(j.ge.ndeg) goto 1002
      if(stk(lw7+i).eq.0.0d0)then
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=0.0d0
      else
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=stk(lw7+i)
         j=j+1
         zzr(j)=stk(lw6+i)
         zzi(j)=-stk(lw7+i)
      endif
      i=i+1
      goto 1001
c     poles

c      if(j.lt.ndeg)then
c         do 1003 i=j+1,ndeg
c            zzr(i)=0.d0
c            zzi(i)=0.d0
c 1003    continue
c      endif

 1002 top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=ndeg
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=1
      lo=sadr(ilo+4)
      call unsfdcopy(length,zzr(1),1,stk(lo),1)
      call unsfdcopy(length,zzi(1),1,stk(lo+length),1)
      lstk(top+1)=lo+2*length

c     poles
      k=0
      i=0
 1004 if(k.ge.ndeg) goto 1005
      if(stk(lw9+i).eq.0.0d0)then
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=0.0d0
      else
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=stk(lw9+i)
         k=k+1
         zpr(k)=stk(lw8+i)
         zpi(k)=-stk(lw9+i)
      endif
      i=i+1
      goto 1004

c      if(k.lt.ndeg)then
c         do 1005 i=k+1,ndeg
c            zpr(i)=0.0d0
c            zpi(i)=0.0d0
c 1005    continue
c      endif

 1005 top=top+1
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=ndeg
      length=istk(ilo+1)*istk(ilo+2)
      istk(ilo+3)=1
      lo=sadr(ilo+4)
      call unsfdcopy(length,zpr(1),1,stk(lo),1)
      call unsfdcopy(length,zpi(1),1,stk(lo+length),1)
      lstk(top+1)=lo+2*length
      return
      end

      subroutine scicorr(fname)
c     implicit undefined (a-z)
      include '../stack.h'
      character*(*) fname
      logical checkrhs,getrmat,getsmat,getscalar
      logical cremat,getexternal,getmat
      integer m1,n1,lr1,m2,n2,lr2,lr,top2, iadr,sadr
      integer topk,topw,gettype
      double precision yyy
c
      logical fort,type
      integer fini,update
c
      external dgetx,dgety,setdgetx,setdgety
      external bgetx,bgety
      character*(nlgh+1) namex,namey
      common / corrname / namex,namey
      common / corradr  / kgxtop,kgytop,ksec,kisc
      common / corrtyp /  itxcorr,itycorr
      common/  iercorr /iero
c
      data fini/15/,update/30/
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     calcul de correlations
      iero=0
      topk=top
      topw=top+1
      top2=top-rhs+1
      il1=iadr(lstk(top2))
      if(istk(il1).eq.10) then
         ich=il1+5+istk(il1+1)*istk(il1+2)
         ich=istk(ich)
         if(ich.eq.fini) goto 41
         if(ich.eq.update) goto 42
      endif
C     usual case [cov,mean]=corr(x,[y],nlags) 
C     ------------------------------------------
      if (.not.checkrhs(fname,2,3)) return
C     getting nlags (last argument)
      if(.not.getscalar(fname,topk,top,lr))return
      lag=int(stk(lr))
      if (.not.cremat(fname,topk+1,0,1,lag,lcxy,lc)) return
      if (.not.cremat(fname,topk+2,0,1,2,lrxy,lc)) return
      if(rhs.eq.3) then
         top=top-1
         if (.not.getrmat(fname,topk,top,m2,n2,lr2)) return
         ny= m2*n2
         top=top-1
         if (.not.getrmat(fname,topk,top,m1,n1,lr1)) return
         nx= m1*n1
         if(nx.ne.ny) then
            call error(60)
            return
         endif
      endif
      if(rhs.eq.2) then
         top=top-1
         if (.not.getrmat(fname,topk,top,m1,n1,lr1)) return
         nx= m1*n1
         lr2=lr1
      endif
      call tscccf(stk(lr1),stk(lr2),nx,stk(lcxy),stk(lrxy),
     $     lag,ierr)
      if(ierr.eq.-1) then
         buf='too many coefficients are required'
         call error(999)
         return
      endif
      top=topk-rhs+1
      call copyobj(fname,topk+1,top)
      if(lhs.eq.2) then
         top=top+1
         if (.not.cremat(fname,top,0,1,rhs-1,lr,lc)) return
         call unsfdcopy(rhs-1,stk(lrxy),1,stk(lr),1)
      endif
      return
C     ----------------------
C     Special case using fft 
C       [cov,mean]=corr('fft',xmacro,[ymacro],n,sect)
C     ----------------------
 41   continue
c     methode de la fft
      if (.not.checkrhs(fname,4,5)) return
c     -------last argument (sect)
      if(.not.getscalar(fname,topk,top,lr))return
      lag=int(stk(lr))
      mm=2*lag
c     --------total size 
      kntop=top-1
      if(.not.getscalar(fname,topk,kntop,lr))return
      n=int(stk(lr))
c     -------external for xmacro 
      kgxtop= top-rhs+2 
      itxcorr=gettype(kgxtop)
      if (.not.getexternal(fname,topk,kgxtop,namex,fort,
     $     setdgetx)) return
      mode=2
c     -------external for ymacro 
      if(rhs.eq.5) then
         kgytop= top-rhs+3
         itycorr=gettype(kgytop)
         if (.not.getexternal(fname,topk,kgytop,namey,type,
     $     setdgety)) return
         if( type.neqv.fort) then
            err=3
            call error(55)
            return
         endif
         mode=3
      endif
c     Working areas 
      if (.not.cremat(fname,top+1,0,1,mm,lxa,lc)) return
      if (.not.cremat(fname,top+2,0,1,mm,lxr,lc)) return
      if (.not.cremat(fname,top+3,0,1,mm,lxi,lc)) return
      if (.not.cremat(fname,top+4,0,1,sadr(mm),lzr,lc)) return
      if (.not.cremat(fname,top+5,0,1,sadr(mm),lzi,lc)) return
      top=top+6
      ksec=top
      if (.not.cremat(fname,ksec,0,1,1,lr,lc))return
      kisc=top+1
      if (.not.cremat(fname,kisc,0,1,1,lr,lc))return
      if(fort) then
         call cmpse2(mm,n,mode,dgetx,dgety,stk(lxa),stk(lxr),
     1        stk(lxi),stk(lzr),stk(lzi),ierr)
         if(ierr.gt.0) then
            buf='fft call : needs power of two!'
            call error(999)
            return
         endif
      else
         call cmpse2(mm,n,mode,bgetx,bgety,stk(lxa),stk(lxr),
     1        stk(lxi),stk(lzr),stk(lzi),ierr)
         if (err.gt.0.or.err1.gt.0) return
         if(ierr.gt.0) then
            buf='fft call : needs a power of 2'
            call error(999)
            return
         endif
      endif
      top=topk-rhs+1
      if (.not.cremat(fname,top,0,1,lag,lr,lc)) return
      call unsfdcopy(lag,stk(lxa),1,stk(lr),1)
      if(lhs.eq.1) return
      top=top+1
      if (.not.cremat(fname,top,0,1,rhs-3,lr,lc)) return
      call unsfdcopy(rhs-3,stk(lxr),1,stk(lr),1)
      return
 42   continue
C     ------------------------
C     special case with Updates 
C     [w,xu]=corr('updt',x1,[y1],w0) etc....
      ichaud=0
      mode=0
c     -------------x1 value 
      top2=top2+1
      if (.not.getrmat(fname,topk,top2,mx,nx,lx)) return
      mnx=mx*nx
c     --------------y1 value or w0
      top2=top2+1
      if (.not.getmat(fname,topk,top2,it,my,ny,ly,lyc)) return 
      mny=ny*my
      if(mny.eq.mnx) then
         mode=1
         if ( it.eq.1) then 
            buf= fname // ' y1 should be a real matrix'
            call error(999)
            return
         endif
      endif
c     --------------third or second argument 
      if(mode.eq.1) top2=top2+1
      if (.not.getmat(fname,topk,top2,it,mz,nz,lzr,lzi)) return 
      mfft=mz*nz
C     if w0 is real we add space for storing an imag part 
      if (it.eq.0) then
         if (.not.cremat(fname,topw,0,1,mfft,lzi,lc)) return
         topw=topw+1
         call dset(mfft,0.d0,stk(lzi),1)
      endif
      if(mode.eq.1.and.rhs.eq.5) ichaud=1
      if(mode.eq.0.and.rhs.eq.4) ichaud=1
c     -----------last argument + working areas 
      if (.not.cremat(fname,topw,1,1,mfft,lwr,lwi)) return
      call dset(2*mfft,0.d0,stk(lwr),1)
      if(ichaud.eq.1) then
         top2=top2+1
         if (.not.getmat(fname,topk,top2,it,mx0,nx0,
     $        lxr,lxi))   return 
         nbx=mx0*nx0
         if ( nbx.gt.mfft) then 
            buf = fname // 'last argument has wrong size '
            call error(999)
            return
         endif
         call unsfdcopy(nbx,stk(lxr),1,stk(lwr),1)
      endif
C     ---------calling the function 
      if(mode.eq.1) then
         call cmpse3(mfft,mnx,mode,stk(lx),stk(ly),stk(lwr),stk(lwi),
     1        stk(lzr),stk(lzi),ierr,ichaud,nbx)
      endif
      if(mode.eq.0) then
         call cmpse3(mfft,mnx,mode,stk(lx),yyy,stk(lwr),stk(lwi),
     1        stk(lzr),stk(lzi),ierr,ichaud,nbx)
      endif
C     ---------back to Scilab
      if(ierr.gt.0) then
         buf='fft call: needs a power of 2'
         call error(999)
         return
      endif
      if (lhs.eq.2) then 
C        keep this in a safe place before the copy for lhs1
         call unsfdcopy(nbx,stk(lx+mnx-nbx),1,stk(lwr),1)
      endif
      if(lhs.ge.1) then
         top=topk-rhs+1
         if (.not.cremat(fname,top,1,mz,nz,lres,lresi)) return
         call unsfdcopy(mfft,stk(lzr),1,stk(lres),1)
         call unsfdcopy(mfft,stk(lzi),1,stk(lresi),1)
      endif
      if(lhs.eq.2) then
         top=top+1
         if (.not.cremat(fname,top,0,1,nbx,lres,lresi)) return
         call unsfdcopy(nbx,stk(lwr),1,stk(lres),1)         
      endif
      return
      end


