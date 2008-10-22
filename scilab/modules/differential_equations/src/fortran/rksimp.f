      subroutine rksimp(fydot2,neqn,y,t,tout,itol,rerr,aerr,
     1    itask,iflag,iopt,work,lrw,iwork,liw,bjac,mf)
c
c     fehlberg fourth-fifth order runge-kutta method
c
c
      integer neqn,iflag,iwork(*)
      double precision y(*),t,tout,rerr,aerr,work(*),h
c
      double precision ae,scale,eeoet,et,esttol,ee
      common/ierode/iero
      external fydot2
c
      integer k1,k2,k3,k4,k5,k6
      iero=0
c
c     compute indices for the splitting of the work array
c
      scale=2.0d0/rerr
      ae=scale*aerr
      k1=1+neqn
      k2=k1+neqn
      k3=k2+neqn
      k4=k3+neqn
      k5=k4+neqn
      k6=k5+neqn
c
c     this interfacing routine merely relieves the user of a long
c     calling list via the splitting apart of two working storage
c     arrays. if this is not compatible with the users compiler,
c     he must use rkfs directly.
c
      h=tout-t
      do 33 k=1,neqn
      work(k6+k-1)=y(k)
 33   continue
      call fehl2(fydot2,neqn,y,t,h,work(1),
     1          work(k1),work(k2),work(k3),work(k4),work(k5),
     2          work(k6))
c
      eeoet=0.0d0
      do 250 k=1,neqn
        et=dabs(work(k6+k-1))+dabs(work(k1-1+k))+ae
        if (et .gt. 0.0d0) go to 240
c
c       inappropriate error tolerance
        iflag=4
        return
c
 240    continue
        ee=dabs((-2090.0d0*work(k)+(21970.0d0*
     1      work(k3-1+k)-15048.0d0*work(k4-1+k)))+
     2    (22528.0d0*work(k2-1+k)-27360.0d0*work(k5-1+k)))
  250   eeoet=dmax1(eeoet,ee/et)
c
      esttol=dabs(h)*eeoet*scale/752400.0d0
c
      if (esttol .le. 1.0d0) then
c        OK
      iflag=2
      t=tout
c      write(6,*) esttol,scale,eeoet,ee,et
      return
      endif
      iflag=3
c
      return
      end

      subroutine fehl2(fydot2,neqn,y,t,h,yp,f1,f2,f3,f4,f5,s)
c
c     fehlberg fourth-fifth order runge-kutta method
c
c
      integer  neqn
      double precision  y(*),t,h,yp(neqn),f1(neqn),f2(neqn),
     1  f3(neqn),f4(neqn),f5(neqn),s(neqn)
c
      double precision  ch
      integer  k
      external fydot2
      common/ierode/iero
c
c      write(6,*) 'inputfelh2:',y(1),y(2)
      call fydot2(neqn,t,y,yp)
      if(iero.gt.0) return
      ch=h/4.0d0
      do 221 k=1,neqn
  221   y(k)=y(k)+ch*yp(k)
      call fydot2(neqn,t+ch,y,f1)
      if(iero.gt.0) return
c
      ch=3.0d0*h/32.0d0
      do 222 k=1,neqn
  222   y(k)=s(k)+ch*(yp(k)+3.0d0*f1(k))
      call fydot2(neqn,t+3.0d0*h/8.0d0,y,f2)
      if(iero.gt.0) return
c
      ch=h/2197.0d0
      do 223 k=1,neqn
  223   y(k)=s(k)+ch*(1932.0d0*yp(k)+(7296.0d0*f2(k)-7200.0d0*f1(k)))
      call fydot2(neqn,t+12.0d0*h/13.0d0,y,f3)
      if(iero.gt.0) return
c
      ch=h/4104.0d0
      do 224 k=1,neqn
  224   y(k)=s(k)+ch*((8341.0d0*yp(k)-845.0d0*f3(k))+
     1                            (29440.0d0*f2(k)-32832.0d0*f1(k)))
      call fydot2(neqn,t+h,y,f4)
      if(iero.gt.0) return
c
      ch=h/20520.0d0
      do 225 k=1,neqn
  225   y(k)=s(k)+ch*((-6080.0d0*yp(k)+(9295.0d0*f3(k)-
     1         5643.0d0*f4(k)))+(41040.0d0*f1(k)-28352.0d0*f2(k)))
      call fydot2(neqn,t+h/2.0d0,y,f5)
      if(iero.gt.0) return
c
c     compute approximate solution at t+h
c
      ch=h/7618050.0d0
      do 230 k=1,neqn
       y(k)=s(k)+ch*((902880.0d0*yp(k)+(3855735.0d0*f3(k)-
     1        1371249.0d0*f4(k)))+(3953664.0d0*f2(k)+
     2        277020.0d0*f5(k)))
c
 230  continue
c      write(6,*) 'endfelh2:',y(1),y(2)
      return
      end
