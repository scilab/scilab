      subroutine demux(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,
     &     rpar,nrpar,ipar,nipar,uy1,nuy1,uy2,nuy2,uy3,nuy3,uy4,
c     Copyright INRIA

     &     nuy4,uy5,nuy5,uy6,nuy6,uy7,nuy7,uy8,nuy8,uy9,nuy9)
c     Scicos block simulator
c
      double precision t,xd(*),x(*),z(*),tvec(*),rpar(*)
      double precision uy1(*),uy2(*),uy3(*),uy4(*),uy5(*),uy6(*)
      double precision uy7(*),uy8(*),uy9(*)
      integer flag,nevprt,nx,nz,ntvec,nrpar,ipar(*)
      integer nipar
      integer k
c

      k=0
      goto(20,30,40,50,60,70,80),ipar(1)-1
c
 20   do 25 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 25   continue
      do 27 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 27   continue
      return
c
 30   do 35 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 35   continue
      do 37 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 37   continue
      do 38 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 38   continue
      return
c
 40   do 41 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 41   continue
      do 42 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 42   continue
      do 43 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 43   continue
      do 44 i=1,nuy5
         k=k+1
         uy5(i)=uy1(k)
 44   continue
      return
c
 50   do 51 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 51   continue
      do 52 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 52   continue
      do 53 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 53   continue
      do 54 i=1,nuy5
         k=k+1
         uy5(i)=uy1(k)
 54   continue
      do 55 i=1,nuy6
         k=k+1
         uy6(i)=uy1(k)
 55   continue
      return
c
 60   do 61 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 61   continue
      do 62 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 62   continue
      do 63 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 63   continue
      do 64 i=1,nuy5
         k=k+1
         uy5(i)=uy1(k)
 64   continue
      do 65 i=1,nuy6
         k=k+1
         uy6(i)=uy1(k)
 65   continue
      do 66 i=1,nuy7
         k=k+1
         uy7(i)=uy1(k)
 66   continue
      return
c
 70   do 71 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 71   continue
      do 72 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 72   continue
      do 73 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 73   continue
      do 74 i=1,nuy5
         k=k+1
         uy5(i)=uy1(k)
 74   continue
      do 75 i=1,nuy6
         k=k+1
         uy6(i)=uy1(k)
 75   continue
      do 76 i=1,nuy7
         k=k+1
         uy7(i)=uy1(k)
 76   continue
      do 77 i=1,nuy8
         k=k+1
         uy8(i)=uy1(k)
 77   continue
      return
c
 80   do 81 i=1,nuy2
         k=k+1
         uy2(i)=uy1(k)
 81   continue
      do 82 i=1,nuy3
         k=k+1
         uy3(i)=uy1(k)
 82   continue
      do 83 i=1,nuy4
         k=k+1
         uy4(i)=uy1(k)
 83   continue
      do 84 i=1,nuy5
         k=k+1
         uy5(i)=uy1(k)
 84   continue
      do 85 i=1,nuy6
         k=k+1
         uy6(i)=uy1(k)
 85   continue
      do 86 i=1,nuy7
         k=k+1
         uy7(i)=uy1(k)
 86   continue
      do 87 i=1,nuy8
         k=k+1
         uy8(i)=uy1(k)
 87   continue
      do 88 i=1,nuy9
         k=k+1
         uy9(i)=uy1(k)
 88   continue
      return
      end
