      subroutine shanph(diag,n,nt,np,y,s,ys,scal,index,io,imp)
c     mise a l echelle de diag par la methode de shanno-phua
c     calcul du facteur d echelle scal
c      diag=(y,(diag-1)y)/(y,s)*diag
c
      implicit double precision (a-h,o-z)
      dimension diag(n),y(nt,n),s(nt,n),ys(nt),index(nt)
      inp=index(np)
      cof=0.
      do 203 i=1,n
203   cof=cof + y(inp,i)**2/diag(i)
      cof=cof/ys(inp)
      if(imp.gt.3)write(io,1203)cof
1203  format(' gcbd. facteur d echelle=',d15.7)
      do 205 i=1,n
205   diag(i)=cof*diag(i)
      scal=0.
      do 206 i=1,n
206   scal=scal + diag(i)
      scal=n/scal
      return
      end
