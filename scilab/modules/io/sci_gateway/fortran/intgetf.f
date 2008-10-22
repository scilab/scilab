c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intgetf
c     
      INCLUDE 'stack.h'
c     
      integer mode(2)
      integer nocomp,profile
      logical opened,eptover,cremat
      integer iadr,sadr
c
      data nocomp/23/,profile/25/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (rstk(pt).eq.904) goto 57
c
      if(rhs.gt.2) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif
c     opening file
      top = top-rhs+1
      il=iadr(lstk(top))
      call v2cunit(top,'rb',lunit,opened,ierr)
c     exec works with C and Fortran opened files
      if(ierr.gt.0) return

c     
      if(rhs.gt.1) then
         ilc=iadr(lstk(top+1))
         icomp=1
         if(istk(ilc).eq.10) then
            if(istk(ilc+5+istk(ilc+1)*istk(ilc+2)).eq.nocomp) then
               icomp=0
            else if(istk(ilc+5+istk(ilc+1)*istk(ilc+2)).eq.profile) then
               icomp=2
            endif
         endif
      else
         icomp=1
      endif
c     loop on all function defined in the file
      nline=0
 55   call getfun(lunit,nline,'getf')
      fun=0
      job=fin
      if(err.gt.0.or.err1.gt.0) then
         if(.not.opened) then
            call clunit(-lunit,buf,mode)
         endif
         return
      endif
      if(job.lt.0) then
c     .  no more function to get
         goto 59
      endif
      if(icomp.eq.0) then
c     .  getf without compilation, just store the variable
         call stackp(idstk(1,top),0)
         if(job.eq.0) goto 55
         goto 59
      endif
      
      if ( eptover(1,psiz)) return
      rstk(pt)=904
      pstk(pt)=job
      ids(1,pt)=lunit
      if(opened) then
         ids(2,pt)=1
      else
         ids(2,pt)=0
      endif
      ids(3,pt)=icomp
      ids(4,pt)=nline
      if(icomp.eq.2) then
         top=top+1
         if (.not.cremat('getf',top,0,1,1,lr1,lc1)) return
         stk(lr1)=2
         rhs=2
      else
         rhs=1
      endif
      lhs=1
      fun=13
      fin=06
c     *call* comp
      return
 57   continue
      job=pstk(pt)
      lunit=ids(1,pt)
      opened=ids(2,pt).eq.1
      icomp=ids(3,pt)
      nline=ids(4,pt)
c
      pt=pt-1
      if (err1.eq.0) top=top-1
      if(job.eq.0) goto 55

c     
 59   call objvide('getf',top)
      comp(1)=0
      fun=0
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999
 999  return
      end
