c      
c     SUBROUTINE wheelg
c      
c     Copyright ENPC
      subroutine wheelg(n,k,uf,vf,wf,xx)
        implicit double precision (t)
        doubleprecision uf(n,k),vf(n,k),wf(n,k)
        doubleprecision uu,vv,ww,r
        integer n,k,i1,i2
        doubleprecision xx(8,k)
        data r / 1.0/
c         
        do 1000, i1 =1,n ,1
c           
          do 1001, i2 =1,k ,1
            cs1 = cos(xx(1,i2))
            cs2 = cos(xx(2,i2))
            si1 = sin(xx(1,i2))
            si2 = sin(xx(2,i2))
            uu = xx(7,i2)+r*(cs2*cs1*uf(i1,i2)-si1*vf(i1,i2)+si2*cs1*wf(
     +i1,i2))
            vv = xx(8,i2)+r*(cs2*si1*uf(i1,i2)+cs1*vf(i1,i2)+si2*si1*wf(
     +i1,i2))
            ww = r*si2+r*(-si2*uf(i1,i2)+cs2*wf(i1,i2))
            uf(i1,i2) = uu
            vf(i1,i2) = vv
            wf(i1,i2) = ww
 1001     continue
c         
 1000   continue
c       
        return
      end
