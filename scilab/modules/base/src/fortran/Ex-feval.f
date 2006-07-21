      subroutine parab(nn,x1,x2,xres,itype,name) 
c     Copyright ENPC 
      double precision x1,x2
      double precision xres(2)
      integer itype,nn
      character*6     name
      if (nn.eq.1) then 
         xres(1)=x1**2
         itype=0
      else
         xres(1)=x1**2+x2**2
         itype=0
      endif
      return
      end

      subroutine parabc(nn,x1,x2,xres,itype,name) 
C     a version which returns complex numbers 
      double precision x1,x2
      double precision xres(2)
      integer itype,nn
      character*6     name
      if (nn.eq.1) then 
         xres(1)=x1**2
         xres(2)=x1
         itype=1
      else
         xres(1)=x1**2+x2**2
         xres(2)=x1+x2
         itype=1
      endif
      return
      end

