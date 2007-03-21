c =========================================
c INRIA
c =========================================  
      subroutine intascii(id)
      include 'stack.h'
c
      integer id(nsiz),tops,vol,sel,eol
      logical ref
      external strord,getfastcode
      integer strord,getfastcode
      character*1 c
      integer iadr,sadr
      data eol/99/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.ne.1) then
         call error(42)
         return
      endif
      if(lhs.ne.1) then
         call error(41)
         return
      endif

      il=iadr(lstk(top))
      ilr=il
      if(istk(il).lt.0) il=iadr(istk(il+1))
      ref=il.ne.ilr

      if(istk(il).eq.1) then
c     argument is a vector of ascii codes return a scilab string
         n=istk(il+1)*istk(il+2)
         l=sadr(il+4)
         if(.not.ref) then
            err=l+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(n,stk(l),1,stk(l+n),1) 
            l=l+n
         else
            err=sadr(ilr+6+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         istk(ilr+4)=1
         istk(ilr+5)=1+n
         lr=ilr+6
         do 20 i=0,n-1
            c=char(int(stk(l+i)))
            istk(lr+i)=getfastcode(c)
 20      continue
         lstk(top+1)=sadr(lr+n)
      elseif(istk(il).eq.10) then
c     argument is a scilab string return a vector of ascii codes
         n=istk(il+4+istk(il+1)*istk(il+2))-1
         l=il+5+istk(il+1)*istk(il+2)
         if(.not.ref) then
            lw=l+n
            err=sadr(lw+n)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call icopy(n,istk(l),1,istk(lw),1) 
            l=lw
         else
            err=sadr(ilr+4)+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
         istk(ilr)=1
         istk(ilr+1)=1
         istk(ilr+2)=n
         istk(ilr+3)=0
         lr=sadr(ilr+4)
         do 30 i=0,n-1
            mc=istk(l+i)
            if(mc.gt.eol) then
               stk(lr+i)=ichar(char(mc-(eol+1)))
            elseif(mc.lt.0) then
               stk(lr+i)=ichar(alfb(abs(mc)+1))
            elseif(mc.eq.eol) then
               stk(lr+i)=10.
            else
               stk(lr+i)=ichar(alfa(mc+1))
            endif
 30      continue
         lstk(top+1)=lr+n
      else
         fun=-1
         call funnam(ids(1,pt+1),'ascii',il)
      endif
      end
c =========================================  

