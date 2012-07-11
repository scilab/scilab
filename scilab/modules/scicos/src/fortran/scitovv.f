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
c     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
c 
c     See the file ./license.txt 
c 

      subroutine scitovv(x,nx)
c     scilab data structure to fortran fortran var2vec  to scilab
c     x is the var2vec fortran image of the variable stored at the top
c     of the stack 
c 
c     Copyright INRIA
      double precision x(*)
      integer  l
c 
      external scidcopy, error
      include "stack.h"
c 
      integer  iadr
c
      iadr(l) = l + l - 1
c
c     scilab variable to transfer
      l=lstk(top)
      n=lstk(top+1)-lstk(top)
      il=iadr(l)
      if(istk(il).eq.1.and.istk(il+1).eq.0.or.istk(il+2).eq.0) then
         top=top-1
         return
      endif
      if((n.eq.nx).or.(nx.eq.0)) then
         call scidcopy(nx,stk(l),1,x,1)
         top=top-1
      else
         call error(98)
      endif
      end 
