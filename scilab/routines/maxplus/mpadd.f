      double precision function mpadd(p,q)
      double precision p,q
      include 'mpstack.h'
c      tmp=1000.d0
c      ZERO=-exp(tmp)
c      PCZERO=ZERO
      if(p.ne.PCZERO)then
         if(q.ne.PCZERO)then
            mpadd=p+q
         else
            mpadd=PCZERO
         endif
      else 
         mpadd=PCZERO
      endif
      return
      end
