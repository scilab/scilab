      subroutine tconex(alpha,iscon,la2,lp2,ls2,mm,n,pile)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),ls2(mm)
      dimension alpha(n),pile(n)
      infe=32700
      do 30 i=1,n
         alpha(i)= - infe
 30   continue
      i0=1
      alpha(i0)=0
      pile(i0)=1
      top=1
      bottom=0
 100  continue
      if(bottom.ge.top)goto 990
      bottom=bottom+1
      i=pile(bottom)
      alpha(i)=-alpha(i)
 200  continue
      if(lp2(i).eq.lp2(i+1))goto 215
      do 210 ll = lp2(i),lp2(i+1)-1
         j=ls2(ll)
         if(alpha(j).ne.-infe) goto 210
         u=la2(ll)
         alpha(j)=-u
         top=top+1
         pile(top)=j
 210  continue
 215  continue
      goto 100
 990  continue
      iscon=1
      if(bottom.eq.n) goto 999
      iscon=0
 999  continue
      end
