      subroutine ivimp(i1,i2,pas,iv)
c     Copyright INRIA
c     generates the vector iv corresponding to scilab syntax iv=i1:pas:i2
      integer i1,i2,pas,iv(*)
      k=0
      do 10 i=i1,i2,pas
         k=k+1
         iv(k)=i
 10   continue
      end
      subroutine dmext(a,na,n,rowsel,nrs,colsel,ncs,x)
c     Copyright INRIA
c     generates the x matrix corresponding to scilab syntax
c     x=a(rowsel,colsel) for a double precision matrix
      double precision a(na,n),x(nrs,ncs)
      integer rowsel(nrs)
      integer colsel(ncs)
c
      do 10 i=1,nrs
         do 05 j=1,ncs
            x(i,j)=a(rowsel(i),colsel(j))
 05      continue
 10   continue
      end
      subroutine dmins(a,na,n,rowsel,nrs,colsel,ncs,x,nx)
c     Copyright INRIA
c     generates the x matrix corresponding to scilab syntax
c     a(rowsel,colsel)=x for a double precision matrix
      double precision a(na,n),x(nx,*)
      integer rowsel(nrs)
      integer colsel(ncs)
c
      do 10 i=1,nrs
         do 05 j=1,ncs
            if(rowsel(i).gt.na.or.colsel(j).gt.n) then
               a(rowsel(i),colsel(j))=0.0d0
            else
               a(rowsel(i),colsel(j))=x(i,j)
            endif
 05      continue
 10   continue
      end
