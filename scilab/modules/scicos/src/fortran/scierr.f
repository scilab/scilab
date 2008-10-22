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

      integer function scierr()
c     returns 1 if  scilab error indicator err is positive and 0 in the
c     other cases
c     

c     Copyright INRIA
      include "stack.h"
      if(err.gt.0) then
         scierr=1
      elseif(err1.gt.0.or.err2.gt.0) then
         scierr=2
      else
         scierr=0
      endif
      return
      end
