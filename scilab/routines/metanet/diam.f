      subroutine diam(fe,che,head,lngt,n,iropt,ii,idmax,p,q,d)
      integer fe,che,head,lngt,p,q,d,r
      dimension fe(n),d(n),p(n),q(*)
      dimension head(*),lngt(*),che(*)
      idmax=0
      do 99 ibou=1,n
         r=ibou
         call l2que(r,fe,che,head,lngt,n,d,p,q)
         id1max=0
         do 98 imeu=1,n
            id1max=max(id1max,d(imeu))
 98      continue
         if(id1max.le.idmax) goto 99
         idmax=id1max
         iropt=r
 99   continue
      r=iropt
      call l2que(r,fe,che,head,lngt,n,d,p,q)
      ii=1
      do 97 imeu=1,n
         if(d(imeu).eq.idmax) ii=imeu
 97   continue
      end
