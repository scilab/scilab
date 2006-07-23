      subroutine busack(c,ex,gamma,is,it,la2,lp2,ma,
     &     markb,mm,n,or,p,phimim,piv,v,valflo,flag)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),or(ma),ex(ma)
      dimension c(ma)
      doubleprecision gamma(ma),markb(n),piv(n)
      dimension phimim(ma)
      dimension p(n)
      doubleprecision infr,zero,eps,x,y,z,errtol,absz
      if (is .lt. 0 .or. is .gt. n .or. it .lt. 0 .or. it .gt. n) then
         call erro('bad internal node number')
         return
      endif
      flag=1
      coderr=0
      valflo=0
      errtol=1.0d-6
      do 3 u=1,ma
         if (c(u).ge.0)goto 3
         coderr=2
         goto 999
    3 continue
      do 5 u=1,ma
         if(gamma(u).ge.0)goto 5
         coderr=3
         goto 999
    5 continue
      zero=0.00001
      infr=10.d6
      infe=32700
      do 10 u=1,ma
         phimim(u)=0
 10   continue
      do 20 i=1,n
         piv(i)=0
 20   continue
 100  continue
      do 110 i=1,n
         markb(i)=infr
         p(i)=-infe
 110  continue
      markb(is)=0
      p(is)=0
      i=is
 120  continue
      if(lp2(i).eq.lp2(i+1))goto 155
      do 150 ll = lp2(i),lp2(i+1)-1
         u=la2(ll)
         if(i.eq.or(u)) goto 130
         if(i.eq.ex(u)) goto 140
 130     j=ex(u)
         if(phimim(u).eq.c(u)) goto 150
         if(p(j).ge.0) goto 150
         x=markb(j)
         y=markb(i) + gamma(u) + piv(i) - piv(j)
         if(x.le.y) goto 150
         markb(j)=y
         p(j)= -u
         goto 150
 140     j=or(u)    
         if(phimim(u).eq.0) goto 150
         if(p(j).ge.0)goto 150
         x=markb(j)
         z= gamma(u) - piv(i) + piv(j)
         absz=abs(z-zero)
         if(absz.gt.errtol)goto 145
         coderr = 2
         goto 999
 145     continue
         y=markb(i) - z
         if(x.le.y) goto 150
         markb(j)=y
         p(j)= -u
 150  continue
 155  continue
      p(i) = -p(i)
      i=0
      x=infr
      do 160 j=1,n
         if(p(j).ge.0) goto 160
         if(markb(j).ge.x) goto 160
         i=j
         x=markb(j)
 160  continue
      if(i.gt.0) goto 120
      if(markb(it).lt.infr) goto 200
      coderr=1
      goto 999
 200  continue
      eps = infr
      i=it
 210  continue
      if(i.eq.is) goto 220
      u=p(i)
      if(i.eq.or(u)) goto 215
      eps=min(eps,dble(c(u)-phimim(u)))
      i=or(u)
      goto 210
 215  continue
      eps=min(eps,dble(phimim(u)))
      i=ex(u)
      goto 210
 220  continue
      eps=min(eps,dble(v-valflo))
      i=it
 230  continue
      if(i.eq.is) goto 240
      u=p(i)
      if(i.eq.or(u))goto 235
      phimim(u)=phimim(u) + eps
      i=or(u)
      goto 230
 235  continue
      phimim(u)=phimim(u) - eps
      i=ex(u)
      goto 230
 240  continue
      do 250 i=1,n
         piv(i)=piv(i)+markb(i)
 250  continue
      valflo=valflo + eps
      if(valflo.lt.v)goto 100
 999  continue
      if (coderr .eq. 0) then
         return
      else if (coderr .eq. 1) then
         call out('constrained flow too big')
         flag=0
         return
      else if (coderr .eq. 2) then
         call erro('negative maximum capacities')
         return
      else if (coderr .eq. 3) then
         call erro('negative costs')
         return
      endif
      end
