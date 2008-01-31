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

      subroutine itosci(x,mx,nx)
C     fortran to scilab
C 
C.. Formal Arguments .. 
c     Copyright INRIA
      integer  x(*)
      integer  mx
      integer  nx
C 
C.. Local Scalars .. 
      integer  il,l
C 
C.. External Calls .. 
      external error, int2db
C 
C.. Include Statements ..
      include "../stack.h"
C 
C.. Statement Functions ..
      integer  iadr
      integer  sadr
      iadr(l) = l + l - 1
      sadr(l) = (l/2) + 1
C 
C ... Executable Statements ...
C 
C     
      m = mx
      n = nx
      if(mx*nx.eq.0) then
         n = 0
         m = 0
      endif

      if (top .ge. bot) then
        call error(18)
      else
        top = top + 1
        il = iadr(lstk(top))
        l = sadr(il+4)
        err = l + m*n - lstk(bot)
        if (err .gt. 0) then
          call error(17)
        else
          istk(il) = 1
          istk(il+1) = m
          istk(il+2) = n
          istk(il+3) = 0
          if(n.ne.0) call int2db(n*m,x,1,stk(l),1)
          lstk(top+1) = l + n*m
        endif
      endif
      end 
