C/MEMBR ADD NAME=WMPCNC,SSI=0
c     Copyright INRIA
      subroutine wmpcnc(mp1r,mp1i,d1,ld1,mp2r,mp2i,d2,ld2,mp3r,mp3i,d3,
     & l,m,n,job)
c!but
c     cette subroutine concatene deux matrices dont les coefficients
c     sont des polynomes a coefficients complexes
c     mp3=<mp1 mp2>
c     ou
c     mp3=<mp1' mp2'>'
c!liste d'appel
c
c    subroutine wmpcnc(mp1r,mp1i,d1,ld1,mp2r,mp2i,d2,ld2,mp3r,mp3i,d3,
c    & l,m,n,job)
c    double precision mp1r(*),mp1i(*),mp2r(*),mp2i(*),mp3r(*),mp3i(*)
c    integer d1(ld1*n+1),d2(ld2*n+1),d3(m*n+1),l,m,n,ld1,ld2,job
c
c     mp1 : tableau  contenant les coefficients des polynomes,
c           le coefficient de degre k du polynome mp1(i,j) est range
c           dans mp1( d1(i + (j-1)*ld1 + k) )
c           mp1 doit etre de taille au moins d1(ld1*n+1)-d1(1)
c     d1 : tableau entier de taille ld1*n+1,  si k=i+(j-1)*ld1 alors
c          d1(k)) contient  l'adresse dans mp1 du coeff de degre 0
c          du polynome mp1(i,j). Le degre du polynome mp1(i,j) vaut:
c          d1(k+1)-d1(k) -1
c     ld1 : entier definissant le rangement dans d1
c
c     mp2,d2,ld2 : definitions similaires a celles de mp1,d1,ld1
c     mp3,d3 : definitions similaires a celles de mp1 et d1, l3 est
c              suppose egal a m
c     l : nombre de ligne de mp1 (et de mp2 et mp3 si job>0)
c     m : nombre de colonnes de mp1 (job>0)
c         nombre de ligne de mp2 (job<0)
c     n : nombre de colonnes de mp2 (job>0)
c         et nombre de colonnes de mp1 et mp3 si job <0
c     job : indique l'operation a effectuer:
c           job >0 mp3=<mp1 mp2>
c           job <0 mp3=<mp1' mp2'>'
c           si abs(job)=1 mp1 et mp2 sont complexes
c           si abs(job)=2 mp1 est reelle (mp1i vide) et mp2 complexe
c           si abs(job)=3 mp1 est complexe,mp2 reelle (mp2i vide)
c!origine
c    s Steer INRIA
c!
      double precision mp1r(*),mp1i(*),mp2r(*),mp2i(*),mp3r(*),mp3i(*)
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
      call dcopy(np,mp1r(d1(i1)),1,mp3r(d3(i3)),1)
      if(job.ne.2) call dcopy(np,mp1i(d1(i1)),1,mp3i(d3(i3)),1)
      if(job.eq.2) call dset(np,0.0d+0,mp3i(d3(i3)),1)
         do 10 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d1(i1+i)-d1(i1+i-1)
   10    continue
   11 continue
      do 21 j=1,n
      i2=i2+ld2
      np=d2(i2+l)-d2(i2)
      call dcopy(np,mp2r(d2(i2)),1,mp3r(d3(i3)),1)
      if(job.ne.3) call dcopy(np,mp2i(d2(i2)),1,mp3i(d3(i3)),1)
      if(job.eq.3) call dset(np,0.0d+0,mp3i(d3(i3)),1)
         do 20 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d2(i2+i)-d2(i2+i-1)
   20    continue
   21 continue
      return
c
   30 do 50 j=1,n
      i1=i1+ld1
      i2=i2+ld2
      np=d1(i1+l)-d1(i1)
      call dcopy(np,mp1r(d1(i1)),1,mp3r(d3(i3)),1)
      if(job.ne.-2) call dcopy(np,mp1i(d1(i1)),1,mp3i(d3(i3)),1)
      if(job.eq.-2) call dset(np,0.0d+0,mp3i(d3(i3)),1)
         do 40 i=1,l
         i3=i3+1
         d3(i3)=d3(i3-1)+d1(i1+i)-d1(i1+i-1)
   40    continue
      np=d2(i2+m)-d2(i2)
      call dcopy(np,mp2r(d2(i2)),1,mp3r(d3(i3)),1)
      if(job.ne.-3) call dcopy(np,mp2i(d2(i2)),1,mp3i(d3(i3)),1)
      if(job.eq.-3) call dset(np,0.0d+0,mp3i(d3(i3)),1)
         do 45 i=1,m
         i3=i3+1
         d3(i3)=d3(i3-1)+d2(i2+i)-d2(i2+i-1)
   45    continue
   50 continue
      return
      end
