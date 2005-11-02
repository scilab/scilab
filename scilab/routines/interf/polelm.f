      subroutine polelm
c     ====================================================================
c     
c     evaluation des fonctions polynomiales elementaires
c     
c     ====================================================================
c     
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' polelm '//buf(1:4))
      endif
c     
c     functions/fin
c     1       2       3       4       5       6       7       8
c     poly     roots  degre   coeff   eval    pdiv  simp     sum
c     
c     9       10      11      12      13      14     15        16
c     prod    diag    triu     tril    bezout sfact simp_mode  varn
c     
c     17
c     cleanp
c    
      goto (10,20,30,40,50,60,73,55,58,25,27,28,70,83,120,100,110) fin
c     
c     poly
 10   call intpoly(id)
      goto 900
c     
c     roots 
 20   call introots(id)
      goto 900
c     
c     diag
 25   call intpdiag(id)
      goto 900
c     
c     triu 
 27   call intptriu(1,id)
      goto 900
c     
c     tril
 28   call intptriu(0,id)
      goto 900
c
c     degree
 30   call intdegree(id)
      goto 900
c     
c     coeff
 40   call intcoeff(id)
      goto 900
c     
c     eval
 50   continue
      goto 900
c     
c     sum
 55   call intpsum(id)
      goto 900
c     
c     prod
 58   call intpprod(id)
      goto 900
c     
c     pdiv
 60   call intpdiv(id)
      goto 900
c     
c     bezout
 70   call intbez(id)
      goto 900
c     
c     simp
 73   call intsimp(id)
      goto 900
c     
c     sfact
c     
 83   call intsfact(id)
      goto 900
c     
c     varn
 100  call intvarn(id)
      goto 900
c     
c     cleanp  
 110  call intpclean(id)
      goto 900
c
c     simp_mode
 120  call intsimpmd(id)
      goto 900

c     
 900  return
      end


      subroutine intpoly(id)

      INCLUDE '../stack.h'
      integer id(nsiz)
      integer iadr, sadr
      integer blank,racine,coeff
      logical roots,ref
      data blank/40/,racine/27/,coeff/12/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)

      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.lt.1.or.rhs.gt.3) then
         call error(42)
         return
      endif
c      
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.1) then
         fun=-1
         call funnam(ids(1,pt+1),'poly',il1)
         return
      endif
      ref=il1.ne.ilr
c
      roots=.true.

      if(rhs.eq.3) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         rhs=rhs-1
         top=top-1
         if(istk(il).ne.10) then
            err=rhs
            call error(55)
            return
         endif
         il=il+5+istk(il+1)*istk(il+2)
         if(abs(istk(il)).ne.racine) then
            roots=.false.
            if(abs(istk(il)).ne.coeff) then
               err=rhs
               call error(36)
               return
            endif
         endif
      else
        roots=.true. 
      endif

c     formal variable
      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.10) then
         err=2
         call error(55)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=2
         call error(89)
         return
      endif
      n=istk(il+5)-1
      if(n.gt.4) call msgs(109,0)
      do 12 i=1,4
         id(i)=blank
         if(i.le.n) id(i)=istk(il+5+i)
 12   continue
      if(abs(id(1)).lt.10.or.abs(id(1)).ge.blank) then
         err=2
         call error(116)
         return
      endif
c
c     first argument
      top=top-1
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
      itr=it1

      if(.not.roots) goto 17
 13   if(mn1.eq.1.or.m1.ne.n1) goto 14
c     polynome caracteristique,decomposition spectrale de la matrice

      if(ref) then
c     necessary while matdsr and matdsc cannot be called by reference
         lr=sadr(ilr+4)
         err=lr+mn1*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(4,istk(il1),1,istk(ilr),1)
         call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lr),1)
         lstk(top+1)=lr+mn1*(it1+1)
      endif
      rhs=1
      call inteig('spec')
      call putlhsvar
      if(err.gt.0) return
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
      mn1=m1
      m1=1
      n1=mn1
      itr=min(it1,istk(il1+3))
      it1=istk(il1+3)
      lw=l1+mn1*(it1+1)
      ref=.false.
      ilr=il1
      goto 14
c
c     polynome defini par ses racines
 14   ild=ilr+8
      lc=sadr(ild+2)
      lr=lc+(mn1+1)*(it1+1)
      if(ref) then
         err=lr-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         err=lr+mn1*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(mn1*(it1+1),stk(l1),1,stk(lr),1)
         l1=lr
      endif
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=itr
      call icopy(4,id,1,istk(ilr+4),1)
      istk(ild)=1
      istk(ild+1)=mn1+2
      lstk(top+1)=lc+(mn1+1)*(itr+1)

      if(it1.eq.0) then
         call dprxc(mn1,stk(l1),stk(lc))
      else
         call wprxc(mn1,stk(l1),stk(l1+mn1),stk(lc),stk(lc+mn1+1))
         do 16 i=1,mn1+1
            l=lc-1+i
            if( stk(l)+stk(l+mn1+1).ne.stk(l)) goto 999
 16      continue
         istk(ilr+3)=0
         lstk(top+1)=lc+mn1+1
      endif
      goto 999
c
c     polynome defini par ses coefficients
 17   if(istk(il1).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(mn1.eq.0) return

      if(n1.ne.1.and.m1.ne.1) then
         err=1
         call error(89)
         return
      endif

      lr=sadr(ilr+10)
      err=lr+mn1*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call unsfdcopy(mn1*(it1+1),stk(l1),-1,stk(lr),-1)
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=it1
      call icopy(4,id,1,istk(ilr+4),1)
      istk(ilr+8)=1
      istk(ilr+9)=1+mn1
      lstk(top+1)=lr+mn1*(it1+1)
      goto 999
c
 999  return
      end

      subroutine introots(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      double precision t,sr,si
      integer vol
      integer fail
      logical ref,eigen
      integer eig
      data eig/14/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
c
      eigen=.false.
      if (rhs.eq.2) then
         ilopt=iadr(lstk(top))
         if(istk(ilopt).lt.0) ilopt=iadr(istk(ilopt+1))
         if(istk(ilopt).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(ilopt+1).ne.1.or.istk(ilopt+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         if(istk(ilopt+5).ne.2.or.istk(ilopt+6).ne.eig) then
            err=2
            call error(116)
            return
         endif
         eigen=.true.
         rhs=rhs-1
         top=top-1
      endif

      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'roots',il1)
         return
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      if(m1*n1.eq.0) return

      it1=istk(il1+3)

      if(istk(il1).eq.1) then
c     for Matlab compatibility root of the vector of coefficients
         vol=mn1
         l1=sadr(il1+4)
         
         n=mn1
         if(.not.ref) then
            call dtild(n*(it1+1),stk(l1),1)
            lc=l1
         else
            lc=lw
            lw=lc+n*(it1+1)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dcopy(n,stk(l1),1,stk(lc),-1)
            if(it1.eq.1)  call dcopy(n,stk(l1+n),1,stk(lc+n),-1)
         endif 
      else
         if(mn1.ne.1) then
            err=1
            call error(43)
            return
         endif
     
         if(istk(il1).lt.2) goto 24

         id1=il1+8
         lc=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         call icopy(4,istk(il1+4),1,id,1)
         n=vol
      endif
      l1=sadr(ilr+4)
 21   n=n-1
      if(n.lt.0) goto 24
      t=abs(stk(lc+n))
      if(it1.eq.1) t=t+abs(stk(lc+n+vol))
      if(t.eq.0.0d+0) goto 21

      if (eigen) goto 22
c
c     real polynomial: rpoly algorithm 
c     this alg is much more speedy, but it may happens that it gives
C     erroneous results without messages : example  
C     roots(%s^31-8*%s^30+9*%s^29+0.995) should have two real roots near
C     1.355 and 6.65 and the other ones inside a circle centered in 0
C     with radius 1
C     

      if(it1.eq.0.and.n.le.100) then
         lp=max(lw,l1+2*n)
         err=lp+n+1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(n+1,stk(lc),1,stk(lp),1)
         call dtild(n+1,stk(lp),1)
         call rpoly(stk(lp),n,stk(l1),stk(l1+n),fail)
         if(fail.eq.1) then
            call error(24)
            return
         elseif(fail.eq.2) then
            call error(74)
            return
         elseif(fail.eq.3) then
            call error(75)
            return

         endif
         istk(ilr)=1
         istk(ilr+1)=n
         istk(ilr+2)=1
         if(n.eq.0) istk(ilr+2)=0
         istk(ilr+3)=1
         lstk(top+1)=l1+2*n
         goto 999
      endif

 22   continue
c
c     Companion matrix method
      lw=lw+n*n*(it1+1)
      err=lw+n*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      sr=stk(lc+n)
      call unsfdcopy(n,stk(lc),-1,stk(lw),1)
      if(it1.eq.0) then
         call dscal(n,-1.0d+0/sr,stk(lw),1)
      else
         si=stk(lc+vol+n)
         t=sr*sr+si*si
         sr=-sr/t
         si=si/t
         call unsfdcopy(n,stk(lc+vol),-1,stk(lw+n),1)
         call wscal(n,sr,si,stk(lw),stk(lw+n),1)
      endif
      call dset(n*n*(it1+1),0.0d+0,stk(l1),1)
      call dset(n-1,1.0d+0,stk(l1+n),n+1)
      call unsfdcopy(n,stk(lw),1,stk(l1),1)
      if(it1.eq.1) call unsfdcopy(n,stk(lw+n),1,stk(l1+n*n),1)
      lstk(top+1)=l1+n*n*(it1+1)
      istk(ilr)=1
      istk(ilr+1)=n
      istk(ilr+2)=n
      istk(ilr+3)=it1
      fin=3
      fun=2
c     *call* matds(r c)
      goto 999
c     polynome de degre 0
 24   istk(ilr)=1
      istk(ilr+1)=0
      istk(ilr+2)=0
      istk(ilr+3)=0
      lstk(top+1)=sadr(ilr+4)
      goto 999
 999  return
      end
      
      subroutine intpdiag(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer volr,vol,tops
      logical ref

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      tops=top
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
c
      kdiag=0
      if (rhs .eq. 2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
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
         kdiag = int(stk(sadr(il+4)))
         top = top-1
      endif

      lw=lstk(top+1)
c
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).ne.2) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'diag',il1)
         return
      endif
c
      id1=il1+8
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1

c
      if(ref) then
         idr=ilr+8
      else
         idr=iadr(lw)
      endif
      if(m1.eq.1.or.n1.eq.1) then
         m1=mn1
         n1=0
         err=sadr(idr+mn1*(mn1+abs(kdiag)))-lstk(bot)
      else
         err=sadr(idr+min(n1,m1))-lstk(bot)
      endif
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      call mpdiag(istk(id1),m1,n1,kdiag,istk(idr),mr,nr)
      if(nr.le.0.or.mr.le.0) then
         istk(ilr)=1
         istk(ilr+1)=0
         istk(ilr+2)=0
         istk(ilr+3)=0
         lstk(top+1)=sadr(ilr+4)
         goto 999
      endif
      lr=sadr(idr+mr*nr+1)
      volr=istk(idr)
      err=lr+volr*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if (it1.eq.0) then
         call dmpins(stk(l1),istk(id1),mn1,1,stk,1,0,0,
     1        stk(lr),istk(idr),mr,nr)
      else
         call wmpins(stk(l1),stk(l1+vol),istk(id1),mn1,1,
     1     stk,stk,1,0,0,stk(lr),stk(lr+volr),istk(idr),mr,nr)
      endif
      if(ref) then
         call icopy(8,istk(il1),1,istk(ilr),1)
         l1=sadr(idr+mr*nr+1)
      else
         call icopy(mr*nr+1,istk(idr),1,istk(id1),1)
         l1=sadr(id1+mr*nr+1)
         call unsfdcopy(volr*(it1+1),stk(lr),1,stk(l1),1)
      endif
      istk(ilr+1)=mr
      istk(ilr+2)=nr
      lstk(top+1)=l1+volr*(it1+1)
      goto 999
 999  return
      end

      subroutine intptriu(job,id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer volr,vol,tops
      logical ref

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
c
      kdiag=0
      if (rhs .eq. 2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
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
         kdiag = int(stk(sadr(il+4)))
         top = top-1
      endif

      lw=lstk(top+1)
c
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'triu',il1)
         return
      endif
c
      id1=il1+8
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1

c
      if(ref) then
         idr=ilr+8
      else
         idr=iadr(lw)
      endif

      lr=sadr(idr+mn1+1)
      err=lr-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      call mptri(istk(id1),m1,n1,kdiag,istk(idr),job)
      volr=istk(idr)
      err=lr+volr*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if (it1.eq.0) then
         call dmpins(stk(l1),istk(id1),m1,n1,stk,1,0,0,
     1        stk(lr),istk(idr),m1,n1)
      else
         call wmpins(stk(l1),stk(l1+vol),istk(id1),m1,n1,
     1        stk,stk,1,0,0,stk(lr),stk(lr+volr),istk(idr),m1,n1)
      endif

      if(ref) then
         call icopy(8,istk(il1),1,istk(ilr),1)
         l1=lr
      else
         call icopy(mn1+1,istk(idr),1,istk(id1),1)
         l1=sadr(id1+mn1+1)
         call unsfdcopy(volr*(it1+1),stk(lr),1,stk(l1),1)
      endif
      istk(ilr+1)=m1
      istk(ilr+2)=n1
      lstk(top+1)=l1+volr*(it1+1)
 999  return
      end

      subroutine intdegree(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer volr,vol
      logical ref
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(42)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'degree',il1)
         return
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1

      if(mn1.eq.0) then
         if (ref) then
            l=sadr(ilr+4)
            err=l-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=l
         endif
         return
      endif
c
      if(istk(il1).eq.2) then
         id1=il1+8
         if(ref) then
            l1=sadr(ilr+4)
            l=l1
         else
            l1=sadr(id1+mn1+1)
            l=sadr(ilr+4)
         endif
         do 31 k=1,mn1
            stk(l1-1+k)=dble(istk(id1+k)-istk(id1+k-1)-1)
 31      continue
         if(ref) then
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1
            istk(ilr+3)=0
         else
            istk(ilr)=1
            istk(ilr+3)=0
            call unsfdcopy(mn1,stk(l1),1,stk(l),1)
         endif
         lstk(top+1)=l+mn1
      elseif(istk(il1).eq.1) then
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1
         istk(ilr+3)=0
         l=sadr(ilr+4)
         call dset(mn1,0.0d+0,stk(l),1)
         lstk(top+1)=l+mn1
c      else
c     ????????
c         fun=15
c         fin=5
      endif
      return
      end

      subroutine intcoeff(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer v2,vol,top0
      logical ref
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1.and.rhs.ne.2) then
         call error(42)
         return
      endif
      top0=top
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'coeff',il1)
         return
      endif
      
      if(rhs.eq.2) then
         ilv=iadr(lstk(top))
         if(istk(ilv).lt.0) ilv=iadr(istk(ilv+1))
         if(istk(ilv).ne.1) then
            err=2
            call error(53)
            return
         endif
         if(istk(ilv+3).ne.0) then
            err=2
            call error(52)
            return
         endif
         nv=istk(ilv+1)*istk(ilv+2)
         if(nv.eq.0) then
            top=top-1
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            return
         endif
         if(istk(ilv+1).ne.1.and.istk(ilv+2).ne.1) then
            err=2
            call error(89)
            return
         endif
         lv=sadr(ilv+4)
         do 10 i=0,nv-1
            if(int(stk(lv+i)).lt.0) then
               err=2
               call error(116)
               return
            endif
 10      continue
         top=top-1
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      mn1=m1*n1
c
      if(ref) then
         lr=sadr(ilr+4)
      else
         lr=lstk(top+1)
      endif
c
      if(istk(il1).eq.2) then
c     coeff of a matrix of polynomials
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1

         if(rhs.eq.1) then
            mx=0
            do 20 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               if(nij.gt.mx) mx=nij
 20         continue
            v2=mn1*mx
            err=lr+v2*(it1+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(v2*(it1+1),0.0d+0,stk(lr),1)
            do 30 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               lij=l1-1+istk(id1+ij-1)
               call unsfdcopy(nij,stk(lij),1,stk(lr-1+ij),mn1)
               if(it1.eq.1) then
                  call unsfdcopy(nij,stk(lij+vol),1,stk(lr+v2-1+ij),mn1)
               endif
 30         continue
            if(.not.ref) then
               l=sadr(ilr+4)
               call unsfdcopy(v2*(it1+1),stk(lr),1,stk(l),1)
               lr=l
            endif
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1*mx
            istk(ilr+3)=it1
            lstk(top+1)=lr+v2*(it1+1)
         else
            lw=max(lstk(top0+1),lr+mn1*nv*(it1+1))
            err=lw+nv-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nv,stk(lv),1,stk(lw),1)
            lv=lw
            call dset(mn1*nv*(it1+1),0.0d+0,stk(lr),1)
            do 41 ij=1,mn1
               nij=istk(id1+ij)-istk(id1+ij-1)
               lij=l1-1+istk(id1+ij-1)
               do 40 k=1,nv
                  mv=int(stk(lv-1+k))
                  if(mv.ge.nij) goto 40
                  stk(lr+ij-1+(k-1)*mn1)=stk(lij+mv)
                  if(it1.ne.0) stk(lr+ij-1+(k-1+nv)*mn1)=stk(lij+mv+vol)
 40            continue
 41         continue
            l=sadr(ilr+4)
            if(.not.ref) call unsfdcopy(mn1*nv*(it1+1),stk(lr),1,
     $           stk(l),1)
            istk(ilr)=1
            istk(ilr+1)=m1
            istk(ilr+2)=n1*nv
            istk(ilr+3)=it1
            lstk(top+1)=l+mn1*nv*(it1+1)
         endif
      else
c     coeff of a matrix of numbers
         if(rhs.eq.1) then
c     .     return the input matrix
            if(ref) then
c     .      copy matrix on the top of the stack
               k=istk(ilr+2)
               err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
               call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1
     $              ,stk(lstk(top)),1)
               lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
            endif
            return
         endif
         l1=sadr(il1+4)
         vol=mn1

         lw=lr+mn1*nv*(it1+1)
         err=lw+nv-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(lv.le.lw) then
            call unsfdcopy(nv,stk(lv),-1,stk(lw),-1)
            lv=lw
         endif
         call dset(mn1*nv*(it1+1),0.0d+0,stk(lr),1)
         do 50 k=1,nv
            if(stk(lv-1+k).eq.0.0d0) then
               call unsfdcopy(mn1,stk(l1),1,stk(lr+(k-1)*mn1),1)
               if(it1.eq.1) call unsfdcopy(mn1,stk(l1+mn1),1,
     $              stk(lr+(k-1)*mn1+mn1*nv),1)
            endif
 50      continue
         l=sadr(ilr+4)
         if(.not.ref) call unsfdcopy(mn1*nv*(it1+1),stk(lr),1,stk(l),1)
         istk(ilr)=1
         istk(ilr+1)=m1
         istk(ilr+2)=n1*nv
         istk(ilr+3)=it1
         lstk(top+1)=l+mn1*nv*(it1+1)
      endif

      return
      end

      
      subroutine intpsum(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      
      integer vol
      logical ref
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         if(rhs.eq.2) then
            call funnam(ids(1,pt+1),'sum',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
         call error(42)
         return
      endif
c
      lw=lstk(top+1)
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'sum',il1)
         return
      endif
c
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1
c
      maxd=0
      do 56 i=1,mn1
         m=istk(id1+i)-istk(id1-1+i)
         if(m.gt.maxd) maxd=m
 56   continue

      if(ref) then
         lr=sadr(ilr+10)
      else
         lr=lw
      endif

      err=lw+maxd*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call dset(maxd*(it1+1),0.0d+0,stk(lr),1)
      do 57 i=1,mn1
         li=l1-1+istk(id1-1+i)
         n=istk(id1+i)-istk(id1-1+i)
         call dadd(n,stk(li),1,stk(lr),1)
         if(it1.eq.1) call dadd(n,stk(li+vol),1,stk(lr+maxd),1)
 57   continue
      if(ref) then
         call icopy(9,istk(il1),1,istk(ilr),1)
         istk(ilr)=2
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
      else
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
         l1=sadr(il1+10)
         call unsfdcopy(maxd*(it1+1),stk(lr),1,stk(l1),1)
         lr=l1
      endif
      lstk(top+1)=lr+maxd*(it1+1)
      return
      end

      subroutine intpprod(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol
      logical ref
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         if(rhs.eq.2) then
            call funnam(ids(1,pt+1),'prod',iadr(lstk(top-rhs+1)))
            fun=-1
            return
         endif
         call error(42)
         return
      endif
c
      lw=lstk(top+1)
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'prod',il1)
         return
      endif

c
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1

      if(ref) then
         lr=sadr(ilr+10)
      else
         lr=lw
      endif

      maxd=istk(id1+mn1)-mn1
      err=lr+maxd*(it1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      stk(lr)=1.0d+0
      if(it1.eq.1) stk(lr+maxd)=0.0d+0
      m=0
      do 59 i=1,mn1
         n=istk(id1+i)-istk(id1+i-1)-1
         li=l1-1+istk(id1+i-1)
         if(it1.eq.0) call dpmul1(stk(li),n,stk(lr),m,stk(lr))
         if(it1.eq.1) call wpmul1(stk(li),stk(li+vol),n,stk(lr),
     1        stk(lr+maxd),m,stk(lr),stk(lr+maxd))
         m=m+n
 59   continue

      if(ref) then
         call icopy(9,istk(il1),1,istk(ilr),1)
         istk(ilr)=2
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
      else
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+9)=1+maxd
         l1=sadr(il1+10)
         call unsfdcopy(maxd*(it1+1),stk(lr),1,stk(l1),1)
         lr=l1
      endif
      lstk(top+1)=lr+maxd*(it1+1)
      return
      end

      subroutine intpdiv(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer voln,vold,v2
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1.and.lhs.ne.2) then
         call error(41)
         return
      endif
      if(rhs.ne.2) then
         call error(42)
         return
      endif
c
      lw=lstk(top+1)
c
      iln=iadr(lstk(top+1-rhs))

      il=iln
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'pdiv',il)
         return
      endif
c
      ild=iadr(lstk(top))

      il=ild
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'pdiv',il)
         return
      endif
c
      if(istk(iln).lt.0) then
c     move d and copy value of n in place of its reference
         k=istk(iln+2)
         voln=lstk(k+1)-lstk(k)
         vold=lstk(top+1)-lstk(top)
         err=lstk(top)+voln+vold-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vold,stk(lstk(top)),-1,stk(lstk(top-1)+voln),-1)
         call unsfdcopy(voln,stk(lstk(k)),1,stk(lstk(top-1)),1)
         lstk(top)=lstk(top-1)+voln
         ild=iadr(lstk(top))
         lstk(top+1)=lstk(top)+vold
      endif

      if(istk(ild).lt.0) then
c      copy value of b in place of its reference
         k=istk(ild+2)
         vold=lstk(k+1)-lstk(k)
         err=lstk(top)+vold-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vold,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vold
      endif
      lw=lstk(top+1)

      il1=iln
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
      if(mn1.ne.1) then
         err=1
         call error(89)
         return
      endif
      if(istk(il1).eq.2) then
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         call icopy(4,istk(il1+4),1,id,1)
         nn=istk(id1+1)-2
      else
         nn=0
         id(1)=0
      endif
c
c     denominateur

      nd=istk(ild+1)*istk(ild+2)
      if(nd.ne.1) then
         err=2
         call error(89)
         return
      endif
      itd=istk(ild+3)
      if(istk(ild).eq.1) then
         ld=sadr(ild+4)
         nd=0
         if (id(1).eq.0) then
            err=2
            call error(54)
            return
         endif 
         v2=1
      else
         if(id(1).eq.0) call icopy(4,istk(ild+4),1,id,1)
         do 61 i=1,4
            if(id(i).ne.istk(ild+3+i)) then
               call error(43)
               return
            endif
 61      continue
         nd=istk(ild+9)-2
         ld=sadr(ild+10)
         v2=istk(ild+9)-1
      endif

c     test degre du denominateur
 64   continue
      if(itd.eq.0) then
         if(stk(ld+nd).ne.0.0d+0) goto 65
      else
         if(abs(stk(ld+nd))+abs(stk(ld+v2+nd)).ne.0.0d+0) goto 65
      endif
      nd=nd-1
      if(nd.lt.0) then
         call error(27)
         return
      endif
      goto 64
c     
 65   if(nn.lt.nd) then
c        . No division to perform
         if(lhs.eq.2) then
            istk(ild)=1
            ld=sadr(ild+4)
            stk(ld)=0.0d+0
            lstk(top+1)=ld+1
         else
            top=top-1
            istk(il1)=1
            stk(l1)=0.0d+0
            lstk(top+1)=l1+1
         endif
         goto 999
      endif
c
      if(it1.eq.0) then
         if (itd.eq.0) then
            call dpodiv(stk(l1),stk(ld),nn,nd)
         else
            l1i=lw
            lw=l1i+nn+1
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ldi=ld+v2
            call dset(nn+1,0.0d0,stk(l1i),1)
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         endif
      else
         l1i=l1+nn+1
         if (itd.eq.0) then
            ldi=lw
            lw=ldi+nd+1
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call dset(nd+1,0.0d0,stk(ldi),1)
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         else
            ldi=ld+v2
            call wpodiv(stk(l1),stk(l1i),stk(ld),stk(ldi),nn,nd,ierr)
         endif
      endif
c     
      itr=max(it1,itd)
      nq=nn-nd

      if(lhs.eq.1) then
c     .  only q is returned
         top=top-1
c     .  set type
         istk(il1+3)=itr
c     .  set length
         istk(il1+9)=nq+2
c     .  install real and imaginary part
         call unsfdcopy(nq+1,stk(l1+nd),1,stk(l1),1)
         if(itr.eq.1) call unsfdcopy(nq+1,stk(l1i+nd),1,stk(l1+nq+1),1)
      else
c     .  computes remainder degree
         nr=nd
 66      nr=nr-1
         if(nr.lt.0) goto 67
         if (itr.eq.0) then
            if(stk(l1+nr).eq.0.0d+0) goto 66
         else
            if(stk(l1+nr).eq.0.0d+0.and.stk(l1i+nr).eq.0.0d+0) goto 66
         endif
 67      continue

c
         if(itr.eq.0) then
c     .     real result
            if(nr.gt.0) then
c     .        remainder is a polynomial
               istk(id1+1)=nr+2
               lstk(top)=l1+(nr+1)
            else
c     .        remainder is a scalar
               istk(il1)=1
               istk(il1+1)=1
               istk(il1+2)=1
               lr=sadr(il1+4)
               stk(lr)=stk(l1)
c????               stk(lr+1)=stk(l1i)
               lstk(top)=lr+1
            endif
            ilq=iadr(lstk(top))
            lq=sadr(ilq+10)
            inc=1 
            if (l1+nd.lt.lq) inc=-1
            call unsfdcopy(nq+1,stk(l1+nd),inc,stk(lq),inc)
            if(nr.lt.0) stk(lr)=0.0d+0
            istk(ilq)=2
            istk(ilq+1)=1
            istk(ilq+2)=1
            istk(ilq+3)=0
            call icopy(4,id,1,istk(ilq+4),1)
            idq=ilq+8
            istk(idq)=1
            istk(idq+1)=nq+2
            lstk(top+1)=lq+nq+1
            goto 999
         else
c     .     complex result
c     .     preserve quotient coeff
            lqs=lw
            lw=lqs+sadr(10)+(nq+1)*2
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(nq+1,stk(l1+nd),1,stk(lqs),1)
            call unsfdcopy(nq+1,stk(l1i+nd),1,stk(lqs+nq+1),1)

c     .     set remainder
            if(nr.ge.0) then
               istk(il1+3)=1
               call unsfdcopy(nr+1,stk(l1i),1,stk(l1+nr+1),1)
               istk(id1+1)=nr+2
               lstk(top)=l1+(nr+1)*2
            else
               istk(il1+3)=0
               stk(l1)=0.0d0
               istk(id1+1)=2
               lstk(top)=l1+1
            endif
c     .     set quotient
            ilq=iadr(lstk(top))
            istk(ilq)=2
            istk(ilq+1)=1
            istk(ilq+2)=1
            istk(ilq+3)=1
            call icopy(4,id,1,istk(ilq+4),1)
            idq=ilq+8
            istk(idq)=1
            istk(idq+1)=nq+2
            lq=sadr(ilq+10)
            call unsfdcopy(2*(nq+1),stk(lqs),1,stk(lq),1)
            lstk(top+1)=lq+(nq+1)*2
         endif
      endif

      goto 999
 999  return
      end
      
      subroutine intbez(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,v2,blank,ipb(6)
      double precision er

      logical refa,refb
      data blank/40/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

      if(rhs.ne.2) then
         call error(42)
         return
      endif
      if(lhs.ne.2.and.lhs.ne.3) then
         call error(41)
         return
      endif
      lw =lstk(top+1)
c
      ilb=iadr(lstk(top))
      ilbr=ilb
      if(istk(ilb).lt.0) ilb=iadr(istk(ilb+1))
      if(istk(ilb).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',ilb)
         return
      endif
      refb=ilbr.ne.ilb
      itb=istk(ilb+3)
      if(itb.ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',ilb)
         return
      endif
      if(istk(ilb+1)*istk(ilb+2).ne.1)then
         err=2
         call error(89)
         return
      endif
      if(istk(ilb).eq.1) then
         nb=0
         lb=sadr(ilb+4)
         id(1)=0
      else
         nb=istk(ilb+9)-2
         lb=sadr(ilb+9)
         call icopy(4,istk(ilb+4),1,id,1)
      endif
c     
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',il1)
         return
      endif
      refa=ilr.ne.il1
      it1=istk(il1+3)
      if(it1.ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',il1)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(89)
         return
      endif
      mn1=1
      if(istk(il1).eq.2) then
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         na=vol-1
         if(id(1).eq.0) call icopy(4,istk(il1+4),1,id,1)
      else
         l1=sadr(il1+4)
         na=0
      endif
      if(id(1).eq.0) call iset(4,blank,id,1)
c     
      lf=lstk(top+1)+na+9
      if(refa) lf=lw+na+9
c
      lw=lf+2*(na+nb)+min(na,nb)+3
      n0=max(na,nb)+1
      err=lw+10*n0+3*n0*n0-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call recbez(stk(l1),na,stk(lb),nb,stk(lf),ipb,stk(lw),er)
c     pgcd
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      call icopy(4,id,1,istk(ilr+4),1)
      l1=sadr(ilr+8+mn1+1)
      istk(ilr+8)=1
      np=ipb(2)-ipb(1)-1
      istk(ilr+9)=2+np
      call unsfdcopy(np+1,stk(lf+ipb(1)-1),1,stk(l1),1)
      lstk(top)=l1+np+1

c     matrice q 
      il=iadr(lstk(top))
      istk(il)=2
      istk(il+1)=2
      istk(il+2)=2
      istk(il+3)=0
      call icopy(4,id,1,istk(il+4),1)
      il=il+8
      istk(il)=1
      l=sadr(il+5)
      do 71 i=1,4
         ii=i+1
         istk(il+1)=istk(il)+ipb(ii+1)-ipb(ii)
         call unsfdcopy(istk(il+1)-istk(il),stk(lf+ipb(ii)-1),1,
     $        stk(l),1)
         l=l+istk(il+1)-istk(il)
         il=il+1
 71   continue
      lstk(top+1)=l
      if(lhs.eq.3) then
c     retour de l'erreur
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=er
         lstk(top+1)=l+1
      endif
      goto 999
 999  return
      end


      subroutine intsimp(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,v2,blank
      double precision er
      integer simpmd
      common/csimp/ simpmd
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(simpmd.eq.0) then
         call ref2val()
         return
      endif
      if(rhs.eq.1) then
         call intrsimp(id)
      elseif(rhs.eq.2) then
         call intpsimp(id)
      else
         call error(39)
      endif
c
      return
      end

      subroutine intpsimp(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vola,volb
      logical chkvar,vcopyobj
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     

c     simp(num,den)
c
      if(lhs.ne.2) then 
         call error(41)
         return
      endif
c
      ila=iadr(lstk(top+1-rhs))

      il=ila
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il)
         return
      endif
c
      ilb=iadr(lstk(top))

      il=ilb
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il)
         return
      endif
c
      if(istk(ila).lt.0) then
c     move b and copy value of a in place of its reference
         k=istk(ila+2)
         vola=lstk(k+1)-lstk(k)
         volb=lstk(top+1)-lstk(top)
         err=lstk(top)+vola+volb-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(volb,stk(lstk(top)),-1,stk(lstk(top-1)+vola),-1)
         call unsfdcopy(vola,stk(lstk(k)),1,stk(lstk(top-1)),1)
         lstk(top)=lstk(top-1)+vola
         ilb=iadr(lstk(top))
         lstk(top+1)=lstk(top)+volb
      endif

      if(istk(ilb).lt.0) then
c      copy value of b in place of its reference
         k=istk(ilb+2)
         volb=lstk(k+1)-lstk(k)
         err=lstk(top)+volb-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(volb,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+volb
      endif
      lw=lstk(top+1)

c
      if(istk(ila+3).ne.0.or.istk(ilb+3).ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',ilb)
         return
      endif
c     
      mna=istk(ila+1)*istk(ila+2)
      id(1)=0
      if(istk(ila).eq.2) then
         ida=ila+8
         la=sadr(ida+mna+1)
         call icopy(4,istk(ila+4),1,id,1)
      else
         la=sadr(ila+4)
         ida=iadr(lw)
         lw=sadr(ida+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 74 i=1,mna+1
            istk(ida+i-1)=i
 74      continue
      endif
c     
      mnb=istk(ilb+1)*istk(ilb+2)
      if(istk(ilb).eq.2) then
         idb=ilb+8
         lb=sadr(idb+mnb+1)
         if(id(1).eq.0) then
            call icopy(4,istk(ilb+4),1,id,1)
         else
            if(.not.chkvar(id,istk(ilb+4))) then
               call error(43)
               return
            endif
         endif
      else
         lb=sadr(ilb+4)
         idb=iadr(lw)
         lw=sadr(idb+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 75 i=1,mna+1
            istk(idb+i-1)=i
 75      continue
      endif

      if(mnb.ne.mna)then
         call error(60)
         return
      endif
c     
c     determine max of the degrees
      na=0
      nb=0
      do 76 i=1,mna
         na=max(na,istk(ida+i)-istk(ida-1+i))
         nb=max(nb,istk(idb+i)-istk(idb-1+i))
 76   continue
c     preserve adress of the beginning of a and b coefficients

      lar=la
      lbr=lb
c     allocate memory for intermediate results 
      law=lw
      lbw=law+na+1
      lw=lbw+nb+1

c     simplify
      la1=la
      lb1=lb
      do 79 i=1,mna
         na=istk(ida+i)-istk(ida-1+i)-1
         nb=istk(idb+i)-istk(idb-1+i)-1
         ierr=lstk(bot)-lw
         call  dpsimp(stk(la),na,stk(lb),nb,stk(law),nnum,
     $        stk(lbw),nden,stk(lw),ierr)
         if(ierr.eq.1) then
            call error(27)
            return
         elseif(ierr.eq.2) then
            call msgs(43,i)
         endif
c     .  copy overwrite initial polynomials with simplified ones
         call dcopy(nnum,stk(law),1,stk(la1),1)
         call dcopy(nden,stk(lbw),1,stk(lb1),1)

         la=la+na+1
         lb=lb+nb+1
         la1=la1+nnum
         lb1=lb1+nden
         istk(ida-1+i)=nnum
         istk(idb-1+i)=nden
 79   continue
c     
c     form vector of pointers from vector of degrees+1
      ma=1
      mb=1
      do 80 i=1,mna+1
         na=istk(ida-1+i)
         nb=istk(idb-1+i)
         istk(ida-1+i)=ma
         istk(idb-1+i)=mb
         ma=ma+na
         mb=mb+nb
 80   continue
c     
c     compute position of the a and b simplified in the result
      lstk(top)=lar+istk(ida+mna)-1
      il=iadr(lstk(top))
c     
c     put new b variable in place
      if(istk(ilb).eq.2) then
c     b matrice de polynome
         l=sadr(il+9+mna)
c     .  move b data up
         call icopy(9+mna,istk(ilb),1,istk(il),1)
         call unsfdcopy(istk(il+8+mna),stk(lbr),1,stk(l),1)
         l=l+istk(il+8+mna)-1
      else
c     b matrice de scalaires
         call icopy(4,istk(ilb),1,istk(il),1)
         l=sadr(il+4)
         call unsfdcopy(mna,stk(lbr),1,stk(l),1)
         l=l+mna
      endif
      lstk(top+1)=l
      end


      subroutine intrsimp(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vola,volb,vol
      double precision er
      logical refr,chkvar,israt,ok
      data blank/40/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)

c     simplification of a transfer function
      if(lhs.ne.1) then 
         call error(41)
         return
      endif
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      refr=il1.ne.ilr
      if(.not.israt(il1,ila,ilb,ildom)) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il1)
         return
      endif
c
      if(istk(ila+3).ne.0.or.istk(ilb+3).ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il1)
         return
      endif
c
      if(refr) then
         k=istk(ilr+2)
         vol=lstk(k+1)-lstk(k)
         err=lstk(top)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vol
         lw=lstk(top+1)
         il1=ilr
         ok=israt(il1,ila,ilb,ildom)
      endif
c
      mna=istk(ila+1)*istk(ila+2)
      id(1)=0
      if(istk(ila).eq.2) then
         ida=ila+8
         la=sadr(ida+mna+1)
         call icopy(4,istk(ila+4),1,id,1)
      else
         la=sadr(ila+4)
         ida=iadr(lw)
         lw=sadr(ida+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 74 i=1,mna+1
            istk(ida+i-1)=i
 74      continue
      endif
c
      mnb=istk(ilb+1)*istk(ilb+2)
      if(istk(ilb).eq.2) then
         idb=ilb+8
         lb=sadr(idb+mnb+1)
         if(id(1).eq.0) then
            call icopy(4,istk(ilb+4),1,id,1)
         else
            if(.not.chkvar(id,istk(ilb+4))) then
               call error(43)
               return
            endif
         endif
      else
         lb=sadr(ilb+4)
         idb=iadr(lw)
         lw=sadr(idb+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 75 i=1,mna+1
            istk(idb+i-1)=i
 75      continue
      endif

      if(mnb.ne.mna)then
         call error(60)
         return
      endif
c     
c     determine max of the degrees
      na=0
      nb=0
      do 76 i=1,mna
         na=max(na,istk(ida+i)-istk(ida-1+i))
         nb=max(nb,istk(idb+i)-istk(idb-1+i))
 76   continue
c
c     set adress where to put the results
      ll=sadr(il1+7)
      ill=iadr(ll)
      la1=la
      lb1=lb
      ida1=ida
      idb1=idb
c
c     allocate memory for intermediate results 
      law=lw
      lbw=law+na+1
      lw=lbw+nb+1

c     beginning of numerator in resulting structure
      l0=ll+istk(ilr+4)-1

c     
      lar=la1
      lbr=lb1
      nta=istk(ida+mna)
      ntb=istk(idb+mnb)
c     simplify
      do 79 i=1,mna
         na=istk(ida+i)-istk(ida-1+i)-1
         nb=istk(idb+i)-istk(idb-1+i)-1
         ierr=lstk(bot)-lw
         call  dpsimp(stk(la),na,stk(lb),nb,stk(law),nnum,
     $        stk(lbw),nden,stk(lw),ierr)
         if(ierr.eq.1) then
            call error(27)
            return
         elseif(ierr.eq.2) then
            call msgs(43,i)
         endif

c     .  copy overwrite initial polynomials with simplified ones
         call dcopy(nnum,stk(law),1,stk(la1),1)
         call dcopy(nden,stk(lbw),1,stk(lb1),1)

         la=la+na+1
         lb=lb+nb+1
         la1=la1+nnum
         lb1=lb1+nden
         istk(ida1-1+i)=nnum
         istk(idb1-1+i)=nden
 79   continue
c     
c     form vector of pointers from vector of degrees+1
      ma=1
      mb=1
      do 80 i=1,mna+1
         na=istk(ida1-1+i)
         nb=istk(idb1-1+i)
         istk(ida1-1+i)=ma
         istk(idb1-1+i)=mb
         ma=ma+na
         mb=mb+nb
 80   continue
c     
c     update size of numerator 
      vola=istk(ilr+4)-istk(ilr+3)+istk(ida1+mna)-nta
      istk(ilr+4)=istk(ilr+3)+vola

c     compute position of new denominator in the result
      ll0=ll+istk(ilr+4)-1
      il=iadr(ll0)
c     
c     put new b variable in place
      if(istk(ilb).eq.2) then
c     b matrice de polynome
         call icopy(9+mna,istk(ilb),1,istk(il),1)
         idb1=il+8
         l=sadr(il+9+mna)
         call unsfdcopy(istk(idb1+mna)-1,stk(lbr),1,stk(l),1)
         l=l+istk(idb1+mna)-1
      else
c     b matrice de scalaires
         call icopy(4,istk(ilb),1,istk(il),1)
         l=sadr(il+4)
         call unsfdcopy(mna,stk(lbr),1,stk(l),1)
         l=l+mna
      endif
      volb=l-ll0

c     copy 4th entry of the list 
      mb=istk(ilr+6)-istk(ilr+5)
      call unsfdcopy(mb,stk(ll+istk(ilr+5)-1),1,stk(l),1)
      l=l+mb
      lstk(top+1)=l
c     adjust components sizes
      istk(ilr+5)=istk(ilr+4)+volb
      istk(ilr+6)=istk(ilr+5)+mb

      
      end

      subroutine intsfact(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol
      logical ref

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      maxit=100
      lw=lstk(top+1)

      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if(rhs.ne.1) then
         call error(42)
         return
      endif

      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'sfact',il1)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      if(it1.ne.0) then
         err=1
         call error(52)
         return
      endif
      mn1=m1*n1
      id1=il1+8
      l1=sadr(id1+mn1+1)
      lr=l1
      vol=istk(id1+mn1)-1

      if(mn1.eq.1) then
c     
c     sfact of a polynomial
         if(ref) then
c     . sfact1 modifies its input argument
            lr=sadr(ilr+8+mn1+1)
            lstk(top+1)=lr+vol
            err=lstk(top+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(8+mn1+1,istk(il1),1,istk(ilr),1)
            call unsfdcopy(vol,stk(l1),1,stk(lr),1)
            lstk(top+1)=lr+vol
         endif
c
c     .  check symmetry
         n1=istk(id1+1)-2
         if (2*int(n1/2).ne.n1) then
            call error(88)
            return
         endif
         n=1+n1/2
         do 81 i=0,n-1
            if(stk(lr+i).ne.stk(lr+n1-i)) then
               call error(88)
               return
            endif
 81      continue
c     
         lw=lstk(top+1)
         err=lw+6*n-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call sfact1(stk(lr),n-1,stk(lw),maxit,ierr)
         if(ierr.eq.2) then
            write(buf,82) n-1
 82         format('No real solution: degree ',i2,' entry is negative!')
            call error(999)
            return
         else if(ierr.eq.1) then
            call error(24)
            return
         else if(ierr.lt.0) then
c     convergence incomplete
            write(buf(1:4),'(i3)') ierr
            call msgs(22,0)
         endif
         lstk(top+1)=lr+n
         istk(ilr+8+1)=n+1
      else
c     multivariable case
         if(m1.ne.n1) then
            err=1
            call error(20)
            return
         endif
c     .  convert matrix of polynomials to a polynomial matrix
         n1=0
         do 87 i=1,mn1
            n1=max(n1,istk(id1+i)-istk(id1+i-1))
 87      continue
c     
         n1=1+(n1-1)/2
         l2=lstk(top+1)
         if(ref) l2=l2+mn1+9+mn1*n1
         lw=l2+mn1*n1
         err=lw+sadr((n1+1)*m1*((n1+1)*m1)+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c     
         call dset(mn1*n1,0.0d+0,stk(l2),1)
         do 88 i1=0,mn1-1
            lij=l1-1+istk(id1+i1)
            mij=2+istk(id1+i1+1)-istk(id1+i1)-1-n1
            if(mij.gt.0) call unsfdcopy(mij,stk(lij+n1-1),1,
     $           stk(l2+i1),mn1)
 88      continue
c     
         maxit=maxit+n1
         call sfact2(stk(l2),m1,n1-1,stk(lw),maxit,ierr)
         if(ierr.lt.0) then
            call error(24)
            return
         endif
         if(ierr.gt.0) then
            call error(88)
            return
         endif
c     .  convert polynomial matrix to matrix of polynomials 
         id1=ilr+8
         lr=sadr(id1+mn1+1)
         l1=lr
         do 89 i=0,mn1-1
            call unsfdcopy(n1,stk(l2+i),mn1,stk(l1),1)
            l1=l1+n1
 89      continue
         do 90 i=0,mn1
            istk(id1+i)=1+n1*i
 90      continue
         if(ref) call icopy(8,istk(il1),1,istk(ilr),1)

         lstk(top+1)=l1
      endif
      return
      end

      subroutine intvarn(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,blank
      logical ref
      data blank/40/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c     
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if(istk(il1).ne.2) then
         if(rhs.eq.2.and.istk(il1).eq.1) then
            top=top-1
            if (ref) then
               k=istk(ilr+2)
               vol=lstk(k+1)-lstk(k)
               err=lstk(top)+vol-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
               call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
               lstk(top+1)=lstk(top)+vol
            endif
            return
         endif
         fun=-1
         call funnam(ids(1,pt+1),'varn',il1)
         return
      endif
      call icopy(4,istk(il1+4),1,id,1)

      if(rhs.eq.1) then
c     get formal variable name
         if(ref) then
            err=sadr(ilr+10)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
c     extraction du nom de la variable muette
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         do 10 ii=4,1,-1
            if(istk(il1+3+ii).ne.blank) then
               nv=ii
               goto 11
            endif
 10      continue
 11      call icopy(nv,istk(il1+4),-1,istk(ilr+6),-1)
         istk(ilr+4)=1
         istk(ilr+5)=nv+1
         lstk(top+1)=sadr(ilr+10)
      elseif(rhs.eq.2) then
c     change formal variable name
c     .  get new variable name
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(36)
            return
         endif
         nc=istk(il+5)-1
         if(istk(il1).eq.1) goto 106
         call icopy(max(4,nc),istk(il+6),1,id,1)
         if(nc.lt.4) call iset(4-nc,blank,id(nc+1),1)
 106     top=top-1
         if(ref) then
c     .     variable is passed by reference copy it on the top of the
c           stack
            k=istk(ilr+2)
            vol=lstk(k+1)-lstk(k)
            err=lstk(top)+vol-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+vol
         endif
         ilr=iadr(lstk(top))
         call icopy(4,id,1,istk(ilr+4),1)
      else
         call error(42)
         return
      endif
      return
      end

      subroutine intpclean(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol
      logical ref
      double precision epsa,epsr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
      if (rhs.lt.1.or.rhs.gt.3) then
         call error(42)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).ne.2) then
         fun=-1
         call funnam(ids(1,pt+1),'clean',il1)
         return
      endif
      ref=il1.ne.ilr

c     get relative and absolute tolerances
      epsr=1.0d-10
      epsa=1.0d-10

      if (rhs.eq.3) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         epsr=stk(sadr(il+4))
         top=top-1
      endif

      if (rhs.ge.2) then
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.1) then
            err=3
            call error(52)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=3
            call error(60)
            return
         endif
         if(istk(il+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         epsa=stk(sadr(il+4))
         top=top-1
      endif

      if(ref) then
         k=istk(ilr+2)
         vol=lstk(k+1)-lstk(k)
         err=lstk(top)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+vol
         il1=ilr
      endif
      it1=istk(il1+3)
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+8
      l1=sadr(id1+mn1+1)
      vol=istk(id1+mn1)-1


      id2=iadr(lstk(top+1))
      err=sadr(id2+mn1+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(it1.eq.0) then
         call dmpcle(stk(l1),istk(id1),m1,n1,istk(id2),epsr,epsa)
      else
         call wmpcle(stk(l1),stk(l1+vol),istk(id1),m1,n1,
     &        istk(id2),epsr,epsa)
      endif
      lstk(top+1)=l1+(istk(id1+mn1)-1)*(it1+1)
      return
      end

      subroutine intsimpmd(id)
      INCLUDE '../stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer simpmd
      common/csimp/ simpmd
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.1) then
         call error(39)
         return
      endif
      if(rhs.le.0) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=4
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=simpmd
         lstk(top+1)=sadr(il+4)
      else
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.4) then
            err=1
            call error(208)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(36)
            return
         endif
         simpmd=istk(il+3)
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=sadr(il+1)
      endif
      return
      end
