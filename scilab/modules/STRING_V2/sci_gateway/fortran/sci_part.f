c =========================================
c INRIA
c =========================================  
      subroutine intpart(id)
      INCLUDE 'stack.h'
c
      integer blank,id(nsiz),vol
      logical ref
      integer iadr,sadr
      data    blank/40/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1


      if(lhs.ne.1.or.rhs.ne.2) then
         call error(39)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).ne.10) then
         fun=-1
         call funnam(ids(1,pt+1),'part',il1)
         return
      endif
c
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
      lw=lstk(top+1)
c
      lr=max(iadr(lw),ilr+mn1+5)
      ilv=iadr(lstk(top))
      if(istk(ilv).lt.0) ilv=iadr(istk(ilv+1))
c
      if(istk(ilv).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      if(istk(ilv+1).gt.1.and.istk(ilv+2).gt.1) then
         err=rhs
         call error(89)
         return
      endif
      if(istk(ilv+3).ne.0) then
         err=rhs
         call error(52)
         return
      endif
      nv=istk(ilv+1)*istk(ilv+2)
c
      if(nv.eq.0) then
         if(ref) then
            call icopy(4,istk(il1),1,istk(ilr),1)
            id1=ilr+4
            l1=id1+mn1+1
         endif
         call iset(mn1+1,1,istk(id1),1)
         top=top-1
         lstk(top+1)=sadr(l1)
         goto 999
      endif

      lv=sadr(ilv+4)
      do 21 k=0,nv-1
         if(int(stk(lv+k)).le.0) then
            err=2
            call error(36)
            return
         endif
 21   continue
      err=sadr(lr+mn1*nv)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call iset(mn1*nv,blank,istk(lr),1)
      lrijk=lr
      lij=l1
      do 23 ij=1,mn1
         nij=istk(id1+ij)-istk(id1+ij-1)
         do 22 k=1,nv
            mv=int(stk(lv-1+k))
            if(mv.le.nij) istk(lrijk)=istk(lij-1+mv)
            lrijk=lrijk+1
 22      continue
         lij=lij+nij
 23   continue
c     
      if(ref) then
         call icopy(4,istk(il1),1,istk(ilr),1)
         id1=ilr+4
         l1=id1+mn1+1
      endif
      istk(id1)=1
      do 24 ij=1,mn1
      istk(id1+ij)=istk(id1+ij-1)+nv
 24   continue
      top=top-1
      if(lr.ne.l1) call icopy(mn1*nv,istk(lr),1,istk(l1),1)
      lstk(top+1)=sadr(l1+istk(id1+mn1))
 999  return
      end
c =========================================  
