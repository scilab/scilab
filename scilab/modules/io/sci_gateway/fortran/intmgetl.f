c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intmgetl

      INCLUDE 'stack.h'
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
      if(.not.checkrhs('mgetl',1,2)) return
      if(.not.checklhs('mgetl',1,1)) return

c     opening file
      call v2cunit(top-rhs+1,'rb',lunit,opened,ierr)
      if(ierr.lt.0) then
         call error(244)
         return
      elseif(ierr.gt.0) then
         return
      endif
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
         mr=m
         do i=1,m
            mnt=0
 09         call readnextline(lunit,buf,bsiz,mn,nr,info)
            if(info.eq.-1) then
               mr=i-1
               goto 10
c               err=i
c               call error(62)
c               if(.not.opened) call clunit(-lunit,buf,mode)
c               return
            endif

            mn=max(0,mn-1)
            err=sadr(li+mn)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call cvstr(mn,istk(li),buf(1:mn),0)
            li=li+mn
            mnt=mnt+mn
            if(info.eq.2) then
c     .        buffer too small for this line
               goto 09
            endif
            ili=ili+1
            istk(ili)=istk(ili-1)+mnt
         enddo
 10      continue
         if(mr.eq.0) then
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         else
            istk(il)=10
            istk(il+1)=mr
            istk(il+2)=1
            if(mr.lt.m) then
               nc=istk(il+4+mr)-1
               call icopy(nc,istk(il+4+m+1),1,istk(il+4+mr+1),1)
               li=il+4+mr+nc+1 
c              +1 added ... see bug 2610 (Thx Bruno Pincon)
            endif
            lstk(top+1)=sadr(li)
         endif
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

         mnt=0
         lic=li+1
 13      call readnextline(lunit,buf,bsiz,mn,nr,info)
         if(info.eq.-1) goto 20

         mn=max(0,mn-1)
         if(mn.gt.0) then
            err=sadr(lic+mn+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
            call cvstr(mn,istk(lic),buf(1:mn),0)
            lic=lic+mn
         endif
         mnt=mnt+mn
         if(info.eq.2) then
c     .     buffer too small for this line
            goto 13
         endif
         istk(li)=mnt
         li=li+mnt+1
         
         if(info.eq.-1) then
            if(mnt.gt.0) i=i+1
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
            err=sadr(li+2+li-ili+1)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               goto 996
            endif
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
