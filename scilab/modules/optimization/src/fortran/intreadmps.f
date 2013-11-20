c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c SCILAB function : readmps, fin = 1
      subroutine intreadmps(id)
c
      include 'stack.h'
c
      integer iadr, sadr
      integer topk,rhsk, topf,mode(2)
      logical checkrhs,checklhs,getsmat,checkval,getvect
      logical listcremat
      logical listcresmat,listcrestring
      logical opened

      double precision big,dlamch,dlobnd,dupbnd
      integer ierr,line
      integer irobj,ptr(19)
      character*8 namec,nameb,namran,nambnd,nammps
      character*2 typrow
      character*8 fname
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      fname='readmps'
c
      rhs = max(0,rhs)
c
      lbuf = 1
      topk = top
      rhsk = rhs
c
      big=dlamch('o')

      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,2,3)) return

c
c     checking variable file (number 1)
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
      topf=top-rhs+1
c
c     checking variable bnd (number 2)
      if(.not.getvect(fname,top,top-rhs+2,it2,m2,n2,lr2,lc2)) return
      if(.not.checkval(fname,n2*m2,2)) return
      dlobnd=stk(lr2)
      dupbnd=stk(lr2+1)
      if (rhs.eq.3) then
c     . checking variable max (number 3)
         if(.not.getvect(fname,top,top-rhs+3,it3,m3,n3,lr3,lc3)) return
         if(.not.checkval(fname,n3*m3,3)) return
         maxm=stk(lr3)
         maxn=stk(lr3+1)
         maxnza=stk(lr3+2)
         m=maxm
         n=maxn
         nza=maxnza
      else
         mode(1)=-1
         mode(2)=0
         call v2unit(topf,mode,lunit,opened,ierr)
         if(ierr.gt.0) return
         call rdmpsz(lunit,m,n,nza,ierr,typrow,line)
         if(ierr.eq.0) then
            call clunit(-lunit,buf,mode)
         elseif(ierr.eq.1) then
            call writebufspa(buf,fname,line)
            call clunit(-lunit,buf,mode)
            call error(998)
            return
         elseif(ierr.eq.2) then
            call writebufspb(buf,fname,typrow,line)
            call clunit(-lunit,buf,mode)
            call error(999)
            return
         endif
         maxm=m
         maxn=n
         maxnza=nza
      endif

      mode(1)=-1
      mode(2)=0
      call v2unit(topf,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c

      top=topk-rhs+1

c     nl number of fields
      call mpstyp(nl,'nfield')
      call mpstyp(ptr,'ptr')
c
c     create tlist structure
      call cretlist(top,nl,ll)
c     tlist type (vector of strings nl)
      l1=iadr(ll)
      if(.not.listcresmat(fname,top,1,ll,nl,1,ptr,3,iltyp)) goto 998
      call mpstyp(istk(l1),'create')
c     irobj (scalar)
      if(.not.listcremat (fname,top,2,ll,0, 1,1,lirobj,lcs)) goto 998
c     namec (string 8)
      if(.not.listcrestring(fname,top,3,ll,8,lnamec)) goto 998
c     nameb (string 8)
      if(.not.listcrestring(fname,top,4,ll,8,lnameb)) goto 998
c     namran (string 8)
      if(.not.listcrestring(fname,top,5,ll,8,lnamran)) goto 998
c     nambnd (string 8)
      if(.not.listcrestring(fname,top,6,ll,8,lnambnd)) goto 998
c     nammps (string 8)
      if(.not.listcrestring(fname,top,7,ll,8,lnammps)) goto 998
c     rownams  (vector of strings mx1)
      if(.not.listcresmat(fname,top,8,ll,m,1,8,1,lrownams)) goto 998
c     colnams  (vector of strings 1xn)
      if(.not.listcresmat(fname,top,9,ll,1,n,8,1,lcolnams)) goto 998
c     rwstat (vector m)
      if(.not.listcremat (fname,top,10,ll,0,m,1,lrwstat,lcs)) goto 998
      ilrwstat=iadr(lrwstat)
c     rowcod (matrix mx2)
      if(.not.listcremat (fname,top,11,ll,0,m,2,lrowcod,lcs)) goto 998
      ilrowcod=iadr(lrowcod)
c     colcod (matrix 2xn)
      if(.not.listcremat (fname,top,12,ll,0,n,2,lcolcod,lcs)) goto 998
      ilcolcod=iadr(lcolcod)
c     rwnmbs (vector nza)
      if(.not.listcremat (fname,top,13,ll,0,nza,1,lrwnmbs,lcs)) goto 998
      ilrwnmbs=iadr(lrwnmbs)
c     clpnts (vector n)
      if(.not.listcremat (fname,top,14,ll,0,1,n+1,lclpnts,lcs)) goto 998
      ilclpnts=iadr(lclpnts)
c     acoef (vector nza)
      if(.not.listcremat (fname,top,15,ll,0,nza,1,lacoef,lcs)) goto 998
c     rhsb (vector m)
      if(.not.listcremat (fname,top,16,ll,0,m,1,lrhsb,lcs)) goto 998
c     ranges (vector m)
      if(.not.listcremat (fname,top,17,ll,0,m,1,lranges,lcs)) goto 998
c     bnds (matrix 2xn)
      if(.not.listcremat (fname,top,18,ll,0,n,2,lbnds,lcs)) goto 998
c     stavar (column vector n)
      if(.not.listcremat (fname,top,19,ll,0,n,1,lstavar,lcs)) goto 998
      ilstavar=iadr(lstavar)

c     work array irow
      lirow=iadr(ll)
      ll=sadr(lirow+n)
c     work array relt
      lrelt=ll
      ll=ll+n
c     rwname
c     cstk replaced by istk to avoid argument passing pb with linux
c      lrwname=cadr(ll)
c      ll=ll+(8*m)/4
      lrwname=iadr(ll)
      ll=sadr(lrwname+(8*m)/4+8)
c     clname
c      cstk replaced by istk to avoid argument passing pb with linux
c      lclname=cadr(ll)
c      ll=ll+(8*n)/4
      lclname=iadr(ll)
      ll=sadr(lclname+(8*m)/4+8)
      err=ll-lstk(bot)
      if(err.gt.0) then
         call error(17)
         goto 998
      endif
      nameb= '        '
      namec= '        '
      namran='        '
      nambnd='        '
      nammps='        '

c      cstk(lrwname:) replaced by istk(lrwname) and
c      cstk(lclname:) replaced by istk(lclname) to avoid argument
c      passing pb with linux

      call rdmps1(ierr,buf,maxm,maxn,maxnza,
     x     m,n,nza,irobj,big,dlobnd,dupbnd,
     x     namec,nameb,namran,nambnd,nammps,lunit,
     x     istk(lrwname),istk(lclname),
     x     istk(ilstavar),istk(ilrwstat),
     x     istk(ilrowcod),istk(ilrowcod+m),
     x     istk(ilcolcod),istk(ilcolcod+n),
     x     istk(ilrwnmbs),istk(ilclpnts),istk(lirow),
     x     stk(lacoef),stk(lrhsb),stk(lranges),
     x     stk(lbnds+n),stk(lbnds),stk(lrelt))


      call clunit(-lunit,buf,mode)
      if(ierr.ne.0) then
         call error(1000+ierr)
         return
      endif
c
c     convert integer data to double
      stk(lirobj)=irobj
      call int2db(m,istk(ilrowcod+m),-1,stk(lrowcod+m),-1)
      call int2db(m,istk(ilrowcod),-1,stk(lrowcod),-1)

      call int2db(n,istk(ilcolcod+n),-1,stk(lcolcod+n),-1)
      call int2db(n,istk(ilcolcod),-1,stk(lcolcod),-1)

      call int2db(nza,istk(ilrwnmbs),-1,stk(lrwnmbs),-1)
      call int2db(n+1,istk(ilclpnts),-1,stk(lclpnts),-1)
      call int2db(m,istk(ilrwstat),-1,stk(lrwstat),-1)
      call int2db(n,istk(ilstavar),-1,stk(lstavar),-1)

c     convert strings   and put them in proper locations
      call cvstr(8,istk(lnamec),namec,0)
      call cvstr(8,istk(lnameb),nameb,0)
      call cvstr(8,istk(lnamran),namran,0)
      call cvstr(8,istk(lnambnd),nambnd,0)
      call cvstr(8,istk(lnammps),nammps,0)
c     convert vector of strings  and put them in proper locations

c      cstk replaced by istk to avoid argument passing pb with linux
c      call cvstr(8*m,istk(lrownams),cstk(lrwname:),0)
c      call cvstr(8*n,istk(lcolnams),cstk(lclname:),0)
      call cvstr(8*m,istk(lrownams),istk(lrwname),0)
      call cvstr(8*n,istk(lcolnams),istk(lclname),0)

      return
998   call clunit(-lunit,buf,mode)
      return
      end
c
      subroutine mpstyp(ivt,job)
c     definition of first field  of tlist's type: mps
c     tlist fields are:
c     irobj
c     namec
c     nameb
c     namran
c     nambnd
c     name
c     rownames
c     colnames
c     rowstat
c     rowcode
c     colcode
c     rownmbs
c     colpnts
c     acoeff
c     rhs
c     ranges
c     bounds
c     stavar
c
      integer ivt(*),l
      character*(*) job
c
      if(job.eq.'size') then
c        size of the data structure
         ivt(1)=136
      elseif(job.eq.'nchar') then
c        number of chars defining the type field
         ivt(1)=112
      elseif(job.eq.'nfield') then
c        number of fields in the tlist
         ivt(1)=19
      elseif(job.eq.'ptr') then
c        pointers on individual strings
         ivt(1)=1
         ivt(2)=4
         ivt(3)=9
         ivt(4)=14
         ivt(5)=19
         ivt(6)=25
         ivt(7)=31
         ivt(8)=35
         ivt(9)=43
         ivt(10)=51
         ivt(11)=58
         ivt(12)=65
         ivt(13)=72
         ivt(14)=79
         ivt(15)=86
         ivt(16)=92
         ivt(17)=95
         ivt(18)=101
         ivt(19)=107
         ivt(20)=113
      else
c        Character string Variable header
         ivt(1)=10
         ivt(2)=1
         ivt(3)=19
         ivt(4)=0
         ivt(5)=1
         l=24
c        entry (1,1) = "mps"
         ivt(l+1)=22
         ivt(l+2)=25
         ivt(l+3)=28
         ivt(6)=ivt(5)+3
         l=l+3
c        entry (2,1) = "irobj"
         ivt(l+1)=18
         ivt(l+2)=27
         ivt(l+3)=24
         ivt(l+4)=11
         ivt(l+5)=19
         ivt(7)=ivt(6)+5
         l=l+5
c        entry (3,1) = "namec"
         ivt(l+1)=23
         ivt(l+2)=10
         ivt(l+3)=22
         ivt(l+4)=14
         ivt(l+5)=12
         ivt(8)=ivt(7)+5
         l=l+5
c        entry (4,1) = "nameb"
         ivt(l+1)=23
         ivt(l+2)=10
         ivt(l+3)=22
         ivt(l+4)=14
         ivt(l+5)=11
         ivt(9)=ivt(8)+5
         l=l+5
c        entry (5,1) = "namran"
         ivt(l+1)=23
         ivt(l+2)=10
         ivt(l+3)=22
         ivt(l+4)=27
         ivt(l+5)=10
         ivt(l+6)=23
         ivt(10)=ivt(9)+6
         l=l+6
c        entry (6,1) = "nambnd"
         ivt(l+1)=23
         ivt(l+2)=10
         ivt(l+3)=22
         ivt(l+4)=11
         ivt(l+5)=23
         ivt(l+6)=13
         ivt(11)=ivt(10)+6
         l=l+6
c        entry (7,1) = "name"
         ivt(l+1)=23
         ivt(l+2)=10
         ivt(l+3)=22
         ivt(l+4)=14
         ivt(12)=ivt(11)+4
         l=l+4
c        entry (8,1) = "rownames"
         ivt(l+1)=27
         ivt(l+2)=24
         ivt(l+3)=32
         ivt(l+4)=23
         ivt(l+5)=10
         ivt(l+6)=22
         ivt(l+7)=14
         ivt(l+8)=28
         ivt(13)=ivt(12)+8
         l=l+8
c        entry (9,1) = "colnames"
         ivt(l+1)=12
         ivt(l+2)=24
         ivt(l+3)=21
         ivt(l+4)=23
         ivt(l+5)=10
         ivt(l+6)=22
         ivt(l+7)=14
         ivt(l+8)=28
         ivt(14)=ivt(13)+8
         l=l+8
c        entry (10,1) = "rowstat"
         ivt(l+1)=27
         ivt(l+2)=24
         ivt(l+3)=32
         ivt(l+4)=28
         ivt(l+5)=29
         ivt(l+6)=10
         ivt(l+7)=29
         ivt(15)=ivt(14)+7
         l=l+7
c        entry (11,1) = "rowcode"
         ivt(l+1)=27
         ivt(l+2)=24
         ivt(l+3)=32
         ivt(l+4)=12
         ivt(l+5)=24
         ivt(l+6)=13
         ivt(l+7)=14
         ivt(16)=ivt(15)+7
         l=l+7
c        entry (12,1) = "colcode"
         ivt(l+1)=12
         ivt(l+2)=24
         ivt(l+3)=21
         ivt(l+4)=12
         ivt(l+5)=24
         ivt(l+6)=13
         ivt(l+7)=14
         ivt(17)=ivt(16)+7
         l=l+7
c        entry (13,1) = "rownmbs"
         ivt(l+1)=27
         ivt(l+2)=24
         ivt(l+3)=32
         ivt(l+4)=23
         ivt(l+5)=22
         ivt(l+6)=11
         ivt(l+7)=28
         ivt(18)=ivt(17)+7
         l=l+7
c        entry (14,1) = "colpnts"
         ivt(l+1)=12
         ivt(l+2)=24
         ivt(l+3)=21
         ivt(l+4)=25
         ivt(l+5)=23
         ivt(l+6)=29
         ivt(l+7)=28
         ivt(19)=ivt(18)+7
         l=l+7
c        entry (15,1) = "acoeff"
         ivt(l+1)=10
         ivt(l+2)=12
         ivt(l+3)=24
         ivt(l+4)=14
         ivt(l+5)=15
         ivt(l+6)=15
         ivt(20)=ivt(19)+6
         l=l+6
c        entry (16,1) = "rhs"
         ivt(l+1)=27
         ivt(l+2)=17
         ivt(l+3)=28
         ivt(21)=ivt(20)+3
         l=l+3
c        entry (17,1) = "ranges"
         ivt(l+1)=27
         ivt(l+2)=10
         ivt(l+3)=23
         ivt(l+4)=16
         ivt(l+5)=14
         ivt(l+6)=28
         ivt(22)=ivt(21)+6
         l=l+6
c        entry (18,1) = "bounds"
         ivt(l+1)=11
         ivt(l+2)=24
         ivt(l+3)=30
         ivt(l+4)=23
         ivt(l+5)=13
         ivt(l+6)=28
         ivt(23)=ivt(22)+6
         l=l+6
c        entry (19,1) = "stavar"
         ivt(l+1)=28
         ivt(l+2)=29
         ivt(l+3)=10
         ivt(l+4)=31
         ivt(l+5)=10
         ivt(l+6)=27
         ivt(24)=ivt(23)+6
         l=l+6
      endif
      return
      end

