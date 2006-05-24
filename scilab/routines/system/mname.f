       subroutine mname(op,id)
c     ================================================
c     searches a macro name for overloaded  operations
c     ================================================
c     Copyright INRIA
      include '../stack.h'
      integer gettype
      logical ilog,getilist,getsmat,first
c
      parameter (nops=33)
      integer op,id(nsiz),name(nlgh),blank,percen,under,id1(nsiz)
      integer ops(nops),code(nops),top1,rhs1,codop
      integer compat
      integer iadr
      data compat/1/
      data blank/40/,percen/56/,under/36/
      data ops /53,45,46,47,48,49,62, 1, 2, 3,4,98,99,100,200,201,202,
     &          44,149,150,151,50,119,57,58,113,61,104,59,60,109,110,5/
      data code/29,10,28,22,27,21,25,12,18,14,15,33,13,26,20, 34, 35, 
     &          11,30, 31, 32, 24, 23, 16, 17,19,5,0,1,2,3,4,6/
c
c     operators codes
c     ---------------
c     '  +  -  *  /  \  **  []  ()  .*  ./  .\  .*.  ./.  .\.   :  *. /.
c     t  a  s  m  r  l  p   c,f e,i  x   d    q   k    y    z   b  u   v
c     w   ==  <>  |  &  .^  ~ .' <  >  <=  >= ()
c     \.   o  n   g  h   j  5 0  1  2  3   4  6
c
c     type codes
c     ----------
c     characters  polynomial macros libraries  mat of numbers  list
c     c           p          m      f          s                l
c
c     booleen sparse   booleen_sparse
c     b       sp       spb
c
c     tlist: up to 8 first characters of the tlist type
c
      iadr(l)=l+l-1
c
      rhs1=rhs
      first=.true.

      if (op.eq.3.or.op.eq.5) then
         rhs1=1
      elseif (op.eq.2) then
         rhs1=2
      else
         rhs1=rhs
      endif

 10   name(1)=percen
      k=2
c
      top1=top-rhs1
c
c     get code associated to first operand 
      top1=top1+1
      call typ2cod(iadr(lstk(top1)),name(k),n)
      if(n.eq.0) goto 99
      k=k+n
c
c     get code associated with operator
      do 52 i=1,nops
         if(ops(i).eq.op) goto 53
   52 continue
      goto 99
   53 codop=code(i)
      if(first) then
         name(k)=under
         name(k+1)=codop
         if(rhs1.gt.1) then 
            name(k+2)=under
            k=k+3
         else
            k=k+2
         endif
      else
         name(k)=codop
         k=k+1
      endif
c
      if(rhs1.eq.1) goto 54
c
c     get code associated to second operand 
      top1=top1+1
      call typ2cod(iadr(lstk(top1)),name(k),n)
      if(n.eq.0) goto 99
      k=k+n
c
   54 call namstr(id,name,k-1,0)
      fin=0
      call funs(id)
      if(fun.eq.0) then
         if(rhs1.eq.1) then
            if (gettype(top).ge.15.and.gettype(top).le.17) then
               name(1)=percen
               name(2)=21
               name(3)=under
               name(4)=codop
               call namstr(id,name,4,0)
               fin=0
               call funs(id)
               if(fun.ne.0) goto 55
            endif
         else
            if ((gettype(top).ge.15.and.gettype(top).le.17).and.
     $           (gettype(top-1).ge.15.and.gettype(top-1).le.17)) then
c     tlist comparison, use general list comparison function 
c     %l_o_l or %l_n_l  instead of undefined type dependent one.
               name(1)=percen
               name(2)=21
               name(3)=under
               name(4)=codop
               name(5)=under
               name(6)=21
               call namstr(id,name,6,0)
               fin=0
               call funs(id)
               if(fun.ne.0) goto 55
            endif
         endif
         if(first) then
            first=.false.
            call putid(id1,id)
            goto 10
         else
            call  putid(ids(1,pt+1),id1)
            call error(144)
            return
         endif
      endif
 55   if(fun.eq.-2) then 
         fin=-1
         call stackg(id)
      endif
      return
c
   99 continue
      call error(43)
      id(1)=blank
      return
      end
