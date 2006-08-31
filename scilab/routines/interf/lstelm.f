      subroutine lstelm
c ================================== ( Inria    ) =============
c
c     evaluate utility list's functions
c
c =============================================================
c     

c     Copyright INRIA
c     
      include '../stack.h'
      integer ltyp
      integer iadr,sadr
c
      external rattyp,lsstyp,gratyp
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' lstelm '//buf(1:4))
      endif
c
c     functions/fin
c     1        2       3       4        5     6     7     8
c     list     tlist   rlist   lsslist glist lstcat mlist definedfields
c     9
c     lstsize

c
      rhs=max(0,rhs)
      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         return
      endif
c
      goto(01,01,20,30,40,50,01,60,70) fin
c
 01   lf=lstk(top+1)
      top=top+1-rhs
      ld=lstk(top)
c
      n=rhs
      err=lf+sadr(n+3)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(fin.eq.2) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=16
      elseif(fin.eq.7) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=17
      else
c     c untyped list
         ltyp=15
      endif

      il=iadr(ld)
      l=sadr(il+n+3)
      call unsfdcopy(lf-ld,stk(ld),-1,stk(l),-1)

      istk(il)=ltyp
      istk(il+1)=n
      istk(il+2)=1
      if(n.ne.0) then
         do 10 i=1,n
            istk(il+2+i)=istk(il+1+i)+lstk(top+i)-lstk(top-1+i)
 10      continue
      endif
      lstk(top+1)=l+lf-ld
      goto 99
c
 20   continue
c     rlist
      call mtlist(rattyp)
      if(err.gt.0) return
      goto 99
c
 30   continue
c     lsslist
      call mtlist(lsstyp)
      if(err.gt.0) return
      goto 99
c
 40   continue
c     glist
      call mtlist(gratyp)
      if(err.gt.0) return
      goto 99

 50   continue
c     lstcat
      call intlstcat()
      if(err.gt.0) return
      goto 99

 60   continue
c     definedfields
      call definedfields()
      if(err.gt.0) return
      goto 99

 70   continue
c     lstsize
      call intlstsize()
      if(err.gt.0) return
      goto 99


 99   return
      end
