C/MEMBR ADD NAME=BELAN,SSI=0
      subroutine belan(ak1,ak2,ak3,ak4,eps,a,wc,wr,
     1                 zerr,zeri,polr,poli)
c!
      implicit double precision(a-h,o-z)
      dimension polr(20),poli(20),zerr(20),zeri(20)
      pi=3.1415926535
      n=(ak4*ak1)/(ak2*ak3)
      n=(n/2)+1
      ndeu=2*n
      u0=(-ak3/ak4)*log((1+sqrt(1.0d+0+eps**2))/eps)
      do 1 i=1,n
      xmag=(2*i-1)*ak1/ndeu
      zerr(i)=-ak3
      zeri(i)=xmag
      polr(i)=u0
      poli(i)=xmag
  1   continue
      dk=wc/wr
      dkp=sqrt(1.0d+0-dk*dk)
      do 4 i=1,ndeu
      if(i.le.n) goto 5
      q=polr(i-n)
      r=poli(i-n)
      goto 6
 5    q=zerr(i)
      r=zeri(i)
      sigma=0.0d+0
  6   a1=sn(q,dkp,ak3,ak1)
      b1=sn(r,dk,ak1,ak3)
      dn=sqrt(1-(dk*b1)**2)
      de=1.0d+0-(a1*dn)**2
      if(i.le.n) goto 8
      sigma=a1*sqrt((1-a1**2)*(1-b1**2))*dn/de
  8   omega=b1*sqrt(1-(dkp*a1)**2)/de
      if(i.le.n) goto 7
      polr(i-n)=sigma*wc
      poli(i-n)=omega*wc
      goto 4
 7    zerr(i)=sigma*wc
      zeri(i)=omega*wc
  4   continue
      return
      end
