      subroutine spelm
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer id(nsiz)

      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' spelm '//buf(1:4))
      endif

c     
c     functions/fin
c     1       2       3       4       5        6     7     8   
c     sparse  spget   full    lufact  lusolve  ludel luget spclean
c     9       10     11    12       13     14      15      16
c     nnz     spmax  spmin spmatrix spchol readmps fadj2sp spcompack
c     17      18      19      20       21     22      23      
c     ordmmd  blkfc1i blkslvi inpnvi   sfinit symfcti bfinit 
c     24              25      26
c     mtlb_sparse   %msp_spget   %msp_full
c     

      goto (10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,
     $     160,170,180,190,200,210,220,230,240,250,260) fin
c     
c     sparse
 10   continue
      call intsparse(id)
      goto 900

 20   continue
c     [ij,v,sz]=spget(sp)
      call intspget(id)
      goto 900

 30   continue
c     [A]=full(sp)
      call intfull(id)
      goto 900

 40   continue
c     lufact
      call intlufact(id)
      goto 900

 50   continue
c     lusolve
      call intlusolve(id)
      goto 900

 60   continue
c     ludel
      call intludel(id)
      goto 900

 70   continue
c     luget
      call intluget(id)
      goto 900

 80   continue
c     spclean
      call intspclean(id)
      goto 900

 90   continue
c     nnz
      call intnnz(id)
      goto 900

 100  continue
c     spmax
      call intspmax('max',id)
      goto 900

 110  continue
c     spmin
      call intspmax('min',id)
      goto 900

 120  continue
c     spmatrix
      call intspmatrix(id)
      goto 900

 130  continue
c     [ac,p]=spchol(a) / a=p*ac*ac'*p'
      call intspchol(id)
      goto 900

 140  continue
c     readmps
      call intreadmps(id)
      goto 900

 150  continue
c     fadj2sp
      call intfadj2sp(id)
      goto 900

 160  continue
c     spcompack
      call intspcompa(id)
      goto 900

 170  continue
c     ordmmd
      call intordmmd(id)
      goto 900

 180  continue
c     blkfc1i
      call intblkfc1i(id)
      goto 900

 190  continue
c     blkslvi
      call intblkslvi(id)
      goto 900

 200  continue
c     inpnvi
      call intinpnvi(id)
      goto 900

 210  continue
c     sfinit
      call intsfinit(id)
      goto 900

 220  continue
c     symfcti
      call intsymfcti(id)
      goto 900

 230  continue
c     bfinit
      call intbfinit(id)
      goto 900

 240  continue
c     mtlb_sparse, 
      call intmsparse(id)
      goto 900

 250  continue
c     %msp_get
      call intmspget(id)
      goto 900

 260  continue
c     %msp_full
      call intmfull(id)
      goto 900


 900  continue
 999  return
      end

      subroutine intsparse(id)
      include '../stack.h'
      integer id(nsiz),top0,tops
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
c     
      lw = lstk(top+1)
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     creation de matrice creuse
      if(rhs.eq.1) then
c     sparse(x)
         il=iadr(lstk(top))
         if(istk(il).eq.5.or.istk(il).eq.6) return
         m=istk(il+1)
         n=istk(il+2)
         if(m*n.eq.0.or.m.eq.-1) return
         if(istk(il).eq.1) then
            it=istk(il+3)
            l=sadr(il+4)
c     
            ilr=iadr(lw)
            lw=sadr(ilr+m+m*n)

            ls=lw
            li=ls+m*n
            lw=ls+m*n*(it+1)
            
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            if(it.eq.0) then
               call dful2sp(m,n,stk(l),nel,istk(ilr),stk(ls),0.0d0)
            else
               call wful2sp(m,n,stk(l),stk(l+m*n),nel,
     $              istk(ilr),stk(ls),stk(li),0.0d0,0.0d0)
            endif
            call icopy(m+nel,istk(ilr),1,istk(il+5),1)
            l=sadr(il+5+m+nel)
            call unsfdcopy(nel,stk(ls),1,stk(l),1)
            if(it.eq.1) call unsfdcopy(nel,stk(li),1,stk(l+nel),1)
            istk(il)=5
            istk(il+4)=nel
            lstk(top+1)=l+nel*(it+1)
         elseif(istk(il).eq.4) then
            l=il+3
            ilr=iadr(lw)
            lw=sadr(ilr+m*n)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call lful2sp(m,n,istk(l),nel,istk(ilr))
            call icopy(m+nel,istk(ilr),1,istk(il+5),1)
            l=sadr(il+5+m+nel)
            istk(il)=6
            istk(il+3)=0
            istk(il+4)=nel
            lstk(top+1)=l
         else
            call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
            fun=-1
            return
         endif
         return
      endif
c
c     sparse([i,j],value,[m,n])
      if (rhs .ne. 3.and.rhs.ne.2) then
         call error(39)
         return
      endif
      m=-1
      n=-1
      if(rhs.eq.3) then
c     [m,n] given
         il=iadr(lstk(top))
         if(istk(il).ne.1.or.istk(il+3).ne.0) then
            err=1
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.2) then
            err=1
            call error(60)
            return
         endif
         l=sadr(il+4)
         m=stk(l)
         n=stk(l+1)
         if(m.lt.0.or.n.lt.0) then
            call error(60)
            return
         endif
         if (m*n.eq.0) then
            top=top0
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)+1
            return
         endif
         top=top-1
         lw=lstk(top+1)
      endif
      
      ilij=iadr(lstk(top-1))
      if(istk(ilij).ne.1.or.istk(ilij+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      if(istk(ilij+1).ne.0.and.istk(ilij+2).ne.2) then
         err=2
         call error(60)
         return
      endif
      nel=istk(ilij+1)*istk(ilij+2)/2
      lij=sadr(ilij+4)
      ilij=iadr(lij)
      if(nel.gt.0) then
         call entier(nel*2,stk(lij),istk(ilij))
      endif
c     test if indices are all strictly positive
      do 05 i=0,2*nel-1
         if (istk(ilij+i).le.0) then
            call error(21)
            return
         endif
 05   continue

c     compute number of rows if is not given
      if(rhs.eq.2) then
         mm=0
         do 10 i=0,nel-1
            mm=max(mm,istk(ilij+i))
 10      continue
      else
         mm=m
      endif
c     
      ilv=iadr(lstk(top))
      if(istk(ilv).ne.1.and.istk(ilv).ne.4) then
         call funnam(ids(1,pt+1),'sparse',iadr(lstk(top)))
         fun=-1
         top=tops
         return
      endif
      if(istk(ilv+1)*istk(ilv+2).ne.nel) then
         err=2
         call error(60)
         return
      endif
      il1=ilij
      if(istk(ilv).eq.1) then
         itv=istk(ilv+3)
         lv=sadr(ilv+4)
         lind=iadr(max(lw,sadr(il1+5+mm+nel)+nel*(itv+1)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         if(itv.eq.0) then
            call dij2sp(m,n,nel,istk(ilij),stk(lv),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         else
            call wij2sp(m,n,nel,istk(ilij),stk(lv),stk(lv+nel0),
     $           istk(lind),mm+nel0,istk(liw),ierr)
         endif
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=5
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=itv
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         inc=1
         if(l.gt.lv) then
            inc=-1
            if(itv.eq.1) then
               call unsfdcopy(nel,stk(lv+nel0),inc,stk(l+nel),inc)
            endif
            call unsfdcopy(nel,stk(lv),inc,stk(l),inc)
         else
            inc=1
            call unsfdcopy(nel,stk(lv),inc,stk(l),inc)
            if(itv.eq.1) then
               call unsfdcopy(nel,stk(lv+nel0),inc,stk(l+nel),inc)
            endif
         endif
         
         lstk(top+1)=l+nel*(itv+1)
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
      else
         lv=ilv+3
         lind=iadr(max(lw,sadr(il1+5+mm+nel)))
         liw=lind+mm+nel
         lw=sadr(liw+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         nel0=nel
         call lij2sp(m,n,nel,istk(ilij),istk(lv),
     $        istk(lind),mm+nel,istk(liw),ierr)
         if(ierr.eq.2) then
            buf='not enough memory'
            call error(9999)
            return
         elseif(ierr.eq.1) then
            call error(42)
            return
         elseif(ierr.eq.3) then
            call error(221)
            return
         endif
         top=top-1
         il1=iadr(lstk(top))
         istk(il1)=6
         istk(il1+1)=m
         istk(il1+2)=n
         istk(il1+3)=0
         istk(il1+4)=nel
         ilr=il1+5
         l=sadr(ilr+m+nel)
         lstk(top+1)=l
         inc=1
         if(ilr.gt.lind) inc=-1
         call icopy(m+nel,istk(lind),inc,istk(ilr),inc)
      endif
      return
      end

      subroutine intspget(id)
      include '../stack.h'
      double precision tv
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne.1) then
         call error(39)
         return
      endif
      if (lhs .gt. 3) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      ityp=istk(il)
      if(ityp.ne.5.and.ityp.ne.6) then
         call funnam(ids(1,pt+1),'spget',iadr(lstk(top)))
         fun=-1
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ilr=il+5
      ilc=ilr+m
      l=sadr(ilc+nel)

      if(nel.eq.0) then
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(lhs.ge.2) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         endif
         if(lhs.eq.3) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=1
            istk(il+2)=2
            istk(il+3)=0
            l=sadr(il+4)
            stk(l)=m
            stk(l+1)=n
            lstk(top+1)=l+2
         endif
         return
      endif
      lij=sadr(il+4)
      ilv=iadr(lij+2*nel)
      if(ityp.eq.5) then
         lv=sadr(ilv+4)
         ilrs=iadr(max(lw,lv+nel*(it+1)))
      else
         ilrs=iadr(max(lw,lij+2*nel))
      endif
      lw=sadr(ilrs+m+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     sauvegarde
      call icopy(m+nel,istk(ilr),1,istk(ilrs),1)
c     constitution des variables de sortie
      if(ityp.eq.5) then
         if(l.ge.lv) then
            call unsfdcopy(nel*(it+1),stk(l),1,stk(lv),1)
         else
            call unsfdcopy(nel*(it+1),stk(l),-1,stk(lv),-1)
         endif
      endif
      call int2db(nel,istk(ilrs+m),1,stk(lij+nel),1)
      i1=0
      do 30 i=1,m
         if(istk(ilrs-1+i).ne.0) then
            tv=i
            call dset(istk(ilrs-1+i),tv,stk(lij+i1),1)
            i1=i1+istk(ilrs-1+i)
         endif
 30   continue
      istk(il)=1
      istk(il+1)=nel
      istk(il+2)=2
      istk(il+3)=0
      lstk(top+1)=lij+2*nel
      if(lhs.ge.2) then
         top=top+1
         if(ityp.eq.5) then
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=nel
            istk(il+2)=1
            istk(il+3)=it
            lstk(top+1)=lv+nel*(it+1)
         else
            il=iadr(lstk(top))
            istk(il)=4
            istk(il+1)=nel
            istk(il+2)=1
            call iset(nel,1,istk(il+3),1)
            lstk(top+1)=sadr(il+3+nel)
         endif
      endif
      if(lhs.eq.3) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=2
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=m
         stk(l+1)=n
         lstk(top+1)=l+2
      endif
      return
      end

      subroutine intfull(id)
      include '../stack.h'
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      if(istk(il).eq.1.or.istk(il).eq.2) return
      if(istk(il).ne.5.and.istk(il).ne.6) then
         call funnam(ids(1,pt+1),'full',iadr(lstk(top)))
         fun=-1
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ilr=il+5
      ilc=ilr+m
      if(istk(il).eq.5) then
c     matrix of scalar
         l=sadr(ilc+nel)
         ils=iadr(max(sadr(il+4)+m*n*(it+1),lw))
         ls=sadr(ils+m+nel)
         lw=ls+nel*(it+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m+nel,istk(ilr),1,istk(ils),1)
         call unsfdcopy(nel*(it+1),stk(l),1,stk(ls),1)
         istk(il)=1
         l=sadr(il+4)
         if(it.eq.0) then
            call dspful(m,n,stk(ls),nel,istk(ils),stk(l))
         else
            call wspful(m,n,stk(ls),stk(ls+nel),nel,istk(ils),
     $           stk(l),stk(l+m*n))
         endif
         lstk(top+1)=l+m*n*(it+1)
      else
c     matrix of boolean
         ils=max(il+3+m*n,iadr(lw))
         lw=sadr(ils+m+nel)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(m+nel,istk(ilr),1,istk(ils),1)
         istk(il)=4
         l=il+3
         call lspful(m,n,nel,istk(ils),istk(l))
         lstk(top+1)=sadr(l+m*n)
      endif
      return
      end

      subroutine intlufact(id)
      include '../stack.h'
      double precision abstol,reltol
      integer id(nsiz),top0,tops,rhss
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
      rhss=rhs

      if (rhs .ne. 1.and.rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
      abstol=stk(leps)
      reltol=0.001d0
      if(rhs.eq.2) then
c     checking variable tol (number 2)
         il=iadr(lstk(top))
         if (istk(il) .ne. 1) then
            err = 1
            call error(53)
            return
         endif
         l=sadr(il+4)
         if (istk(il+2)*istk(il+1).eq.1) then
            abstol=stk(l)
         elseif (istk(il+2)*istk(il+1).eq.2) then
            abstol=stk(l)
            reltol=stk(l+1)
         else
            err = 1
            call error(89)
            return
         endif
         top=top-1
         rhs=rhs-1
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     
c     checking variable sp (number 1)
c     
      il1 = iadr(lstk(top))
      if (istk(il1) .ne. 5) then
         call funnam(ids(1,pt+1),'lufact',iadr(lstk(top)))
         fun=-1
         top=tops
         rhs=rhss
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      if(it.ne.0) then
         err=1
         call error(52)
         return
      endif

      lw5=lw
      lw=lw+1
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
      mx=max(m,n)
      call lufact1(stk(l),istk(il1+5),istk(il1+5+m),mx,nel,
     $     stk(lw5),abstol,reltol,nrank,ierr)
      if(ierr.gt.0) then
         buf='not enough memory'
         call error(9999)
         return
      endif        
c     
      top=top-rhs
c     
c     output variable: fmat
c     
      top=top+1
      il=iadr(lstk(top))
      istk(il)=128
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
      l=sadr(il+4)
      stk(l)=stk(lw5)
      lstk(top+1)=l+1
c     
      if(lhs .eq.2) then
c     
c     output variable: rank
c     
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=nrank
         lstk(top+1)=l+1
      endif
      return
      end

      subroutine intlusolve(id)
      include '../stack.h'
      double precision abstol,reltol,hand
      integer id(nsiz),top0
      integer iadr, sadr
      logical fact

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .gt. 1) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .eq. 128) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         l1 = sadr(il1+4)
         hand=stk(l1)
         fact=.true.
      elseif(istk(il1) .eq. 5) then
         m1=istk(il1+1)
         n1=istk(il1+2)
         if(m1.ne.n1) then
            err=1
            call error(20)
            return
         endif
         it1=istk(il1+3)
         nel1=istk(il1+4)
         l1=sadr(il1+5+m1+nel1)
         if(it1.ne.0) then
            call error(220)
            return
         endif
c     
         abstol=stk(leps)
         reltol=0.001d0
         mx=max(m1,n1)
         call lufact1(stk(l1),istk(il1+5),istk(il1+5+m1),mx,nel1,
     $        hand,abstol,reltol,nrank,ierr)
         if(ierr.gt.0) then
            buf='not enough memory'
            call error(9999)
            return
         endif  
         if(nrank.ne.m1) then
            call ludel1(hand)
            call error(19)
            return
         endif
         fact=.false.
      else
         call funnam(ids(1,pt+1),'lusolve',iadr(lstk(top-rhs+1)))
         fun=-1
         return
      endif
c     checking variable b (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1.and.istk(il2) .ne. 5) then
         call funnam(ids(1,pt+1),'lusolve',iadr(lstk(top-rhs+2)))
         fun=-1
      endif
      m2 = istk(il2+1)
      n2 = istk(il2+2)
      it2 = istk(il2+3)
      if(m2.ne.m1) then
         call error(60)
         return
      endif
      l2 = sadr(il2+4)
      l2i=l2+m2*n2
c     
      if(istk(il2).eq.1) then
c     b is full
         lw3=lw
         lw3i=lw3+m2*n2
         lw=lw+m2*n2*(it2+1)
         err=lw-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     
         do 40 j=0,n2-1
            call lusolve1(hand,stk(l2+j*m2),stk(lw3+j*m2))
            if(it2.eq.1) then
               call lusolve1(hand,stk(l2i+j*m2),stk(lw3i+j*m2))
            endif
            if (err .gt. 0) return
 40      continue
         if(.not.fact) call ludel1(hand)
c     
         top=top-rhs
         lw0=lw
         mv=lw0-l0
c     
c     output variable: x
c     
         top=top+1
         il=iadr(lstk(top))
         l=sadr(il+4)
         err=l+m2*(it2+1)-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(il)=1
         istk(il+1)=m2
         istk(il+2)=n2
         istk(il+3)=it2
         call unsfdcopy(m2*n2*(it2+1),stk(lw3),1,stk(l),1)
         lstk(top+1)=l+m2*n2*(it2+1)
      else
c     b is sparse
         call error(222)
         return
      endif
      return
      end

      subroutine intludel(id)
      include '../stack.h'
      integer id(nsiz),top0
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))

      if (istk(il1) .ne. 128) then
         err=1
         call error(218)
         return
      endif
      l1 = sadr(il1+4)

      call ludel1(stk(l1))
      if (err .gt. 0) return
c     
      top=top-rhs
c     no output variable
      top=top+1
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end


      subroutine intluget(id)
      include '../stack.h'
      double precision ptr
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 4) then
         call error(41)
         return
      endif
c     checking variable fmat (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 128) then
         err=1
         call error(218)
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      it1=istk(il1+3)
      l1 = sadr(il1+4)
      ptr=stk(l1)
      call lusiz1(ptr,nl,nu)
      ilp=il1
      lp=sadr(ilp+5+m+m)
      lw=lp+m*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ill=iadr(lstk(top))
      ll=sadr(ill+5+m+nl)
      lw=ll+nl*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ilu=iadr(lstk(top))
      lu=sadr(ilu+5+n+nu)
      lw=lu+nu*(it1+1)
      lstk(top+1)=lw
c     
      top=top+1
      ilq=iadr(lstk(top))
      lq=sadr(ilq+5+n+n)
      lw=lq+n*(it1+1)
      lstk(top+1)=lw
c     
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ilp)=5
      istk(ilp+1)=n
      istk(ilp+2)=n
      istk(ilp+3)=it1
      istk(ilp+4)=n
c     
      istk(ill)=5
      istk(ill+1)=n
      istk(ill+2)=n
      istk(ill+3)=it1
      istk(ill+4)=nl
c     
      istk(ilu)=5
      istk(ilu+1)=n
      istk(ilu+2)=n
      istk(ilu+3)=it1
      istk(ilu+4)=nu
c     
      istk(ilq)=5
      istk(ilq+1)=n
      istk(ilq+2)=n
      istk(ilq+3)=it1
      istk(ilq+4)=n
c     
      call luget1(ptr,istk(ilp+5),stk(lp),istk(ill+5),stk(ll),
     $     istk(ilu+5),stk(lu),istk(ilq+5),stk(lq))
      
      return
      end
      
      subroutine intspclean(id)
      include '../stack.h'
      double precision abstol,reltol
      integer id(nsiz),top0,tops,rhss
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      top0=top+1-rhs
      tops=top
      rhss=rhs
c
      abstol=1.0d-10
      reltol=1.0d-10
      if(rhs.eq.2) then
c     checking variable tol (number 2)
         il=iadr(lstk(top))
         if (istk(il) .ne. 1) then
            err = 1
            call error(53)
            return
         endif
         l=sadr(il+4)
         if (istk(il+2)*istk(il+1).eq.1) then
            abstol=stk(l)
         elseif (istk(il+2)*istk(il+1).eq.2) then
            abstol=stk(l)
            reltol=stk(l+1)
         else
            err = 1
            call error(89)
            return
         endif
         top=top-1
         rhs=rhs-1
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     
c     checking variable sp (number 1)
c     
      il1 = iadr(lstk(top))
      if (istk(il1) .ne. 5) then
         call funnam(ids(1,pt+1),'clean',iadr(lstk(top)))
         fun=-1
         top=tops
         rhs=rhss
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)
      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      ilind=iadr(lw)
      lw=sadr(ilind+m+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it.eq.0) then
         call dspcle(m,n,stk(l),nel,istk(il1+5),stk(l),nelr,
     $        istk(ilind),abstol,reltol)
      else
         call  wspcle(m,n,stk(l),stk(l+nel),nel,istk(il1+5),stk(l),
     $        stk(l+nel),nelr,istk(ilind),abstol,reltol)
      endif
      if(nelr.eq.nel) return
      call icopy(m+nelr,istk(ilind),1,istk(il1+5),1)
      l1=sadr(il1+5+m+nelr)
      call unsfdcopy(nelr,stk(l),1,stk(l1),1)
      if(it.eq.1) call unsfdcopy(nelr,stk(l+nel),1,stk(l1+nelr),1)
      istk(il1+4)=nelr
      lstk(top+1)=l1+nelr*(it+1)
      return
      end

      subroutine intnnz(id)
      include '../stack.h'
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     
c     checking variable sp (number 1)
c     
      il1 = iadr(lstk(top))
      if (istk(il1) .eq. 5) then
         m=istk(il1+1)
         n=istk(il1+2)
         it=istk(il1+3)
         nel=istk(il1+4)
      elseif (istk(il1) .eq. 1) then
         m=istk(il1+1)
         n=istk(il1+2)
         it=istk(il1+3)
         l=sadr(il1+4)
         nel=0
         if(it.eq.0) then
            do 50 i=0,m*n-1
               if(stk(l+i).ne.0.0d0) nel=nel+1
 50         continue
         else
            li=l+m*n
            do 51 i=0,m*n-1
               if(abs(stk(l+i))+abs(stk(li+i)).ne.0.0d0) nel=nel+1
 51         continue
         endif
      else
         call funnam(ids(1,pt+1),'nnz',iadr(lstk(top)))
         fun=-1
         return
      endif
      istk(il1)=1
      istk(il1+1)=1
      istk(il1+2)=1
      istk(il1+3)=0
      l=sadr(il1+4)
      stk(l)=nel
      lstk(top+1)=l+1
      return
      end

      subroutine intspmax(fname,id)
      include '../stack.h'
      integer id(nsiz),top0
      character*(*) fname
      double precision tv
      integer iadr, sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      top0=top+1-rhs

      if (rhs .lt. 1) then
         call error(39)
         return
      endif

      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs.eq.1) then
         if (lhs .gt.2) then
            call error(41)
            return
         endif
         il1 = iadr(lstk(top))
         if (istk(il1) .ne. 5) then
            err=1
            call error(217)
            return
         endif
         m1=istk(il1+1)
         n1=istk(il1+2)
         it1=istk(il1+3)
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)
         if(it1.ne.0) then
            err=1
            call error(52)
            return
         endif
         tv=0.0d0
         if(nel1.gt.0) then
            tv=stk(l1)
            im=0
            if(fin.eq.10) then
               do 60 i=0,nel1-1
                  if (stk(l1+i).gt.tv) then
                     tv=stk(l1+i)
                     im=i
                  endif
 60            continue
               if(tv.lt.0.0d0.and.nel1.lt.m1*n1) tv=0.0d0
            else
               do 61 i=0,nel1-1
                  if (stk(l1+i).lt.tv) then
                     tv=stk(l1+i)
                     im=i
                  endif
 61            continue
               if(tv.gt.0.0d0.and.nel1.lt.m1*n1) tv=0.0d0
            endif
         endif
c
         if(lhs.eq.2) then
            jm=istk(irc1+m1+im)
            im=im+1
            ii=0
            do 62 i=0,m1-1
               ii=ii+istk(irc1+i)
               if(ii.ge.im) goto 63
 62         continue
 63         im=i+1
         endif
c
         istk(il1)=1
         istk(il1+1)=1
         istk(il1+2)=1
         istk(il1+3)=0
         l=sadr(il1+4)
         stk(l)=tv
         lstk(top+1)=l+1
         if(lhs.eq.2) then
            top=top+1
            il2=iadr(lstk(top))
            err=lstk(top)+4-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(il2)=1
            istk(il2+1)=1
            istk(il2+2)=1
            istk(il2+3)=0
            l=sadr(il2+4)
            stk(l)=im+(jm-1)*m1
            lstk(top+1)=l+1
         endif
         return
      endif
c     
c     checking variable sp2 (number 2)
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      do 65 i=1,rhs-1
         il2 = iadr(lstk(top))
         if (istk(il2) .ne. 5) then
            err=2
            call error(217)
            return
         endif

         m2=istk(il2+1)
         n2=istk(il2+2)
         it2=istk(il2+3)
         nel2=istk(il2+4)
         irc2=il2+5
         if(it2.ne.0) then
            err=2
            call error(52)
            return
         endif
         l2=sadr(irc2+m2+nel2)
c     
c     checking variable sp1 (number 1)
c     
         top=top-1
         il1 = iadr(lstk(top))
         if (istk(il1) .ne. 5) then
            err=1
            call error(217)
            return
         endif
         m1=istk(il1+1)
         n1=istk(il1+2)
         it1=istk(il1+3)
         nel1=istk(il1+4)
         irc1=il1+5
         l1=sadr(irc1+m1+nel1)

         if(it1.ne.0) then
            err=1
            call error(52)
            return
         endif
         if(m1.ne.m2.or.n1.ne.n2) then
            call error(60)
            return
         endif
         irc=iadr(lw)
         nelmx=(iadr(lstk(bot))-irc-m1-10)/3
         lc=sadr(irc+m1+nelmx)
         lw=lc+nelmx
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif  
         nel=nelmx
         if(fname.eq.'max') then
            call dspmax(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $           istk(irc2),stk(lc),nel,istk(irc),ierr)
         else
            call dspmin(m1,n1,stk(l1),nel1,istk(irc1),stk(l2),nel2,
     $           istk(irc2),stk(lc),nel,istk(irc),ierr)
         endif
         if(ierr.ne.0) then
            call error(17)
            return
         endif
         istk(il1+3)=0
         istk(il1+4)=nel
         call icopy(m1+nel,istk(irc),1,istk(irc1),1)
         l1=sadr(irc1+m1+nel)
         call unsfdcopy(nel,stk(lc),1,stk(l1),1)
         lstk(top+1)=l1+nel
 65   continue
      return
      end

      subroutine intspmatrix(id)
      include '../stack.h'
      integer id(nsiz),top0,tops
      integer iadr, sadr
      double precisionx sz, szr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)
      tops=top
      top0=top+1-rhs

      if (rhs .lt.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
c     checking variable ne
      il=iadr(lstk(top))
      if(istk(il).ne.1.or.istk(il+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      if(rhs.eq.2) then
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).eq.1) then
            mr=int(stk(sadr(il+4)))
            nr=1
         elseif(istk(il+1)*istk(il+2).eq.2) then
            mr=int(stk(sadr(il+4)))
            nr=int(stk(sadr(il+4)+1))
         else
            top=tops
            call ref2val
            call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
      elseif(rhs.gt.3) then
            top=tops
            call ref2val
            call funnam(ids(1,pt+1),'matrix',iadr(lstk(top-rhs+1)))
            fun=-1
            return
      elseif(rhs.eq.3) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         l=sadr(il+4)
         nr=stk(l)
c     checking variable mr
         top=top-1
         il=iadr(lstk(top))
         if(istk(il).ne.1.or.istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         l=sadr(il+4)
         mr=stk(l)
      endif
c     checking variable sp
      top=top-1
      il1 = iadr(lstk(top))
      if (istk(il1) .ne. 5.and.istk(il1) .ne. 6) then
         err = 1
         call error(219)
         return
      endif
      m=istk(il1+1)
      n=istk(il1+2)

C     to avoid integer overflow
      sz=dble(m)*dble(n)
      szr=dble(mr)*dble(nr)
      
      if(sz.ne.szr) then
         call error(60)
         return
      endif
      if(mr.eq.m) return

      it=istk(il1+3)
      nel=istk(il1+4)
      l=sadr(il1+5+m+nel)
      if(istk(il1).eq.5) then
         if(mr.gt.m) then
            ls=sadr(il1+5+mr+nel)
            ils=iadr(ls+nel*(it+1))
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nel*(it+1),stk(l),-1,stk(ls),-1)
            call icopy(nel+m,istk(il1+5),-1,istk(ils),-1)
         else
            ls=sadr(il1+5+mr+nel)
            ils=iadr(lw)
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),1,istk(ils),1)
            call unsfdcopy(nel*(it+1),stk(l),1,stk(ls),1)
         endif
         if(it.eq.0) then
            call dspmat(m,n,stk(ls),nel,istk(ils),mr,istk(il1+5),
     $           istk(iw))
         else
            call wspmat(m,n,stk(ls),stk(ls+nel),nel,istk(ils),mr,
     $           istk(il1+5),istk(iw))
         endif
         lstk(top+1)=ls+nel*(it+1)
      else
         if(mr.gt.m) then
            ils=il1+5+mr+nel
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),-1,istk(ils),-1)
         else
            ls=sadr(il1+5+mr+nel)
            ils=iadr(lw)
            iw=ils+nel+m
            lw=sadr(iw+3*nel)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(nel+m,istk(il1+5),1,istk(ils),1)
         endif
         call lspmat(m,n,nel,istk(ils),mr,istk(il1+5),istk(iw))
         lstk(top+1)=sadr(il1+5+mr+nel)
      endif
      istk(il1+1)=mr
      istk(il1+2)=nr
      return
      end

      subroutine intspchol(id)
c     
      include '../stack.h'
      integer id(nsiz)
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      rhs = max(0,rhs)

      lw = lstk(top+1)
      l0=  lstk(top+1-rhs)
      if (rhs .ne.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 2) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      ityp=istk(il)
      if(ityp.ne.5.and.ityp.ne.6) then
         err=1 
         call error(217)
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      if (m.ne.n) then
         call error(20)
         return
      endif
      it=istk(il+3)
      if (it.ne.0) then
         call error(52)
         return
      endif
      iinda=il+5
      ia=sadr(iinda+nel+n)
      ixadjf=iadr(ia+nel)
      iperm=ixadjf+n+1
      iinvp=iperm+n
      icolcnt=iinvp+n
      isnode=icolcnt+n
      ixadj=isnode+n
      iadjncy=ixadj+n+1
      ixsuper=iadjncy+nel-n
      iiwork=ixsuper+n+1
      iindlnz=iiwork+7*n+3
      ilnz=sadr(iindlnz+n+nel)
      idiag=ilnz+nel
      err = idiag+n-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      call spcho1(n,stk(ia),nel,istk(iinda),istk(ixadjf),istk(iperm),
     &     istk(iinvp),istk(icolcnt),istk(isnode),nnzl,nsub,nsuper,
     &     istk(ixsuper),istk(iiwork),istk(ixadj),istk(iadjncy),
     &     istk(iindlnz), stk(ilnz),stk(idiag),ierr)
C     
      if (ierr .eq. 1) then
         call error(29)
         if(n.le.0) then
            call blkslv(0,0,0,0,0,0.d0,0.d0)
         endif
         return
      endif
      iwsiz=nsuper+2*n+1
C************************************************************************
      iiwork=ixsuper+nsuper+1
      ixlindx=iiwork+2*nsuper+2*n+1
      ilindx=ixlindx+nsuper+1
      ixlnz=ilindx+nsub
      ilnz=sadr(ixlnz+n+1)
      isplit=iadr(ilnz+nnzl)
      itmpvec=sadr(isplit+n)
      CALL SYMFCT (N,nel-n,istk(iXADJ),istk(iADJNCY), istk(iPERM), 
     &     istk(iINVP),istk(iCOLCNT),NSUPER,istk(iXSUPER),istk(iSNODE),
     &     nsub , istk(iXLINDX), istk(iLINDX),istk(iXLNZ),nsuper+2*n+1,
     &     istk(iiwork) , IFLAG    )
C     
      CALL  BFINIT ( n , NSUPER,istk(iXSUPER),istk(iSNODE) , 
     &     istk(iXLINDX),istk(iLINDX), 16, iTMPSIZ, istk(iSPLIT) )
c     
      itmpsiz=max(itmpsiz,n)
      iindlnz=iadr(itmpvec+itmpsiz)
      err = sadr(iindlnz+n+nnzl)-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
C     
      CALL INPNV (n,istk(iXADJF),istk(iinda+n),stk(ia),istk(iPERM),
     &     istk(iINVP),NSUPER, istk(iXSUPER),istk(iXLINDX),istk(iLINDX),
     &     istk(iXLNZ),stk(iLNZ),istk(iIWORK)     )
C     
      CALL  SPCHO2 ( n , NSUPER, istk(iXSUPER),istk(iSNODE) ,
     &     istk(iSPLIT) , istk(iXLINDX), istk(iLINDX) ,istk(iXLNZ)  , 
     &     stk(iLNZ)  , istk(iIWORK) ,iTMPSIZ,stk(iTMPVEC),
     &     istk(iindlnz),nnzl,nsub)
      nel=istk(ixlnz+n)-1
      ia=sadr(iinda+nel+n)
      ierr=5
      do 32 i=1,nel
         if (stk(iLNZ+i-1) .gt. 5.D+63) then
            ierr=1
         endif
 32   continue
      if (ierr .eq. 1) then
         call basout(io,wte,
     &        'WARNING: Possible not positive definite matrix!')
      endif
      call icopy(n,istk(iinvp),1,istk(iindlnz-n),1)
      if (ia+nel .GE. sadr(ixlnz)) then
         ixln=iadr(ia+nel)
         iln=sadr(ixln+n+1)
         isplit=iadr(iln+nnzl)
         itmpvec=sadr(isplit+n)
         iindn=iadr(itmpvec+itmpsiz)
         do 33 i=1,n+nnzl
            istk(iindn+n+nnzl-i)=istk(iindlnz+n+nnzl-i)
 33      continue
         iindlnz=iindn
         do 34 i=1,nnzl
            stk(iln+nnzl-i)=stk(ilnz+nnzl-i)
 34      continue
         ilnz=iln
         do 35 i=1,n+1
            istk(ixln+n+1-i)=istk(ixlnz+n+1-i)      
 35      continue
         ixlnz=ixln
         call icopy(n,istk(iinvp),1,istk(iindlnz-n),1)
         err = sadr(iindlnz+n+nnzl)-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
      endif
C     
      call dspt(n,n,stk(ilnz),nel,istk(iindlnz),istk(ixlnz),
     &     stk(ia),istk(isplit),istk(iinda))
      istk(il+4)=nel
      top=top+1 

      lstk(top)=ia+nel
      lnl=iadr(ia+nel)
      istk(lnl)=ityp
      istk(lnl+1)=n
      istk(lnl+2)=n
      istk(lnl+3)=0
      istk(lnl+4)=n        
      call icopy(n,istk(iindlnz-n),1,istk(lnl+5+n),1)
      ilp=sadr(lnl+5+2*n)
      do 70 i=1,n
         istk(lnl+4+i)=1
         stk(ilp+i-1)=1.0d0
 70   continue

      lstk(top+1)=ilp+n
c     
      return
      end


      subroutine intfadj2sp(id)
      include '../stack.h'
c**********************************************************
      logical getrhsvar, createvar, putlhsvar
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************
       minrhs=3
       maxrhs=3
       minlhs=1
       maxlhs=1
c
       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return

c*****************************************************
c      2-If necessary, create additional variables
c          (working arrays, default values, ...)
c*****************************************************
        k=maxrhs
        if(.not.createvar(k+1,'i',istk(l3),1,l4)) return
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call spind(istk(l4),istk(l1),istk(l2),istk(l3))
c******************************************************
c      5- Set lhs parameters
c******************************************************
        lhsvar(1)=4
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
      if(.not.putlhsvar()) return
c
       return
      end

      subroutine intspcompa(id)
c**********************************************************
      logical getrhsvar, createvar, putlhsvar
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=3
       maxrhs=3
       minlhs=1
       maxlhs=1
c
       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif
c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar(1,'i',m1,n1,l1)) return
       if(.not.getrhsvar(2,'i',m2,n2,l2)) return
       if(.not.getrhsvar(3,'i',m3,n3,l3)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************
       k=maxrhs+1
       if(.not.createvar(k,'i',istk(l1+m1-1)-1,1,l4)) return
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call spcompack(m1*n1-1,m2*n2-1,m3*n3,istk(l1+m1*n1-1)-1,
     $ istk(l2),istk(l3),istk(l1),istk(l4))

c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar(1)=4
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c
       return
       end

c SCILAB function ordmmd
      subroutine intordmmd(id)
c**********************************************************
      logical getrhsvar, createvar, putlhsvar
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=3
       maxrhs=3
       minlhs=3
       maxlhs=3
c
       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar(1,'i',m1,n1,l1)) return
       if(.not.getrhsvar(2,'i',m2,n2,l2)) return
       if(.not.getrhsvar(3,'i',m3,n3,l3)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

       k=maxrhs+1
       if(.not.createvar(k,  'i',istk(l3),1,l4)) return
       if(.not.createvar(k+1,'i',istk(l3),1,l5)) return
       if(.not.createvar(k+2,'i',1,1,l6)) return
       if(.not.createvar(k+3,'i',4*istk(l3),1,l7)) return 
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call ordmmd(istk(l3),istk(l1),istk(l2),istk(l5),
     $ istk(l4),4*istk(l3),istk(l7),istk(l6),iflag)

c******************************************************
c      4-Display error message(s)
c******************************************************
       if(iflag .NE. 0) then 
        buf = ' insufficient working storage'
        call error(9999)
        return
       endif
c
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar(1)=4
       lhsvar(2)=5
       lhsvar(3)=6
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c
       return
       end


c SCILAB function blkfc1i
      subroutine intblkfc1i(id)
      logical getrhsvar, createvar, putlhsvar      
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=15
       maxrhs=15
       minlhs=2
       maxlhs=2
c

       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'i', m7, n7, l7)) return
       if(.not.getrhsvar( 8,'i', m8, n8, l8)) return
       if(.not.getrhsvar( 9,'d', m9, n9, l9)) return
       if(.not.getrhsvar(10,'i',m10,n10,l10)) return
       if(.not.getrhsvar(11,'i',m11,n11,l11)) return
       if(.not.getrhsvar(12,'i',m12,n12,l12)) return
       if(.not.getrhsvar(13,'d',m10,n13,l13)) return
       if(.not.getrhsvar(14,'i',m11,n14,l14)) return
       if(.not.getrhsvar(15,'i',m12,n15,l15)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************
    
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call blkfc1(istk(l1),istk(l2),istk(l3) ,istk(l4),
     $ istk( l5),istk( l6),istk( l7),istk( l8), stk(l9),
     $ istk(l10),istk(l11),istk(l12), stk(l13),
     $ istk(l14),istk(l15))
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 9
       lhsvar( 2)= 14

c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
      end

c SCILAB function blkslvi
      subroutine intblkslvi(id)
      logical getrhsvar, createvar, putlhsvar            
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=7
       maxrhs=7
       minlhs=1
       maxlhs=1
c

       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'d', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'d', m7, n7, l7)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call blkslv(istk(l1),istk(l2),istk(l3),istk(l4),
     $ istk(l5),stk(l6),stk(l7))
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 7
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
      end

c SCILAB function inpnvi
      subroutine intinpnvi(id)
      logical getrhsvar, createvar, putlhsvar      
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=13
       maxrhs=13
       minlhs=1
       maxlhs=1
c

       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'d', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'i', m7, n7, l7)) return
       if(.not.getrhsvar( 8,'i', m8, n8, l8)) return
       if(.not.getrhsvar( 9,'i', m9, n9, l9)) return
       if(.not.getrhsvar(10,'i',m10,n10,l10)) return
       if(.not.getrhsvar(11,'i',m11,n11,l11)) return
       if(.not.getrhsvar(12,'d',m12,n12,l12)) return
       if(.not.getrhsvar(13,'i',m13,n13,l13)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call inpnv(istk(l1),istk(l2),istk(l3),stk(l4),
     $ istk(l5) ,istk(l6) ,istk(l7) ,istk(l8),istk(l9),
     $ istk(l10),istk(l11), stk(l12),istk(l13))
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 12
c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
       end

c SCILAB function sfinit
      subroutine intsfinit(id)
      logical getrhsvar, createvar, putlhsvar
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=8
       maxrhs=8
       minlhs=9
       maxlhs=9
c
c
       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'i', m7, n7, l7)) return
       if(.not.getrhsvar( 8,'i', m8, n8, l8)) return
**************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

       k=maxrhs+1
       if(.not.createvar(k  ,'i',istk(l1),1,l9)) return
       if(.not.createvar(k+1,'i',1,1,l10)) return
       if(.not.createvar(k+2,'i',1,1,l11)) return
       if(.not.createvar(k+3,'i',1,1,l12)) return
       if(.not.createvar(k+4,'i',istk(l1),1,l13)) return
       if(.not.createvar(k+5,'i',istk(l1)+1,1,l14)) return
       if(.not.createvar(k+6,'i',1,1,l15)) return
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call sfinit(istk(l1),istk(l2),istk(l3) ,istk(l4),
     $  istk(l5), istk(l6) ,istk(l9) ,istk(l10),istk(l11),
     $  istk(l12),istk(l13),istk(l14),
     $  istk(l7), istk(l8) ,istk(l15))

c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 5
       lhsvar( 2)= 6
       lhsvar( 3)= 9
       lhsvar( 4)= 10
       lhsvar( 5)= 11
       lhsvar( 6)= 12
       lhsvar( 7)= 13
       lhsvar( 8)= 14
       lhsvar( 9)= 15

c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
      end
c SCILAB function symfcti
      subroutine intsymfcti(id)
      logical getrhsvar, createvar, putlhsvar
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=13
       maxrhs=13
       minlhs=4
       maxlhs=4
c

       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'i', m7, n7, l7)) return
       if(.not.getrhsvar( 8,'i', m8, n8, l8)) return
       if(.not.getrhsvar( 9,'i', m9, n9, l9)) return
       if(.not.getrhsvar(10,'i',m10,n10,l10)) return
       if(.not.getrhsvar(11,'i',m11,n11,l11)) return
       if(.not.getrhsvar(12,'i',m12,n12,l12)) return
       if(.not.getrhsvar(13,'i',m13,n13,l13)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

       k=maxrhs+1
       if(.not.createvar(k  ,'i',istk(l8)+1,1,l14)) return
       if(.not.createvar(k+1,'i',istk(l11) ,1,l15)) return
       if(.not.createvar(k+2,'i',istk(l1)+1,1,l16)) return
       if(.not.createvar(k+3,'i',1,1,l17)) return
 
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call symfct(istk(l1),istk(l2),istk(l3),istk(l4),
     $ istk(l5) ,istk(l6)  ,istk(l7) ,istk(l8),istk(l9),
     $ istk(l10),istk(l11) ,istk(l14),istk(l15),
     $ istk(l16),istk(l12) ,istk(l13),istk(l17))
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 14
       lhsvar( 2)= 15
       lhsvar( 3)= 16
       lhsvar( 4)= 17

c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
      end

c SCILAB function bfinit
      subroutine intbfinit(id)
      logical getrhsvar, createvar, putlhsvar
      include '../stack.h'
c
       nbvars=0
c*****************************************************
c      0-Check number of rhs and lhs arguments
c*****************************************************       
       minrhs=7
       maxrhs=7
       minlhs=2
       maxlhs=2
c

       if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
          call erro('wrong number of rhs arguments')
          return
       endif
       if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
          call erro('wrong number of lhs arguments')
          return
       endif

c*******************************************************
c      1-Get rhs parameters and set their Fortran types
c*******************************************************
       if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
       if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
       if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
       if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
       if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
       if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
       if(.not.getrhsvar( 7,'i', m7, n7, l7)) return

c*****************************************************
c      2-If necessary, create additional variables 
c          (working arrays, default values, ...)
c*****************************************************

       k=maxrhs+1
       if(.not.createvar(k  ,'i',1,1,l8)) return
       if(.not.createvar(k+1,'i',istk(l1),1,l9)) return
c******************************************************
c      3-Routine call
c      stk  <-> double
c      sstk <-> real
c      istk <-> integer
c      cstk <-> character
c*****************************************************
       call bfinit(istk(l1),istk(l2),istk(l3),istk(l4),
     $ istk(l5),istk(l6),istk(l7),istk(l8),istk(l9))
c******************************************************
c      5- Set lhs parameters
c******************************************************
       lhsvar( 1)= 8
       lhsvar( 2)= 9

c******************************************************
c      6-Sending lhs variables to Scilab
c******************************************************
       if(.not.putlhsvar()) return
c      .
       return
      end

