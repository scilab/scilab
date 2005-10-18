      subroutine impelm
c ====================================================================
c
c     evaluate utility functions
c
c ====================================================================
c
      include '../stack.h'
      include 'mpstack.h'
c      double precision PCZERO,PCONE,PCTOP,ZERO,TOPM
c      integer PCALGCODE


      double precision s,sr,si,t,x,x1,eps,epsr,epsa,t1
c
      integer id(nsiz)
      integer unifor,normal,seed,magi,frk,top2,hilb
      double precision dsum,pythag,round,urand,norm
      integer iadr,sadr
      integer sel,row,col
      integer typecode,algtype
      logical getscalar
      integer fd

      data unifor/30/,normal/23/,seed/28/,magi/22/,frk/15/
      data row/27/,col/12/,star/47/
      data hilb/17/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c      call commons(PCZERO,PCTOP,PCALGCODE)
c      tmp=1000.d0
c      ZERO=-exp(tmp)
      call commons(ZERO)
      PCONE=0.d0
      TOPM=-ZERO
      PCZERO=ZERO
      PCTOP=TOPM
      PCALGCODE=1

c      write(*,*) 'entree',fin,rstk(pt)

c
      if (ddt .eq. 4) then
c         write(buf(1:4),'(i4)') fin
         call basout(io,wte,'impelm'//buf(1:4))
      endif

c      if ((typecode(gettype(top))).eq.5) then
c         call impspelm
c         goto 999
c      endif
c
c    

c      write(*,*) top,rhs,lhs,bot
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
      if(rhs.lt.0) then
         call error(39)
         return
      endif
c
      call ref2val
      if(rhs.eq.0) goto 05
      il=iadr(lstk(top))
      if(typecode(istk(il)).gt.10) goto 05
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

      if(typecode(istk(il)).eq.5) then
         mn=istk(il+4)
         l=sadr(il+5+m+mn)
      elseif(typecode(istk(il)).eq.1) then
         l=sadr(il+4)
         mn=m*n
      elseif(typecode(istk(il)).eq.2) then
         l=sadr(il+9+m*n)
         mn=istk(il+8+m*n)-1
         id1=il+8
      endif

      lw=lstk(top+10)
c
c---------------------------------------------------------------- 
c     functions/fin
c     abs  real imag conj roun int  size sum  prod diag triu tril
c      1    2    3    4    5    6    7    8    9    10   11   12
c      %eye  rand  %ones maxi mini sort kron matr sin  cos  atan exp
c      13     14    15   16   17   18  19-21 22   23   24   25   26
c     sqrt log   ^  sign clean floor ceil tropical setalg karp
c      27   28   29  30   31    32    33   34        35     36
c     save load print
c     37   38    39
c------------------------------------------------------------------     
c
c      write(*,*)fin
   05 goto (10,15,20,25,30,35,40,50,45,60,60,60,
     1     70,70,70,90,90,105,110,110,110,130,140,150,160,170,
     2    180,190,200,210,220,37,39,230,240,250,350,450,550),fin
c-------------------------------------------------------------
c    abs
c-------------------------------------------------------------
 10   if (lhs .ne. 1) then
         call error(41)
         return
      endif
      l1=l-1
      if(it.eq.1) goto 12
      do 11 i=1,mn
   11 stk(l1+i)=abs(stk(l1+i))
      goto 999
   12 continue
      k1=l1+mn
      do 13 i=1,mn
   13 stk(l1+i)=pythag(stk(l1+i),stk(k1+i))
      istk(il+3)=0
      lstk(top+1)=l+mn
      goto 999
c------------------------------------------------------------
c     real
c------------------------------------------------------------
   15 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      istk(il+3)=0
      lstk(top+1)=l+mn
      goto 999
c----------------------------------------------------------
c     imag
c----------------------------------------------------------
   20 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      istk(il+3)=0
      if(it.eq.1) call dcopy(mn,stk(l+mn),1,stk(l),1)
      if(it.eq.0) call dset(mn,0.0d+0,stk(l),1)
      lstk(top+1)=l+mn
      goto 999
c--------------------------------------------------------
c     conjg
c-------------------------------------------------------
   25 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      if(it.eq.1) call dscal(mn,-1.0d+0,stk(l+mn),1)
      goto 999
c-----------------------------------------------------
c     round
c-----------------------------------------------------
   30 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      if(it.eq.1) mn=2*mn
      do 31 i=1,mn
       i1=i-1
       stk(l+i1)=anint(stk(l+i1))
   31 continue
      goto 999
c------------------------------------------------------
c     int
c------------------------------------------------------
   35 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(it.eq.1) mn=2*mn
      do 36 i=1,mn
       i1=i-1
       stk(l+i1)=aint(stk(l+i1))
   36 continue
      goto 999
c---------------------------------------------------
c     floor
c---------------------------------------------------
   37 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

      if(it.eq.1) mn=2*mn
      call vfloor(mn,stk(l),1,stk(l),1)

c      do 38 i=1,mn
c        i1=i-1
c        t=stk(l+i1)
c        t1=round(t-0.5d0)
c        if(t.ge.0.d0.and.((t-t1).eq.1.0d0)) t1=t
c        stk(l+i1)=t1
c   38 continue
      goto 999
c----------------------------------------------------
c     ceil
c----------------------------------------------------
   39 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif

c      if(it.eq.1) mn=2*mn
c      do 32 i=1,mn
c       i1=i-1
c        t=stk(l+i1)
c        t1=round(t+0.5d0)
c        if(t.le.0.d0.and.((t1-t).eq.1.0d0)) t1=t
c        stk(l+i1)=t1
c   32 continue
      call vceil(mn,stk(l),1,stk(l),1)
      goto 999
c----------------------------------------------------
c     size
c---------------------------------------------------      
 40   if(rhs.gt.2) then
         call error(42)
         return
      endif
      sel=-1
      if(rhs.eq.2) then
         if(lhs.ne.1) then
            call error(41)
            return
         endif
c     row or column size
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
c modif sg
         if(istk(il).eq.1) then
            if(istk(il+1)*istk(il+2).ne.1) then
               err=2
               call error(89)
               return
            endif
            sel=stk(sadr(il+4))
            if(sel.ne.1.and.sel.ne.2) then
               err=2
               call error(44)
               return
            endif
         elseif (istk(il).eq.10) then
            if(istk(il+1)*istk(il+2).ne.1) then
               err=2
               call error(89)
               return
            endif
            if(istk(il+6).eq.row) then
               sel=1
            elseif(istk(il+6).eq.col) then
               sel=2
            elseif(istk(il+6).eq.star) then
               sel=3
            else
               err=2
               call error(44)
               return
            endif
         else
            err=2
            call error(44)
            return
         endif
         top=top-1
      endif
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
c     matrix type variable type
      if(lhs.gt.2) then
         call error(41)
         return
      endif
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
      if(err.gt.0) return
      if(lhs.eq.1) then
         istk(il)=1
         istk(il+1)=1
         if(sel.eq.-1) then
            istk(il+2)=2
            istk(il+3)=0
            stk(l) = m
            stk(l+1) = n
            lstk(top+1)=l+2
         elseif(sel.eq.1) then
            istk(il+2)=1
            istk(il+3)=0
            stk(l) = m
            lstk(top+1)=l+1
         elseif(sel.eq.2) then
            istk(il+2)=1
            istk(il+3)=0
            stk(l) = n
            lstk(top+1)=l+1
         elseif(sel.eq.3) then
            istk(il+2)=1
            istk(il+3)=0
            stk(l) = m*n
            lstk(top+1)=l+1
         endif
      else
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         stk(l) = m
         lstk(top+1)=l+1
         top = top + 1
         il=iadr(l+1)
         l=sadr(il+4)
         err=l+1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         stk(l) = n
         lstk(top+1)=l+1
      endif
      goto 999
c--------------------------------------------------------------
c    produit max-plus  (sum standard)
c--------------------------------------------------------------
   45 continue
      if(rhs.ne.1) then
         call error(39)
         return
      endif

      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(istk(il).ne.257.and.istk(il).ne.5) goto 900
      istk(il)=257
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=it
      l1=sadr(il+4)
      stk(l1)=dsum(mn,stk(l),1)
c      if(it.eq.1) stk(l1+1)=dsum(mn,stk(l+mn),1)
      lstk(top+1)=l1+(it+1)
      go to 999
c---------------------------------------------
c     sum max-plus (a partir de l'ancien prod) 
c--------------------------------------------
   50 continue
      if(rhs.ne.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(istk(il).ne.257) goto 900
      istk(il+1)=1
      istk(il+2)=1
      lstk(top+1)=l+it+1
      sr = PCZERO
c      si = 0.0d+0
c      if(it.eq.1) goto 52
      do 51 i = 1, mn
         ls = l+i-1
         sr=max(sr,stk(ls))
   51 continue
      stk(l)=sr
      goto 999
c   52 continue
c      do 53 i=1,mn
c      ls=l+i-1
c      s=sr*stk(ls)-si*stk(ls+mn)
c      si=sr*stk(ls+mn)+si*stk(ls)
c      sr=s
c   53 continue
c      stk(l) = sr
c      stk(l+1) = si
c      go to 999
c------------------------------------------------
c     diag, triu, tril
c------------------------------------------------
   60 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      k = 0
      il=iadr(lstk(top+1-rhs))
      if(typecode(istk(il)).ne.1) then
         if(typecode(istk(il)).eq.2) then
c           *call* polelm
            fun=16
            return
         endif
      endif
      if (rhs .ne. 2) go to 61
      il=iadr(lstk(top-1))
      k = int(stk(l))
      top = top-1
      m=istk(il+1)
      n=istk(il+2)
      mn=m*n
      it=istk(il+3)
      l=sadr(il+4)
   61 if (fin .ge. 11) go to 64
      if (m .eq. 1 .or. n .eq. 1) go to 63
c     diag(a,k)
      if (k.ge.0) mn=max(0,min(m,n-k))
      if (k.lt.0) mn=max(0,min(m+k,n))
      if(mn.eq.0) then
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)+1
         goto 999
      endif
      istk(il+1)=mn
      istk(il+2)=1
      istk(il+3)=it
      lstk(top+1)=l+istk(il+1)*(it+1)
      if(k.ge.0) call dcopy(mn,stk(l+k*m),m+1,stk(l),1)
      if(k.lt.0) call dcopy(mn,stk(l-k),m+1,stk(l),1)
      if(it.eq.0) goto 999
      if(k.ge.0) call dcopy(mn,stk(l+m*n+k*m),m+1,stk(l+mn),1)
      if(k.lt.0) call dcopy(mn,stk(l+m*n-k),m+1,stk(l+mn),1)
      go to 999
c     diag(vecteur,k)
   63 nn = max(m,n)+abs(k)
      l1=l+nn*nn*(it+1)
      err = l1 + mn*(it+1) -  lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(il+1)=nn
      istk(il+2)=nn
      lstk(top+1)=l+nn*nn*(it+1)
      call dcopy(mn*(it+1),stk(l),-1,stk(l1),-1)
      call dset(nn*nn*(it+1),PCZERO,stk(l),1)
      if(k.ge.0) call dcopy(mn,stk(l1),1,stk(l+nn*k),nn+1)
      if(k.lt.0) call dcopy(mn,stk(l1),1,stk(l-k),nn+1)
      if(it.eq.0) goto 999
      if(k.ge.0) call dcopy(mn,stk(l1+mn),1,stk(l+nn*nn+k*nn),nn+1)
      if(k.lt.0) call dcopy(mn,stk(l1+mn),1,stk(l+nn*nn-k),nn+1)
      go to 999
c
c     tril, triu
   64 if(fin.eq.12) goto 67
      if(k.le.0) goto 65
      call dset(m*k,PCZERO,stk(l),1)
      if(it.eq.1) call dset(m*k,PCZERO,stk(l+mn),1)
      l=l+m*k
      n=n-k
      k=0
   65 ls=l+1-k
      ll=m-1+k
      do 66 j=1,n
      if(ll.le.0) goto 999
      call dset(ll,PCZERO,stk(ls),1)
      if(it.eq.1) call dset(ll,PCZERO,stk(ls+mn),1)
      ll=ll-1
      ls=ls+m+1
   66 continue
      goto 999
   67 if(k.lt.0) goto 68
      l=l+m*(k+1)
      n=n-k-1
      k=-1
   68 ls=l
      ll=-k
      do 69 j=1,n
      if(ll.gt.m) ll=m
      call dset(ll,PCZERO,stk(ls),1)
      if(it.eq.1) call dset(ll,PCZERO,stk(ls+mn),1)
      ls=ls+m
      ll=ll+1
   69 continue
      go to 999
c---------------------------------------------------
c     eye, rand, ones
c--------------------------------------------------
 70   continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.gt.2) then
         call error(39)
         return
      endif
      if(rhs.eq.0) then
c     rand sans argument
         top=top+1
         il=iadr(lstk(top))
         istk(il)=257
         m=1
         n=1
         if(fin.eq.13) then
c            write(*,*)"eye()"
            m=-1
            n=-1
         endif
      elseif(rhs.eq.1) then
         if(fin.eq.14 .and. typecode(istk(il)).eq.10) goto 80
c     eye(matrice),rand(matrice),ones(matrice)
      elseif(rhs.eq.2) then
c     eye(m,n),rand(m,n),ones(m,n)
         if(typecode(istk(il)).ne.1) then
            err=1
            call  error(53)
            return
         endif
         if(istk(il+3).ne.0) then
            err=1
            call  error(52)
            return
         endif
         if(m*n.ne.1) then
            err=1
            call error(89)
            return
         endif
         n=max(int(stk(l)),0)
         top=top-1
         il=iadr(lstk(top))
         if(fin.eq.14 .and. typecode(istk(il)).eq.10) goto 80
         if(typecode(istk(il)).ne.1) then
            err=1
            call  error(53)
            return
         endif
         if(istk(il+3).ne.0) then
            err=1
            call  error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(89)
            return
         endif
         l=sadr(il+4)
         m=max(int(stk(l)),0)
      endif
c
   72 mn=m*n
      if(m.eq.0) n=0
      if(n.eq.0) m=0
      l=sadr(il+4)
      err = l+m*n - lstk(bot)
      if(err.gt.0) then
c         write(*,*)"()"
         call error(17)
         return
      endif
      istk(il)=algtype(PCALGCODE,1)
c      write(*,*)"NTYPE"
c     write(*,*)istk(il)
c     write(*,*)PCZERO
c     write(*,*)PCONE
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=l+mn
      if(mn.eq.0) goto 999
      goto(74,73) fin-13
c--------------------------------------------------
c     eye
c--------------------------------------------------
       m=abs(m)
      call dset(mn,PCZERO,stk(l),1)
      call dset(min(m,abs(n)),PCONE,stk(l),m+1)
      goto 999
c---------------------------------------------------
c     ones
c--------------------------------------------------
   73 call dset(mn,PCONE,stk(l),1)
      goto 999
c---------------------------------------------------
c     rand
c--------------------------------------------------
   74 continue
      ll=l-1
      do 76 j = 1, mn
        if (ran(2).eq.0) stk(ll+j) = urand(ran(1))
        if (ran(2).eq.0) go to 76
   75      sr=2.0d+0*urand(ran(1))-1.0d+0
           si=2.0d+0*urand(ran(1))-1.0d+0
           t = sr*sr+si*si
           if (t .gt. 1.0d+0) go to 75
        stk(ll+j) = sr*sqrt(-2.0d+0*log(t)/t)
   76 continue
      go to 999
c
 80   l=il+istk(il+1)*istk(il+2)+5
      ist=abs(istk(l))
      if(ist.eq.unifor.or.ist.eq.normal) goto 81
      if(ist.eq.seed) goto 82
      call error(31)
      return
c
c     switch uniform and normal
   81 ran(2) = ist - unifor
      istk(il)=0
      go to 999
c
c     seed
c
   82 if(rhs.eq.2) goto 83
      istk(il)=257
      istk(il+1)=1
      istk(il+2) = 1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l) = ran(1)
      lstk(top+1)=l+1
      go to 999
   83 ran(1)=max(n,0)
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      goto 999
c-------------------------------------------------------
c      maxi mini
c------------------------------------------------------
   90 continue
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
      if(istk(il).eq.257) goto 100
      if(istk(il).eq.5) then
         fin=fin-6
         call spelm
         if(err.gt.0) return
         goto 999
      endif
      if(rhs.gt.1) goto 96
      if(rhs.ne.1) then
         call error(39)
         return
      endif
      if(mn.le.0) then
         err=1
         call error(45)
         return
      endif
c     maxi mini d'un vecteur
      if(istk(il).ne.257) then
         err=1
         call error(53)
         return
      endif
      if(it.ne.0) then
         err=1
         call error(52)
         return
      endif
      k=l
      l1=l
      x1=stk(k)
      if(fin.eq.17) goto 92
c--------------------------------------------------
c     maxi
c--------------------------------------------------
      do 91 i=2,mn
        l1=l1+1
        if(stk(l1).le.x1) goto 91
        k=l1
        x1=stk(k)
   91 continue
      goto 94
c--------------------------------------------------
c      mini
c-------------------------------------------------
   92 continue
      do 93 i=2,mn
        l1=l1+1
        if(stk(l1).ge.x1) goto 93
        k=l1
        x1=stk(k)
   93 continue
      goto 94
c
   94 continue
      stk(l)=x1
      istk(il+1)=1
      istk(il+2)=1
      lstk(top+1)=l+1
      if(lhs.eq.1) goto 999
      k=k-l+1
      top=top+1
      il=iadr(lstk(top))
      l=sadr(il+4)
      err=l+2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=257
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      stk(l)=dble(k)
      lstk(top+1)=l+1
      if(m.eq.1.or.n.eq.1) goto 999
      kc=k/m
      kl=k-kc*m
      if(kl.ne.0) goto 95
      kc=kc-1
      kl=m
   95 istk(il+2)=2
      stk(l)=dble(kl)
      stk(l+1)=dble(kc+1)
      lstk(top+1)=l+2
      goto 999
c
 96   continue
      ilw=iadr(lstk(top+1))
      err=sadr(ilw+mn)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(ilw-1+rhs)=sadr(il+4)-1
c     maxi mini a plusieur argument
      do 97 i=rhs-1,1,-1
       ili=iadr(lstk(top-rhs+i))
       if(istk(ili).ne.1) then
          err=i
          call error(53)
          return
       endif
       if(istk(ili+3).ne.0) then
          err=i
          call error(52)
          return
       endif
       if(istk(ili+1).ne.m.or.istk(ili+2).ne.n) then
          err=i
          call error(42)
          return
       endif
       istk(ilw-1+i)=sadr(ili+4)-1
 97   continue
      x=-1.0d+0
      if(fin.eq.17) x=1.0d+0
      do 99 i=1,mn
        li=istk(ilw)+i
        ji=1
        do 98 j=2,rhs
           if(x*(stk(li)-stk(istk(ilw-1+j)+i)).gt.0) then
                 stk(li)=stk(istk(ilw-1+j)+i)
                 ji=j
           endif
 98     continue
        stk(istk(ilw+1)+i)=real(ji)
 99    continue
       top=top-rhs+lhs
       goto 999
c
 100  continue
c     maxi mini des vecteurs d'une liste
      ne=istk(il+1)
      if(ne.eq.1) then
         err=1
         call error(42)
         return
      endif
      ilw=il+2
      ld=sadr(il+3+ne)
      n1=istk(il+3)-1
      n2=istk(il+4)-istk(il+3)
      do 101 i=1,ne
       ili=iadr(ld+istk(il+1+i)-1)
       if(istk(ili).ne.1) then
           err=i
           call error(53)
           return
       endif
       if(istk(ili+3).ne.0) then
          err=i
          call error(52)
          return
       endif
       if(i.eq.1) then
          m=istk(ili+1)
          n=istk(ili+2)
                  else
          if(istk(ili+1).ne.m.or.istk(ili+2).ne.n) then
             call error(42)
             return
          endif
       endif
      istk(ilw-1+i)=sadr(ili+4)-1
 101  continue
      mn=m*n
      x=-1.0d+0
      if(fin.eq.17) x=1.0d+0
      do 103 i=1,mn
        li=istk(ilw)+i
        ji=1
        do 102 j=2,ne
           if(x*(stk(li)-stk(istk(ilw-1+j)+i)).gt.0) then
              stk(li)=stk(istk(ilw-1+j)+i)
              ji=j
           endif
 102    continue
        stk(istk(ilw+1)+i)=real(ji)
 103  continue
      call dcopy(n1+n2 ,stk(ld),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+n1
      if(lhs.eq.2) then
         top=top+1
         lstk(top+1)=lstk(top)+n2
      endif
      goto 999
c------------------------------------------------------
c sort
c------------------------------------------------------
 105  continue
      if (lhs .gt. 2) then
         call error(41)
         return
      endif
      if(istk(il).ne.257) then
         err=1
         call error(53)
         return
      endif
      if(it.ne.0) then
         err=1
         call error(52)
         return
      endif
      lw=iadr(lstk(top+1))
      err=sadr(lw+n)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dsort(stk(l),mn,istk(lw))
      if(lhs.eq.1) goto 999
      top=top+1
      il=lw
      l1=sadr(il+4)+mn
      l2=lw+mn
      err=l1-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lstk(top+1)=l1
      do 106 i=1,mn
      stk(l1-i)=dble(istk(l2-i))
  106 continue
      istk(il)=257
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      goto 999
c--------------------------------------------------------
c     kronecker product
c-------------------------------------------------------
  110 if (rhs .ne. 2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      mb=m
      nb=n
      itb=it
      lb=l
      mnb=mn
      top = top - 1
      il=iadr(lstk(top))
      ma=istk(il+1)
      na=istk(il+2)
      ita=istk(il+3)
      la=sadr(il+4)
      mna=ma*na
c
      if(fin.eq.19) goto 115

c      if(fin.eq.20) goto 111
      if(fin.eq.20) then
c         write(*,*)"ici2"
         call error(52)
      return
      endif
      l=la
      mn=mna
      it=ita
  111 if(it.eq.1) goto 113
      do 112 k=1,mn
      lk=l+k-1
      if(stk(lk).eq.0.0d+0) then
         call error(27)
         return
      endif
  112 stk(lk)=1.0d+0/stk(lk)
      goto 115
  113 do 114 k=1,mn
      lk=l+k-1
      sr=stk(lk)
      si=stk(lk+mn)
      s=sr*sr+si*si
      if(s.eq.0.0d+0) then
         call error(27)
         return
      endif
      stk(lk)=sr/s
  114 stk(lk+mn)=-si/s
c
  115 istk(il+1)=mb*ma
      istk(il+2)=nb*na
      istk(il+3)=max(itb,ita)
      l=la
      l1=l+mnb*mna*(max(itb,ita)+1)
      lstk(top+1)=l1
c
c move a and b
      err=l1+mnb*(itb+1)+mna*(ita+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dcopy(mnb*(itb+1),stk(lb),-1,stk(l1+mna*(ita+1)),-1)
      lb=l1+mna*(ita+1)
      call dcopy(mna*(ita+1),stk(la),-1,stk(l1),-1)
      la=l1
c      goto(117,116,118) itb+2*ita
      goto(116,116,116) itb+2*ita
c a et b sont reelles
      call mpkronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
      goto 999

c a est complexe b est reelle
  116  call error(52)
       return
c     all kronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
c      call kronr(stk(la+mna),ma,ma,na,stk(lb),mb,mb,nb,stk(l+mnb*mna),
c     1 ma*mb)
c      goto 999
cc a est reelle b conplexe
c  117 call kronr(stk(la),ma,ma,na,stk(lb),mb,mb,nb,stk(l),ma*mb)
c      call kronr(stk(la),ma,ma,na,stk(lb+mnb),mb,mb,nb,
c     1 stk(l+mnb*mna),ma*mb)
c      goto 999
c  118 call kronc(stk(la),stk(la+mna),ma,ma,na,stk(lb),stk(lb+mnb),
c     1 mb,mb,nb,stk(l),stk(l+mnb*mna),ma*mb)
c      goto 999
c---------------------------------------------
c    matrices de test
c---------------------------------------------
  130 continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      top2=top-rhs+1
      il2=iadr(lstk(top2))
      if(rhs.ne.2) goto 135
      if(istk(il2).ne.10) then
         err=1
         call error(55)
         return
      endif
      kletr=abs(istk(il2+5+istk(il2+1)*istk(il2+2)))
      il=iadr(lstk(top))
      l=sadr(il+4)
      n=max(int(stk(l)),0)
c
      top=top-1
      il=iadr(lstk(top))
      istk(il)=257
      istk(il+1)=n
      istk(il+2)=n
      istk(il+3)=0
      l=sadr(il+4)
      lstk(top+1)=l+n*n
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(kletr.eq.magi) goto 131
      if(kletr.eq.hilb) goto 132
      if(kletr.eq.frk) goto 133
c--------------------------------------------------- 
c     carre magique
c---------------------------------------------------
  131 if (n .eq. 2) n = 0
      if (n .gt. 0) call magic(stk(l),n,n)
      istk(il+1)=n
      istk(il+2)=n
      lstk(top+1)=l+n*n
      go to 999
c
c     hilbert
 132  call hilber(stk(l),n,n)
      go to 999
c
c matrice de franck
  133 continue
      job=0
      if (n .gt. 0) call franck(stk(l),n,n,job)
      go to 999
c--------------------------------------------------
c      changement de dimension d'une matrice
c-------------------------------------------------
  135 continue
      il=iadr(lstk(top+1-rhs))
      if(istk(il).eq.5.or.istk(il).eq.6) then
         fin=12
         call spelm
         return
      endif
      il=iadr(lstk(top))
c bug ? 
      if(istk(il).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=3
         call error(89)
         return
      endif
      if(istk(il+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      n=int(stk(sadr(il+4)))
c
      top=top-1
      il=iadr(lstk(top))
c bug ?
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=2
         call error(89)
         return
      endif
      if(istk(il+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      m=int(stk(sadr(il+4)))
c 
      top=top-1
      il=iadr(lstk(top))
      if((istk(il).ne.257).and. (istk(il).gt.10)) then
         err=1
         call error(55)
         return
      endif
      if(m*n.ne.istk(il+1)*istk(il+2)) then
         call error(60)
         return
      endif
      if(m*n.eq.0) then
         istk(il+1)=0
         istk(il+2)=0
      else
         istk(il+1)=m
         istk(il+2)=n
      endif
      goto 999
c
c     sin
c
 140  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(it.eq.0) then
         do 141 i=0,mn-1
            stk(l+i)=sin(stk(l+i))
 141     continue
      else
         do 142 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            stk(l+i)=sin(sr)*cosh(si)
            stk(l+i+mn)=cos(sr)*sinh(si)
 142     continue
      endif
      goto 999
c---------------------------------------------------------
c     cos
c---------------------------------------------------------
 150  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(it.eq.0) then
         do 151 i=0,mn-1
            stk(l+i)=cos(stk(l+i))
 151     continue
      else
         do 152 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            stk(l+i)=cos(sr)*cosh(si)
            stk(l+i+mn)=-sin(sr)*sinh(si)
 152     continue
      endif
      goto 999
c--------------------------------------------------------
c     atan
c--------------------------------------------------------
 160  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(istk(il).ne.257) then
         err=1
         call  error(53)
         return
      endif
      if(rhs.eq.1) then
         if(it.eq.0) then
            do 161 i=0,mn-1
               stk(l+i)=atan(stk(l+i))
 161        continue
         else
            do 162 i=0,mn-1
               sr=stk(l+i)
               si=stk(l+mn+i)
               if(abs(sinh(sr)).eq.1.0d+0.and.si.eq.0.0d+0) then
                  call error(32)
                  return
               endif
               call watan(sr,si,stk(l+i),stk(l+i+mn))
 162        continue
         endif
      elseif(rhs.eq.2) then
         top=top-1
         il1=iadr(lstk(top))
         if(istk(il1).ne.1) then
            err=1
            call  error(53)
            return
         endif
         mn1=istk(il1+1)*istk(il1+2)
         if(istk(il1+3).ne.0.or.it.ne.0) then
            call error(43)
            return
         endif            
         if(mn1.ne.mn) then
            call error(60)
            return
         endif
         l1=sadr(il1+4)
         do 163 i=0,mn
            if(abs(stk(l1+i))+abs(stk(l+i)).eq.0.0d+0) then
               call error(32)
               return
            endif
            stk(l1+i)=atan2(stk(l1+i),stk(l+i))
 163     continue
      endif
      goto 999
c------------------------------------------------
c     exp
c------------------------------------------------
 170  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(m.eq.n.and.n.gt.1) then
         nn=mn
         if(it.eq.0) then
            le=lstk(top+1)
            lw=le+nn
            ilb=iadr(lw+4*nn+5*n)
            err=sadr(ilb+n+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dexpm1(n,n,stk(l),stk(le),n,stk(lw),istk(ilb),err)
            if(err.ne.0) then
               call error(24)
               return
            endif
            call dcopy(nn,stk(le),1,stk(l),1)
         else
            le=lstk(top+1)
            lw=le+2*nn
            ilb=iadr(lw+8*nn+7*n)
            err=sadr(ilb+n+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call wexpm1(n,stk(l),stk(l+nn),n,stk(le),stk(le+nn),
     *           n,stk(lw),istk(ilb),err)
            if(err.ne.0) then
               call error(24)
               return
            endif
            call dcopy(2*nn,stk(le),1,stk(l),1)
         endif
         goto 999
      endif
      if(it.eq.0) then
         do 171 i=0,mn-1
            stk(l+i)=exp(stk(l+i))
 171     continue
      else
         do 172 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            stk(l+i)=exp(sr)*cos(si)
            stk(l+i+mn)=exp(sr)*sin(si)
 172     continue
      endif
      goto 999
c-------------------------------------------------
c     sqrt
c-------------------------------------------------
 180  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(it.eq.0) then
         itr=0
         do 181 i=0,mn-1
            if(stk(l+i).lt.0.0d+0) then
               itr=1
               goto 182
            endif
 181     continue
 182     if(itr.eq.0) then
            do 183 i=0,mn-1
               stk(l+i)=sqrt(stk(l+i))
 183        continue
         else
            err=l+2*mn-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            lstk(top+1)=l+2*mn
            do 184 i=0,mn-1
               call wsqrt(stk(l+i),0.0d+0,stk(l+i),stk(l+mn+i))
 184        continue
            istk(il+3)=itr
         endif
      else
         do 185 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            call wsqrt(sr,si,stk(l+i),stk(l+i+mn))
 185     continue
      endif
      goto 999
c---------------------------------------------------
c     log
c--------------------------------------------------
 190  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
      if(it.eq.0) then
         itr=0
         do 191 i=0,mn-1
            if(stk(l+i).lt.0.0d+0) then
               itr=1
               goto 192
            elseif(stk(l+i).eq.0.0d+0) then
               call error(32)
               return
            endif
 191     continue
 192     if(itr.eq.0) then
            do 193 i=0,mn-1
               stk(l+i)=log(stk(l+i))
 193        continue
         else
            err=l+2*mn-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            lstk(top+1)=l+2*mn
            do 194 i=0,mn-1
               call wlog(stk(l+i),0.0d+0,stk(l+i),stk(l+mn+i))
 194        continue
            istk(il+3)=itr
         endif
      else
         do 195 i=0,mn-1
            sr=stk(l+i)
            si=stk(l+mn+i)
            if(sr*sr+si*si.eq.0.0d+0) then
               call error(32)
               return
            endif
            call wlog(sr,si,stk(l+i),stk(l+i+mn))
 195     continue
      endif
      goto 999
c------------------------------------------------
c  ** puissance non entiere des matrices carres
c-----------------------------------------------
 200  continue
      call error(43)
      return
c
c     sign
c
 210  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(mn.eq.0) goto 999
 
      if(it.eq.0) then
         do 211 i=0,mn-1
            if(stk(l+i).gt.0.d0) then
               stk(l+i)=1.d0
            elseif(stk(l+i).lt.0.d0) then
               stk(l+i)=-1.d0
            else
               stk(l+i)=0.0d0
            endif
 211     continue
      else
         err=1
         call error(52)
      endif
      goto 999
c----------------------------------------------
c     clean
c----------------------------------------------
 220  continue
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      ilp=iadr(lstk(top+1-rhs))
      if (rhs.eq.3) then
         call error(39)
c     clean(p,epsa,epsr)
c         iler=iadr(lstk(top))
c         ler=sadr(iler+4)
c         ilea=iadr(lstk(top-1))
c         lea=sadr(ilea+4)
c         epsr=stk(ler)
c         epsa=stk(lea)
c         top=top-2
      else if (rhs.eq.2) then
c     clean(p,epsa)
         ilea=iadr(lstk(top))
         lea=sadr(ilea+4)
         top=top-1
         epsr=-1.0d10
         epsa=stk(lea)
      else if(rhs.eq.1) then
         epsr=-1.0d10
         epsa=-1.0d10
      endif
      m=istk(ilp+1)
      n=istk(ilp+2)
      it=istk(ilp+3)
      l=sadr(ilp+4)
c      if(it.eq.0) then
c         norm=dasum(m*n,stk(l),1)
          norm=1
c      else
c         norm=wasum(m*n,stk(l),stk(l+m*n),1)
c      endif
      eps=max(epsa,epsr*norm)
c      do 201 kk=0,m*n*(it+1)
      do 201 kk=0,m*n
         if (stk(l+kk).le.eps) stk(l+kk)=PCZERO
 201  continue

      goto 999
c-------------------------------------------------------
c    tropical
c-------------------------------------------------------
 230  continue
c      write(6,*)"tropical"
      if (rhs .ne. 2) then
         call error(39)
         return
      endif
c
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il=iadr(lstk(top))
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l=sadr(il+4)

      top=top-1
c
      il1=iadr(lstk(top))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
c
      istk(il1)=algtype(int(stk(l)),typecode(istk(il1)))
c     write(*,*)"istk(il1)"
c     write(*,*)istk(il1)
      goto 999
c--------------------------------------------------------
c    setalg
c--------------------------------------------------------
 240  continue
      if (rhs .ne. 4) then
         call error(39)
         return
      endif
      top=top-1
c
      il3=iadr(lstk(top))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      top=top-1

      il2=iadr(lstk(top))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      top=top-1

      il1=iadr(lstk(top))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)

c      PCALGCODE=int(stk(l1))
       PCALGCODE=1
c      PCZERO=stk(l2)
c       call commons(PCZERO,PCTOP,PCALGCODE)
       PCZERO=ZERO
c      PCONE=stk(l3)
        PCONE=0.d0
c      PCTOP=stk(l)
       PCTOP=TOPM
c       write(*,*)PCZERO,ZERO,PCTOP,TOPM,tutu,PCALGCODE
c     write(*,*)"ALGCODES"
c     write(*,*)PCALGCODE
c     write(*,*)PCZERO
c     write(*,*)PCONE
c     write(*,*)PCTOP

      goto 999
c----------------------------------
c    karp
c----------------------------------
 250  continue
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if(typecode(istk(il)).ne.1.and.typecode(istk(il)).ne.5) then
         top=topk
         fun=-1
         call funnam(ids(1,pt+1),'skarp',iadr(lstk(top-rhs+1)))
      endif
      m=algcode(istk(il))
      istk(il)=257
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=it
      l1=sadr(il+4)
c     stk(l1)=mpkarp(m,n,stk(l),mn,istk(il+5),m)
c     if(it.eq.1) stk(l1+1)=mpkarp(mn,stk(l+mn),1)
      lstk(top+1)=l1+(it+1)
      go to 999
c
  900 continue
      if(istk(il).ne.2) then
         err=1
         call error(54)
         return
      endif
c     *call* polelm
      fun=16
      return

c---------------------------------
c    save et load maxplus matrices
c---------------------------------
 350  continue
c     get pointer on variable to save
c      write(*,*)'save'
      il=iadr(lstk(top-1))
      if(istk(il).lt.0) il=iadr(istk(il+1))
c     get logical unit
      if(.not.getscalar('save',top,top,lr)) return
      fd=stk(lr)
      call savemat(fd,il,ierr)
      if(ierr.ne.0) return
      return
 450  continue
c      write(*,*)'load'
      if(.not.getscalar('save',top,top,lr)) return
      fd=stk(lr)
      top=top-1
      il=iadr(lstk(top))
      call loadmat(fd,il,n,ierr)
      if(ierr.ne.0) return   
      istk(il)=257
      ids(4,pt)=n
      lstk(top+1)=sadr(il+n)
      return
 550  call inttalg_p()
      return
c
  999 return
      end
c
      subroutine inttalg_p()
c     Copyright INRIA
      include '../stack.h'
c     
      integer mode,m,n,it,lr
      logical crewimat
      integer iadr, sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lineln=lct(5)
      mode=lct(6)
      ndgt=lct(7)
c     
      if (lct(1) .lt. 0) then
         lk=0
         return
      endif
      call basout(io,lunit,' ')
  
c     -------scalar matrices 
      il=iadr(lstk(top))
      m=istk(il+1)
      n=istk(il+2)
      it=0
      lr=sadr(il+4)

C     working area 
      if (.not.crewimat("talg_p",topk,1,m*n+2*n,lw)) return
      if(m*n.eq.0) then
         call basout(io,lunit,'     []')
      endif
c      call talg_p(stk(lr),m,m,n,ndgt,mode,lineln,lunit,buf,istk(lw))
      return
      end

