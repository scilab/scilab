      subroutine intmgetl
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer mode(2)
      integer iadr,sadr
      logical checkrhs,checklhs,getscalar
      logical opened
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c           
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,1,2)) return
      if(.not.checklhs(fname,1,1)) return

c     opening file
      call v2cunit(top-rhs+1,'rb',lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      if(rhs.eq.2) then
         if (.not.getscalar('mgetl',top,top,lr)) return
         m=stk(lr)
         top=top-1
      else
         m=-1
      endif

      il=iadr(lstk(top))
      ili=il+4


      if(m.gt.0) then
c     .  specified number of lines
         err=sadr(ili+2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         li=ili+m+1
         istk(ili)=1
         do 10 i=1,m
            call readnextline(lunit,buf,bsiz,mn,nr,info)
            if(info.eq.-1) then
               err=i
               call error(62)
               if(.not.opened) call clunit(-lunit,buf,mode)
               return
            endif
            mn=max(0,mn-1)
            err=sadr(li+mn)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call cvstr(mn,istk(li),buf(1:mn),0)
            li=li+mn
            ili=ili+1
            istk(ili)=istk(ili-1)+mn
 10      continue
         istk(il)=10
         istk(il+1)=m
         istk(il+2)=1
         lstk(top+1)=sadr(li)
      elseif(m.eq.0) then
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
      else
c     .  unspecified number of lines
         li=ili
         i=-1
 12      i=i+1
         call readnextline(lunit,buf,bsiz,mn,nr,info)
         if(info.eq.-1) goto 20
         mn=max(0,mn-1)
         if(mn.gt.0) then
            err=sadr(li+mn+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
            call cvstr(mn,istk(li+1),buf(1:mn),0)
         endif
         istk(li)=mn
         li=li+mn+1
         if(info.eq.-1) then
            if(mn.gt.0) i=i+1
            goto 20
         endif
         goto 12

 20      m=i
         if(m.le.0) then
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         else
            call icopy(li-ili+1,istk(ili),-1,istk(li+2),-1)
            lis=li+2
            istk(il)=10
            istk(il+1)=m
            istk(il+2)=1
            istk(ili)=1
            li=ili+m+1
            do 30 j=1,m
               mn=istk(lis)
               istk(ili+1)=istk(ili)+mn
               call icopy(mn,istk(lis+1),1,istk(li),1)
               lis=lis+mn+1
               li=li+mn
               ili=ili+1
 30         continue
            lstk(top+1)=sadr(li+1)
         endif
      endif

 996  if(.not.opened) call clunit(-lunit,buf,mode)
      return
      end
