      subroutine frang(i0,lp1,ls1,m,n,pile,rang1,rang)
      implicit integer (a-z)
      dimension lp1(*),ls1(m),rang(n),pile(n),rang1(n)
      i0=0
      do 10 i=1,n
         rang(i)=0
         pile(i)=0
 10   continue
      do 30 i=1,n
         if(lp1(i).eq.lp1(i+1))goto 30
         do 20 ll=lp1(i),lp1(i+1)-1
            j=ls1(ll)
            rang(j)=rang(j)-1
 20      continue
 30   continue
      k=0
      newtop=0
      do 40 i=1,n
         if(rang(i).lt.0)goto 40
         newtop=newtop+1
         pile(newtop)=i
 40   continue
      oldtop=newtop
      bottom=0
 100  continue
      if(bottom.eq.oldtop)goto 200
      bottom=bottom+1
      i=pile(bottom)
      rang(i)=k
      if(lp1(i).eq.lp1(i+1))goto 100
      do 130 ll=lp1(i),lp1(i+1)-1
         j=ls1(ll)
         rang(j)=rang(j)+1
         if(rang(j).ne.0)goto 130
         newtop=newtop+1
         pile(newtop)=j
 130  continue
      goto 100
 200  continue
      if(bottom.eq.n)goto 999
      if(oldtop.ne.newtop)goto 210
      goto 300
 210  continue
      k=k+1
      oldtop=newtop
      goto 100
 300  continue
      do 309 i=1,n
         rang1(i)=rang(i)
 309  continue
      do 310 i0=1,n
         if(rang1(i0).ge.0) goto 310
 315     continue
         do 320 i=1,n
            pile(i)=0
            rang(i)=0
 320     continue
         top=0
         bottom=0
         i=i0
 335     continue
         if(lp1(i).eq.lp1(i+1))goto 345
         do 340 ll=lp1(i),lp1(i+1)-1
            j=ls1(ll)
            if(rang(j).gt.0)goto 340
            rang(j)=i
            top=top+1
            pile(top)=j
 340     continue
         if(rang(i0).ne.0)goto 350
 345     continue
         bottom=bottom + 1
         if (bottom.gt.top) goto 310
         i=pile(bottom)
         goto 335
 310  continue
 350  continue
 999  return
      end

