C/MEMBR ADD NAME=ROMEG,SSI=0
      subroutine romeg(nmaxi,maxdeg,ityp,nzm,zm,rom)
c!purpose
c realized frequencies omega
c!
c
      implicit double precision (a-h,o-z)
      dimension rom(*), nzm(*)
      double precision zm(maxdeg,*)
c
      n2 = nzm(2)
      n3 = nzm(3)
      go to (10, 20, 30, 40), ityp
  10  rom(1) = zm(n2,2)
      rom(2) = zm(1,3)
      go to 50
  20  rom(1) = zm(1,3)
      rom(2) = zm(n2,2)
      go to 50
  30  rom(1) = zm(n3,3)
       rom(2) = zm(1,2)
      rom(3) = zm(n2,2)
      rom(4) = zm(1,3)
      go to 50
  40  n2 = n2/2
      rom(1) = zm(n2,2)
      rom(4) = zm(n2+1,2)
      rom(3) = zm(1,3)
      rom(2) = zm(n3,3)
  50  return
      end
