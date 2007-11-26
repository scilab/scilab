c =========================================
c INRIA
c =========================================  
      subroutine intssort(id)
      INCLUDE 'stack.h'
c
      integer id(nsiz),tops,vol,sel
      logical ref
      external strord
      integer strord
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.gt.2) then
         call error(41)
         return
      endif

      tops=top

c     select type of sort to perform
      sel=0
      if(rhs.eq.2) then
         call  getorient(top,sel)
         if(err.gt.0) return
         top=top-1
      endif


      if(sel.eq.2) then
         top=tops
         fun=-1
         call funnam(ids(1,pt+1),'sort',iadr(lstk(top+1-rhs)))
         return
      endif
      if(rhs.eq.2) rhs=rhs-1
      call ref2val
c
      il1=iadr(lstk(top))
      ilr=il1

      m=istk(il1+1)
      n=istk(il1+2)
      mn=m*n
      id1=il1+4
      l1=id1+mn+1
      vol=istk(id1+mn)-1

c
      id1r=id1
      ls=iadr(lstk(top+1))
      lsz=ls+vol


      lind=lsz+mn
      lw=lind+mn
      err=sadr(lw)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      call icopy(vol,istk(l1),1,istk(ls),1)

      do 91 i=0,mn-1
         istk(lsz+i)=istk(id1+i+1)-istk(id1+i)
 91   continue

c     
      if(sel.eq.0) then
c     sort(a) <=> sort(a,'*')
         call rcsort(strord,istk(lsz),istk(id1),istk(ls),mn,istk(lind))
      elseif(sel.eq.1) then
c     sort(a,'r')  <=>  sort(a,1)
         lszi=lsz
         idi=id1
         lindi=lind
         do 95 i=0,n-1
            call rcsort(strord,istk(lszi),istk(idi),istk(ls),m,
     $           istk(lindi))
            lszi=lszi+m
            idi=idi+m
            lindi=lindi+m
 95      continue   
      endif

      l2=ilr+4+mn+1
      do 93 i=0,mn-1
         call icopy(istk(lsz+i),istk(ls-1+istk(id1+i)),1,istk(l2),1)
         l2=l2+istk(lsz+i)
 93   continue
      lstk(top+1)=sadr(l2)

      istk(id1)=1
      do 94 i=0,mn-1
         istk(id1+i+1)=istk(id1+i)+istk(lsz+i)
 94   continue
      if(lhs.eq.1) goto 999
      top=top+1
      il=iadr(lstk(top))

      l=sadr(il+4)
      inc=-1
      if(sadr(lind).gt.l) inc=1
      call int2db(mn,istk(lind),inc,stk(l),inc)
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0
      lstk(top+1)=l+mn

      go to 999
 999  return
      end
c =========================================  
