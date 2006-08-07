c =========================================
c INRIA
c =========================================  
      subroutine intstrsplit(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz),vol,tops
      logical ref1,ref2
c
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c     strsplit(str,v)
c

      if(rhs.ne.2) then
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
      lw=lstk(top+1)

      il2=iadr(lstk(top))
      ref2=istk(il2).lt.0
      if(ref2) il2=iadr(istk(il2+1))

      if(istk(il2).ne.1) then 
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsplit',il2)
         return
      endif
      n2=istk(il2+1)*istk(il2+2)
      l2=sadr(il2+4)
      if (stk(l2).lt.1) then
         err=2
         call error(116)
         return
      endif
      do i=1,n2-1
         if (stk(l2+i).lt.stk(l2+i-1)) then
            err=2
            call error(99)
            return
         endif
      enddo
c
      top=top-1
      il1=iadr(lstk(top))
      ref1=istk(il1).lt.0
      if(ref1) il1=iadr(istk(il1+1))

      if(istk(il1).eq.1) then
         if(istk(il1+1)*istk(il1+2).eq.0) then
            ilr=iadr(lstk(top))
            istk(ilr)=1
            istk(ilr+1)=0
            istk(ilr+2)=0
            istk(ilr+3)=0
            lstk(top+1)=sadr(ilr+4)
            return
         endif
      endif
      if(istk(il1).ne.10) then
         fun=-1
         top=tops
         call funnam(ids(1,pt+1),'strsplit',il1)
         return
      endif
      if (istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(89)
         return
      endif
      id1=il1+4
      l1=id1+2
      vol=istk(id1+1)-1
      if (stk(l2+n2-1).ge.vol) then
         err=2
         call error(116)
         return
      endif
      if(ref1.and.ref2) then
         ilr=iadr(lstk(top))
      else
         ilr=iadr(lw)
         err = sadr(ilr+4+n2+2+vol) - lstk(bot)
         if ( err.gt.0 ) then
            call error(17)
            return
         endif
      endif
      istk(ilr)=10
      istk(ilr+1)=n2+1
      istk(ilr+2)=1
      istk(ilr+3)=0

      istk(ilr+4)=1
      do i=1,n2
         istk(ilr+4+i)=stk(l2+i-1)+1
      enddo
      istk(ilr+5+n2)=vol+1
      call icopy(vol,istk(l1),1,istk(ilr+6+n2),1)
      if(ref1.and.ref2) then
         lstk(top+1)=sadr(ilr+6+n2+vol)
      else
         il=iadr(lstk(top))
         call icopy(6+n2+vol,istk(ilr),1,istk(il),1)
         lstk(top+1)=sadr(il+6+n2+vol)
      endif
      end
c =========================================  
