c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intclearglobal(fname)
c     clear variables in the global area
      include 'stack.h'
c     
      character*(*) fname
      integer top0,id(nsiz),vk
      logical getsmat,checkval,checklhs,eqid
      integer iadr
      integer a, blank,percen
      data a/10/,blank/40/,percen/56/
c
      iadr(l)=l+l-1
c
      if(.not.checklhs(fname,1,1)) return

      if(rhs.le.0) then
c     clear all variable but those created by the startup
c     %modalWarning %toolboxes %toolboxes_dir
         nprotect=3
         nv=gtop-(isiz+1)-nprotect
         if(nv.gt.0) then 
            call iset(nv*nsiz,0,idstk(1,isiz+2+nprotect),1)
            gtop=isiz+1+nprotect
         endif
         goto 100
      endif
     
      top0=top
      do 10 k=1,rhs
         if(.not.getsmat(fname,top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval(fname,m*n,1)) return
         top=top-1
         if(nlr.eq.0) then
c     .     blank string, ignored
            goto 10
         endif

c     .  check if variable name is valid 
         do 05 i=0,nlr-1
            ic=abs(istk(lr+i))
            if((ic.gt.blank.and.(i.gt.0.and.ic.eq.percen)).or.
     $           (i.eq.0.and.ic.lt.a)) then
               err=rhs+1-k
               call error(248)
               return
            endif
 05      continue
         call namstr(id,istk(lr),nlr,0)
c
c     .  look for this variable in the global area
         kg=isiz+1
 06      continue
         kg=kg+1
         if(kg.gt.gtop) then
c     .     no such variable exists, ignored
            goto 10
         endif
         if (.not.eqid(idstk(1,kg),id)) goto 06
c     . destroy this variable and pack storage if necessary
         if (kg.ne.gtop) then
c     .     pack
            ls = lstk(kg+1)
            ll = lstk(kg)
            vk=ls-ll
c     .     translate "values" up
            call unsfdcopy(lstk(gtop+1)-lstk(kg+1),stk(ls),1,stk(ll),1)
            do 09 i = kg, gtop
c     .        translate names up
               call putid(idstk(1,i),idstk(1,i+1))
c     .        translate property up
               infstk(i)=infstk(i+1)
c     .        update pointers 
               lstk(i) = lstk(i+1)-vk
               if (i.lt.gtop) then
c     .           update pointers in variables which refer this global var
                  do 07 j=bot,isiz-1
                     if(infstk(j).eq.2) then
                        if(eqid(idstk(1,j),idstk(1,i))) then
c     .                    variable j refers this global var
                           ilj=iadr(lstk(j))
                           istk(ilj+1)=lstk(i)
                           istk(ilj+2)=i
                        endif
                     endif
 07               continue
               endif
 09         continue
         endif
         gtop=gtop-1
 10   continue
c
c     return a null matrix
 100  top=top+1
      il = iadr(lstk(top))
      istk(il) = 0
      lstk(top+1) = lstk(top) + 1
      end
