c -----------------------
c  Fort : with the brief form (i.e when the keywords 'out' and 'sort' 
c         are not used ) 
c  in this case the foobare1 routine must be programmed as an 
c  interface ( see examples/addinter-examples ) 
c  the first argument transmited to the interface is 
c  the function name 
c   
c  [y1,y2,...,ym]=fort('routinename',x1,x2,...,xn)
c  the input variables xi's and the output variables yi's
c  are (matrices) of integers, real or double, or strings 
c
c         -->x=[1,2,3];y=x;z=x;
c         -->[a,b,c,d]=fort('foobar1','mul',x,y,z)
c-----------------------------------------------------------
c     Copyright INRIA
      subroutine foobar1(fname)
      character*(*) fname
      logical getrhsvar, createvar,  putlhsvar
      include '../stack.h'
c
      nbvars=0
      minrhs=5
      maxrhs=5
      minlhs=1
      maxlhs=4
c
      if(.not.((rhs.ge.minrhs).and.(rhs.le.maxrhs))) then
         call erro('wrong number of rhs arguments')
         return
      endif
      if(.not.((lhs.ge.minlhs).and.(lhs.le.maxlhs))) then
         call erro('wrong number of lhs arguments')
         return
      endif
c     
c     when using fort the first parameter is the function name 
      if(.not.getrhsvar(1,'c',m1,n1,l1)) return
      if(.not.getrhsvar(2,'c',m2,n2,l2)) return
      if(.not.getrhsvar(3,'i',m3,n3,l3)) return
      if(.not.getrhsvar(4,'r',m4,n4,l4)) return
      if(.not.getrhsvar(5,'d',m5,n5,l5)) return
c
      if(.not.createvar(6,'d',m5,n5,l6)) return
      if(.not.createvar(7,'d',m5,n5,l7)) return
c
      call foubare(cstk(l2:l2+m2*n2),istk(l3),n3*m3,sstk(l4),n4*m4,
     $     stk(l5),m5,n5,stk(l6),stk(l7))
c     
      lhsvar(1)=3
      lhsvar(2)=4
      lhsvar(3)=5
      lhsvar(4)=6
      if(.not.putlhsvar()) return
c     
      end

      subroutine foubare(ch,a,ia,b,ib,c,mc,nc,d,w)
c     -----------------------------------------
c     -----------   EXAMPLE   -----------------
c     inputs:  ch, a,b and c; ia,ib and mc,nc
c     ch=character, a=integer, b=real and c=double 
c     ia,ib and [mc,nc] are the dimensions of a,b and c resp.
c     outputs: a,b,c,d
c     if ch='mul'   a,b and c = 2 * (a,b and c) 
c     and d of same dimensions as c with
c     d(i,j)=(i+j)*c(i,j)
c     if ch='add' a,b and c = 2 + (a,b and c)
c     d(i,j)=(i+j)+c(i,j)
c     w is a working array of size [mc,nc]
c     WARNING : do not use function len on parameter ch 
c             it will give erroneous results.
c     -------------------------------------------
      character*(*) ch
      integer a(*)
      real b(*)
      double precision c(mc,*),d(mc,*), w(mc,*)
      if(ch(1:3).eq.'mul') then
         do 1 k=1,ia
            a(k)=2*a(k)
 1       continue
         do 2 k=1,ib
            b(k)=2.0*b(k)
 2       continue
         do 3 i=1,mc
            do 6 j=1,nc
               c(i,j)=2.0d0*c(i,j)
 6          continue
 3       continue
         do 4 i=1,mc
            do 5 j=1,nc
               w(i,j)=i+j
               d(i,j)=w(i,j)*c(i,j)
 5          continue
 4       continue
      elseif(ch(1:3).eq.'add') then
         do 10 k=1,ia
            a(k)=2+a(k)
 10      continue
         do 20 k=1,ib
            b(k)=2.0+b(k)
 20      continue
         do 30 i=1,mc
            do 31 j=1,nc
               c(i,j)=2.0d0+c(i,j)
 31         continue
 30      continue
         do 40 i=1,mc
            do 41 j=1,nc
               w(i,j)=dble(i+j)
               d(i,j)=w(i,j)+c(i,j)
 41         continue
 40      continue
      endif
      end
      
