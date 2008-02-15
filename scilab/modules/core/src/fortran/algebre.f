c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      integer function basetype(l)
      integer l
      basetype=mod(l,100)
      return
      end

      integer function algtyp2(m1,m2)
      integer m1,m2
      if (m1.eq.0) then
         algtyp2=m2
      else
         algtyp2=900+100*m1+m2
      endif
      return
      end

c      integer function algcode(l)
c      integer l
c      integer typecode
c      if (l.le.999) then
c         algcode=0
c      else
c         algcode=(l-900-typecode(l))/100
c      endif
c      return
c      end

      double precision function addid(l)
      integer l
      if (l.eq.1) then
         addid=-(10000.3**10000.3)
      elseif (l.eq.2) then
         addid=(10000.3**10000.3)
      else 
         addid=0.0d0
      endif
      return
      end

      double precision function peak(l)
      integer l
         if (l.eq.2) then
            peak=-(10000.3**10000.3)
         else
            peak=(10000.3**10000.3)
         endif
      return
      end

