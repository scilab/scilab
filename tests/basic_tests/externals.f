      subroutine ext1f(n,a,b,c)
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
      subroutine ext2f(n,a,b,c)
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
      subroutine ext3f(ch,n,a,b,c)
c     ----------------------------
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
      subroutine ext4f(n,a,b,c)
c     example 4 (reading a chain)
c     -->link('ext4f.o','ext4f');
c     -->a=[1,2,3];b=[4,5,6];n=3;yes='yes'
c     -->c=call('ext4f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
c     -->c=sin(a)+cos(b)
c     -->yes='no'
c     -->c=a+b
c     -->clear yes  --> undefined variable : yes
      double precision a(*),b(*),c(*)
      logical creadchain
      parameter (ichmax=10)
      character ch*(ichmax)

c     If chain named yes exists reads it in ch else return
      lch=ichmax
      if(.not.creadchain('yes'//char(0),lch,ch)) return
c     *********************************     
      if(ch(1:lch).eq.'yes') then
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
      
      subroutine ext5f(b,c)
c     example 5
c     reading a vector in scilab internal stack using creadmat
c     (see SCIDIR/system2/creadmat.f)
c     -->link('ext5f.o','ext5f')
c     -->a=[1,2,3];b=[2,3,4];
c     -->c=call('ext5f',b,1,'d','out',[1,3],2,'d')
c     -->c=a+2*b
      double precision a(3),b(*),c(*)
      logical creadmat

c     If 'a' exists reads it else return
      if(.not.creadmat('a'//char(0),m,n,a)) then
         write(6,*) 'ext5', m,n,a(1),a(2),a(3)
         return
      endif
      do 1 k=1,n
         c(k)=a(k)+2.0d0*b(k)
 1    continue
      return
      end
      subroutine ext6f(aname,b,c)
c     example 6
c     reading a vector in scilab internal stack using readmat interface
c     -->link('ext6f.o','ext6f')
c     -->a=[1,2,3];b=[2,3,4];
c     -->c=call('ext6f','a',1,'c',b,2,'d','out',[1,3],3,'d')
c     -->c=a+2*b
      double precision a(3),b(*),c(*)
      logical creadmat
      character*(*) aname

c     If aname exists reads it (in a) else return
      if(.not.creadmat(aname,m,n,a)) return
c     
c     [m,n]=size(a)  here m=1 n=3
      do 1 k=1,n
         c(k)=a(k)+2.0d0*b(k)
 1    continue
      return
      end
      subroutine ext7f(a,b)
c     example 7
c     creating vector c in scilab internal stack
c     -->link('ext7f.o','ext7f')
c     -->a=[1,2,3]; b=[2,3,4];
c     c does not exist (c made by the call to matz)
c     -->call('ext7f',a,1,'d',b,2,'d','out',1);
c     c now exists
c     -->c=a+2*b
      double precision a(3),b(3),c(3),w
      logical cwritemat,cwritechain
      do 1 k=1,3
         c(k)=a(k)+2.0d0*b(k)
 1    continue
c     sending c array values to c Scilab variable 
c     of size [1,3]
      if (.not.cwritemat('c'//char(0),1,3,c)) return
c     sending string 'test' (size 4) to Scilab variable d
      if (.not.cwritechain('d'//char(0),4,'test')) return
      return
      end
      subroutine ext8f(y)
c     -------------------------------------------
c     same example with call to cmatptr
c     -->param=[1,2,3];         
c     -->link('ext8c.o','ext8c','C');     
c     -->y=call('ext8c','out',size(param),1,'d');
      logical cmatptr
      double precision y(*)
      include '../routines/stack.h'
c     If variable 'param' exists gets its pointer in 
c     internal stack else return
      if(.not.cmatptr('param'//char(0),m,n,lp)) return
c     ********************************
c     param entries are in stk(lp+i),i=0,m*n
      do 10 i=1,m*n
         y(i) = stk(lp+i-1)
 10   continue
      return
      end
      subroutine ext9f(n, t, y, ydot)
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
c     call this ext9f routine: 
c     ode([1;0;0],0,[0.4,4],'ext9f')
c
c     With dynamic link: 
c     -->link('ext9f.o','ext9f')
c     -->ode([1;0;0],0,[0.4,4],'ext9f')
c
      double precision t, y, ydot
      dimension y(3), ydot(3)
      ydot(1) = -.0400d+0*y(1) + 1.0d+4*y(2)*y(3)
      ydot(3) = 3.0d+7*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      end

      subroutine ext10f(n, t, y, ydot)
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
c     link('ext10f.o','ext10f')
c
c     passing a parameter to ext10f routine by a list:
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
      subroutine ext11f(neq, t, y, ydot)
c     -------------------------------------------
c     exemple with a call to readmat routine
c     -->param=[0.04,10000,3d+7];
c     -->link('ext11f.o','ext11f')
c     -->y=ode([1;0;0],0,[0.4,4],'ext11f')
c     param must be defined as a scilab variable
      double precision t, y, ydot, param
      logical creadmat
      dimension y(3), ydot(3), param(3)

c     If "param" does not exists return else loads param
      if(.not.creadmat('param'//char(0),m,n,param)) return
c     ***********************************

      ydot(1) = -param(1)*y(1) + param(2)*y(2)*y(3)
      ydot(3) = param(3)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end
      subroutine ext12f(neq, t, y, ydot)
c     -------------------------------------------
c     same example with call to cmatptr
c     param must be defined as a scilab variable
c     exemple with a call to cmatptr function
c     -->param=[0.04,10000,3d+7];
c     -->link('ext12f.o','ext12f')
c     -->y=ode([1;0;0],0,[0.4,4],'ext12f')
      double precision t, y, ydot
      logical cmatptr
      dimension y(3), ydot(3)
c
      include '../routines/stack.h'
c

c     If variable 'param' exists gets its pointer in 
c     internal stack else return
      if(.not.cmatptr('param'//char(0),m,n,lp)) return
c     ********************************
c     param entries are in stk(lp),stk(lp+1),stk(lp+2)
c     i.e. param(1)= stk(lp), ...
c     m,n = dimensions of param = 3,1 (or 1,3 if row v.)

c     (note that vector param not used in this example)
      ydot(1) = -stk(lp)*y(1) + stk(lp+1)*y(2)*y(3)
      ydot(3) = stk(lp+2)*y(2)*y(2)
      ydot(2) = -ydot(1) - ydot(3)
      return
      end

      subroutine ext13f(n,a)
      common/cmntest/b(10)
      real a(*)
      do 1 k=1,max(n,10)
 1       b(k)=a(k)
      end
      subroutine ext14f(n,c)
      common/cmntest/b(10)
      real c(*)
      do 1 k=1,max(n,10)
 1    c(k)=b(k)
      end

