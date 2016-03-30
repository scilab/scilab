c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
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
