c fin = 1 
c SCILAB function : scicos
c     Copyright INRIA
      subroutine intsscicos
      include '../stack.h'
      integer start,run,finish,flag,linear
      parameter (start=28,run=27,finish=15,linear=21)
      integer id(nsiz)
      double precision simpar(7)
      integer pointi
      integer funnum
      external funnum
c
      integer solver

      common /dbcos/ idb

      integer kfun
      common /curblk/ kfun
c
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      rhs = max(0,rhs)
c

        lbuf = 1
        lw = lstk(top+1)
        l0 = lstk(top+1-rhs)
        if (rhs .ne. 6) then
          call error(39)
          return
        endif
        if (lhs .gt. 2) then
          call error(41)
          return
        endif
c       checking variable state (number 1)
c       
        il1 = iadr(lstk(top-rhs+1))
        if (istk(il1) .ne. 16) then
          err = 1
          call error(56)
          return
        endif
        n1=istk(il1+1)
        l1=sadr(il1+n1+3)
c      
c       --   subvariable x0(state) --
        il1e2=iadr(l1+istk(il1+3)-1)
        n1e2 = istk(il1e2+1)
        l1e2 = sadr(il1e2+4)
c      
c       --   subvariable z0(state) --
        il1e3=iadr(l1+istk(il1+4)-1)
        n1e3 = istk(il1e3+1)
        l1e3 = sadr(il1e3+4)
c      
c       --   subvariable work(state) --
        il1e4=iadr(l1+istk(il1+5)-1)
        n1e4 = istk(il1e4+1)
        l1e4 = sadr(il1e4+4)
c      
c       --   subvariable tevts(state) --
        il1e5=iadr(l1+istk(il1+6)-1)
        n1e5 = istk(il1e5+1)
        l1e5 = sadr(il1e5+4)
c      
c       --   subvariable evtspt(state) --
        il1e6=iadr(l1+istk(il1+7)-1)
        n1e6 = istk(il1e6+1)
        l1e6 = sadr(il1e6+4)
c      
c       --   subvariable pointi(state) --
        il1e7=iadr(l1+istk(il1+8)-1)
        l1e7 = sadr(il1e7+4)
        pointi=stk(l1e7)
c      
c       --   subvariable outtb(state) --
        il1e8=iadr(l1+istk(il1+9)-1)
        n1e8 = istk(il1e8+1)
        nout = n1e8
        l1e8 = sadr(il1e8+4)

c      
c       checking variable tcur (number 2)
c       
        il2 = iadr(lstk(top-rhs+2))
        if (istk(il2) .ne. 1) then
          err = 2
          call error(53)
          return
        endif
        if (istk(il2+1)*istk(il2+2) .ne. 1) then
          err = 2
          call error(89)
          return
        endif
        l2 = sadr(il2+4)
c       checking variable tf (number 3)
c       
        il3 = iadr(lstk(top-rhs+3))
        if (istk(il3) .ne. 1) then
          err = 3
          call error(53)
          return
        endif
        if (istk(il3+1)*istk(il3+2) .ne. 1) then
          err = 3
          call error(89)
          return
        endif
        l3 = sadr(il3+4)
c       checking variable sim (number 4)
c       
        il4 = iadr(lstk(top-rhs+4))
        if (istk(il4) .ne. 16) then
          err = 4
          call error(56)
          return
        endif
        n4=istk(il4+1)
        l4=sadr(il4+n4+3)
c      
c       --   subvariable funptr(sim) --
        il4e2=iadr(l4+istk(il4+3)-1)
        if (istk(il4e2) .ne. 15) then
           err = 4
           call error(56)
           return
        endif
        nblk =istk(il4e2+1)
        l4e2 = sadr(il4e2+nblk+3)
c      
c       --   subvariable xptr(sim) --
        il4e3=iadr(l4+istk(il4+4)-1)
        n4e3 = istk(il4e3+1)
        l4e3 = sadr(il4e3+4)
c      
c       --   subvariable zptr(sim) --
        il4e4=iadr(l4+istk(il4+5)-1)
        n4e4 = istk(il4e4+1)
        l4e4 = sadr(il4e4+4)
c      
c       --   subvariable zcptr (sim) 
        il4e5=iadr(l4+istk(il4+6)-1)
        n4e5 = istk(il4e5+1)
        l4e5 = sadr(il4e5+4)
c      
c       --   subvariable inpptr(sim) --
        il4e6=iadr(l4+istk(il4+7)-1)
        n4e6 = istk(il4e6+1)
        ninpptr = n4e6
        l4e6 = sadr(il4e6+4)
c      
c       --   subvariable outptr(sim) --
        il4e7=iadr(l4+istk(il4+8)-1)
        n4e7 = istk(il4e7+1)
        noutptr = n4e7
        l4e7 = sadr(il4e7+4)
c      
c       --   subvariable inplnk(sim) --
        il4e8=iadr(l4+istk(il4+9)-1)
        n4e8 = istk(il4e8+1)
        ninplnk = n4e8
        l4e8 = sadr(il4e8+4)
c      
c       --   subvariable outlnk(sim) --
        il4e9=iadr(l4+istk(il4+10)-1)
        n4e9 = istk(il4e9+1)
        noutlnk =  n4e9
        l4e9 = sadr(il4e9+4)
c      
c       --   subvariable lnkptr(sim) --
        il4e10=iadr(l4+istk(il4+11)-1)
        n4e10 = istk(il4e10+1)
        nlnkptr = n4e10
        l4e10 = sadr(il4e10+4)
c      
c       --   subvariable rpar(sim) --
        il4e11=iadr(l4+istk(il4+12)-1)
        n4e11 = istk(il4e11+1)
        nrpar = n4e11
        l4e11 = sadr(il4e11+4)
c      
c       --   subvariable rpptr(sim) --
        il4e12=iadr(l4+istk(il4+13)-1)
        n4e12 = istk(il4e12+1)
        l4e12 = sadr(il4e12+4)
c      
c       --   subvariable ipar(sim) --
        il4e13=iadr(l4+istk(il4+14)-1)
        n4e13 = istk(il4e13+1)
        nipar = n4e13
        l4e13 = sadr(il4e13+4)
c      
c       --   subvariable ipptr(sim) --
        il4e14=iadr(l4+istk(il4+15)-1)
        n4e14 = istk(il4e14+1)
        l4e14 = sadr(il4e14+4)
c      
c       --   subvariable clkptr(sim) --
        il4e15=iadr(l4+istk(il4+16)-1)
        n4e15 = istk(il4e15+1)
        l4e15 = sadr(il4e15+4)
c      
c       --   subvariable ordptr(sim) --
        il4e16=iadr(l4+istk(il4+17)-1)
        n4e16 = istk(il4e16+1)*istk(il4e16+2)
        l4e16 = sadr(il4e16+4)
c      
c       --   subvariable execlk(sim) --
        il4e17=iadr(l4+istk(il4+18)-1)
        n4e17 = istk(il4e17+1)
        m4e17 = istk(il4e17+2)
        l4e17 = sadr(il4e17+4)
c      
c       --   subvariable ordclk(sim) --
        il4e18=iadr(l4+istk(il4+19)-1)
        n4e18 = istk(il4e18+1)
        m4e18 = istk(il4e18+2)
        l4e18 = sadr(il4e18+4)
c      
c       --   subvariable cord(sim) --
        il4e19=iadr(l4+istk(il4+20)-1)
        n4e19 = istk(il4e19+1)
        m4e19=  istk(il4e19+2)
        l4e19 = sadr(il4e19+4)
c      
c       --   subvariable oord(sim) --
        il4e20=iadr(l4+istk(il4+21)-1)
        n4e20 = istk(il4e20+1)
        m4e20=istk(il4e20+2)
        l4e20 = sadr(il4e20+4)
c      
c       --   subvariable zord(sim) --
        il4e21=iadr(l4+istk(il4+22)-1)
        n4e21 = istk(il4e21+1)
        m4e21=istk(il4e21+2)
        l4e21 = sadr(il4e21+4)
c      
c       --   subvariable critev(sim) --
        il4e22=iadr(l4+istk(il4+23)-1)
        n4e22 = istk(il4e22+1)
        l4e22 = sadr(il4e22+4)
c      
c       --   subvariable nb(sim) --
        il4e23=iadr(l4+istk(il4+24)-1)
        l4e23 = sadr(il4e23+4)
c
        if (stk(l4e23) .ne. nblk) then
          call error(42)
          return
       endif
c      
c       --   subvariable ztyp(sim) --
        il4e24=iadr(l4+istk(il4+25)-1)
        n4e24 = istk(il4e24+1)
        l4e24 = sadr(il4e24+4)
        lztyp=l4e24
c      
c       --   subvariable ndblk(sim) --
        il4e25=iadr(l4+istk(il4+26)-1)
        l4e25 = sadr(il4e25+4)
        ndblk=stk(l4e25)
c      
c       --   subvariable ndcblk(sim) --
        il4e26=iadr(l4+istk(il4+27)-1)
        l4e26 = sadr(il4e26+4)
c      
c       --   subvariable subscr(sim) --
        il4e27=iadr(l4+istk(il4+28)-1)
        n4e27 = istk(il4e27+1)
        l4e27 = sadr(il4e27+4)
c      
c       --   subvariable funtyp(sim) --
        il4e28=iadr(l4+istk(il4+29)-1)
        n4e28 = istk(il4e28+1)
        l4e28 = sadr(il4e28+4)
c      
c       --   subvariable iord(sim) --
        il4e29=iadr(l4+istk(il4+30)-1)
        n4e29 = istk(il4e29+1)
        m4e29=istk(il4e29+2)
        niord=n4e29
        l4e29 = sadr(il4e29+4)
c      
c       --   subvariable labels(sim) --
        if(n4.ge.30) then
           il4e30=iadr(l4+istk(il4+31)-1)
           n4e30 = istk(il4e30+1)*istk(il4e30+2)
           labptr=il4e30+4
           llab=labptr+n4e30+1
           nlab=istk(llab-1)-1
        else
           n4e30=nblk+1
           labptr=iadr(lw)
           llab=labptr+n4e30
           err=sadr(llab+1)-lstk(bot)
           if(err.gt.0) then
              call error(17)
              return
           endif
           nlab=1
           call iset(n4e30,1,istk(labptr),1)
        endif

c       --   subvariable modptr(sim) --
        il4e31=iadr(l4+istk(il4+32)-1)
        n4e31 = istk(il4e31+1)
        l4e31 = sadr(il4e31+4)
c      
c 

c
c       checking variable flag (number 5)
        il5 = iadr(lstk(top-rhs+5))
        if (istk(il5) .ne. 10) then
          err = 5
          call error(55)
          return
        endif
        if (istk(il5+1)*istk(il5+2) .ne. 1) then
          err = 5
          call error(89)
          return
        endif
        if(istk(il5+6).eq.start) then 
           flag=1
        elseif(istk(il5+6).eq.run) then 
           flag=2
        elseif(istk(il5+6).eq.finish) then 
           flag=3
        elseif(istk(il5+6).eq.linear) then 
           flag=4
        else
           err=5
           call error(44)
           return
        endif

        if(rhs.ge.6) then
c     checking variable simpar (number 6) 
c      [atol  rtol ttol, deltat, scale, impl, hmax]
c     
           il6 = iadr(lstk(top-rhs+6))
           if (istk(il6) .ne. 1) then
              err = 6
              call error(53)
              return
           endif
           m6 = istk(il6+1)*istk(il6+2)
           if (m6 .ne. 5.and. m6.ne.4.and. m6.ne.6.and. m6.ne.7) then
              err = 6
              call error(89)
              return
           endif
           l6 = sadr(il6+4)
        endif
c     
c       cross variable size checking
c     
        if (n1e5 .ne. n1e6) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e4) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e5) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e6) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e7) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e12) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e14) then
          call error(42)
          return
        endif
        if (n4e3 .ne. n4e15) then
          call error(42)
          return
        endif
c        if (m4e16.ne.2.and.n4e16.ne.0) then
c          call error(42)
c          return
c        endif
c        if (m4e17.ne.2.and.n4e17.ne.0) then
c          call error(42)
c          return
c        endif
        if (m4e18.ne.2.and.n4e18.ne.0) then
          call error(42)
          return
        endif
        if (m4e19.ne.2.and.n4e19.ne.0) then
          call error(42)
          return
        endif
        if (m4e20.ne.2.and.n4e20.ne.0) then
          call error(42)
          return
        endif
        if (m4e21.ne.2.and.n4e21.ne.0) then
          call error(42)
          return
        endif
        if (m4e29.ne.2.and.n4e29.ne.0) then
          call error(42)
          return
        endif

c     
c       cross formal parameter checking
c       not implemented yet
c     
c       cross equal output variable checking
c       not implemented yet
        call entier(n4e3,stk(l4e3),istk(iadr(l4e3)))
        call entier(n4e4,stk(l4e4),istk(iadr(l4e4)))
        call entier(n4e5,stk(l4e5),istk(iadr(l4e5)))
        call entier(n1e6,stk(l1e6),istk(iadr(l1e6)))
        call entier(n4e6,stk(l4e6),istk(iadr(l4e6)))
        call entier(n4e7,stk(l4e7),istk(iadr(l4e7)))
        call entier(n4e8,stk(l4e8),istk(iadr(l4e8)))
        call entier(n4e9,stk(l4e9),istk(iadr(l4e9)))
        call entier(n4e10,stk(l4e10),istk(iadr(l4e10)))
        call entier(n4e12,stk(l4e12),istk(iadr(l4e12)))
        call entier(n4e13,stk(l4e13),istk(iadr(l4e13)))
        call entier(n4e14,stk(l4e14),istk(iadr(l4e14)))
        call entier(n4e15,stk(l4e15),istk(iadr(l4e15)))
        call entier(n4e16,stk(l4e16),istk(iadr(l4e16)))
        call entier(n4e31,stk(l4e31),istk(iadr(l4e31)))
        call entier(n4e18*m4e18,stk(l4e18),istk(iadr(l4e18)))
        call entier(n4e19*m4e19,stk(l4e19),istk(iadr(l4e19)))
        call entier(n4e20*m4e20,stk(l4e20),istk(iadr(l4e20)))
        call entier(n4e21*m4e21,stk(l4e21),istk(iadr(l4e21)))
        call entier(n4e22,stk(l4e22),istk(iadr(l4e22)))
        call entier(1,stk(l4e23),istk(iadr(l4e23)))
        call entier(n4e24,stk(l4e24),istk(iadr(l4e24)))
        ilztyp=iadr(l4e24)
        call entier(1,stk(l4e25),istk(iadr(l4e25)))
        call entier(1,stk(l4e26),istk(iadr(l4e26)))
        call entier(n4e27,stk(l4e27),istk(iadr(l4e27)))
        call entier(n4e28,stk(l4e28),istk(iadr(l4e28)))
        ilfuntyp=iadr(l4e28)
        call entier(n4e29*m4e29,stk(l4e29),istk(iadr(l4e29)))
c
        if(m6.eq.4) then
           call dcopy(4,stk(l6),1,simpar,1)
           simpar(5)=  0.d0
           simpar(6)=  0.d0
           simpar(7)=  0.d0
        elseif(m6.eq.5) then
           call dcopy(5,stk(l6),1,simpar,1)
           simpar(6)=  0.d0
           simpar(7)=  0.d0
        elseif(m6.eq.6) then
           call dcopy(6,stk(l6),1,simpar,1)
           simpar(7)=  0.d0
        else
           call dcopy(7,stk(l6),1,simpar,1)
        endif
        solver=simpar(6)
c
        lfunpt=iadr(lw)
        lw=sadr(lfunpt+nblk)
c
        linpptr=iadr(l4e6)
        llnkptr=iadr(l4e10)
        iloutptr=iadr(l4e7)
c

c
c        ng=istk(iadr(l4e5)+n4e5-1)-1

        ilinp=iadr(l4e4)
        err=lw-lstk(bot)
        if (err .gt. 0) then
          call error(17)
          return
        endif
c
c     lock working area
        lstk(top+1)=lw
c     Set function table for blocks
        lf=l4e2
        do 10 i=1,nblk
           ilf=iadr(lf)
           if(istk(ilf).eq.11.or.istk(ilf).eq.13) then
C     Block is defined by a scilab function given in the structure
              istk(lfunpt-1+i)=-lf
           elseif(istk(ilf).eq.10) then
              buf=' '
              nn=istk(ilf+5)-1
              call cvstr(nn,istk(ilf+6),buf,1)
              buf(nn+1:nn+1)=char(0)
              ifun=funnum(buf(1:nn+1))
              if (ifun.gt.0) then
C     Block is defined by a C or Fortran procedure
                 istk(lfunpt-1+i)=ifun
              else
C     Block is defined by a predefined scilab function 
                 call namstr(id,istk(ilf+6),nn,0)
                 fin=0
                 call funs(id)
                 if (fun.eq.-1.or.fun.eq.-2) then 
                    istk(lfunpt-1+i)=-lstk(fin)
                 else
                    kfun=i
                    buf='unknown block :'//buf(1:nn)
                    call error(888)
                    return
                 endif
              endif
           else
              err=4
              call error(44)
              return
           endif
           lf=lf+istk(il4e2+2+i)-istk(il4e2+1+i)
 10     continue
c
        if(ddt.ne.0) idb=1
c
        call scicos(stk(l1e2),istk(iadr(l4e3)),stk(l1e3),
     $       stk(l1e4),istk(ilinp),
     $       istk(iadr(l4e31)),
     &       istk(llab),istk(labptr),stk(l2),stk(l3),stk(l1e5),
     $       istk(iadr(l1e6)),n1e5,pointi,stk(l1e8),nout,
     $       istk(lfunpt),istk(iadr(l4e28)),istk(linpptr),
     $       istk(iloutptr),istk(iadr(l4e8)),istk(iadr(l4e9)),
     $       istk(llnkptr),nlnkptr,stk(l4e11),istk(iadr(l4e12)),
     $       istk(iadr(l4e13)),istk(iadr(l4e14)),istk(iadr(l4e15)),
     $       istk(iadr(l4e16)),n4e16,
     $       istk(iadr(l4e18)),istk(iadr(l4e19)),n4e19,
     $       istk(iadr(l4e29)),n4e29,
     $       istk(iadr(l4e20)),n4e20,istk(iadr(l4e21)),n4e21,
     $       istk(iadr(l4e22)),nblk,istk(iadr(l4e24)),istk(iadr(l4e5)),
     $       istk(iadr(l4e27)),n4e27,simpar,flag,ierr)
        idb=0
        if (ierr .gt. 0 ) then
           if(ierr.eq.1) then
              buf='scheduling problem'
              kfun=0
           elseif(ierr.eq.2) then
              buf='input to zero-crossing stuck on zero'
              kfun=0
           elseif(ierr.eq.6) then
              buf='a block has been called with input out of its domain'
           elseif(ierr.eq.7) then
              buf='singularity in a block'
           elseif(ierr.eq.8) then
              buf='block produces an internal error'
           elseif(ierr.ge.1000) then
              buf='unknown or erroneous block'
           elseif(ierr.ge.100) then
              istate=-(ierr-100)
              write(buf,'(''integration problem istate='',i5)') istate
              kfun=0
           elseif(ierr.eq.3) then
              buf='event conflict'
              kfun=0
           elseif(ierr.eq.20) then
              buf='initial conditions not converging'
              kfun=0
           elseif(ierr.eq.4) then
              buf='algrebraic loop detected'
              kfun=0
           elseif(ierr.eq.5) then
              buf='cannot allocate memory'
              kfun=0
           elseif(ierr.eq.21) then
              write(buf,'(''cannot allocate memory in block='',i5)') 
     $             kfun
              kfun=0
           elseif(ierr.eq.22) then
              buf='sliding mode condition, cannot integrate'
              kfun=0
           else
              buf='scicos unexpected error,please report...'
              kfun=0
           endif
           call error(888)
           fun=0
           return
        endif
        if (err .gt. 0) return
c     
        fun=0
        top=top-rhs
c     
        if(lhs .ge. 1) then
c     
c       output variable: x0(x)
c     
c     change pointi to double
           stk(l1e7)=pointi
c     change iz to double
c           call int2db(n1e4,istk(iadr(l1e4)),-1,stk(l1e4),-1)
c     change evtspt to double
           call int2db(n1e6,istk(iadr(l1e6)),-1,stk(l1e6),-1)
           top=top+1
        endif
c     
        if(lhs .ge. 2) then
c     
c       output variable: t
c     
        top=top+1
        endif
c     
        return
      end
c
      subroutine intctree
      include '../stack.h'
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw = lstk(top+1)
      ilw=iadr(lw)
      top=top-rhs+1
      itop=top
      ilv=iadr(lstk(top))
      lv=sadr(ilv+4)
      nb=istk(ilv+1)*istk(ilv+2)
      call entier(nb,stk(lv),istk(iadr(lv)))
      top=top+1
      iln=iadr(lstk(top))
      ln=sadr(iln+4)
      nnb=istk(iln+1)*istk(iln+2)
      call entier(nnb,stk(ln),istk(iadr(ln)))
      top=top+1
      ild=iadr(lstk(top))
      ld=sadr(ild+4)
      nnb=istk(ild+1)*istk(ild+2) 
      call entier(nnb,stk(ld),istk(iadr(ld)))
      top=top+1
      ilo=iadr(lstk(top))
      lo=sadr(ilo+4)
      nnb=istk(ilo+1)*istk(ilo+2) 
      call entier(nnb,stk(lo),istk(iadr(lo)))
      top=top+1
      ilc=iadr(lstk(top))
      lc=sadr(ilc+4)
      nnb=istk(ilc+1)*istk(ilc+2) 
      call entier(nnb,stk(lc),istk(iadr(lc)))
      ilord=ilw
      ilw=ilw+nb
      lw = sadr(ilw + nb)
      err = lw - lstk(bot )
      if (err .gt. 0) then
         call error(17 )
         return
      endif
      call sctree(nb,istk(iadr(lv)),istk(iadr(ln)),
     &     istk(iadr(ld)),istk(iadr(lo)),
     &     istk(iadr(lc)),istk(ilord),nord,iok,istk(ilw))
      top=itop
      istk(ilv+1)=nord
      istk(ilv+2)=1
      call int2db(nord,istk(ilord),1,stk(lv),1)
      lstk(top+1)=lv+nord
      top=top+1
      ilv1=iadr(lstk(top))
      istk(ilv1)=1
      istk(ilv1+1)=1  
      istk(ilv1+2)=1
      istk(ilv1+3)=0
      lv1=sadr(ilv1+4)
      stk(lv1)=dble(iok)
      lstk(top+1)=lv1+1
      return
      end


c  interface function 
c     ********************
      subroutine  intscicos
c     
      include '../stack.h'
c     
      logical putlhsvar,checkrhs,checklhs
      character*(nlgh) fname
c
      goto (1,2,3,4,5,6,7,8) fin 
      return
 1    call intsscicos
      return
 2    call intctree
      return
 3    continue
      fname='sci_tree2'
c     [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,4,4)) return
      call intree2(fname)
      goto 9988
 4    continue
      fname='sci_tree3'
c     [ord,ok]=tree3(vec,dep_ut,typ_l,bexe,boptr,blnk,blptr)
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,7,7)) return
      call intree3(fname)
      goto 9988
 5    continue
      fname='sci_tree4'
c     [r1,r2,ok]=tree4(vec,outoin,outoinptr,dep_ut,typ_r)
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,5,5)) return
      call intree4(fname)
      goto 9988
 6    continue
      fname='realtimeinit'
      call intsrealtimeinit(fname)
      goto 9988
 7    continue
      fname='realtime'
      call intsrealtime(fname) 
      goto 9988
 8    continue
c     scicos_debug(i)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,1,1)) return
      fname='scicos_debug'
      call scicosdebug(fname)
      goto 9988
 9988 if(.not.putlhsvar())return
      end


c     ********************
      subroutine  intree2(fname)
c     
      include '../stack.h'
c     
      logical createvar,getrhsvar
      character*(*) fname
c
      iadr(l)=l+l-1
      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
      if(.not.getrhsvar(2,'i',noin,moin,ipoin)) return
      if(.not.getrhsvar(3,'i',noinr,moinr,ipoinr)) return
      if(.not.getrhsvar(4,'i',ndep,mdep,ipdep)) return
c
      if(.not.createvar(5,'i',nvec*mvec,1,ipord)) return
      if(.not.createvar(6,'i',1,1,ipok)) return
c
      call ftree2(istk(ipvec),nvec*mvec,istk(ipdep),
     $     istk(ipoin),istk(ipoinr),istk(ipord),
     $     nord,istk(ipok))
      lhsvar(1)=5
      lhsvar(2)=6
c      nbrows(5)=nord
      istk(iadr(iwhere(5))+1)=nord
      end


c     ********************
      subroutine  intree3(fname)
c     
      include '../stack.h'
c     
      logical createvar,getrhsvar
      character*(*) fname
c
      iadr(l)=l+l-1
      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
      if(.not.getrhsvar(2,'i',ndep,mdep,ipdep)) return
      if(.not.getrhsvar(3,'i',ntyp,mtyp,iptyp)) return
      if(.not.getrhsvar(4,'i',nbex,mbex,ipbex)) return
      if(.not.getrhsvar(5,'i',nbop,mbop,ipbop)) return
      if(.not.getrhsvar(6,'i',nbln,mbln,ipbln)) return
      if(.not.getrhsvar(7,'i',nblr,mblr,ipblr)) return

c
      if(.not.createvar(8,'i',nvec*mvec,1,ipord)) return
      if(.not.createvar(9,'i',1,1,ipok)) return
      if(.not.createvar(10,'i',1,nvec*mvec,ipkk)) return
c
      call ftree3(istk(ipvec),nvec*mvec,istk(ipdep),
     $     istk(iptyp),istk(ipbex),istk(ipbop),istk(ipbln),
     $     istk(ipblr),istk(ipkk),istk(ipord),nord,
     $     istk(ipok))
      lhsvar(1)=8
      lhsvar(2)=9
c      nbrows(8)=nord
      istk(iadr(iwhere(8))+1)=nord
      end


c     ********************
      subroutine  intree4(fname)
c     
      include '../stack.h'
c     
      logical createvar,getrhsvar
      character*(*) fname
c
      iadr(l)=l+l-1
      if(.not.getrhsvar(1,'i',nvec,mvec,ipvec)) return
      if(.not.getrhsvar(2,'i',noin,moin,ipoin)) return
      if(.not.getrhsvar(3,'i',noinr,moinr,ipoinr)) return
      if(.not.getrhsvar(4,'i',nnd,mnd,ipnd)) return
      if(.not.getrhsvar(5,'i',ntyp,mtyp,iptyp)) return

c
      if(.not.createvar(6,'i',1,nnd*mnd,ipr1)) return
      if(.not.createvar(7,'i',1,nnd*mnd,ipr2)) return
c
      call ftree4(istk(ipvec),nvec*mvec,istk(ipnd),mnd,
     $     istk(iptyp),istk(ipoin),istk(ipoinr),istk(ipr1),
     $     istk(ipr2),nr)
      lhsvar(1)=6
      lhsvar(2)=7
c      nbcols(6)=nr
      istk(iadr(iwhere(6))+2)=nr
c      nbcols(7)=nr
      istk(iadr(iwhere(7))+2)=nr
      end

c     ********************
      subroutine  scicosdebug(fname)
c     
      include '../stack.h'
c     
      common /cosdebug/ cosd
      logical getrhsvar
      integer cosd
      character*(*) fname
c
      if(.not.getrhsvar(1,'i',n,m,i)) return
      cosd=istk(i)
      lhsvar(1)=0
      end
