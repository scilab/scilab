      subroutine pcchna(i0,lp,ls,m,n,pan,pani,pile)
      implicit integer (a-z)
      dimension lp(*),ls(m),pani(n),pan(n),pile(n)
      infe=32700
      if (i0 .lt. 0 .or. i0 .gt. n) then
         call erro('bad internal node number')
         return
      endif
      do 10 i=1,n
         pani(i)=infe
         pan(i)=-infe
         pile(i)=0
 10   continue
      top=0
      bottom=0
      pani(i0)=0
      pan(i0)=0
      j=i0
 100  continue
      if(lp(j).eq.lp(j+1))goto 120
      do 110 ll=lp(j),lp(j+1)-1
         i=ls(ll)
         if(pan(i).ge.0)goto 110
         pani(i)=pani(j)+1
         pan(i)=j
         top=top+1
         pile(top)=i
 110  continue
 120  continue
      bottom=bottom + 1
      if(bottom.gt.top) go to 999
      j=pile(bottom)
      goto 100
 999  continue
      do 20,i=1,n
         if (pani(i) .eq. infe) then
            pani(i)=-1
         endif
 20   continue
      end


