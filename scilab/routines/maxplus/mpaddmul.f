      double precision function mpaddmul(r,p,q)
      double precision r,p,q
      include 'mpstack.h'
      if(p.ne.PCZERO)then
         if(q.ne.PCZERO)then
            mpaddmul=max(r,p+q)
         endif
      else
         mpaddmul=r
      endif
      return
      end
