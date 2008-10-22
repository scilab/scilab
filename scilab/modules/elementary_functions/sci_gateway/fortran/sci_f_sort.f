c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txtc 

c =========================================  
      subroutine intsort(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops,sel
      integer iadr,sadr
      integer modtest,rptest
      external modtest,rptest
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top
c
      il=iadr(lstk(top+1-rhs))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.1) then
         call funnam(ids(1,pt+1),'sort',il)
         fun=-1
         return
      endif

c     select type of sort to perform
      sel=0
      if(rhs.eq.2) then
         call  getorient(top,sel)
         if(err.gt.0) return
         top=top-1
      endif

      if(sel.eq.2) then
c        sort(a,'c')   <=>  sort(a,2)   The lazy way...
         top=tops
         call funnam(ids(1,pt+1),'sort',il)
         fun=-1
         return
      endif

      il0=iadr(lstk(top))
      if(istk(il0).lt.0) then
c     array is passed by reference copy it on the top of the stack
         k=istk(il0+2)
         err=lstk(top)+lstk(k+1)-lstk(k)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(lstk(k+1)-lstk(k),stk(lstk(k)),1
     $        ,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(k+1)-lstk(k)
      endif

      m=istk(il0+1)
      n=istk(il0+2)
      it=istk(il0+3)
      mn=m*n
c     
      if(mn.eq.0) then
         if(lhs.eq.1) return
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         return
      endif
c
      lw=iadr(lstk(top+1))
      err=sadr(lw+mn)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      if(sel.eq.0) then
c     sort(a) <=> sort(a,'*')
         istk(il0)=1
         istk(il0+1)=m
         istk(il0+2)=n
         istk(il0+3)=it
         l1=sadr(il0+4)
         if(it.eq.0) then
            call dsort(stk(l1),mn,istk(lw))
         else
            call wsort(stk(l1),stk(l1+mn),mn,istk(lw),modtest)
         endif
         lstk(top+1)=l1+mn*(it+1)
         if(lhs.eq.1) return
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
  106    continue
         istk(il)=1
         istk(il+1)=m
         istk(il+2)=n
         istk(il+3)=0
      elseif(sel.eq.1) then
c     sort(a,'r')  <=>  sort(a,1)
         istk(il0)=1
         istk(il0+1)=m
         istk(il0+2)=n
         istk(il0+3)=it
         l1=sadr(il0+4)
         lw1=lw
         if(it.eq.0) then
            do 107 k=0,n-1
               call dsort(stk(l1+k*m),m,istk(lw1))
               lw1=lw1+m
 107        continue
         else
            do 108 k=0,n-1
               call wsort(stk(l1+k*m),stk(l1+mn+k*m),mn,istk(lw1),
     $              modtest)
              lw1=lw1+m
 108        continue
         endif
         lstk(top+1)=l1+mn*(it+1)
c              cccccccccccc
         if(lhs.eq.1) return
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
         do 109 i=1,mn
           stk(l1-i)=dble(istk(l2-i))
  109    continue
         istk(il)=1
         istk(il+1)=m
         istk(il+2)=n
         istk(il+3)=0
      elseif(sel.eq.2) then
c     implemented by a call to a function see above
      endif
      return
      end

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

      tops=top

c     select type of sort to perform
      sel=0
      if(rhs.eq.2) then
         call  getorient(top,sel)
         if(err.gt.0) return
         top=top-1
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
      integer function strord(r1,l1,r2,l2)
      integer r1(*),r2(*),c1,c2
      integer blank
      data blank/40/
c
      iord=0
      if(l1.eq.0) then
         if(l2.gt.0) then
            strord=-1
            return
         else
            strord=0
            return
         endif
      else
         if(l2.eq.0) then
            strord=1
            return
         endif
      endif
      ll=min(l1,l2)
      do 10 i=1,max(l1,l2)
         if(i.le.l1) then
            c1=r1(i)
         else
            c1=blank
         endif
         if(i.le.l2) then
            c2=r2(i)
         else
            c2=blank
         endif
         if(c1.ge.0) c1=256-c1
         if(c2.ge.0) c2=256-c2
         if(c1.gt.c2) then
            strord=1
            return
         elseif(c1.lt.c2) then
            strord=-1
            return
         endif
 10   continue
      strord=0
      return
      end
c =========================================
