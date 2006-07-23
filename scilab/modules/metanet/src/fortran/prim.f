      subroutine prim(alpha,la2,lp2,ls2,ma,mm,n,piw,wa)
      implicit integer (a-z)
      dimension lp2(*),la2(mm),ls2(mm)
      doubleprecision wa(ma),piw(n)
      dimension alpha(n)
      real pimin,infr
      infe=32700
      infr=10.d6
      do 30 i=1,n
         piw(i)=  infr
         alpha(i)= - infe
 30   continue
      i0=1
      piw(i0)=0.
      alpha(i0)=0
      imin=i0
      goto 200
 100  continue
      imin=0
      pimin=infr
      do 110 i=1,n
         if(alpha(i).ge.0) goto 110    
         if(piw(i).ge.pimin) goto 110
         imin=i
         pimin=piw(i)
 110  continue
      if(imin.eq.0) goto 990
      alpha(imin)=-alpha(imin)
 200  continue
      if(lp2(imin).eq.lp2(imin+1))goto 220
      do 210 ll = lp2(imin),lp2(imin+1)-1
         j=ls2(ll)
         if(alpha(j).ge.0) goto 210
         u=la2(ll)
         if(wa(u).ge.piw(j)) goto 210
         alpha(j)=-u
         piw(j)=wa(u)
 210  continue  
 220  continue
      goto 100
 990  continue
      end
