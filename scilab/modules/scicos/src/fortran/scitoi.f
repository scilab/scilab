c     Scicos 
c 
c     Copyright (C) INRIA - METALAU Project <scicos@inria.fr> 
c 
c     This program is free software; you can redistribute it and/or modify 
c     it under the terms of the GNU General Public License as published by 
c     the Free Software Foundation; either version 2 of the License, or 
c     (at your option) any later version. 
c 
c     This program is distributed in the hope that it will be useful, 
c     but WITHOUT ANY WARRANTY; without even the implied warranty of 
c     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
c     GNU General Public License for more details. 
c 
c     You should have received a copy of the GNU General Public License 
c     along with this program; if not, write to the Free Software 
c     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
c 
c     See the file ./license.txt 
c 

      subroutine scitoi(x,mx,nx)
C    scilab to fortran

c     Copyright INRIA
      integer  x(*)
      integer  mx
      integer  nx
C.. Local Scalars .. 
      integer  il,l,lx
C.. External Calls .. 
      external entier, error
C.. Include Statements ..
      include "stack.h"
C     
      integer  iadr
      integer  sadr
c
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
C
      if(mx*nx.eq.0) then
         top=top-1
         return
      endif
c
      il = iadr(lstk(top))
      if (istk(il+1)*istk(il+2).eq.0) then
         top=top-1
         return
      endif
      if (istk(il) .ne. 1) then
        call error(98)
      elseif (istk(il+1).ne.mx.or.istk(il+2).ne.nx.or.istk(il+3).ne.0)
     &then
        call error(98)
      else
        lx = sadr(il+4)
        if(nx*mx.gt.0) call entier(nx*mx,stk(lx),x)
        top = top - 1
      endif
      end 
