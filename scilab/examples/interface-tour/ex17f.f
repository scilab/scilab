      subroutine ex17f(x,nx,y,ny,z,f)
      double precision x(*),y(*),z(nx,ny),zz
      integer nx,ny
      external f
      do 1 i=1,nx
      do 1 j=1,ny
         call f(x(i),y(j),zz)
         z(i,j)=zz
  1   continue
      end

      subroutine f1f(x,y,z) 
      double precision x,y,z
      z=x+y
      end

      subroutine f2f(x,y,z)
      double precision x,y,z
      z=x*x+y*y
      end

