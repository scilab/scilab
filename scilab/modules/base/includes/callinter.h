c     Copyright INRIA 

c     this  include file contains code relative to interfaces calling. 
c     We use
c     include file instead of subroutine to avoid recursion pbs. This file
c     must be included in each routine which compute an external
 60   call  parse
      if(fun.eq.99) then
         fun=0
         goto 200
      endif
      if(err.gt.0) goto 97
c     

      if(int(rstk(pt)/100).eq.9) then
         ir=rstk(pt)-900
         if(ir.eq.1) then
c     .     back to matsys
            k=13
         elseif(ir.ge.2.and.ir.le.9) then
c     .     back to matio
            k=5
         elseif(ir.eq.10) then
c     .     end of overloaded function
            goto 96
         elseif(ir.gt.40) then
c     .     back to matus2
            k=24
         elseif(ir.gt.20) then
c     .     back to matusr
            k=14
         else
            goto 89
         endif
         iflagint=0
         goto 95
      endif
c
 89   if(top.lt.rhs ) then
         call error(22)
         goto 97
      endif

      if(top-rhs+lhs+1.ge.bot) then
         call error(18)
         goto 97
      endif
      toperr=top-max(0,rhs)
      goto 91
c     
 90   if(err.gt.0) goto 97
 91   k=fun
      fun=0
      if(k.eq.krec) then
         krec=-1
         call error(22)
         goto 97
      endif
      krec=-1
      if (k.eq.0 ) goto 60 
 95   continue

      if (.not.allowptr(k)) call ref2val
      krec=k

      call callinterf(k,iflagint)
C      if (k.eq.krec) krec=99999
      krec=-1
      if(fun.ge.0) then
         if (top-lhs+1.gt.0) call iset(rhs,0,infstk(top-lhs+1),1)
         if(paus.gt.0) goto 91
	 if (err1.gt.0) top=toperr
         goto 90
      endif
c     called interface ask for a scilab function to perform the function (fun=-1)
c     the function name is given in ids(1,pt+1)

c     call ref2val removed here because if forces overloading function to
C     be called by value
c     call ref2val 

      fun=0
      call funs(ids(1,pt+1))
      if(err.gt.0) goto 97
      if(fun.gt.0) then
         if (isbyref(fun).eq.0) call ref2val
         goto 91
      endif
      if(fin.eq.0) then
         call error(246)
         if(err.gt.0) goto 97
	 goto 90
      endif
      pt=pt+1
      fin=lstk(fin)
      rstk(pt)=910
      icall=5
      fun=0
c     *call*  macro
      goto 60
 96   pt=pt-1
      goto 90
c     error handling
 97   if(niv.gt.0.and.paus.gt.0) then
         fun=0
         goto 60
      endif
      goto 9999
