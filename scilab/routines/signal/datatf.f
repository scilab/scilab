      subroutine datatf
c     Copyright INRIA
c ================================== ( Inria    ) =============
c     manipulation de donnees
c =============================================================
      include '../stack.h'
c
      common /tg02bd/ knot
      integer lr(4),per,iadr,sadr
      double precision vals(4)
      logical period
      data per/25/
c
c
c     fonction  spline inte
c                 1     2
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' datatf '//buf(1:4))
      endif
      if(rhs.le.0) then
                call error(39)
                return
      endif
      goto (10,20) fin
c
c spline
c
   10 if(rhs.gt.3) then
         call error(39)
         return
      endif
      period=.false.
      if(rhs.eq.3) then
        il=iadr(lstk(top))
        if(istk(il).ne.10) then
           err=rhs
           call error(55)
           return
        endif
        if(istk(il+1)*istk(il+2).ne.1) then
           err=rhs
           call error(89)
           return
        endif
        if(abs(istk(il+6)).eq.per) period=.true.
        top=top-1
      endif
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
       err=2
       call error(52)
       return
      endif
      nf=istk(il+1)*istk(il+2)
      lf=sadr(il+4)
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      nx=istk(il+1)*istk(il+2)
      n=min(nf,nx)
      lx=sadr(il+4)
c
      ld=lf+n
      lw=ld+n
      err=lw+3*n-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(.not.period) then
         call dspln(n,stk(lx),stk(lf),stk(ld),stk(lw),err)
         if(err.eq.1) then
            err=1
            call error(99)
            return
         endif
      else
         call dpspln(n,stk(lx),stk(lf),stk(ld),stk(lw),err)
         if(err.eq.1) then
            err=1
            call error(99)
            return
         endif
         if(err.eq.2) then
            err=2
            call error(101)
            return
         endif
      endif
      call unsfdcopy(n,stk(ld),1,stk(lx),1)
      goto 99
c
c
c interpolation
c
   20 if(rhs.ne.4) then
         call error(39)
         return
      endif
      lw=lstk(top+1)
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=4
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=4
         call error(52)
         return
      endif
      n=istk(il+1)*istk(il+2)
      ld=sadr(il+4)
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      n=istk(il+1)*istk(il+2)
      lf=sadr(il+4)
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      n=istk(il+1)*istk(il+2)
      lx=sadr(il+4)
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      m0=istk(il+1)
      n0=istk(il+2)
      mn0=m0*n0
      lx0=sadr(il+4)
      lr(1)=lx0
      if(lhs.ge.2) then
            err=lw+(lhs-1)*mn0-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            do 21 k=2,lhs
            lr(k)=lw
   21       lw=lw+mn0
      endif
      mode=-1
      do 23 i=1,mn0
      call tg02ad(mode,n,stk(lx),stk(lf),stk(ld),stk(lx0+i-1),
     1            vals)
      mode=1
      do 22 k=1,lhs
   22 stk(lr(k)-1+i)=vals(k)
      mode=+1
   23 continue
      if(lhs.eq.1) goto 99
      do 24 k=2,lhs
      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=n0
      istk(il+2)=m0
      istk(il+3)=0
      l=sadr(il+4)
      call unsfdcopy(mn0,stk(lr(k)),1,stk(l),1)
      lstk(top+1)=l+mn0
   24 continue
      goto 99
c
   99 continue
      return
      end
