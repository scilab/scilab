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

      subroutine vvtosci(x,nx)
c     fortran var2vec  to scilab
c     x is supposed to be a fortran image of var2vec result
c
c     Copyright INRIA
      double precision x(*)
      integer  l
c
      external scidcopy, error
      include "stack.h"
c
      integer iadr
c
      iadr(l) = l + l - 1
c
      if (top .ge. bot) then
        call error(18)
      else
        top = top + 1
        l=lstk(top)

       if (nx.eq.0) then
        err = l + 4 - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
          il = iadr(lstk(top))
          istk(il) = 1
          istk(il+1) = 0
          istk(il+2) = 0
          istk(il+3) = 0
          lstk(top+1) = l + 4
       else
        err = l + nx - lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
        call scidcopy(nx,x,1,stk(l),1)
        lstk(top+1) = l + nx
       endif
      endif
      end
