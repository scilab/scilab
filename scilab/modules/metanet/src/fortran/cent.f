      subroutine cent(fe,che,head,lngt,n,iropt,idmax,r,p,q,d)
      integer fe,che,head,lngt,p,q,d,r
      dimension fe(n),d(n),p(n),q(*)
      dimension head(*),lngt(*),che(*)
      idmax=999999999
      do 99 ibou=1,n
         r=ibou
         call l2que(r,fe,che,head,lngt,n,d,p,q)
         id1max=0
         do 98 imeu=1,n
            id1max=max(id1max,d(imeu))
 98      continue
         if(id1max.ge.idmax) goto 99
         idmax=id1max
         iropt=r
 99   continue
      r=iropt
      call l2que(r,fe,che,head,lngt,n,d,p,q)
      end
