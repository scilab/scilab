      subroutine r8tx(nxtlt, nthpo, lengt, cr0, cr1, cr2, cr3, cr4,
     &    cr5, cr6, cr7, ci0, ci1, ci2, ci3, ci4, ci5, ci6, ci7)
c!
c subroutine:  r8tx
c sous-programme d'iteration en base 8
c!
      double precision cr0(*), cr1(*), cr2(*), cr3(*), cr4(*),
     &    cr5(*), cr6(*), cr7(*), ci1(*), ci2(*), ci3(*), ci4(*),
     &    ci5(*), ci6(*), ci7(*), ci0(*)
      double precision arg,c1,s1,c2,s2,c3,s3,c4,s4,c5,s5,c6,s6,c7,s7,
     &    ar0,ar1,ar2,ar3,ar4,ar5,ar6,ar7,ai0,ai1,ai2,ai3,ai4,
     &    ai5,ai6,ai7,br0,br1,br2,br3,br4,br5,br6,br7,bi0,bi1,
     &    bi2,bi3,bi4,bi5,bi6,bi7,tr,ti,scale,pi2,p7
c
      pi2=8.0d+0*atan(1.0d+0)
      p7=1.0d+0/sqrt(2.0d+0)
      scale=pi2/dble(lengt)
      do 30 j=1,nxtlt
      arg=dble(j-1)*scale
      c1=cos(arg)
      s1=sin(arg)
      c2=c1**2-s1**2
      s2=c1*s1+c1*s1
      c3=c1*c2-s1*s2
      s3=c2*s1+s2*c1
      c4=c2**2-s2**2
      s4=c2*s2+c2*s2
      c5=c2*c3-s2*s3
      s5=c3*s2+s3*c2
      c6=c3**2-s3**2
      s6=c3*s3+c3*s3
      c7=c3*c4-s3*s4
      s7=c4*s3+s4*c3
      do 20 k=j,nthpo,lengt
      ar0=cr0(k)+cr4(k)
      ar1=cr1(k)+cr5(k)
      ar2=cr2(k)+cr6(k)
      ar3=cr3(k)+cr7(k)
      ar4=cr0(k)-cr4(k)
      ar5=cr1(k)-cr5(k)
      ar6=cr2(k)-cr6(k)
      ar7=cr3(k)-cr7(k)
      ai0=ci0(k)+ci4(k)
      ai1=ci1(k)+ci5(k)
      ai2=ci2(k)+ci6(k)
      ai3=ci3(k)+ci7(k)
      ai4=ci0(k)-ci4(k)
      ai5=ci1(k)-ci5(k)
      ai6=ci2(k)-ci6(k)
      ai7=ci3(k)-ci7(k)
      br0=ar0+ar2
      br1=ar1+ar3
      br2=ar0-ar2
      br3=ar1-ar3
      br4=ar4-ai6
      br5=ar5-ai7
      br6=ar4+ai6
      br7=ar5+ai7
      bi0=ai0+ai2
      bi1=ai1+ai3
      bi2=ai0-ai2
      bi3=ai1-ai3
      bi4=ai4+ar6
      bi5=ai5+ar7
      bi6=ai4-ar6
      bi7=ai5-ar7
      cr0(k)=br0+br1
      ci0(k)=bi0+bi1
      if(j.le.1)goto 10
      cr1(k)=c4*(br0-br1)-s4*(bi0-bi1)
      ci1(k)=c4*(bi0-bi1)+s4*(br0-br1)
      cr2(k)=c2*(br2-bi3)-s2*(bi2+br3)
      ci2(k)=c2*(bi2+br3)+s2*(br2-bi3)
      cr3(k)=c6*(br2+bi3)-s6*(bi2-br3)
      ci3(k)=c6*(bi2-br3)+s6*(br2+bi3)
      tr=p7*(br5-bi5)
      ti=p7*(br5+bi5)
      cr4(k)=c1*(br4+tr)-s1*(bi4+ti)
      ci4(k)=c1*(bi4+ti)+s1*(br4+tr)
      cr5(k)=c5*(br4-tr)-s5*(bi4-ti)
      ci5(k)=c5*(bi4-ti)+s5*(br4-tr)
      tr=-p7*(br7+bi7)
      ti=p7*(br7-bi7)
      cr6(k)=c3*(br6+tr)-s3*(bi6+ti)
      ci6(k)=c3*(bi6+ti)+s3*(br6+tr)
      cr7(k)=c7*(br6-tr)-s7*(bi6-ti)
      ci7(k)=c7*(bi6-ti)+s7*(br6-tr)
      goto 20
10    cr1(k)=br0-br1
      ci1(k)=bi0-bi1
      cr2(k)=br2-bi3
      ci2(k)=bi2+br3
      cr3(k)=br2+bi3
      ci3(k)=bi2-br3
      tr=p7*(br5-bi5)
      ti=p7*(br5+bi5)
      cr4(k)=br4+tr
      ci4(k)=bi4+ti
      cr5(k)=br4-tr
      ci5(k)=bi4-ti
      tr=-p7*(br7+bi7)
      ti=p7*(br7-bi7)
      cr6(k)=br6+tr
      ci6(k)=bi6+ti
      cr7(k)=br6-tr
      ci7(k)=bi6-ti
20    continue
30    continue
      return
      end
