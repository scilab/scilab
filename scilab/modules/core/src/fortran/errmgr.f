c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

c
c			
c
      subroutine errmgr(n,errtyp)
c     -------------------------
c     this routines handle errors: recursion stack cleaning, error
c     recovery, display of calling tree
c     n      : the error number
c     errtyp : error type (recoverable:0 or not:1)
c!
      include 'stack.h'
      integer errtyp,n
c
      integer num,imess,imode,lunit
      integer ll,r,p,mode(2),pt0
      logical first,trace,pflag,erecmode
c
      ll=lct(5)
      first=.true.
      lunit=wte
c
      call errmds(num,imess,imode)
      trace=.not.((num.lt.0.or.num.eq.n).and.imess.ne.0)
c
      erecmode=(num.eq.n.or.num.lt.0).and.imode.ne.0.and.imode.ne.3
c     
      pt0=0
      if(pt.le.pt0) goto 50
      if(erecmode) then
c     error recovery mode
         p=pt+1
c        . looking if error has occurred in execstr deff getf or comp
 20      p=p-1
         if(p.le.errpt) then
            pt0=pt
            goto 50
         endif
         if(rstk(p).eq.1001.or.rstk(p).eq.1002) then
c     .     error has occurred in an external
            errtyp=0
            pt0=p
         elseif(rstk(p).eq.808.or.rstk(p).eq.618) then
c     .     error has occurred in a try instructions
            errtyp=0
            pt0=p
         elseif(rstk(p).eq.501.and.catch.eq.0) then
            if (rstk(p-1).eq.909) then
c     .        exec of a function
               goto 20
            endif
c     .     error has occurred in a compiled macro
            errtyp=0
            pt0=p
         elseif(rstk(p).eq.502) then 
            if(rstk(p-1).eq.903.and.catch.eq.0) then
c     .     error has occurred in execstr
               errtyp=0
               pt0=p
            elseif(rstk(p-1).eq.904.or.rstk(p-1).eq.901) then
c     .     error has occurred in comp
               errtyp=0
               pt0=p
            else
               goto 20
            endif
         elseif(rstk(p).eq.612) then
c     .      error has occurred in a for called by a compiled macro
            if (catch.ne.0) then
c     .        under errcatch('continue') or errcatch('pause') mode: let the loop continue
               pt0=p
               errtyp=0
            else
c     .        under try or execstr(...,'errcatch'): terminates the loop
c     .        take into account the for loop variable (in a compiled macro)
               toperr=ids(4,p)
               goto 20
            endif
         else
            goto 20
         endif
      endif

c
c depilement de l'environnement
      lct(4)=2
      pt=pt+1
 35   pt=pt-1
      if(pt.eq.pt0) goto 50
      r=rstk(pt)
      goto(36,36,37) r-500
      if(r.eq.904) then
         if (ids(2,pt).ne.0) then
c     .     getf(  'c') case, close the file
            mode(1)=0
            call clunit(-ids(2,pt),buf,mode)
         endif
      endif
      goto 35
c     
c     on depile une fonction
 36   call depfun(lunit,trace,first)
      goto 35
c     
c     on depile un exec ou une pause
 37   call depexec(lunit,trace,first,pflag)
      if(.not.pflag) goto 35
c     
 50   continue
c     if(pt.gt.0.and.rstk(pt).eq.1001) pt=pt+1
      if(erecmode) then
         if(errtyp.eq.0) then
c     .     recoverable error
            top=toperr
            if(err2.eq.0) then
               err1=n
            else
               err1=err2
            endif
            err=0
         else
            comp(1)=0
            comp(3)=0
            err=n
         endif
      else
         comp(1)=0
         comp(3)=0
         err=n
      endif
      if(trace) call basout(io,lunit,' ')
c     
      return
      end
c     -------------------------
