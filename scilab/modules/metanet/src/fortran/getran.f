      subroutine getran(ir,n,l,rn,y1,y2,ia,ia5,bunny,inseed)
      dimension ia(35,2),bunny(35)
      dimension ia5(35,2),ir(2)
      rn=0.
      con=6.28318530717959
      y1=0.
      y2=0.
      if (n.gt.1) go to 22
      call seed(inseed,bunny,ia5,ia)
      go to 25
22    do 26 m=1,l
        do 26 i=1,35
          ite=ia(34,m)
          if(i.eq.1) itemp=ia(34,m)
          if(i.eq.2) itemp=ia(35,m)
          if(i.gt.2) itemp=ia5(i-2,m)
          ia5(i,m)=itemp+ia(i,m)
          if(ia5(i,m).eq.2) ia5(i,m)=0
26        ia(i,m)=ia5(i,m)
25    if(l.eq.1) go to 60
      do 28 j=1,35
        j1=36-j
        if(ia(j1,1).eq.1) y1=y1+bunny(j)
28      if(ia(j1,2).eq.1) y2=y2+bunny(j)
      rn=sqrt(-2.*alog(y1))*sin(con*y2)
      go to 62
60    do 61 j=1,35
        j1=36-j
61      if (ia(j1,1).eq.1) y1=y1+bunny(j)
62    return
      end
