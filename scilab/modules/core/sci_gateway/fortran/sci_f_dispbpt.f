c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intdispbpt
      include 'stack.h'
      logical checkrhs,checklhs
      character tmpbuf * (bsiz)
c
      rhs=max(rhs,0)
      if(.not.checklhs('dispbpt',1,1)) return
      if(.not.checkrhs('dispbpt',0,0)) return

      if(nmacs.gt.0) then
         do 375 kk=1,nmacs
            call cvname(macnms(1,kk),buf(1:nlgh),1)
            call msgs(27,0)
            do 373 kl=lgptrs(kk),lgptrs(kk+1)-1
               write(tmpbuf(1:10),'(5x,i5)') bptlg(kl)
               call basout(io,wte,tmpbuf(1:10))
 373        continue
 375     continue
      endif
      top=top+1
      call objvide('dispbpt',top)
      return
      end
      
