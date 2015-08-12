c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Copyright (C) ENPC
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intexec
c     interface of exec function

      INCLUDE 'stack.h'
c     
      integer flag,semi,typ
      integer mode(2)
      logical opened
      integer iadr,sadr
c
      save opened,lunit
c     
      data semi/43/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         call writebufexec(buf,fin)
         call basout(io,wte,' intexec '//buf(1:4))
      endif
c     
      if(int(rstk(pt)/100).ne.9) goto 01
      if(rstk(pt).eq.902) goto 12
      if(rstk(pt).eq.909) goto 16

 01   lw=lstk(top+1)

      if(rhs.gt.3.or.rhs.lt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     options handling
c---------------------
c     default options values
      flag = 3
      if (sym .eq. semi) flag = 0
      icheck=0
c
      if(rhs.gt.1) then
c     . specified options
         do 11 ir=2,rhs
            ilopt=iadr(lstk(top))
            if(istk(ilopt).lt.0) ilopt=iadr(istk(ilopt+1))
            if(istk(ilopt).eq.1) then
c     .        mode given
               flag = int(stk(sadr(ilopt+4)))
            elseif(istk(ilopt).eq.10) then
c     .        error control
               icheck=1
            else
               call error(44)
               return
            endif
            top=top-1
 11      continue
      endif
      if(flag.ge.4) then
         call basout(io,wte,
     $        'step-by-step mode: enter carriage return to proceed')
      endif
      il=iadr(lstk(top))
      typ=abs(istk(il))
      if(typ.eq.1.or.typ.eq.10) then
c     .  exec of a file , opening file
         call v2cunit(top,'rb',lunit,opened,ierr)
         if(ierr.gt.0)  return
         top=top-1
         typ=1
      elseif(typ.eq.11.or.typ.eq.13) then
c     .  exec of a function
         typ=0
      else
         err=1
         call error(44)
         return
      endif
c
      pt=pt+1
c     preserve current error recovery modes
      ids(2,pt)=errct
      ids(3,pt)=err2
      ids(4,pt)=err1
      ids(5,pt)=errpt
c     set error recovery modes
      if(icheck.eq.0) then
         ids(1,pt)=0
      else
c         ids(1,pt)=1+toperr
c         toperr=top
         ids(1,pt)=1+top
         errpt=pt
         imode=1
         imess=1
         num=-1
         errct=(8*imess+imode)*100000+abs(num)
         if(num.lt.0) errct=-errct
      endif

      if(typ.eq.0) goto 15

c     exec of a file
c     ---------------
      pstk(pt)=rio
      rio = lunit
      rstk(pt)=902

      ids(6,pt)=0
      if(opened) ids(6,pt)=1
      icall=5
      fin=flag
c     *call*  macro
      go to 999
 12   continue
      opened=ids(6,pt).eq.1
      if(.not.opened) call clunit(-rio,buf,mode)
      rio=pstk(pt)
      top=top+1
      lhs=1
      if(ids(1,pt).gt.0) then
c     return error number
         top=ids(1,pt)
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=max(err1,err2)
         lstk(top+1)=l+1
         fun=0
         err2=ids(3,pt)
         err1=ids(4,pt)
      else
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
         err2=max(ids(3,pt),err2)
         err1=max(ids(4,pt),err1)
      endif
c     restore error recovery modes
      errct=ids(2,pt)
      errpt=ids(5,pt)
      pt=pt-1
      goto 999


c     exec of a function
c     ------------------
 15   continue
      fin=lstk(top)
      pstk(pt)=flag

      rstk(pt)=909
      icall=5
c     *call*  macro
      go to 999
 16   lhs=1
      if(ids(1,pt).gt.0) then
c     return error number
         top=ids(1,pt)-1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=max(err1,err2)
         lstk(top+1)=l+1
         fun=0
         err2=ids(3,pt)
         err1=ids(4,pt)
      else
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
         err2=max(ids(3,pt),err2)
         err1=max(ids(4,pt),err1)
      endif
c     restore error recovery modes
      errct=ids(2,pt)
      errpt=ids(5,pt)
      pt=pt-1
      goto 999
c
 999  return
      end
c     ====================================        
c     required by f2c :(
c     ====================================        
      subroutine writebufexec(buffer,line)
      
      character*(*) buffer
      integer  line
      write(buffer(1:4),'(i4)') line
      
      end
c     ====================================        