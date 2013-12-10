c
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine daskri(fname)
c ====================================================================
C     daskr
c ====================================================================
c
c     This code was inspired by sci_f_dasrt.f from the same folder.
c     daskr() has more optional parameters than dasrt(), so the gateway
c has to adapt to the new options
      INCLUDE 'stack.h'
c
      character*(*) fname
      character*(nlgh+1) namjac
      common/cjac/namjac
      integer iadr,sadr,gettype
c
      double precision atol,rtol,t0
      integer info(20),topk,topw
      integer LENWP, LENIWP, maxl, kmp, nrmax, mxnit, mxnj, mxnh, lsoff
      logical hotstart,type,getexternal,getrvect
      logical checkrhs,checklhs,getrmat,cremat,getscalar
      double precision tout,tstop,maxstep,stepin
      double precision epli, stptol, epinit
      character*(nlgh+1) namer,namej,names,namep,namepj
      common /dassln/ namer,namej,names,namep,namepj
      external bresd,bjacd,bsurfd,bpsold,bpjacd
      external setfresd,setfjacd,setfsurfd,setfpsold,setfpjacd
c
      data atol/1.d-7/,rtol/1.d-9/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c     SCILAB function : daskr
c     --------------------------
c     [y0,nvs[,hotdata]]=daskr(y0,t0,t1[,atol[,rtol]],res[,jac],nh,h[,info
c     [,psol][,pjac]][,hotdata])
      ierror=0
      maxord=5
      lbuf = 1
      topk=top
      topw=top+1
      lw = lstk(topw)
      l0 = lstk(top+1-rhs)
      if (.not.checkrhs(fname,6,13)) return
      if (.not.checklhs(fname,2,3)) return
c     Checking variable y0 (number 1)
c     -------------------------------
      ky=top-rhs+1
      if(.not.getrmat(fname,topk,ky,n1,m1,l1))return
      neq=n1
      lydot=l1+n1
      info(11)=0
      if (m1 .eq.1) then
         if (.not.cremat(fname,topw,0,n1,1,lydot,lc)) return
         topw=topw+1
         info(11)=1
         call dset(n1,0.0d0,stk(lydot),1)
      elseif(m1.ne.2) then
         err = 1
         call error(89)
         return
      else
         il1 = iadr(lstk(top-rhs+1))
         istk(il1+2)=1
      endif
c     Checking variable t0 (number 2)
c     ----------------------------
      kt0=top-rhs+2
      if(.not.getscalar(fname,topk,kt0,lr2))return
      t0=stk(lr2)
c     Checking variable t1 (number 3)
c     -------------------------------
      if(.not.getrmat(fname,topk,top-rhs+3,m3,n3,l3))return
      nt=m3*n3
c
c     Checking variable atol (number 4)
c     --------------------------------
      iskip=0
      itype = gettype(top-rhs+4)
      if ( itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,latol,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(latol)=atol
         stk(lrtol)=rtol
         info(2)=0
         iskip=iskip+2
         goto 1105
      endif
      if(.not.getrvect(fname,topk,top-rhs+4,m4,n4,latol))return
      m4 = m4*n4
c     Checking variable rtol (number 5)
c     --------------------------------
      itype = gettype(top-rhs+5)
      if (itype .ne. 1) then
         if (.not.cremat(fname,topw,0,1,1,lrtol,lc)) return
         topw=topw+1
         stk(lrtol)=lrtol
         info(2)=0
         iskip=iskip+1
         goto 1105
      endif
      if(.not.getrvect(fname,topk,top-rhs+5,m5,n5,lrtol))return
      m5 = m5*n5
      if(m5.ne.m4.or.(m5.ne.1.and.m5.ne.neq)) then
         call error(60)
         return
      endif
      if(m5.eq.1) then
         info(2)=0
      else
         info(2)=1
      endif

c     Checking variable res (number 6)
c     --------------------------------
 1105 kres=top-rhs+6-iskip
      if (.not.getexternal(fname,topk,kres,namer,type,
     $     setfresd)) return

c     Checking variable number 7
c     -----------------------------
      kjac=top-rhs+7-iskip
      if(kjac.gt.top) then
         iskip=iskip+1
         info(5)=0
      else
         is7 = gettype(kjac)
         if(is7.eq.15) then
c     .     info or jac ? get first element type to decide
            il7=iadr(lstk(kjac))
            if (istk(il7).lt.0)  il7=istk(il7+1)
            nelt=istk(il7+1)
            l71=sadr(il7+3+nelt)
            if (abs(istk(iadr(l71))).eq.11.or.
     $           abs(istk(iadr(l71))).eq.13) then
c     .        jac
               is7=istk(iadr(l71))
            endif
         endif
         if((is7.ne.10).and.(is7.ne.11).and.(is7.ne.13)) then
            iskip=iskip+1
            info(5)=0
         else
            info(5)=1
            if (.not.getexternal(fname,topk,kjac,namej,type,
     $           setfjacd)) return
         endif
      endif
c     DASKR nh, h
c     Checking variable number 8
c     -----------------------------
      if(.not.getscalar(fname,topk,top-rhs+8-iskip,lr8))return
      nh=int(stk(lr8))
c     Checking variable number 9
      ksurf=top-rhs+9-iskip
      if (.not.getexternal(fname,topk,ksurf,names,type,
     $        setfsurfd)) return
c
c     Checking variable info (number 10)
c     ------------------------------------
      kinfo = top-rhs+10-iskip
      if (kinfo.gt.top) then
         info(4)=0
         info(3)=0
         info(6)=0
         info(7)=0
         info(8)=0
         info(10)=0
         info(11)=0
         info(12)=0
         info(13)=0
         info(14)=0
         info(15)=0
         info(16)=0
         info(17)=0
         info(18)=0
         iskip=iskip+1
         goto 8
      endif
      il10 = iadr(lstk(top-rhs+10-iskip))
      if (istk(il10) .ne. 15) then
c     Default info values
         info(4)=0
         info(3)=0
         info(6)=0
         info(7)=0
         info(8)=0
         info(10)=0
         info(11)=0
         info(12)=0
         info(13)=0
         info(14)=0
         info(15)=0
         info(16)=0
         info(17)=0
         info(18)=0
         iskip=iskip+1
         goto 8
      endif
      n10=istk(il10+1)
      l10=sadr(il10+n10+3)
c
c     --   subvariable tstop(info) --
      il10e1=iadr(l10+istk(il10+1+1)-1)
      l10e1 = sadr(il10e1+4)
      m10e1 = istk(il10e1+1)*istk(il10e1+2)
      if(m10e1.eq.0) then
         info(4)=0
      else
         info(4)=1
         tstop=stk(l10e1)
      endif

c
c     --   subvariable imode(info) --
      il10e2=iadr(l10+istk(il10+1+2)-1)
      l10e2 = sadr(il10e2+4)
      info(3)=stk(l10e2)

c
c     --   subvariable band(info) --
      il10e3=iadr(l10+istk(il10+1+3)-1)
      m10e3 =istk(il10e3+2)*istk(il10e3+2)
      l10e3 = sadr(il10e3+4)
      if(m10e3.eq.0) then
         info(6)=0
      elseif(m10e3.eq.2) then
         info(6)=1
         ml=stk(l10e3)
         mu=stk(l10e3+1)
      else
         err=10-iskip
         call error(89)
         return
      endif

c
c     --   subvariable maxstep(info) --
      il10e4=iadr(l10+istk(il10+1+4)-1)
      m10e4 =istk(il10e4+2)*istk(il10e4+2)
      l10e4 = sadr(il10e4+4)
      if(m10e4.eq.0) then
         info(7)=0
      else
         info(7)=1
         maxstep=stk(l10e4)
      endif

c
c     --   subvariable stepin(info) --
      il10e5=iadr(l10+istk(il10+1+5)-1)
      m10e5 =istk(il10e5+2)*istk(il10e5+2)
      l10e5 = sadr(il10e5+4)
      if(m10e5.eq.0) then
         info(8)=0
      else
         info(8)=1
         stepin=stk(l10e5)
      endif

c
c     --   subvariable nonneg(info) --
      il10e6=iadr(l10+istk(il10+1+6)-1)
      l10e6 = sadr(il10e6+4)
      info(10)=stk(l10e6)

c
c     --   subvariable consistent(info) --
      il10e7=iadr(l10+istk(il10+1+7)-1)
      m10e7 =istk(il10e7+2)*istk(il10e7+2)
      l10e7 = sadr(il10e7+4)
      if(m10e7.eq.0.or.(m10e7.eq.1.and.stk(l10e7).eq.0)) then
c        info is then [] or [0]
         info(11)=0
      else
c        info then looks like list(..., [+-1 +-1 ... +-1 +-1],...)
         info(11)=1
         if (info(10).eq.0.or.info(10).eq.2) then
            LID = 40
         else
            LID = 40+neq
         endif
c        Make no further changes here, but copy the [+-1] tab
c in 'iwork' once its size 'liw' is defined.
      endif

c
c     --   subvariable iteration(info) --
      il10e8=iadr(l10+istk(il10+1+8)-1)
      l10e8 = sadr(il10e8+4)
      iter=stk(l10e8)
      if(iter.eq.1) then
         info(12)=1
      else
         info(12)=0
      endif

c
c     --   subvariable defaultKrylov(info) --
      il10e9=iadr(l10+istk(il10+1+9)-1)
      m10e9 =istk(il10e9+2)*istk(il10e9+2)
      l10e9 = sadr(il10e9+4)
      if(m10e9.eq.0) then
         info(13)=0
c        maxl and kmp need default values
         maxl  = min(5,neq)
         kmp   = maxl
      else
c     info then looks like list(..., [maxl kmp nrmax epli],...)
         info(13)=1
         maxl  = stk(l10e9)
         kmp   = stk(l10e9+1)
         nrmax = stk(l10e9+2)
         epli  = stk(l10e9+3)
      endif

c
c     --   subvariable justConsistentComp(info) --
      il10e10=iadr(l10+istk(il10+1+10)-1)
      l10e10 = sadr(il10e10+4)
      proceed=stk(l10e10)
      if(proceed.eq.0) then
         info(14)=0
      else
c        Check that info(11) = 1, meaning that the provided initial values
c        are not consistent
         if (info(11).eq.1)  info(14) = 1
      endif

c
c     --   subvariable psolJac(info) --
      il10e11=iadr(l10+istk(il10+1+11)-1)
      l10e11 = sadr(il10e11+4)
      psolJac=stk(l10e11)
      if (psolJac.eq.0) then
         info(15)=0
      else
         info(15)=1
      endif

c
c     --   subvariable excludeAlgebraic(info) --
      il10e12=iadr(l10+istk(il10+1+12)-1)
      m10e12 =istk(il10e12+2)*istk(il10e12+2)
      l10e12 = sadr(il10e12+4)
      if (m10e12.eq.0) then
         info(16)=0
      else
c     info then looks like list(..., [+-1 +-1 ... +-1 +-1],...)
         info(16)=1
         if (info(10).eq.0.or.info(10).eq.2) then
            LID = 40
         else
            LID = 40+neq
         endif
c        Make no further changes here, but copy the [+-1] tab
c in 'iwork' once its size 'liw' is defined.
      endif

c
c     --   subvariable defaultHeuristic(info) --
      il10e13=iadr(l10+istk(il10+1+13)-1)
      m10e13 =istk(il10e13+2)*istk(il10e13+2)
      l10e13 = sadr(il10e13+4)
      if (m10e13.eq.0) then
         info(17)=0
      else
c  info then looks like list(..., [mxnit mxnj lsoff stptol epinit],...)
         info(17)=1
         mxnit  = stk(l10e9)
         mxnj   = stk(l10e9+1)
         mxnh   = stk(l10e9+2)
         lsoff  = stk(l10e9+3)
         stptol = stk(l10e9+4)
         epinit = stk(l10e9+5)
      endif

c
c     --   subvariable verbosity(info) --
      il10e14=iadr(l10+istk(il10+1+14)-1)
      l10e14 = sadr(il10e14+4)
      verbosity=stk(l10e14)
      if (verbosity.eq.1) then
         info(18)=1
      elseif(verbosity.eq.2) then
         info(18)=2
      else
         info(18)=0
      endif
c
c     Checking variable psol (number 11)
c     --------------------------------------
8      kpsol=top-rhs+11-iskip
      if (kpsol.gt.top) then
         iskip = iskip + 1
         go to 9
      endif
      il11  = iadr(lstk(top-rhs+11-iskip))
      pType = istk(il11)
      if((pType.ne.10).and.(pType.ne.11).and.(pType.ne.13)) then
         iskip = iskip + 1
         go to 9
      endif
      if (.not.getexternal(fname,topk,kpsol,namep,type,
     $        setfpsold)) return
c
c     Checking variable pjac (number 12)
c     --------------------------------------
9      kpjac=top-rhs+12-iskip
      if (kpjac.gt.top) then
         iskip = iskip + 1
         go to 10
      endif
      il12  = iadr(lstk(top-rhs+12-iskip))
      pType = istk(il12)
      if((pType.ne.10).and.(pType.ne.11).and.(pType.ne.13)) then
         iskip = iskip + 1
         go to 10
      endif
      if (.not.getexternal(fname,topk,kpjac,namepj,type,
     $        setfpjacd)) return

 10   hotstart=.false.
      if(rhs.eq.13-iskip) then
         hotstart=.true.
c
c     Checking variable hotdata (number 13)
c     --------------------------------------
         il13 = iadr(lstk(top-rhs+13-iskip))
         if (istk(il13) .ne. 1) then
            err = 13-iskip
            call error(53)
            return
         endif
         n13 = istk(il13+1)*istk(il13+2)
         lhot = sadr(il13+4)
      elseif(rhs.ne.12-iskip) then
         call error(39)
         return
      endif

c     --------------------Work Tables
      if (.not.cremat(fname,topw,0,1,2,lw15,lc)) return
      topw=topw+1
      if (.not.cremat(fname,topw,0,1,30,lw17,lc)) return
      topw=topw+1
      il17=iadr(lw17)
c     Set ipar and rpar to 0 by default
      do 98 i=0,1
         istk(iadr(lw15)+i) = 0
98    continue
      do 99 i=0,29
         istk(il17+i) = 0
99    continue
c     daskr needs more
      if (.not.cremat(fname,topw,0,1,nh,lgr,lc)) return
      topw=topw+1
      lgroot=iadr(lgr)
c
c     base = lrw = 60 then augment size
c     according to the case (full dense, banded, ...)
      base = 60
      if (info(12).eq.0) then
         lrw = base + max(maxord+4,7)*neq + 3*nh
         if (info(6).eq.0) then
c        For the full (dense) JACOBIAN case
            lrw = lrw + neq**2
         elseif(info(5).eq.1) then
c        For the banded user-defined JACOBIAN case
            lrw = lrw + (2*ml+mu+1)*neq
         elseif(info(5).eq.0) then
c        For the banded finite-difference-generated JACOBIAN case
            lrw = lrw + (2*ml+mu+1)*neq + 2*(neq/(ml+mu+1)+1)
         endif
      elseif(info(12).eq.1) then
c        LENWP is the length ot the rwork segment containing
c        the matrix elements of the preconditioner P
         LENWP = neq*neq
         lrw = base + (maxord+5)*neq + 3*nh
     $                   + (maxl + 3 + min(1,maxl-kmp))*neq
     $                   + (maxl+3)*maxl + 1 + LENWP
      endif
      if(info(16).eq.1) lrw = lrw + neq

c
c     base = liw = 40, then augment size according to the case
      base = 40
      if (info(12).eq.0) then
         liw = base + neq
      elseif(info(12).eq.1) then
c        LENIWP is the length ot the iwork segment containing
c        the matrix indexes of the preconditioner P
c        (compressed sparse row format)
         LENIWP = 2*neq*neq
         liw = base + LENIWP
      endif
      if(info(10).eq.1.or.info(10).eq.3) liw = liw + neq
      if(info(11).eq.1.or.info(16).eq.1) liw = liw + neq
      if(.not.hotstart) then
         if (.not.cremat(fname,topw,0,1,lrw,lrwork,lc)) return
         topw=topw+1
         if (.not.cremat(fname,topw,0,1,sadr(liw)+1,liwork,lc)) return
         topw=topw+1
      else
         if(lrw+liw.gt.n13) then
c           Test if output (/input) array 'hotdata' is large enough
c           to hold at least rwork and iwork
            err=12-iskip
            call error(89)
            return
         endif
         lrwork=lhot
         liwork=lhot+lrw
         call entier(liw,stk(liwork),istk(iadr(liwork)))
      endif
c
      if(info(4).eq.1) then
         stk(lrwork)=tstop
      endif
      if(info(7).eq.1) then
         stk(lrwork+1)=maxstep
      endif
      if(info(8).eq.1) then
         stk(lrwork+2)=stepin
      endif
      if(info(6).eq.1) then
         istk(iadr(liwork))=ml
         istk(iadr(liwork)+1)=mu
      endif
      if(info(11).eq.1) then
         do 100 i=0,neq-1
            istk(iadr(liwork)+LID+i) = stk(l10e7+i)
100      continue
      endif
      if(info(12).eq.1) then
         istk(iadr(liwork)+26) = LENWP
         istk(iadr(liwork)+27) = LENIWP
      endif
      if(info(13).eq.1) then
         istk(iadr(liwork)+23) = maxl
         istk(iadr(liwork)+24) = kmp
         istk(iadr(liwork)+25) = nrmax
         stk(lrwork+10)        = epli
      endif
      if (info(15).eq.1) then
c        Set ipar and rpar
         istk(iadr(lw15)) = neq
         istk(iadr(lw15)+1) = neq
         istk(iadr(lw15)+2) = 2
         istk(iadr(lw15)+3) = 2
         istk(iadr(lw15)+4) = 1
         istk(il17) = 0.005
         istk(il17+1) = 0.05
      endif
      if(info(16).eq.1) then
         do 101 i=0,neq-1
            istk(iadr(liwork)+LID+i) = stk(l10e12+i)
101      continue
      endif
      if(info(17).eq.1) then
         istk(iadr(liwork)+31) = mxnit
         istk(iadr(liwork)+32) = mxnj
         istk(iadr(liwork)+33) = mxnh
         istk(iadr(liwork)+34) = lsoff
         stk(lrwork+13)        = stptol
         stk(lrwork+14)        = epinit
      endif
      istk(iadr(liwork)+16) = lrw
      istk(iadr(liwork)+17) = liw

c     create header of wp
      if (.not.cremat(fname,topw,0,neq*neq,1,lr,lc)) return
      iheaderwp = topw
      topw=topw+1
c     create header of wp and iwp
      if (.not.cremat(fname,topw,0,neq*neq,2,lr,lc)) return
      iheaderiwp = topw
      topw=topw+1

c     Structure d'info pour les externals
      top=topw
      lw=lstk(top)
      ilext=iadr(lw)
      istk(ilext)=5
      istk(ilext+1)=ilext+6
      istk(ilext+2)=ilext+10
      istk(ilext+3)=ilext+14
      istk(ilext+4)=ilext+17
      istk(ilext+5)=ilext+21
      istk(ilext+6)=kres
      istk(ilext+7)=neq
      istk(ilext+8)=kt0
      istk(ilext+9)=ky
      istk(ilext+10)=kjac
      istk(ilext+11)=neq
      istk(ilext+12)=kt0
      istk(ilext+13)=ky
      istk(ilext+14)=ksurf
      istk(ilext+15)=kt0
      istk(ilext+16)=ky
c
c     function psol
      istk(ilext+17)=kpsol
c     header used to create wp
      istk(ilext+18)=iheaderwp
c     header used to create iwp
      istk(ilext+19)=iheaderiwp
c     header of y used to create b
      istk(ilext+20)=ky
c
c     function pjac
      istk(ilext+21)=kpjac
c     header of t0 used to create neq, t0, h, cj
      istk(ilext+22)=kt0
c     header of y used to create y, ydot, rewt and savr
      istk(ilext+23)=ky
      lw=sadr(ilext)+24

      lw0=lw
      ilyr=iadr(lw)
      istk(ilyr)=1
      istk(ilyr+1)=2*n1+1
      istk(ilyr+3)=0
      lyr=sadr(ilyr+4)
      lyri=lyr-(2*n1+1)
      k=0
      info(1)=0
      if(hotstart) info(1)=1
      info(9)=0
      do 1120 i=0,nt-1
         tout=stk(l3+i)
c
 1115    k=k+1
         lyri=lyri+(2*n1+1)
         lw=lyri+(2*n1+1)
         lstk(top+1)=lw
         margin=(k-1)*(2*n1+1)+4
         lw1=lw+margin
         if(lhs.eq.3) lw1=lw1+4+lrw+liw
         if(lw1-lstk(bot).gt.0) then
c     Not enough memory
            call msgstxt('Not enough memory to go further')
            k=k-1
            goto 1125
         endif
         if (tout .eq. t0) then
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            t0=tout
            goto 1120
         else
            stk(lyri)=tout
            call unsfdcopy(n1,stk(l1),1,stk(lyri+1),1)
            call unsfdcopy(n1,stk(lydot),1,stk(lyri+n1+1),1)
            l1=lyri+1
            lydot=lyri+n1+1
            if (info(15).eq.1) then
            call ddaskr(bresd, n1, t0, stk(l1), stk(lydot),
     &           stk(lyri), info, stk(lrtol), stk(latol), idid,
     &           stk(lrwork), lrw, istk(iadr(liwork)), liw, stk(lw15),
     &           istk(il17), bpjacd, bpsold, bsurfd, nh, istk(lgroot))
            else
            call ddaskr(bresd, n1, t0, stk(l1), stk(lydot),
     &           stk(lyri), info, stk(lrtol), stk(latol), idid,
     &           stk(lrwork), lrw, istk(iadr(liwork)), liw, stk(lw15),
     &           istk(il17), bjacd, bpsold, bsurfd, nh, istk(lgroot))
            endif
C     SUBROUTINE DDASKR(RES, NEQ, T, Y, YPRIME, TOUT, INFO, RTOL, ATOL,
C     *   IDID, RWORK, LRW, IWORK, LIW, RPAR, IPAR, JAC, PSOL,
C     *   RT, NRT, JROOT)
         endif
         if(err.gt.0.or.err1.gt.0)  return
         if(idid.eq.1) then
C     A step was successfully taken in the intermediate-output mode.
C     The code has not yet reached TOUT.
            stk(lyri)=t0
            info(1)=1
            goto 1115

         elseif(idid.eq.2.or.idid.eq.4) then
C     The integration to TSTOP was successfully completed (T=TSTOP)
            goto 1125

         elseif(idid.eq.3) then
C     The integration to TOUT was successfully completed (T=TOUT) by
C     stepping past TOUT. Y and Ydot are obtained by interpolation.
            t0=tout
            info(1)=1
            goto 1120
         elseif(idid.eq.5) then
C     One or more root found
            stk(lyri)=t0
C     stk(lrw+41)
            goto 1125
         elseif(idid.eq.-1) then
C     A large amount of work has been expended (About 500 steps)
            call msgstxt('Too many steps necessary to reach next '//
     &           'required time discretization point.')
            call msgstxt('Change discretization of time vector t '//
     &           'or decrease accuracy.')
            stk(lyri)=t0
            goto 1125
         elseif(idid.eq.-2) then
C     The error tolerances are too stringent.
            t0=tout
            info(1)=1
            goto 1115
c     buf='The error tolerances are too stringent'
c     call error(9982)
c     return
         elseif(idid.eq.-3) then
C     The local error test cannot be satisfied because you specified
C     a zero component in ATOL and the corresponding computed solution
C     component is zero. Thus, a pure relative error test is impossible
C     for this component.
            buf='atol and computed test value are zero.'
            call error(9983)
            return
         elseif(idid.eq.-5) then
C     There were repeated failures in the evaluation
C     or processing of the preconditioner (in JAC).
            buf='Cannot evaluate the preconditioner.'
            call error(9983)
            return
         elseif(idid.eq.-6) then
C     Repeated error test failures on the last attempted step.
            call msgstxt('A singularity in the solution '//
     &           'may be present.')
            goto 1125
         elseif(idid.eq.-7) then
C     The corrector could not converge.
            call msgstxt('May be inaccurate or ill-conditioned '//
     &           'JACOBIAN.')
            goto 1125
         elseif(idid.eq.-8) then
C     The matrix of partial derivatives is singular.
            buf='The matrix of partial derivatives is singular'//
     &           'Some of your equations may be redundant.'
            call error(9986)
            return
         elseif(idid.eq.-9) then
C     The corrector could not converge. there were repeated error
c     test failures in this step.
            call msgstxt('Either ill-posed problem or '//
     &           'discontinuity or singularity encountered.')
            goto 1125
         elseif(idid.eq.-10) then
            call msgstxt('External ''res'' returned many times'//
     &           'with ires=-1.')
            goto 1125
         elseif(idid.eq.-11) then
C     IRES equal to -2 was encountered and control is being returned to the
C     calling program.
            buf='Error in external ''res''.'
            call error(9989)
            return
         elseif(idid.eq.-12) then
C     DDASKR failed to compute the initial Yprime.
            buf='daskr failed to compute the initial Ydot.'
            call error(9990)
            return
         elseif(idid.eq.-13) then
C     An unrecoverable error was encountered inside the user's PSOL routine,
C     and control is being returned to the calling program.
            buf='Error in external ''Psol''.'
            call error(9990)
            return
         elseif(idid.eq.-14) then
C     The Krylov linear system solver could not achieve convergence.
            buf='The Krylov linear system solver did not converge.'
            call error(9990)
            return
         elseif(idid.eq.-33) then
C     The code has encountered trouble from which
C     it cannot recover. A message is printed
C     explaining the trouble and control is returned
C     to the calling program. For example, this occurs
C     when invalid input is detected.
            call msgstxt('daskr encountered trouble.')
            goto 1125
         endif
         t0=tout
         info(1)=1
 1120 continue
c
 1125 top=topk-rhs
      mv=lw0-l0
c
c     Variable de sortie: y0
c
      top=top+1
      if(k.eq.0) istk(ilyr+1)=0
      istk(ilyr+2)=k
      lw=lyr+(2*n1+1)*k
      lstk(top+1)=lw-mv
c
c     Variable de sortie: roots
c
      top=top+1
      ilw=iadr(lw)
      err=lw+4+nh+1-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=1
      istk(ilw+2)=1
      istk(ilw+3)=0
      l=sadr(ilw+4)
      stk(l)=t0
      kkk=1
      do 1153 i=0,nh-1
         if(istk(lgroot+i).ne.0) then
            l=l+1
            kkk=kkk+1
            istk(ilw+2)=istk(ilw+2)+1
            stk(l)=i+1
         endif
 1153 continue
      lw=l+1
      lstk(top+1)=lw-mv
      if(lhs.eq.2) goto 1150
c
c     Variable de sortie: rwork
c
      top=top+1
      ilw=iadr(lw)
      err=lw+4+lrw+liw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=lrw+liw
      istk(ilw+2)=1
      istk(ilw+3)=0
      lw=sadr(ilw+4)
      call unsfdcopy(lrw,stk(lrwork),1,stk(lw),1)
      call int2db(liw,istk(iadr(liwork)),1,stk(lw+lrw),1)
      lw=lw+lrw+liw
      lstk(top+1)=lw-mv
c
c     Remise en place de la pile
 1150 call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return
      end
