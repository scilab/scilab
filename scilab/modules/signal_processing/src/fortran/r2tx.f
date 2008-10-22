      subroutine r2tx(nthpo, cr0, cr1, ci0, ci1)
c!
c subroutine:  r2tx
c sous-programme d'iteration en base 2
c!
      double precision  cr0(*), cr1(*), ci0(*), ci1(*)
      double precision  r1,fi1
      do 10 k=1,nthpo,2
      r1=cr0(k)+cr1(k)
      cr1(k)=cr0(k)-cr1(k)
      cr0(k)=r1
      fi1=ci0(k)+ci1(k)
      ci1(k)=ci0(k)-ci1(k)
      ci0(k)=fi1
10    continue
      return
      end
