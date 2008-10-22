c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      integer function setgetmode(id)
c     Determine if stackg may get the variable id by reference (-4) or
c      by value (0)
c     
      include 'stack.h'
c
      integer plus,minus,comma,rparen
      external allowptr
      logical allowptr
      data plus/45/,minus/46/,comma/52/,rparen/42/



      imode=0
      if(sym.eq.plus.or.sym.eq.minus) then
c         imode=-2
         call cvname(id,buf,1)
c         call basout(io,wte,' setgetmode '//buf(1:nlgh)//'+- ')
         goto 33
      endif

      if(rstk(pt).ne.201.or.
     $     rstk(pt-1).ne.101.or.pstk(pt-1).ne.301.or.
     $     rstk(pt-2).ne.307) goto 33
c     pstk(pt-1).ne.301 checks if name is not preceeded by a minus sign
c     name is a first factor of an argument or subscript 
      ip=pt-2

 31   if (sym.ne.comma.and.sym.ne.rparen) goto 33
c     name is followed by a , or a )
      
c     variable is an argument of a function
 32   continue
      ifun=ids(1,ip)
      if (allowptr(ifun).or.ifun.lt.0) then
         if(ddt.ge.4) then
            call cvname(id,buf,1)
            write(buf(nlgh+1:nlgh+16),'(2i8)') ifun,rstk(ip)
            call basout(io,wte,' setgetmode '//buf(1:nlgh)//' '//
     $           buf(nlgh+1:nlgh+16))
         endif
         imode=-4
      endif
 33   setgetmode=imode
      return
      end

