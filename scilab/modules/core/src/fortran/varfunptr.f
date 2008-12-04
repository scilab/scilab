c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine varfunptr(id,ffun,ffin)
c     ---------------------------------
c     creates a variable with is a pointer on a primitive function 
C     ---------------------------------
      include 'stack.h'

      logical compil
      integer bl(nsiz),id(*),ffun,ffin,iadr,sadr
      data bl/nsiz*673720360/
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(err1.gt.0) return
c     compilation  [27]
      if(compil(27,id,ffun,ffin,0)) return 
      if(top+2 .ge. bot) then
         call error(18)
         return
      endif
      top = top+1
      call putid(idstk(1,top),bl)
      infstk(top)=0
      il=iadr(lstk(top))
      istk(il)=130
      istk(il+1)=ffun
      istk(il+2)=ffin
      call putid(istk(il+3),id)
      lstk(top+1)=sadr(il+3+nsiz)
      return
      end

      subroutine isafunptr(k,id,ffun,ffin)
      include 'stack.h'
      integer iadr
      integer id(*),ffun,ffin

      iadr(l)=l+l-1

      ffun=0
      ffin=0
      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).eq.130) then
         ffun=istk(il+1)
         ffin=istk(il+2)
         call putid(id,istk(il+3))
      endif
      return
      end
