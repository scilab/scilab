c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 2014 - Scilab Enterprises - Paul Bignier: bugs #11001 #13136 fixed
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


      subroutine intexists(job)
      include 'stack.h'
      integer topk
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval,crebmat
      integer flag
      integer local,nolocal
      integer iadr, str
c
      data local/21/
      data nolocal/23/
      data all/10/
c
      iadr(l)=l+l-1
c
      topk=top

      if(.not.checklhs('exists',1,1)) return
      if(.not.checkrhs('exists',1,2)) return

      flag=0
c
      if(rhs.eq.2) then
         if(.not.getsmat('exists',topk,top,m,n,1,1,il,n1)) return
         if(.not.checkval('exists',m*n,1) ) return
c     only the first char is tested
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

      if(job.eq.0) then
c        exists
         if(.not.cremat('exists',top,0,m1,n1,l,lc)) return
      else
c        isdef
          if(.not.crebmat('exists',top,m1,n1,l)) return
      endif

c     Initializing variables to prevent overwrite
      nacc=0
      nextLength=istk(iadr(lstk(top))+5)
      nextnextLength=istk(iadr(lstk(top))+6)
      do 10, i=1, m1*n1

c        Start by reading the size of the input string
c        The size of the first element was calculated by getsmat()
         if(i.gt.1) then
            n=nextLength-curLength
         endif
         call namstr(id,istk(il+nacc),n,0)
         nacc=nacc+n !nacc allows reading the strings one by one in the 'il' array
c        look for variables in the stack
         if(flag.eq.2) then
c        in the full calling context
c        but not in the local environment
            fin=-7
            call stackg(id)
         elseif(flag.eq.1) then
c        just in local environment
            fin=-3
            str=istk(il+m1*n1-1) !because stackg(id) provokes istk(il+m1*n1-1)=-1
            call stackg(id)
            ilw=iadr(lstk(top))
            top=top-1
            if(istk(ilw).lt.0) then
               fin=1
            else
               fin=0
            endif
            istk(il+m1*n1-1)= str !to retrieve istk(il+m1*n1-1)
         else
c        in all the stack
            fin=-1
            call stackg(id)
            if (fin.le.0) then
c           look for libraries functions
               fin=-3
               kfun=fun
               call funs(id)
               fun=kfun
            endif
         endif
c
         if(fin.le.0) then
c        variable not found, look for primitives
            call funtab(id, fin, 1, 'NULL_NAME', 0)
         endif
c
c        To prevent the input from being progressively overwritten by the return,
c        save the next useful information (string lengths)
         curLength=nextLength
         nextLength=nextnextLength
         nextnextLength=istk(iadr(lstk(top))+4+i+2)
         if(job.eq.0) then
c        exists returns 0 or 1
            if (fin.gt.0) then
               stk(l+i-1)=1.0d0
            else
               stk(l+i-1)=0.0d0
            endif
         else
c        isdef returns %f or %t
            if (fin.gt.0) then
               istk(l+i-1)=1
            else
               istk(l+i-1)=0.0d0
            endif
         endif

 10   continue

      fin=1
      return
      end
