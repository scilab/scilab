      subroutine dperm(vv,nv,ind)
c apply an in-situ  permutation 
c     Copyright INRIA
c     Copyright INRIA
      double precision vv(nv),x
      integer ind(nv)
c
      x=vv(1)
      i3=1
      i0=1
      i=i0
 10   continue
      if(ind(i).eq.i0) then 
c     end of a permutation cycle 
        vv(i)=x
        ind(i)=-ind(i)
c       look for first positive ind(i)
        i0=i3
 11     i0=i0+1
        if(i0.gt.nv) goto 20
        if(ind(i0).lt.0) goto 11
c       beginning of a permutation cycle 
        i3=i0
        i=i0
        x=vv(i0)
      else
         vv(i)=vv(ind(i))
         ind(i)=-ind(i)
         i=-ind(i)
      endif
      goto 10
 20   continue

      do 30 i=1,nv
         ind(i)=-ind(i)
 30   continue
      end
