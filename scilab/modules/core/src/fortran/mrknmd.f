c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine mrknmd()
      INCLUDE 'stack.h'
      logical compil,c
      character*(6) str
c      integer iadr,sadr

      if(ddt.eq.4) then
         call cvname(ids(1,pt),buf,1)
         write(str,'(i6)') top
         call basout(io,wte,'mrknmd top = '//str//
     $        ' name = '//buf(1:24))
      endif

      if(lhs.le.0) return
      if(comp(1).eq.0) then
         infstk(top)=1
         call putid(idstk(1,top),ids(1,pt))
         pt=pt-1
      else
         c=compil(18,ids(1,pt),0,0,0)
         pt=pt-1
      endif
      return
      end
