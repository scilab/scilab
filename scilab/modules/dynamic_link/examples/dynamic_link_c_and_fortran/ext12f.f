c This file is released into the public domain

      subroutine ext12if(n,a)

      common/cmntest/b(10)
      real a(*)
      do 1 k=1,max(n,10)
 1       b(k)=a(k)
      end
      subroutine ext12of(n,c)
      common/cmntest/b(10)
      real c(*)
      do 1 k=1,max(n,10)
 1    c(k)=b(k)
      end

