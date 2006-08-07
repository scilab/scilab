c =========================================
c INRIA
c =========================================  
      subroutine instrcat(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz),vol,tops
      logical ref
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strcat(tr [,ins])
c
      if (rhs.eq.3) then 
         call intstrcat('strcat') 
         return 
      endif

      if(rhs.ne.1.and.rhs.ne.2) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
c     
      tops=top
c
      if(rhs.eq.2) then
         il2=iadr(lstk(top))
         if(istk(il2).lt.0) il2=iadr(istk(il2+1))
         if(istk(il2).ne.10) then 
            fun=-1
            top=tops
            call funnam(ids(1,pt+1),'strcat',il2)
            return
         endif
         if(istk(il2+1)*istk(il2+2).ne.1) then
            err=2
            call error(36)
            return
         endif
         l2=il2+6
         nv2=istk(il2+5)-1
         top=top-1
      endif
c
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1

      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            istk(ilr)=10
            istk(ilr+1)=1
            istk(ilr+2)=1
            istk(ilr+3)=0
            istk(ilr+4)=1
            istk(ilr+5)=1
            lstk(top+1)=sadr(ilr+6)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strcat',il1)
         return
      endif

      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1
      id1=il1+4
      l1=id1+mn1+1
      vol=istk(id1+mn1)-1
c
      if(rhs.eq.1) then
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+vol
         l=ilr+6
         call icopy(vol,istk(l1),1,istk(l),1)
         lstk(top+1)=sadr(l+vol)
      else
         lw = iadr(lstk(tops+1))
         err=sadr(lw+vol+mn1*nv2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         ll1=l1
         ll2=lw
         do 10 i=1,mn1
            ln=istk(id1+i)-istk(id1-1+i)
            call icopy(ln,istk(ll1),1,istk(ll2),1)
            ll1=ll1+ln
            ll2=ll2+ln
            call icopy(nv2,istk(l2),1,istk(ll2),1)
            ll2=ll2+nv2
 10      continue
         vol=vol+(mn1-1)*nv2
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+vol
         l=ilr+6
         call icopy(vol,istk(lw),1,istk(l),1)
         lstk(top+1)=sadr(l+vol)
      endif
      end
c =========================================  