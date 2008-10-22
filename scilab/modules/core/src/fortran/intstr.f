c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine intstr(n,istk,ni,job)
c!but
c     converti la representation decimale d'un entier en une chaine 
c     de caractere scilab
c!
      parameter (nc=20)
      character*(nc) buf
      integer istk(*)
c
      write(buf,'(i20)') n
      i=0
 10   i=i+1
      if(buf(i:i).eq.' ') goto 10
      ni=nc+1-i
      call cvstr(ni,istk,buf(i:nc),0)
      end

