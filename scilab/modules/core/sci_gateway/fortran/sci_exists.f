c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


      subroutine intexists(job)
      include 'stack.h'
      integer topk
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval,crebmat
      integer flag
      integer local,nolocal
      integer iadr,sadr
c
      data local/21/
      data nolocal/23/
      data all/10/
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      topk=top

      if(.not.checklhs('exists',1,1)) return
      if(.not.checkrhs('exists',1,2)) return

      flag=0
c
      if(rhs.eq.2) then
         if(.not.getsmat('exists',topk,top,m,n,1,1,il,n1)) return
         if(.not.checkval('exists',m*n,1) ) return
c     on the first char ist tested
         if(istk(il).eq.local)then
            flag=1
         elseif(istk(il).eq.nolocal) then
            flag=2
         elseif(istk(il).eq.all) then
            flag=0
         else
            err=2
            call error(36)
         endif
         top=top-1
      endif


      if(.not.getsmat('exists',topk,top,m1,n1,1,1,il,n)) return
      if(.not.checkval('exists',m1*n1,1) ) return
      call namstr(id,istk(il),n,0)

c     look for  variables in the stack
      if(flag.eq.2) then
c      in the full calling context
c      but not in the local environment
         fin=-7
         call stackg(id)
      elseif(flag.eq.1) then
c      just in local environment
         fin=-3
         call stackg(id)
         ilw=iadr(lstk(top))
         top=top-1
         if(istk(ilw).lt.0) then
            fin=1
         else
            fin=0
         endif
      else
c      in all the stack
         fin=-1
         call stackg(id)
         if (fin.le.0) then
c      look for libraries functions
            fin=-3
            kfun=fun
            call funs(id)
            fun=kfun
         endif
      endif
c
      if(job.eq.0) then
c     exists returns 0 or 1
         if(.not.cremat('exists',top,0,1,1,l,lc)) return
         if (fin.gt.0) then
            stk(l)=1.0d0
         else
            stk(l)=0.0d0
         endif
      else
c     isdef returns %f or %t
         if(.not.crebmat('exists',top,1,1,l)) return
         if (fin.gt.0) then
            istk(l)=1
         else
            istk(l)=0.0d0
         endif
      endif
      fin=1
      return
      end
