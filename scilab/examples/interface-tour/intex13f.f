      subroutine intex13f(fname)
      include 'stack.h'
c ------------------------------------
      character*(*) fname
      logical getrhscvar
      logical checklhs,checkrhs

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=1

       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

c   Get variable #1 , complex , double, real part in
c   stk(lr1), stk(lr1+1), ...           imag part in
c   stk(lc1), stk(lc1+1), ...

       if(.not.getrhscvar(1, 'd', it, m1, n1, lr1, lc1)) return
       call f99f(m1*n1, it, stk(lr1), stk(lc1))

       lhsvar(1)=1
       return
       end

       subroutine f99f(n, it, ar, ai)
       double precision ar(*), ai(*)
       do 1 k=1,n
          ar(k)=2.0d0*ar(k)
 1     continue
       if(it.eq.1) then
          do 2 k=1,n
          ai(k)=3.0d0*ai(k)
 2        continue
       endif
       return
       end

