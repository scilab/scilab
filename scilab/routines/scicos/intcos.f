c fin = 1 
c SCILAB function : scicos
c     Copyright INRIA

c      interface function
c     ********************
      subroutine  intscicos
c
      include '../stack.h'
      integer id(nsiz)
      integer kfun
      common /curblk/ kfun
      double precision ptr
      integer iadr,sadr
      logical putlhsvar,checkrhs,checklhs
      character*(nlgh) fname
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' intcos '//buf(1:4))
      endif
c
c     functions/fin
c    var2vec  vec2var  getblocklabel scicos_debug  debug_count
c       1        2          3            4            5
c
c    sctree  sci_tree2  sci_tree3   sci_tree4   realtimeinit
c       6        7          8           9            10
c
c    realtime curblock
c      11        12
c
      goto (1,2,3,4,5,6,7,8,9,10,11,12) fin
c
c     var2vec
 1    continue
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il=iadr(lstk(top))
      il1=il
      l1=sadr(il1+4)
      n=lstk(top+1)-lstk(top)
      call unsfdcopy(n,stk(lstk(top)),-1,stk(l1),-1)
      istk(il1)=1
      istk(il1+1)=n
      istk(il1+2)=1
      istk(il1+3)=0
      lstk(top+1)=l1+n
      goto 999
c
c     vec2var
 2    continue
      if (rhs .ne. 1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      il1=iadr(lstk(top))
      if(istk(il1).ne.1) then
         err=1
         call error(44)
         return
      endif
      l1=sadr(il1+4)

      n=lstk(top+1)-l1
      call unsfdcopy(n,stk(l1),1,stk(lstk(top)),1)
      lstk(top+1)=lstk(top)+n
      goto 999
c
c     getblocklabel
 3    continue
      call intgetlabel
      goto 999
c
c     scicos_debug(i)
 4    continue
      fname='scicos_debug' 
      call scicosdebug(fname)
      goto 998
c
c     scicos_debug_count
 5    continue
      fname='scicos_debug_count'
      call scicosdebugcount(fname)
      goto 998
c
c     ctree
 6    call intctree
      return
c
c     [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
 7    continue
      fname='sci_tree2'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,4,4)) return
      call intree2(fname)
      goto 998
c
c     [ord,ok]=tree3(vec,dep_ut,typ_l,bexe,boptr,blnk,blptr)
 8    continue
      fname='sci_tree3'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,7,7)) return
      call intree3(fname)
      goto 998
c
c     [r1,r2,ok]=tree4(vec,outoin,outoinptr,dep_ut,typ_r)
 9    continue
      fname='sci_tree4'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,5,5)) return
      call intree4(fname)
      goto 998
c
c     [...]=realtimeinit(...)
 10   continue
      fname='realtimeinit'
c
      call intsrealtimeinit(fname)
      goto 998
c
c     [...]=realtime(...)
 11   continue
      fname='realtime'
c
      call intsrealtime(fname)
      goto 998
c
c     curblock
 12   continue
      call intcurblk
      goto 999
 998  if(.not.putlhsvar())return
 999  return
      end

c     ********************
      subroutine intcurblk
c
c
      include '../stack.h'
      integer kfun
      common /curblk/ kfun
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      top=top+1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=kfun
      lstk(top+1)=l+1
      return
      end

c     **********************
      subroutine intgetlabel
c
c
      include '../stack.h'
      integer iadr, sadr
      integer kfun
      integer getscilabel
      external getscilabel
      common /curblk/ kfun
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.gt.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.eq.1) then
         il=iadr(lstk(top))
         if(istk(il).ne.1) then 
            err=1
            call error(54)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(36)
            return
         endif
         kf=stk(sadr(il+4))
         top=top-1
      else
         kf=kfun
      endif
c
      lw = lstk(top+1)
      top=top+1
      il1=iadr(lstk(top))

      id1=il1+4
      l1=id1+2
      err=sadr(l1+50)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ierr=getscilabel(kf,istk(l1),n)
      if(ierr.ne.0) then
         buf='scicosim is not running'
         call error(999)
         return
      endif
      istk(il1)=10
      istk(il1+1)=1
      istk(il1+2)=1
      istk(il1+3)=0
      istk(il1+4)=1
      istk(il1+5)=1+n
      lstk(top+1)=sadr(l1+n)
      return
      end


c     *****************************
      subroutine scicosdebug(fname)
c
c
      include '../stack.h'
      common /cosdebug/ cosd
      logical getrhsvar, createvar,checklhs,checkrhs
      integer cosd
      character*(*) fname
c
      rhs = max(0,rhs)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,-1,1)) return
      if (rhs.eq.1) then
         if(.not.getrhsvar(1,'i',n,m,i)) return
         cosd=istk(i)
         lhsvar(1)=0
      else
         if(.not.createvar(1,'d',1,1,ipr1)) return
         stk(ipr1)=cosd
         lhsvar(1)=1 
      endif

      end


c     **********************************
      subroutine scicosdebugcount(fname)
c
c
      include '../stack.h'
      common /cosdebugcounter/ counter
      logical getrhsvar, createvar,checklhs,checkrhs
      integer counter
      character*(*) fname
c
      rhs = max(0,rhs)
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,-1,0)) return
      if(.not.createvar(1,'d',1,1,ipr1)) return
      stk(ipr1)=counter
      lhsvar(1)=1 
      end


c     *******************
      subroutine intctree
c
c
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


c     **************************
      subroutine  intree2(fname)
c
c
      include '../stack.h'
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


c     *************************
      subroutine intree3(fname)
c
c
      include '../stack.h'
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
      istk(iadr(iwhere(8))+1)=nord
      end


c     **************************
      subroutine  intree4(fname)
c
c
      include '../stack.h'
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
      istk(iadr(iwhere(6))+2)=nr
      istk(iadr(iwhere(7))+2)=nr
      end
