c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine v2cunit(k,mode,fd,opened,ierr)
c     given variable #k (scalar or string) and mode 
c     v2unit return a  logical unit attached to corresponding file

      INCLUDE 'stack.h'
c
      logical opened
      integer fd
      character*2 mode
      double precision w
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
  
c
      ierr=0
      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if (istk(il).eq.1) then
         fd = int(stk(sadr(il+4)))
         if (istk(il+1)*istk(il+2).ne.1.or.istk(il+3).ne.0.or.
     $        fd .lt. 0) then
            err=1
            ierr=1
            call error(36)
            return
         endif
         call getfileinfo(fd,ifa,iswap,ltype,mod,buf,lb,info)
         if(info.eq.1) then
            err=1
            ierr=1
            call error(36)
         elseif(info.eq.2) then
            err=fd
            ierr=2
            call error(245)
            return
         elseif(info.eq.0.and.ltype.eq.1) then
c     ierr=-1 line used for compatibility instead of error
            ierr=-1
            opened=.true.
c            call error(244)
            return
         endif
         opened=.true.
      elseif(istk(il).eq.10) then
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            ierr=1
            call error(36)
            return
         endif
         mn=istk(il+5)-1
         buf=' '
         call cvstr(mn,istk(il+5+istk(il+1)*istk(il+2)),buf,1)
         
         call cluni0(buf(1:mn), buf(mn+2:),mn1)
         lunit = 0
         buf(mn+mn1+2:mn+mn1+2)=char(0)
         l=lnblnk(mode)
         call mopen(fd,buf(mn+2:),mode(1:l)//char(0),1,w,ierr)

         if(ierr.gt.0) then
            if (ierr.eq.4) then
c INVALID_FILENAME            
               err = 241 
            elseif(ierr.eq.2) then
               if(mode(1:1).eq.'r') then
                  err=241
               else
                  err=240
               endif
            elseif(ierr.eq.1) then
               err=66
            endif
            
            buf(mn+1:)=' '
            call error(err)
            return
         endif
         
         opened=.false.
      else
         err=1
         ierr=1
         call error(36)
         return
      endif
      end
