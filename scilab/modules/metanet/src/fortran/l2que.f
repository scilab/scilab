      subroutine l2que(r,fe,che,head,lngt,n,d,p,q)
      integer r,fe,che,head,lngt,d,p,q,u,v,dv,pntr
      dimension fe(n),d(n),p(n),q(*)
      dimension head(*),lngt(*),che(*)
      inf = 999999999
      do 10 i=1,n
         q(i) = 0
         d(i) = inf
 10   continue
      q(r) = - 1
      d(r)=0
      p(r) = 0
      nn = n + 1
      q(nn) = nn
      last = nn
      pntr = nn
      u = r
 20   continue

      j=fe(u)
 25   if (j.gt.0) then
         v = head(j)
         dv = d(u) + lngt(j)
         if ( d(v) .gt. dv ) then
            d(v) = dv
            p(v) = u
            CRES=q(v)
            if (CRES .lt. 0) then
               goto 30
            elseif (CRES .eq. 0) then
               goto 40
            else
               goto 50
            endif
 30         q(v) = q(pntr)
            q(pntr) = v
            if ( last .eq. pntr ) last=v
            pntr = v
            go to 50
 40         q(last) = v
            q(v) = nn
            last = v
 50         continue
         end if
         j=che(j)
         goto 25
      end if
 60   u = q(nn)
      q(nn) = q(u)
      q(u) = - 1
      if ( last .eq. u ) last = nn
      if ( pntr .eq. u ) pntr = nn
      if ( u .le. n ) go to 20
      return
      end
