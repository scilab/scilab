      subroutine ext8f(y)
c     -------------------------------------------
c     same example with call to cmatptr
c     -->param=[1,2,3];         
c     -->link('ext8c.o','ext8c','C');     
c     -->y=call('ext8c','out',size(param),1,'d');
c     Copyright INRIA
      logical cmatptr
      double precision y(*)
      include '../../routines/stack.h'
c     If variable 'param' exists gets its pointer in 
c     internal stack else return
      if(.not.cmatptr('param'//char(0),m,n,lp)) return
c     ********************************
c     param entries are in stk(lp+i),i=0,m*n
c     we can change param 
      stk(lp)=18.0d0
c     and read param 
      do 10 i=1,m*n
         y(i) = stk(lp+i-1)
 10   continue
      return
      end
