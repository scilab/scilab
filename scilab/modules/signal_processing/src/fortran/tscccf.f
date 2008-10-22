       subroutine tscccf(x,y,n,cxy,xymean,lag,ierr)
c!
c     purposes  :  function tscccf computes lag crosscovariance cxy
c                  n-dimensioned time series x and y, x leading y,
c                  starting at lag 0.
c
c     usage     :  call  tscccf(x,y,n,cxy,xymean,lag)*
c
c     parameters       on entry             on return
c     ----------       --------             ---------
c          x    :      time series x        unchanged
c          y    :      time series y        unchanged
c          n    :      series length        unchanged
c        cxy    :      unusedcrosscovariance function
c        lag    :      number of lags       unchanged
c
c
c     remarks   :  lag must be strictly positive, inferior to n.
c
c     error flag tscccf is set to the following values:
c           ierr =  0 :  no errors.
c           ierr = -1 :  wrong input parameter n or lag.
c
c     routines called  :  dset
c
c!
      double precision x,y,cxy,xymean,eyy,f,exx
      dimension       x(*),y(*),cxy(*),xymean(2)
c
      if(lag.le.0 .or. lag.gt.n .or. n.le.0) then
         ierr=-1
         return
      endif
      call dset(lag,0.0d+0,cxy,1)
      call dset(2,0.0d+0,xymean,1)
      exx=0.0d+0
      eyy=0.0d+0
      do 10 k=1,n
        exx=exx+x(k)
        eyy=eyy+y(k)
 10   continue
      f = 1.0d+0/dble(n)
      exx=exx*f
      eyy=eyy*f
      xymean(1)=exx
      xymean(2)=eyy
      do 1000 m=1,lag
         do 100 j=1,n-m+1
           cxy(m) = cxy(m)+(x(j)-exx)*(y(j+m-1)-eyy)
100      continue
         cxy(m) = cxy(m)*f
1000  continue
      ierr=0
c
      return
      end
