c =========================================
c INRIA
c =========================================      
      integer function strord(r1,l1,r2,l2)
      integer r1(*),r2(*),c1,c2
      integer blank
      data blank/40/
c
      iord=0
      if(l1.eq.0) then
         if(l2.gt.0) then
            strord=-1
            return
         else
            strord=0
            return
         endif
      else
         if(l2.eq.0) then
            strord=1
            return
         endif
      endif
      ll=min(l1,l2)
      do 10 i=1,max(l1,l2)
         if(i.le.l1) then
            c1=r1(i)
         else
            c1=blank
         endif
         if(i.le.l2) then
            c2=r2(i)
         else
            c2=blank
         endif
         if(c1.ge.0) c1=256-c1
         if(c2.ge.0) c2=256-c2
         if(c1.gt.c2) then
            strord=1
            return
         elseif(c1.lt.c2) then
            strord=-1
            return
         endif
 10   continue
      strord=0
      return
      end
c =========================================
