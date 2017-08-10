c     -------------------------------------------      
      subroutine ext1f(n,a,b,c)
c     -------------------------------------------
c     (very) simple example 1
c     -->link('ext1f.o','ext1f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=call('ext1f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     c=a+b
      double precision a(*),b(*),c(*)
      do 1 k=1,n
         c(k)=a(k)+b(k)
 1    continue
      return
      end
c
c     -------------------------------------------      
      subroutine ext2f(n,a,b,c)
c     -------------------------------------------      
c     simple example 2 (using sin and cos)
c     -->link('ext2f.o','ext2f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=call('ext2f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     -->c=sin(a)+cos(b)
      double precision a(*),b(*),c(*)
      do 1 k=1,n
         c(k)=sin(a(k))+cos(b(k))
 1    continue
      return
      end
c      
c     -------------------------------------------      
      subroutine ext3f(ch,n,a,b,c)
c     -------------------------------------------
c     example 3 (passing a chain)
c     -->link('ext3f.o','ext3f');
c     -->a=[1,2,3];b=[4,5,6];n=3;
c     -->c=call('ext3f','yes',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d')
c     -->c=sin(a)+cos(b)
c     -------------------------------------
      double precision a(*),b(*),c(*)
      character*(*) ch
      if(ch(1:3).eq.'yes') then
         do 1 k=1,n
            c(k)=sin(a(k))+cos(b(k))
 1       continue
      else
         do 2 k=1,n
            c(k)=a(k)+b(k)
 2       continue
      endif
      return
      end
c      
c     -------------------------------------------      
      subroutine ext8f(n, t, y, ydot)
c     -------------------------------------------      
c     argument function for ode
c     input variables n, t, y
c     n = dimension of state vector y
c     t = time
c     y = state variable 
c     output variable = ydot
c
c     external routine must 
c     load ydot(1) with d/dt ( y(1)(t) )
c          ydot(2) with d/dt ( y(2)(t) )
c          ...
c     i.e. ydot vector of derivative of state y
c
c     Example:
c     call this ext8f routine: 
c     ode([1;0;0],0,[0.4,4],'ext9f')
c
c     With dynamic link: 
c     -->link('ext8f.o','ext8f')
c     -->ode([1;0;0],0,[0.4,4],'ext8f')
c
      double precision t, y, ydot
      dimension y(3), ydot(3)
      ydot(1) = -.0400d+0*y(1) + 1.0d+4*y(2)*y(3)
      ydot(3) = 3.0d+7*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      end
c      
c     -------------------------------------------
      subroutine ext9f(n, t, y, ydot)
c     -------------------------------------------      
c     external fonction for ode
c     input variables n, t, y
c     n = dimension of state vector y
c     t = time
c     y = state variable 
c     output variable = ydot
c
c     external routine must 
c     load ydot(1) with d/dt ( y(1)(t) )
c          ydot(2) with d/dt ( y(2)(t) )
c          ...
c     i.e. ydot vector of derivative of state y
c
c     With dynamic link: 
c     link('ext9f.o','ext9f')
c
c     passing a parameter to ext9f routine by a list:
c     -->param=[0.04,10000,3d+7];    
c     -->y=ode([1;0;0],0,[0.4,4],list('ext10f',param))
c     param is retrieved in ext10f by:
c     param(1)=y(n+1) , param(2)=y(n+2) etc 
c     with this calling sequence y is a n+np vector
c     where np=dimension of scilab variable param
c
      double precision t, y, ydot, param
      dimension y(3), ydot(3),param(3)
      param(1)=y(n+1)
      param(2)=y(n+2)
      param(3)=y(n+3)
      ydot(1) = -param(1)*y(1) + param(2)*y(2)*y(3)
      ydot(3) = param(3)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      end
c
c     -------------------------------------------
      subroutine ext11f(n,a)
c     -------------------------------------------
      common/cmntest/b(10)
      real a(*)
      do 1 k=1,max(n,10)
 1       b(k)=a(k)
      end
c
c     -------------------------------------------
      subroutine ext12f(n,c)
c     -------------------------------------------
      common/cmntest/b(10)
      real c(*)
      do 1 k=1,max(n,10)
 1    c(k)=b(k)
      end
c     -------------------------------------------
c

