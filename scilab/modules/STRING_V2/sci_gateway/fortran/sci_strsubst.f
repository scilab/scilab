c =========================================
c INRIA
c =========================================  
      subroutine intstrsubst(id)
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
c     strsubst(str1,str2,str3)
c
      if(rhs.ne.3) then
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
      lw = iadr(lstk(top+1))
c     
      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      if(istk(il3).ne.10) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il3)
         return
      endif
      if(istk(il3+1)*istk(il3+2).ne.1) then
         err=3
         call error(36)
         return
      endif
      l3=il3+6
      nv3=istk(il3+5)-1
      top=top-1

      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.10) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il2)
         return
      endif
      if(istk(il2+1)*istk(il2+2).ne.1) then
         err=2
         call error(36)
         return
      endif
      l2=il2+6
      nv2=istk(il2+5)-1
      if(nv2.eq.0) then
         err=2
         call error(249)
         return
      endif
      top=top-1

c     
      il1=iadr(lstk(top))
      ilr1=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=il1.ne.ilr1
      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            istk(ilr1)=1
            istk(ilr1+1)=0
            istk(ilr1+2)=0
            istk(ilr1+3)=0
            lstk(top+1)=sadr(ilr1+4)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsubst',il1)
         return
      endif

      mn1=istk(il1+1)*istk(il1+2)
      id1=il1+4
      l1=id1+mn1+1
      
c
      if(ref) then
         id1r=lw
         istk(id1r)=1
         lw=lw+mn1+1
      else
         id1r=id1
      endif
      ilr=lw
      ilr0=ilr
      err=sadr(ilr)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      vol=istk(id1+1)-istk(id1)
c
      do 40 ij=1,mn1
         mc=0
         if(vol.eq.0) goto 36
         ll0=l1
         ll1=l1-1
 10      ll1=ll1+1
         if(ll1-l1.ge.vol+1-nv2) goto 35
         if(istk(ll1).ne.istk(l2)) goto 10
c     first character matches
         k=0
 15      k=k+1
         if(k.ge.nv2) goto 30
         if(istk(ll1+k).eq.istk(l2+k)) goto 15
         goto 10
 30      continue
c     a match found
         err=sadr(ilr+ll1-ll0+nv3) -lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         mc=mc+1
         call icopy(ll1-ll0,istk(ll0),1,istk(ilr),1)
         ilr=ilr+ll1-ll0
         ll0=ll1+nv2

         call icopy(nv3,istk(l3),1,istk(ilr),1)
         ilr=ilr+nv3
         ll1=ll1+nv2-1
         goto 10
 35      continue
c     end of string reached
         err=sadr(ilr+vol-(ll0-l1))-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call icopy(vol-(ll0-l1),istk(ll0),1,istk(ilr),1)
         ilr=ilr+vol-(ll0-l1)
         l1=l1+vol
 36      vol=istk(id1+ij+1)-istk(id1+ij)
         istk(id1r+ij)=istk(id1r+ij-1)+ilr-ilr0
         ilr0=ilr
 40   continue
      id1=ilr1+4
      if(ref) then
         call icopy(4,istk(il1),1,istk(ilr1),1)
         call icopy(mn1+1,istk(id1r),1,istk(id1),1)
      endif
      call icopy(ilr-lw,istk(lw),1,istk(id1+mn1+1),1)
      lstk(top+1)=sadr(id1+mn1+1+ilr-lw)

      return
      end
c =========================================  
