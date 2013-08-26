c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c

      subroutine shanph(diag,n,nt,np,y,s,ys,scal,index,io,imp)
c     mise a l echelle de diag par la methode de shanno-phua
c     calcul du facteur d echelle scal
c      diag=(y,(diag-1)y)/(y,s)*diag
c
      implicit double precision (a-h,o-z)
      dimension diag(n),y(nt,n),s(nt,n),ys(nt),index(nt)
      character bufstr*(4096)
      
      inp=index(np)
      cof=0.
      do 203 i=1,n
203   cof=cof + y(inp,i)**2/diag(i)
      cof=cof/ys(inp)
      if(imp.gt.3) then
        write(bufstr,1203) cof
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
        endif
1203  format(' gcbd. facteur d echelle=',d15.7)
      do 205 i=1,n
205   diag(i)=cof*diag(i)
      scal=0.
      do 206 i=1,n
206   scal=scal + diag(i)
      scal=n/scal
      return
      end
