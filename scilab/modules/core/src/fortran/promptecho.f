c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine promptecho(lunit,string,strl)
      include 'stack.h'
      character*(*) string
      character*(bsiz) temp
      character*10 prpt
      integer l,strl,lnblnk
      external lnblnk
      prpt=' '
      if(paus.eq.0) then
         prpt='-->'
      elseif(paus.lt.10) then
         write(prpt,'(''-'',i1,''->'')') paus
      elseif(paus.lt.100) then
         write(prpt,'(''-'',i2,''->'')') paus
      elseif(paus.lt.1000) then
         write(prpt,'(''-'',i3,''->'')') paus
      else
         prpt='-*->'
      endif
      l=lnblnk(prpt)
      temp = prpt(1:l)//string(1:strl)
      call basout(io,lunit,temp(1:l+strl))
      return 
      end

