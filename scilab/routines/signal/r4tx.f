      subroutine r4tx(nthpo, cr0, cr1, cr2, cr3, ci0, ci1, ci2, ci3)
c!
c subroutine:  r4tx
c sous-programme d'iteration en base 4
c!
      double precision cr0(*), cr1(*), cr2(*), cr3(*),
     & ci0(*), ci1(*), ci2(*), ci3(*)
      double precision r1,r2,r3,r4,fi1,fi2,fi3,fi4
c
      do 10 k=1,nthpo,4
      r1=cr0(k)+cr2(k)
      r2=cr0(k)-cr2(k)
      r3=cr1(k)+cr3(k)
      r4=cr1(k)-cr3(k)
      fi1=ci0(k)+ci2(k)
      fi2=ci0(k)-ci2(k)
      fi3=ci1(k)+ci3(k)
      fi4=ci1(k)-ci3(k)
      cr0(k)=r1+r3
      ci0(k)=fi1+fi3
      cr1(k)=r1-r3
      ci1(k)=fi1-fi3
      cr2(k)=r2-fi4
      ci2(k)=fi2+r4
      cr3(k)=r2+fi4
      ci3(k)=fi2-r4
  10  continue
      return
      end
