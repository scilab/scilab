C/MEMBR ADD NAME=DMPCNC,SSI=0
c     Copyright INRIA
      subroutine dmpcnc(pm1,d1,ld1,pm2,d2,ld2,pm3,d3,l,m,n,job)
c!but
c     cette subroutine concatene deux matrices dont les coefficients
c     sont des polynomes:
c     pm3=<pm1 pm2>
c     ou
c     pm3=<pm1' pm2'>'
c!liste d'appel
c
c    subroutine dmpcnc(pm1,d1,ld1,pm2,d2,ld2,pm3,d3,l,m,n,job)
c    double precision pm1(*),pm2(*),pm3(*)
c    integer d1(ld1*n+1),d2(ld2*n+1),d3(m*n+1),l,m,n,ld1,ld2,job
c
c     pm1 : tableau  contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome pm1(i,j) est range
c           dans pm1( d1(i + (j-1)*ld1 + k) )
c           pm1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans pm1 du coeff de degre 0
c          du polynome pm1(i,j). Le degre du polynome pm1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     ld1 : entier definissant le rangement dans d1
c
c     pm2,d2,ld2 : definitions similaires a celles de pm1,d1,ld1
c     pm3,d3 : definitions similaires a celles de pm1 et d1, l3 est
c              suppose egal a m
c     l : nombre de ligne de pm1 (et de pm2 et pm3 si job>0)
c     m : nombre de colonnes de pm1 (job>0)
c         nombre de ligne de pm2 (job<0)
c     n : nombre de colonnes de pm2 (job>0)
c         et nombre de colonnes de pm1 et pm3 si job <0
c     job : indique l'operation a effectuer:
c           job >0 pm3=<pm1 pm2>
c           job <0 pm3=<pm1' pm2'>'
c!origine
c    s Steer INRIA
c!
      double precision pm1(*),pm2(*),pm3(*)
      integer d1(*),d2(*),d3(*),l,m,n,ld1,ld2
c
      integer i1,i2,i3,np,i,j
c
      i3=1
      d3(1)=1
      i1=1-ld1
      i2=1-ld2
c
      if(job.lt.0) goto 30
c
      do 11 j=1,m
      i1=i1+ld1
      np=d1(i1+l)-d1(i1)
      call dcopy(np,pm1(d1(i1)),1,pm3(d3(i3)),1)
         do 10 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d1(i1+i)-d1(i1+i-1)
   10    continue
   11 continue
      do 21 j=1,n
      i2=i2+ld2
      np=d2(i2+l)-d2(i2)
      call dcopy(np,pm2(d2(i2)),1,pm3(d3(i3)),1)
         do 20 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d2(i2+i)-d2(i2+i-1)
   20    continue
   21 continue
      return
c
 30   do 50 j=1,n
      i1=i1+ld1
      i2=i2+ld2
      np=d1(i1+l)-d1(i1)
      call dcopy(np,pm1(d1(i1)),1,pm3(d3(i3)),1)
         do 40 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d1(i1+i)-d1(i1+i-1)
   40    continue
      np=d2(i2+m)-d2(i2)
      call dcopy(np,pm2(d2(i2)),1,pm3(d3(i3)),1)
         do 45 i=1,m
         i3=i3+1
         d3(i3)=d3(i3-1)+d2(i2+i)-d2(i2+i-1)
   45    continue
   50 continue
      return
      end
