      subroutine mpdspdrsp(p,q,r,a,nela,inda,b,nelb,indb,
     $      ia,at,iat,indat,v,c,mrc,mpzero)
      integer p, q, r, nela, nelb, inda(*), indb(*), ia(*), 
     $      iat(*), indat(*), mrc
cSG      double precision a(*),b(*),c(mrc,r),at(*),v(*),c(*)
      double precision a(*),b(*),at(*),v(*),c(mrc,r)
      double precision mpzero
c
c calculate c=a/b
c

c p=rows in a
c q=cols in a
c r=rows in b


c
c 1. negate b
c
      call dscal(nelb,-1.0d+0,b,1)
c
c 2. transpose b
c

      call dspt(r,q,b,nelb,indb,ia,at,iat,indat)
c
c 3. convert a to full
c
      call mpdspful(p,q,a,nela,inda,v,mpzero)
c
c 4. calculate a*b
c
      call mndsmsp(p,q,r,v,p,at,nelb,indat,c,mrc,mpzero)
      end
