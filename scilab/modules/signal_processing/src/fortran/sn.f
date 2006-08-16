C/MEMBR ADD NAME=SN,SSI=0
      double precision function sn(y,a,ak1,ak3)
c!
      implicit double precision (a-h,o-z)
c
      pi=3.1415926535
      ns=sqrt(50.0*ak1/(pi*ak3))+2
      x=y*0.50d+0/ak1
      q=exp(-pi*ak3/ak1)
      sup=2.0d+0*(q**(1.0d+0/4.0d+0))*sin(pi*x)
      den=1.0d+0
      i=-2
      n=1
 1    sup=sup+i*(q**((n+0.5)**2))*sin((2*n+1)*pi*x)
      den=den+i*(q**(n**2))*cos(2.*n*pi*x)
      i=-i
      n=n+1
      if(n.le.ns) goto 1
      sn=sup/(den*sqrt(a))
      return
      end
