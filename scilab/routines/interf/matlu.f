      subroutine matlu
c ====================================================================
c
c     evaluate functions involving gaussian elimination
c
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz),io
c
c     fonction / fin
c     -2   -1    1    2     3    4   5    6    7     
c      \    /   inv  det  rcond  lu      chol  rref 
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matlu '//buf(1:4))
      endif
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
c
      go to (20,10,99,30,40,50,60,99,80,85),fin+3
c
 10   continue
c     matrix right division, a/a2
      call intrdiv
      goto 99
c
 20   continue
      call intldiv
      goto 99
c
 30   continue
c     inv
      call intinv
      goto 99
c
 40   continue
c     det
      call intdet
      return
c
 50   continue
c     rcond
      call intrcond
      goto 99
c
 60   continue
c     lu
      call intlu
      goto 99
c
 80   continue
c     cholesky
      call intchol
      go to 99
c
 85   continue
c     rref
      call intrref
      go to 99

 99   return
      end

      subroutine intlu
c     lu
      include '../stack.h'
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     Check number of arguments
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 2 .and.lhs .ne. 3) then
         call error(41)
         return
      endif
c     Check input argument type
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('lu',top-rhs+1)
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)

      nn=n*n

      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif

c     form output matrices
      top = top+1
      ilu=iadr(lstk(top))
      lu=sadr(ilu+4)
      lstk(top+1)=lu+nn*(it+1)
      if(lhs.eq.3) then
         top = top+1
         ile=iadr(lstk(top))
         le=sadr(ile+4)
         lstk(top+1)=le+nn
      endif
      ilp=iadr(lstk(top+1))
c     check for space
      err = sadr(ilp+n) -lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      istk(ilu)=1
      istk(ilu+1)=n
      istk(ilu+2)=n
      istk(ilu+3)=it
      if(lhs.eq.3) then
         istk(ile)=1
         istk(ile+1)=n
         istk(ile+2)=n
         istk(ile+3)=0
      endif
      if(nn.le.0) then
c     lu of an empty matrix
         go to 99
      endif

c     Compute factorisation
      if(it.eq.0) then
         call dgefa(stk(l),m,n,istk(ilp),info)
      else
         call wgefa(stk(l),stk(l+nn),m,n,istk(ilp),info)
      endif
c     Form output matrices
      call dset(nn*(it+1),0.0d0,stk(lu),1)
      do 12 kb = 1, n
         k = n+1-kb
         ll=l+(k-1)*n
         luk=lu+(k-1)*n
c     .  copy upper triangle in U
         call unsfdcopy(k,stk(ll),1,stk(luk),1)
         if(k.ne.n) then
            call dscal(n-k,-1.0d+0,stk(ll+k),1)
         endif
c     .  set lower triangle of L to 0
         call dset(k,0.0d+0,stk(ll),1)
         stk(ll+k-1)=1.0d+0
c     
         i = istk(ilp+k-1)
         if (i .ne. k) then
            li = l+i-1+(k-1)*n
            lk = l+k-1+(k-1)*n
            call dswap(n-k+1,stk(li),n,stk(lk),n)
         endif
 12   continue
      if(it.eq.1) then
c     L and U Imaginary part
         do 14 kb = 1, n
            k = n+1-kb
            ll=l+nn+(k-1)*n
            luk=lu+nn+(k-1)*n
            call unsfdcopy(k,stk(ll),1,stk(luk),1)
            if(k.ne.n) then
               call dscal(n-k,-1.0d+0,stk(ll+k),1)
            endif
            call dset(k,0.0d0,stk(ll),1)
 14      continue
         i = istk(ilp+k-1)
         if (i .ne. k) then
            li = l+i-1+(k-1)*n
            lk = l+k-1+(k-1)*n
            call dswap(n-k+1,stk(li),n,stk(lk),n)
         endif
      endif
      if(lhs.eq.3) then
         call dset(nn,0.0d0,stk(le),1)
         call dset(n,1.0d0,stk(le),n+1)
         do 16 kb = 1, n
            k = n+1-kb
            i = istk(ilp+k-1)
            if (i .ne. k) then
               call dswap(n,stk(le+(i-1)*n),1,stk(le+(k-1)*n),1)
            endif
 16      continue
         do 17 k = 1, n
            i = istk(ilp+k-1)
            if (i .ne. k) then
               call dswap(n,stk(l+(i-1)),n,stk(l+(k-1)),n)
               if(it.eq.1) then
                  call dswap(n,stk(l+nn+(i-1)),n,stk(l+nn+(k-1)),n)
               endif
            endif
 17      continue

      endif
 99   continue
      return
      end

      subroutine intchol
c     Cholesky
      include '../stack.h'
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     Check number of arguments
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     Check argument type
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('chol',top-rhs+1)
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      nn=n*n
c     Check argument dimension
      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif
c
      if(nn.le.0) return
      if(it.eq.1) call wpofa(stk(l),stk(l+nn),m,n,err)
      if(it.eq.0) call dpofa(stk(l),m,n,err)
      if (err .ne. 0) then
         call error(29)
         return
      endif
      do 81 j = 1, n
         ll = l+j+(j-1)*m
         call unsfdcopy(m-j,0.0d+0,0,stk(ll),1)
         if(it.eq.1) call unsfdcopy(m-j,0.0d+0,0,stk(ll+nn),1)
 81   continue
      return
      end

      subroutine intrref
      include '../stack.h'
      double precision eps
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      eps=stk(leps)
c
c     Check number of arguments
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

c     Check argument type
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('rref',top-rhs+1)
         fun=-1
         return
      endif

      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn=m*n
      if(mn.le.0) return
c
      if(it.eq.0) then
         call drref(stk(l),m,m,n,eps)
      else
         call wrref(stk(l),stk(l+mn),m,m,n,eps)
      endif
      return
      end

      subroutine intrcond
      include '../stack.h'
      double precision sr,si,rcond
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c     Check number of arguments
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
c
c     Check  argument type
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('rcond',top-rhs+1)
         fun=-1
         return
      endif

      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      nn=n*n
c     Check  argument size
      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif


      l3 = l + nn*(it+1)
      err = l3+n*(it+1)+sadr(n) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif

      if(nn.le.0) then
c     empty matrix case
         rcond=1.0d0
      else
         if(it.eq.0) then
            call dlslv(stk(l),m,n,sr,0,0,stk(l3),rcond,ierr,0)
         else
            call wlslv(stk(l),stk(l+nn),m,n,sr,si,0,0,stk(l3),
     $           rcond,ierr,0)
         endif
      endif
      stk(l) = rcond
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      lstk(top+1)=l+1
      if (lhs .gt. 1) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         if(nn.eq.0) then
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)+1
         else
            istk(il+1)=n
            istk(il+2)=1
            istk(il+3)=it
            l=sadr(il+4)
            call unsfdcopy(n*(it+1),stk(l3+sadr(n)),1,stk(l),1)
            lstk(top+1)=l+n*(it+1)
         endif
      endif
      return
      end

      subroutine intdet
      include '../stack.h'
      double precision dtr(2),dti(2),sr,si,t
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
c
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('det',top-rhs+1)
         fun=-1
         return
      endif
c
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn=m*n
      nn=n*n

      if (m .ne. n) then
         err=1
         call error(20)
         return
      endif
      if(mn.le.0) goto 47
      ilp=iadr(l+mn*(it+1))
      lw=sadr(ilp+n)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(it.eq.0) then
         call dgefa(stk(l),m,n,istk(ilp),info)
         call dgedi(stk(l),m,n,istk(ilp),dtr,sr,10)
      else
       call wgefa(stk(l),stk(l+mn),m,n,istk(ilp),info)
       call wgedi(stk(l),stk(l+mn),m,n,istk(ilp),dtr,dti,
     &     sr,si,10)
      endif
      k = int(dtr(2))

      if(lhs.eq.1) then
         stk(l) = dtr(1)*10.0d+0**int(dtr(2))
         if(it.eq.1) stk(l+1) = dti(1)*10.0d+0**int(dtr(2))
         istk(il+1)=1
         istk(il+2)=1
         lstk(top+1)=l+it+1
      else
         stk(l) = dtr(2)
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         lstk(top+1)=l+1
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=it
         l=sadr(il+4)
         stk(l)=dtr(1)
         if(it.eq.1) stk(l+1)=dti(1)
         lstk(top+1)=l+(it+1)
      endif
      go to 99
c cas de la matrice vide
   47 continue
      if(lhs.eq.1) then
         istk(il+1)=1
         istk(il+2)=1
         lstk(top+1)=l+it+1
         stk(l)=1.0d+0
         if (it.eq.1) stk(l+1)=0.0d+0
      else
         stk(l) = 0.0d0
         istk(il+1)=1
         istk(il+2)=1
         istk(il+2)=0
         lstk(top+1)=l+1
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=it
         l=sadr(il+4)
         stk(l)=1.0d+0
         if (it.eq.1) stk(l+1)=0.0d+0
         lstk(top+1)=l+(it+1)
      endif
      go to 99
 99   continue
      return
      end

      subroutine intinv
      include '../stack.h'
      double precision sr,si,rcond,t
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         call putfunnam('inv',top-rhs+1)
         fun=-1
         return
      endif

      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn=m*n
      nn=n*n

      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif
c
      n=abs(n)
      if(mn.le.0) return
      l3 = l + nn*(it+1)
      err = l3+n*(it+1)+sadr(n) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         call dlslv(stk(l),n,n,sr,0,0,stk(l3),rcond,err,3)
      else
         call wlslv(stk(l),stk(l+nn),n,n,sr,si,0,0,stk(l3),
     $     rcond,err,3)
      endif
      if (rcond .eq. 0.0d+0) then
         call error(19)
         return
      endif
      t = 1.0d+0 + rcond
      if (t .eq. 1.0d+0) then
         write(buf(1:13),'(1pd13.4)') rcond
c       matrice est quasi singuliere ou mal normalisee')
         call msgs(5,0)
      endif
      return
      end

      subroutine intldiv
c     matrix left division a backslash a2
      include '../stack.h'
      double precision sr,si,rcond,t
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         err=rhs
         call error(53)
         return
      endif
c
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn=m*n
      nn=n*n

      top=top-1
      il2=iadr(lstk(top+1))
      if(istk(il2).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
c
      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif
      if (mn2 .eq. 1) go to 26
      if (mn2.le.0) then
         err=rhs
         call error(45)
         return
      endif
      if (mn.le.0) then
         lstk(top+1)=l
         return
      endif
      if (m2 .ne. n) then
         call error(12)
         return
      endif
      l3=l2+(max(it,it2)+1)*mn2
      err = l3+n*(it+1)+sadr(n) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      if(it.eq.1) then
         if(it2.eq.0) call unsfdcopy(mn2,0.0d+0,0,stk(l2+mn2),1)
         call wlslv(stk(l),stk(l+mn),m,n,stk(l2),stk(l2+mn2),
     &        m2,n2,stk(l3),rcond,err,1)
      else
         call dlslv(stk(l),m,n,stk(l2),m2,n2*(it2+1),stk(l3),
     &        rcond,err,1)
      endif
c
      if (rcond .eq. 0.0d+0) then
         call error(19)
         return
      endif
      t = 1.0d+0 + rcond
c
      if (t .eq. 1.0d+0) then
         write(buf(1:13),'(1pd13.4)') rcond
         call msgs(5,0)
      endif
c
      istk(il+2)=n2
      it=max(it,it2)
      istk(il+3)=it
      lstk(top+1)=l+mn2*(it+1)
      call unsfdcopy(mn2*(it+1),stk(l2),1,stk(l),1)
      go to 99
 26   sr = stk(l2)
      si=0.0d+0
      if(it2.eq.1) si = stk(l2+1)

      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif
      n=abs(n)
      if(mn.le.0) return
      l3 = l + nn*(it+1)
      err = l3+n*(it+1)+sadr(n) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         call dlslv(stk(l),n,n,sr,0,0,stk(l3),rcond,err,3)
      else
         call wlslv(stk(l),stk(l+nn),n,n,sr,si,0,0,stk(l3),
     $     rcond,err,3)
      endif
      if (rcond .eq. 0.0d+0) then
         call error(19)
         return
      endif
      t = 1.0d+0 + rcond
      if (t .eq. 1.0d+0) then
         write(buf(1:13),'(1pd13.4)') rcond
c       matrice est quasi singuliere ou mal normalisee')
         call msgs(5,0)
      endif
      goto(33,34,35) (it+2*it2)
 33   call dscal(nn*(it+1),sr,stk(l),1)
      istk(il+3)=it
      lstk(top+1)=l+m*n*(it+1)
      goto 99
 34   call unsfdcopy(nn,stk(l),1,stk(l+nn),1)
      call dscal(nn,sr,stk(l),1)
      call dscal(nn,si,stk(l+nn),1)
      istk(il+3)=1
      lstk(top+1)=l+2*nn
      goto 99
 35   call wscal(nn,sr,si,stk(l),stk(l+nn),1)
      go to 99
 99   continue
      return
      end

      subroutine intrdiv
      include '../stack.h'
      double precision eps,sr,si,rcond,t
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      eps=stk(leps)

      il=iadr(lstk(top-rhs+1))
      if (istk(il).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn=m*n
      nn=n*n
c
      top=top-1
      il2=iadr(lstk(top+1))
      if(istk(il2).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      itr=max(it,it2)
      l2=sadr(il2+4)
      mn2=m2*n2
c
      if (m2 .ne. n2) then
         err=rhs
         call error(20)
         return
      endif
      if (mn .eq. 1) go to 16
      if (mn2.le.0) then
         err=2
         call error(45)
         return
      endif
      if (mn.le.0) then
         lstk(top+1)=l
         return
      endif
      if (n .ne. n2) then
         call error(11)
         return
      endif
      l3=max(l+(itr+1)*mn,l2)+(it2+1)*mn2
      err=l3+n2*(it2+1)+(n2+1)/2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      if(it.eq.1.or.it2.eq.0) goto 9
      inc=1
      if(l2.le.l+2*mn) inc=-1
      call unsfdcopy(mn2*(it2+1),stk(l2),inc,stk(l+2*mn),inc)
      call unsfdcopy(mn,0.0d+0,0,stk(l+mn),1)
      istk(il+3)=1
      l2=l+2*mn
      lstk(top+1)=l2
    9 continue
      if(it2.eq.1) call wlslv(stk(l2),stk(l2+mn2),m2,n2,
     $     stk(l),stk(l+mn),m,m,stk(l3),rcond,err,2)
      if(it2.eq.0) call dlslv(stk(l2),m2,n2,stk(l),m,m,stk(l3),
     $     rcond,err,2)
      if(it2.eq.0.and.it.eq.1) call dlslv(stk(l2),m2,n2,stk(l+mn),m,m,
     $     stk(l3),rcond,err,-2)
      if (rcond.eq.0.0d+0) then
         call error(19)
         return
      endif
      t = 1.0d+0 + rcond
      if (t.eq.1.0d+0 .and. fun.ne.21) then
         write(buf(1:13),'(1pd13.4)') rcond
         call msgs(5,0)
      endif
      if (t.eq.1.0d+0 .and. fun.eq.21) then
         write(buf(1:13),'(1pd13.4)') rcond
         call msgs(6,0)
      endif
c
c     check for imaginary roundoff in matrix functions
      if(it2.eq.0.and.it.eq.0) goto 99
      do 13 i=1,nn
         sr=abs(stk(l+i-1))
         si=abs(stk(l+mn+i-1))
         if(si.gt.eps*sr) goto 99
 13   continue
      istk(il+3)=0
      lstk(top+1)=l+mn
      go to 99
c
 16   sr = stk(l)
      si=0.0d+0
      if(it.eq.1) si = stk(l+1)
      n = n2
      m = n
      mn=m*n
      nn=n*n
      istk(il+1)=n
      istk(il+2)=n
      istk(il+3)=it2
      lstk(top+1)=l+nn*(it2+1)
      call unsfdcopy(nn*(it2+1),stk(l2),1,stk(l),1)
      it2=it
      it=istk(il+3)

      if (m .ne. n) then
         err=rhs
         call error(20)
         return
      endif
      n=abs(n)
      if(mn.le.0) return
      l3 = l + nn*(it+1)
      err = l3+n*(it+1)+sadr(n) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         call dlslv(stk(l),n,n,sr,0,0,stk(l3),rcond,err,3)
      else
         call wlslv(stk(l),stk(l+nn),n,n,sr,si,0,0,stk(l3),
     $     rcond,err,3)
      endif
      if (rcond .eq. 0.0d+0) then
         call error(19)
         return
      endif
      t = 1.0d+0 + rcond
      if (t .eq. 1.0d+0) then
         write(buf(1:13),'(1pd13.4)') rcond
c       matrice est quasi singuliere ou mal normalisee')
         call msgs(5,0)
      endif
      goto(33,34,35) (it+2*it2)
 33   call dscal(nn*(it+1),sr,stk(l),1)
      istk(il+3)=it
      lstk(top+1)=l+m*n*(it+1)
      goto 99
 34   call unsfdcopy(nn,stk(l),1,stk(l+nn),1)
      call dscal(nn,sr,stk(l),1)
      call dscal(nn,si,stk(l+nn),1)
      istk(il+3)=1
      lstk(top+1)=l+2*nn
      goto 99
 35   call wscal(nn,sr,si,stk(l),stk(l+nn),1)
      go to 99
 99   continue
      return
      end
