      subroutine matsvd
c ================================== ( Inria    ) =============
c     evaluate functions involving singular value decomposition
c
c =============================================================
c     Copyright INRIA
      include '../stack.h'
c
      double precision s,t,tol,eps
      double precision ddot,dlamch
      integer rang, iadr,sadr
c
c    fin     1        2        3        4        5      6
c           svd      pinv     cond     norm     rank   sva
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matsvd '//buf(1:4))
      endif
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(top+lhs-rhs+1.ge.bot) then
         call error(18)
         return
      endif
c
      eps=stk(leps)
c
      go to (50,70,10,30,70,80), fin
c
c     cond
c
   10 continue
      il=iadr(lstk(top+1-rhs))
      if(istk(il).ne.1) then
         call putfunnam('cond',top-rhs+1)
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn = m*n
      m1=min(m+1,n)
      if(mn.le.0) then
        k=1
        goto 78
      endif
      ld = l + mn*(it+1)
      l1 = ld + m1*(it+1)
      l2 = l1 + n*(it+1)
      err = l2+m*(it+1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if(it.eq.0) goto 11
      call wsvdc(stk(l),stk(l+mn),m,m,n,stk(ld),stk(ld+m1),
     1 stk(l1),stk(l1+n),t,t,1,t,t,1,stk(l2),stk(l2+m),0,ierr)
      goto 12
   11 call dsvdc(stk(l),m,m,n,stk(ld),stk(l1),t,1,t,1,stk(l2),0,ierr)
   12 continue
c 
      if (ierr.gt.1) call msgs(3,ierr)

      s = stk(ld)
      ld = ld + min(m,n) - 1
      t = stk(ld)
      if (t .eq. 0.0d+0) then
         t=dlamch('o')
      else
         t=s/t
      endif
      stk(l) = t
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      lstk(top+1)=l+1
      go to 99
c
c     vector and matrix norm
c
   30 continue
      call intnorm("norm")
      goto 99
c
c     svd
c
   50 continue
      il=iadr(lstk(top+1-rhs))
      if(istk(il).ne.1) then
         call putfunnam('svd',top-rhs+1)
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn = m*n
      m1=min(m+1,n)
c
      tol = -1.0d+0
      if (rhs.eq.2) then
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(il1+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         l1=sadr(il1+4)
         tol=stk(l1)
         top = top-1
      endif


      if(mn.le.0) then
c     empty matrix
c     svd([])=[]          
         if(lhs.eq.1) return 
c     [u,s,v]=svd([]) -> u=[],v=[],s=[]
         if(lhs.ge.3) then
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
c     
            top = top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
c     
            top = top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
            if(lhs.eq.3) return
            top=top+1
            il=iadr(lstk(top))
            l=sadr(il+4)
            istk(il)=1
            istk(il+1)=1
            istk(il+2)=1
            istk(il+3)=0
            stk(l)=0.d0
            lstk(top+1)=l+1
            return
         endif     
      endif
      if (lhs .lt. 3) go to 52
      k = m
      if (rhs .eq. 2 .and. lhs .ne. 4) k = min(m,n)
      lu=l
      ld=lu+m*k*(it+1)+sadr(5)-1
      lv=ld+k*n+sadr(5)-1
      l=lv+n*n*(it+1)
      l1=l+m*n*(it+1)
      l2=l1+n*(it+1)
      lsv=l2+m*(it+1)
c
      err = lsv+m1*(it+1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      call unsfdcopy(m*n*(it+1),stk(lu),1,stk(l),1)
      job = 11
      if (rhs .eq. 2 .and. lhs .ne. 4) job = 21
      if(it.eq.0) call dsvdc(stk(l),m,m,n,stk(lsv),stk(l1),
     1 stk(lu),m,stk(lv),n,stk(l2),job,ierr)
      if(it.eq.1) call wsvdc(stk(l),stk(l+m*n),m,m,n,stk(lsv),
     1 stk(lsv+m1),stk(l1),stk(l1+n),stk(lu),stk(lu+m*k),m,stk(lv),
     2 stk(lv+n*n),n,stk(l2),stk(l2+m),job,ierr)
c     set matrix of singular values
c
      if (ierr.gt.1) call msgs(3,ierr)
c
      if(rhs.eq.1) tol=dble(max(m,n))*eps*stk(lsv)
      rang=0
      do 51 j = 1, n
         do 51 i = 1, k
            ll = ld+i-1+(j-1)*k
            ls = lsv+i-1
            if (i.eq.j) then
               stk(ll) = stk(ls)
               if(stk(ls).gt.tol) rang=i
            else
               stk(ll)=0.0d+0
            endif
 51      continue
c     
c     set headers for lhs
      istk(il+1)=m
      istk(il+2)=k
      istk(il+3)=it
      lstk(top+1)=lu+m*k*(it+1)
c
      top = top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=k
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=ld+k*n
c
      top = top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=n
      istk(il+2)=n
      istk(il+3)=it
      lstk(top+1)=lv+n*n*(it+1)
      if(lhs.eq.3) goto 99
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      lr=lv+n*n*(it+1)+sadr(5)-1
      stk(lr)=dble(rang)
      lstk(top+1)=lr+1
      go to 99
c
   52 ld = l + m*n*(it+1)
      l1 = ld + m1*(it+1)
      l2 = l1 + n*2
      err = l2+m*(it+1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if(it.eq.0) call dsvdc(stk(l),m,m,n,stk(ld),
     $           stk(l1),t,1,t,1,stk(l2),
     $           0,ierr)
      if(it.eq.1) call wsvdc(stk(l),stk(l+m*n),m,m,n,
     1 stk(ld),stk(ld+m1),stk(l1),stk(l1+n),t,t,1,t,t,1,
     2 stk(l2),stk(l2+m),0,ierr)
c
      if (ierr.gt.1) call msgs(3,ierr)

      k = min(m,n)
      call unsfdcopy(k,stk(ld),1,stk(l),1)
      istk(il+1)=k
      istk(il+2)=1
      istk(il+3)=0
      lstk(top+1)=l+k
      go to 99
c
c     pinv and rank
c
   70 continue
      il=iadr(lstk(top+1-rhs))
      if(istk(il).ne.1) then
         if(fin.eq.5) then
            call putfunnam('rank',top-rhs+1)

         else
            call putfunnam('pinv',top-rhs+1)
         endif
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn = m*n
      m1=min(m+1,n)
      tol = -1.0d+0
      if (rhs.eq.2) then
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(il1+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         l1=sadr(il1+4)
         tol=stk(l1)
         top = top-1
      endif
      if (mn.le.0 ) then
        if (fin.eq.2) then
c           pinv([])=[]
c           err=1
c           call error(45)
           return
        endif
        k=0
        goto 78
      endif
      lu = l + m*n*(it+1)
      ld = lu + m*m*(it+1)
      if (fin .eq. 5) ld = l + m*n*(it+1)
      lv = ld + m1*(it+1)
      l1 = lv + n*n*(it+1)
      if (fin .eq. 5) l1 = ld + n*(it+1)
      l2 = l1 + n*(it+1)
      err = l2+m*(it+1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      if (fin .eq. 2) job = 11
      if (fin .eq. 5) job = 0
      if(it.eq.0) call dsvdc(stk(l),m,m,n,stk(ld),
     $        stk(l1),stk(lu),m,stk(lv),
     $        n,stk(l2),job,ierr)
      if(it.eq.1) call wsvdc(stk(l),stk(l+m*n),m,m,n,
     1 stk(ld),stk(ld+m1),stk(l1),stk(l1+n),
     2 stk(lu),stk(lu+m*m),m,stk(lv),stk(lv+n*n),n,
     3 stk(l2),stk(l2+m),job,ierr)
c
      if (ierr.gt.1) call msgs(3,ierr)

      if (tol .lt. 0.0d+0) tol = dble(max(m,n))*eps*stk(ld)
      mn = min(m,n)
      k = 0
      do 72 j = 1, mn
        ls = ld+j-1
        s = stk(ls)
        if (s .le. tol) go to 73
        k = j
        ll = lv+(j-1)*n
      if (fin.eq.2) call dscal(n,1.0d+0/s,stk(ll),1)
      if (fin.eq.2.and.it.eq.1) call dscal(n,1.0d+0/s,stk(ll+n*n),1)
   72 continue
   73 if (fin .eq. 5) go to 78
      do 74 j=1,m
      do 74 i=1,n
      ll=l+i-1+(j-1)*n
      l1=lv+i-1
      l2=lu+j-1
      stk(ll)=ddot(k,stk(l2),m,stk(l1),n)
   74 continue
      if(it.eq.0) goto 77
      do 76 j = 1, m
      do 76 i = 1, n
        ll = l+i-1+(j-1)*n
        l1 = lv+i-1
        l2 = lu+j-1
        stk(ll) = stk(ll)+ddot(k,stk(l2+m*m),m,stk(l1+n*n),n)
        stk(ll+m*n)=ddot(k,stk(l2),m,stk(l1+n*n),n)
     1 -ddot(k,stk(l2+m*m),m,stk(l1),n)
   76 continue
   77 continue
      istk(il+1)=n
      istk(il+2)=m
      istk(il+3)=it
      lstk(top+1)=l+m*n*(it+1)
      go to 99
   78 stk(l) = dble(k)
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      lstk(top+1)=l+1
      go to 99
c
c sva
c
   80 continue
      il=iadr(lstk(top+1-rhs))
      if(istk(il).ne.1) then
         call putfunnam('sva',top-rhs+1)
         fun=-1
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)
      mn = m*n
      m1=min(m+1,n)
c
      tol = -1.0d+0
      if (rhs.eq.2) then
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(il1+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         l1=sadr(il1+4)
         tol=stk(l1)
         top = top-1
      endif
c
      if(lhs.ne.3) then
         call error(41)
         return
      endif
      k=min(m,n)
      lu=l
      ld=lu+m*k*(it+1)+sadr(5)-1
      lv=ld+k*n+sadr(5)-1
      l=lv+n*n*(it+1)
      l1=l+m*n*(it+1)
      l2=l1+n*(it+1)
      lsv=l2+m*(it+1)
c
      err = lsv+m1*(it+1) - lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c
      call unsfdcopy(m*n*(it+1),stk(lu),1,stk(l),1)
      job=21
      if(it.eq.0) call dsvdc(stk(l),m,m,n,stk(lsv),stk(l1),
     1 stk(lu),m,stk(lv),n,stk(l2),job,ierr)
      if(it.eq.1) call wsvdc(stk(l),stk(l+m*n),m,m,n,stk(lsv),
     1 stk(lsv+m1),stk(l1),stk(l1+n),stk(lu),stk(lu+m*k),m,stk(lv),
     2 stk(lv+n*n),n,stk(l2),stk(l2+m),job,ierr)
      if(tol.ge.1.0d+0) goto 81
c    calcul du rang numerique
      if(rhs.eq.1) tol=dble(max(m,n))*eps*stk(lsv)
      rang=0
      do 82 j=1,k
      ls=lsv+j-1
      s=stk(ls)
      if(s.le.tol) goto 83
      rang=j
  82  continue
  81  if(tol.ge.1.0d+0) rang=int(tol)
  83  continue
c     set matrix of singular values
      ld=lu+m*rang*(it+1)+sadr(5)-1
      do 84 jb = 1, rang
      do 84 i = 1, rang
        j = rang+1-jb
        ll = ld+i-1+(j-1)*rang
        if (i.ne.j) stk(ll) = 0.0d+0
        ls = lsv+i-1
        if (i.eq.j) stk(ll) = stk(ls)
   84 continue
c
      if (ierr.gt.1) call msgs(3,ierr)
c
c     set headers for lhs
      istk(il+1)=m
      istk(il+2)=rang
      istk(il+3)=it
      lstk(top+1)=lu+m*rang*(it+1)
c
      top = top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=rang
      istk(il+2)=rang
      istk(il+3)=0
      lst=ld+n*k
      lstk(top+1)=ld+rang*rang
c
      lvn=ld+rang*rang+sadr(5)-1
      call unsfdcopy(n*rang*(it+1),stk(lv),1,stk(lvn),1)
      top = top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=n
      istk(il+2)=rang
      istk(il+3)=it
      lstk(top+1)=lvn+n*rang*(it+1)
      go to 99

   99 return
      end

      subroutine intnorm(fname)
c     -------------------------------
c     norm of matrix or vector
c     -------------------------------
      character*(*) fname
      character*(4) type
      INCLUDE '../stack.h'
      logical checkrhs,checklhs,getsmat,getscalar,cremat,getmat
      logical inf,fro 
      integer gettype,itype,topk,isanan
      double precision infinity
      double precision p,wnrm2
      double precision dnrm2,dasum,dlamch,dres,t
      ires=0
      inf = .false.
      fro = .false.
      p=2.0d+0
      topk=top
      if (.not.checklhs(fname,1,1)) return
      if (.not.checkrhs(fname,1,2)) return
c     second argument : optional 'inf','fro' or p ( p: double 
c                       or %inf or %nan )
c     --------------------------------
      if ( rhs.eq.2 ) then 
         itype=gettype(topk)
         if ( itype.eq.10 ) then 
            if(.not.getsmat(fname,topk,top,m2,n2,1,1,lr2,nlr2))return
            call cvstr(min(nlr2,3),istk(lr2),type,1)
            if ( type(1:1).eq.'i') then 
               inf = .true. 
            else if ( type(1:1).eq.'f') then 
               fro = .true.
            else
               buf=fname//' : second argument string inf or fro '
               call error(999)         
               return
            endif
         else if (itype.eq.1) then 
            if(.not.getscalar(fname,topk,top,lr2))return
            p=stk(lr2)
         else 
            buf=fname
     $           //' : second argument string inf or fro or integer '
            call error(999)         
            return
         endif
         top=top-1
      endif
c     the first argument matrix or vector  
c     ---------------------------------
      itype = gettype(top) 
      if ( itype.ne.1 ) then 
         call putfunnam('norm',top)
         top=topk
         fun=-1
         return
      endif
      if(.not.getmat(fname,topk,top,it1,m,n,lr1,lc1)) return      
      mn = m*n
      if (mn.eq.0.or. m.eq.1.or.n.eq.1) then 
c     ==============================vector case 
         if (mn.eq.0) then 
            dres = 0.0d+0
         else if ( isanan(p).eq.1) then 
	    dres = p
         else if (p.gt.dlamch('o').or.inf) then 
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            i = idamax(mn,stk(lr1),1)
            dres= abs(stk(lr1+i-1))
         else if ( p.eq.1.0d+0) then
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres = dasum(mn,stk(lr1),1)
         else if (fro.or. p.eq.2.0d+0) then 
            if (it1.eq.1) then 
               dres=  wnrm2(mn,stk(lr1),stk(lc1),1)
            else
               dres=  dnrm2(mn,stk(lr1),1)
            endif
         else
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            i=idamax(mn,stk(lr1),1)+lr1-1
            dres =abs(stk(i))
            if ( dres .ne.0.0d+0) then 
               t=0.0d+0
               do 12 i=0,mn-1
                  t = t + (abs(stk(lr1+i))/dres)** p
 12            continue
               if (p.ne.0.0d+0) then 
                  dres = dres*t**(1.0d+0/p)
               else
c     Must return %inf : 
                  dres = infinity(0.0d0)
               endif
            endif
         endif
         if (.not.cremat(fname,top,0,1,1,lr1,lc1)) return
         stk(lr1)=dres
      else
c     ==============================Matrix case 
         if ( isanan(p).eq.1) then 
            dres = p
         else if (p.gt.dlamch('o').or.inf) then 
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres = 0.0d+0
            do 45 i = 1, m
               li = lr1+i-1
               t = dasum(n,stk(li),m)
               dres = max(dres,t)
 45         continue
         else if (fro) then
            if (it1.eq.1) then 
               dres=  wnrm2(mn,stk(lr1),stk(lc1),1)
            else
               dres=  dnrm2(mn,stk(lr1),1)
            endif
         else if ( p.eq.1.0d+0) then
            if (it1.eq.1) call vpythag(mn,stk(lr1),stk(lc1))
            dres=0.0d+0
            do 48 j = 1, n
               lj = lr1+(j-1)*m
               t = dasum(m,stk(lj),1)
               dres = max(dres,t)
 48         continue
         else if ( p.eq.2.0d+0) then 
            m1=min(m+1,n)
            if (.not.cremat(fname,topk+1,it1,1,m1,ld,lcd)) return            
            if (.not.cremat(fname,topk+2,it1,1,n,l1,l1c)) return            
            if (.not.cremat(fname,topk+3,it1,1,m,l2,l2c)) return            
            ierr=1
            if(it1.eq.0) then 
               call dsvdc(stk(lr1),m,m,n,stk(ld),stk(l1),
     $              t,1,t,1,stk(l2),0,ierr)
            else
               call wsvdc(stk(lr1),stk(lc1),m,m,n,stk(ld),stk(lcd),
     $              stk(l1),stk(l1c),t,t,1,t,t,1,
     $              stk(l2),stk(l2c),0,ierr)
            endif
            if (ierr.gt.1) call msgs(3,ierr)
            dres = stk(ld)
         else
            call error(23)
            return
         endif
         if (.not.cremat(fname,top,0,1,1,lr1,lc1)) return
         stk(lr1)=dres
      endif
      return
      end


