      subroutine ext5f(b,c)
c     example 5
c     reading a vector in scilab internal stack using creadmat
c     (see SCIDIR/system2/creadmat.f)
c     -->link('ext5f.o','ext5f')
c     -->a=[1,2,3];b=[2,3,4];
c     -->c=call('ext5f',b,1,'d','out',[1,3],2,'d')
c     -->c=a+2*b
c     Copyright INRIA
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
