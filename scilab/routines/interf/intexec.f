      subroutine intexec
c     interface of exec function

c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer flag,semi
      integer mode(2),retu(6)
      logical opened
      integer iadr,sadr
c
      save opened,lunit
c     
      data semi/43/
      data retu/27,14,29,30,27,23/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matio '//buf(1:4))
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


      pt=pt+1
c     error control
      ids(2,pt)=errct
      ids(3,pt)=err2
      ids(4,pt)=err1
      ids(5,pt)=errpt
      if(icheck.eq.0) then
         ids(1,pt)=0
      else
         errpt=pt
         ids(1,pt)=1
         imode=1
         imess=1
         num=-1
         errct=(8*imess+imode)*100000+abs(num)
         if(num.lt.0) errct=-errct
      endif

      il=iadr(lstk(top))
      if(istk(il).eq.11.or.istk(il).eq.13) goto 15
c     opening file
      call v2cunit(top,'rb',lunit,opened,ierr)
      if(ierr.gt.0) return
      top=top-1



      pstk(pt)=rio
      rio = lunit
      rstk(pt)=902
      icall=5
      fin=flag
c     *call*  macro
      go to 999
      
 12   continue
      call clunit(-rio,buf,mode)
      rio=pstk(pt)
      top=top+1
      goto 17

c     exec of a function
 15   continue
      fin=lstk(top)
      pstk(pt)=flag
      rstk(pt)=909
      icall=5
c     *call*  macro
      go to 999
 16   lhs=1
 17   if(ids(1,pt).eq.1) then
c     return error number
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=err1
         lstk(top+1)=l+1
         errct=ids(2,pt)
         err2=ids(3,pt)
         err1=ids(4,pt)
         fun=0
      else
         errpt=ids(5,pt)
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
         err1=0
      endif
      pt=pt-1
      goto 999
 999  return
      end
