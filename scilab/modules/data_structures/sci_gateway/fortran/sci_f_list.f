c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine scilist(fname)

      character*(*) fname
      include 'stack.h'
      integer ltyp
      integer iadr,sadr

c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

 01   lf=lstk(top+1)
      top=top+1-rhs
      ld=lstk(top)
c
      n=rhs
      err=lf+sadr(n+3)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if(fin.eq.2) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=16
      elseif(fin.eq.7) then
c     typed list
c     check if first element is a string
         if (istk(iadr(ld)).ne.10) then
            err=1
            call error(55)
            return
         endif
         ltyp=17
      else
c     c untyped list
         ltyp=15
      endif

      il=iadr(ld)
      l=sadr(il+n+3)
      call unsfdcopy(lf-ld,stk(ld),-1,stk(l),-1)

      istk(il)=ltyp
      istk(il+1)=n
      istk(il+2)=1
      if(n.ne.0) then
         do 10 i=1,n
            istk(il+2+i)=istk(il+1+i)+lstk(top+i)-lstk(top-1+i)
 10      continue
      endif
      lstk(top+1)=l+lf-ld
      goto 99

 99   return
      end
