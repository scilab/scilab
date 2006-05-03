      subroutine ref2val
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer vol,topk,volk
      logical ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

      if(rhs.le.0.or.top.lt.rhs) return

      vol=0
      ref=.false.
      topk=top+1-rhs
      do 10 i=0,rhs-1
         il=iadr(lstk(topk))
         if(istk(il).lt.0) then
            ref=.true.
            k=istk(il+2)
            if (k.eq.0) then
               volk=istk(il+3)
            else
               volk=lstk(k+1)-lstk(k)
            endif
         else
            volk=lstk(topk+1)-lstk(topk)
         endif
         vol=vol+volk
         topk=topk+1
 10   continue
      if(.not.ref) return
c
      le=lstk(top+1-rhs)+vol
      err=le-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      topk=top
      do 20 i=0,rhs-1
         il=iadr(lstk(topk))
         if(istk(il).lt.0) then
c     .     next line to reset the global property if it is set
            infstk(topk)=0
            k=istk(il+2)
            if (k.eq.0) then
               volk=istk(il+3)
               lk=istk(il+1)
            else
               volk=lstk(k+1)-lstk(k)
               lk=lstk(k)
            endif
         else
            volk=lstk(topk+1)-lstk(topk)
            lk=lstk(topk)
         endif
c         if(istk(il).lt.0) k=istk(il+2)
c         volk=lstk(k+1)-lstk(k)
         lstk(topk+1)=le
c         lk=lstk(k)
         le=le-volk
         if(lk.ne.le) call unsfdcopy(volk,stk(lk),-1,stk(le),-1)
         topk=topk-1
 20   continue
      return
      end

     
