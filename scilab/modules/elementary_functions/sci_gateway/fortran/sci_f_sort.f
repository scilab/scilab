c     -------------------------------
c			INRIA
c     -------------------------------
      subroutine intsort(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops,sel
      integer iadr,sadr
      integer modtest,rptest
      external modtest,rptest
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      il=iadr(lstk(top+1-rhs))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.10) then
c     *call* strelm
         fun=21
         fin=8
         return
      endif
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

c     -------------------------------
