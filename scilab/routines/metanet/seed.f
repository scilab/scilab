C seed.for
      subroutine seed(inseed,bunny,ia5,ia)
      dimension ia(35,2),bunny(35)
      dimension ia2(35,2),ia3(35,2),ia5(35,2),ia1(35,2),ir(2)
      l=1
      ir(1)=inseed
      ir(2)=0
      do 75 i=1,35
75    bunny(i)=.5**i
      do 2 m=1,l
        do 1 i=2,34,2
1       ia2(i,m)=1
        do 9 i=1,33,2
9       ia2(i,m)=0
2       ia2(35,m)=1
      n1=35
      do 21 mi=1,l
18      do 17 i=1,35
          ige=ia2(34,mi)
          if(i.eq.1)itemp=ia2(34,mi)
          if(i.eq.2)itemp=ia2(35,mi)
          if(i.gt.2)itemp=ia3(i-2,mi)
          ia3(i,mi)=itemp+ia2(i,mi)
          if(ia3(i,mi).eq.2) ia3(i,mi)=0
          ia1(i,mi)=ia2(i,mi)
17        ia2(i,mi)=ia3(i,mi)
        max=35+ir(mi)-1
        n1=n1+35
        if(n1.ge.max) go to 3
        go to 18
3       n2=n1-max
        n3=35-n2
        iz=0
        n4=n3+1
        if(n2.eq.0) go to 33
        do 19 i=n4,35
          iz=iz+1
19        ia(iz,mi)=ia1(i,mi)
33      continue
        if(n3.eq.0) go to 21
        do 49 i=1,n3
          iz=iz+1
49        ia(iz,mi)=ia3(i,mi)
21      n1=35
        end
