      subroutine fsom(a,n,b,m,c)
      real a(*),b(*),c(*)
      kk=min(n,m)
      do 2 k=1,m
 2       c(k)=1.0
      do 1 k=1,kk
 1       c(k)=c(k)+b(k)+a(k)
      end

      
