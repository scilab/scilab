      subroutine intreadb
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
      integer top2
      integer mode(2)
      logical opened
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      lw=lstk(top+1)
      il=iadr(lstk(top))
      if(lhs.ne.1) then
         call error(42)
         return
      endif
      iacces=0
      mode(2)=0
      if(rhs.eq.4) then
         iacces=top
         ilb=iadr(lstk(iacces))
         if(istk(ilb+3).ne.0) then
            err=top
            call error(52)
            return
         endif
         nb=istk(ilb+1)*istk(ilb+2)
         lb=sadr(ilb+4)
         top=top-1
         rhs=rhs-1
         mode(1)=-111
      elseif(rhs.eq.3) then  
         mode(1)=-101
      else
         call error(39)
         return
      endif
c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))

      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      if(lunit.eq.wte) then
         call error(49)
         goto 996
      endif
c     
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=3
         call error(53)
         goto 996
      endif
      if(istk(il+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=3
         call error(89)
         goto 996
      endif
      l=sadr(il+4)
      n=int(stk(l))
c     
      top2=top2-1
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=2
         call error(53)
         goto 996
      endif
      if(istk(il+3).ne.0) then
         err=2
         call error(52)
         goto 996
      endif
      if(istk(il+1)*istk(il+2).ne.1) then
         err=2
         call error(89)
         goto 996
      endif
      l=sadr(il+4)
      m=int(stk(l))
      if(iacces.ne.0) then
         if(m.ne.nb) then
            call error(42)
            goto 996
         endif
      endif
c     
      il=iadr(lstk(top))
      if(m.eq.0.or.n.le.0) then
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(.not.opened) call clunit(-lunit,buf,mode)
         goto 999
      endif

      l=sadr(il+4)
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=0

      if(m.gt.0) then
         lw=l+m*n
         if(iacces.ne.0) then
            ilb=iadr(lw)
            lw=sadr(ilb+nb)
         endif
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            goto 996
         endif
      endif
c     
      if(iacces.eq.0) then
c     .  sequential acces
         if(m.gt.0) then
            do 191 i=0,m-1
               read(lunit,end=997,err=998) (stk(l+i+j*m),j=0,n-1)
 191        continue
         else
            i=-1
 192        i=i+1
            li=l+n*i
            err=li+n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
            read(lunit,end=193,err=998) (stk(li+j),j=0,n-1)
            goto 192
 193        m=i
            lstk(top+1)=l+m*n
            err=lstk(top+1)+m*n-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
            l1=l+m*n
            call unsfdcopy(m*n,stk(l),1,stk(l1),1)
            call mtran(stk(l1),n,stk(l),m,n,m)
            istk(il+1)=m
         endif
      else
c     .  direct access
         call entier(nb,stk(lb),istk(ilb))
         do 194 i=0,m-1
            read(lunit,rec=istk(ilb+i),err=998)
     $           (stk(l+i+j*m),j=0,n-1)
 194     continue
      endif
      lstk(top+1)=l+m*n
      if(.not.opened) call clunit(-lunit,buf,mode)
      goto 999


 996  if(.not.opened) call clunit(-lunit,buf,mode)
      return
 997  err=i
      call error(62)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return
 998  call error(49)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return

 999  return
      end
