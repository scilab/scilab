      subroutine flomax(b,c,ex,is,it,la2,lp2,ma,mark,
     &     mm,n,or,phima,pile,valflo)
      implicit integer (a-z)
      dimension lp2(*),la2(mm)
      dimension or(ma),ex(ma),b(ma),c(ma)
      dimension phima(ma)
      dimension mark(n),pile(n)
      if (is .lt. 0 .or. is .gt. n .or. it .lt. 0 .or. it .gt. n) then
         call erro('bad internal node number')
         return
      endif
      infe=32700
      valflo=0
 100  continue
      do 110 i=1,n
         pile(i)=0
         mark(i)=0
 110  continue
      top=0
      bottom=0
      mark(is)=0
      j=is
 120  continue
      if(lp2(j).eq.lp2(j+1))goto 150
      do 140 ll=lp2(j),lp2(j+1)-1
         u=la2(ll)
         if(j.eq.ex(u)) goto 130
         if(phima(u).eq.c(u)) goto 140
         i=ex(u)
         if(i.eq.is) goto 140
         if(mark(i).ne.0) goto 140
         top=top + 1
         pile(top)=i
         mark(i)=u
         goto 140
 130     continue
         if(phima(u).eq.b(u)) goto 140
         i=or(u)
         if(i.eq.is) goto 140
         if(mark(i).ne.0) goto 140
         top=top + 1
         pile(top)=i
         mark(i)=-u
 140  continue
 150  continue
      bottom=bottom + 1
      if(bottom.gt.top) goto 160
      j=pile(bottom)
      goto 120
 160  continue
      if(mark(it).eq.0) goto 999
      eps = infe
      j=it
 210  continue
      if(j.eq.is) goto 220
      u=mark(j)
      if(u.lt.0) goto 215
      eps=min(eps,(c(u)-phima(u)))
      j=or(u)
      goto 210
 215  continue
      u=-u
      eps=min(eps,(phima(u)-b(u)))
      j=ex(u)
      goto 210
 220  continue
      j=it
 230  continue
      if(j.eq.is) goto 240
      u=mark(j)
      if(u.lt.0) goto 235
      phima(u)=phima(u) + eps
      j=or(u)
      goto 230
 235  continue
      u=-u
      phima(u)=phima(u) - eps
      j=ex(u)
      goto 230
 240  continue
      valflo=valflo + eps
      goto 100
 999  continue
      end
