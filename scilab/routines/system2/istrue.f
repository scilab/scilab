      function istrue(mod)
c     Copyright INRIA
      logical istrue
c
      include '../stack.h'
      integer iadr,sadr

c      
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
c
      if(err1.gt.0) goto 20
      istrue=.true.
c
      il=iadr(lstk(top))
      if(mod.ne.0) top=top-1
      if(istk(il).eq.4) then
         m=istk(il+1)*istk(il+2)
         if(m.eq.0) goto 20
         do 10 i=1,m
            if(istk(il+2+i).eq.0) goto 20
 10      continue
      elseif(istk(il).eq.6) then
         m=istk(il+1)*istk(il+2)
         nel=istk(il+4)
         if(m.eq.0.or.nel.ne.m) goto 20
      elseif(istk(il).eq.1.and.istk(il+3).eq.0) then
         m=istk(il+1)*istk(il+2)
         if(m.eq.0) goto 20
         l=sadr(il+4)
         do 11 i=0,m-1
            if(stk(l+i).eq.0.0d+0) goto 20
 11      continue
      else
         err=1
         call error(44)
      endif
      return

 20   istrue=.false.
      return
      end

      
