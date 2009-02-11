      subroutine delip(n,resr,resi,x,ck)
c!purpose
c  vector valued elliptic integral :
c  r = integral from 0 to x of :
c  dt/sqrt((1-t^2)*(1-ck^2*t^2)) for x real positive
c  x=argument r=resr+%i*resi=result
c  ck = parameter
c  n size of x and r
c  Author F.D.
c!calling sequence
c     subroutine delip(n,resr,resi,x,ck)
c     double precision resr(n),resi(n),x(n),ck
c!
      double precision resr(n),resi(n),x(n)
      double precision ck,r,xx,compk,compk1,yy,m,m1,ck1
      double precision xxxxx
      m=ck*ck
      m1=1.0d+0-m
      ck1=sqrt(m1)
      call compel(ck,compk)
      call compel(ck1,compk1)
      do 1 k=1,n
         xx=x(k)
         if(xx.ge.0.0d+0.and.xx.le.1.0d+0) then
            call deli2(1,r,xx,ck)
            resr(k)=r
            resi(k)=0.0d+0
         elseif (xx.gt.1.0d+0.and.xx.le.(1.0d+0/ck)) then
            xxxxx=1.0d+0/ck-xx
c     .     change of variable : integral from 1 to xx of
c     .     dt/sqrt((1-mt^2)(t^2-1)) = integral from o to yy of
c     .     dt/sqrt((1-t^2)(1-m1t^2))  with m1=1-m and
c     .     yy = (1/sqrt(m1))*sqrt((x^2-1)/x^2)
            yy=(1.0d+0/ck1)*sqrt((xx*xx-1.0d+0)/(xx*xx))
            call deli2(1,r,yy,ck1)
            resi(k)=r
            resr(k)=compk
         elseif(xx.ge.(1.0d+0/ck)) then
            xxxxx=1.0d+0/(ck*xx)
            call deli2(1,r,xxxxx,ck)
            resi(k)=compk1
            resr(k)=r
         endif
 1    continue
      return
      end
