c------------------------------------------------------------------------
c     PVM functions interfaces
c     Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved 
c------------------------------------------------------------------------
c-----SCILAB function : pvmjoingroup, fin = 1
      subroutine intspvmjoingroup(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer inum
      logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c     
      lbuf = 1
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      call scipvmjoingroup(buf(lbufi1:lbuff1),nlr1,inum)
c     
c     output variable: inum
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=inum
      return
      end
c
c-----SCILAB function : pvmlvgroup, fin = 2
      subroutine intspvmlvgroup(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer res
      logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c     
      lbuf = 1

      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      call scipvmlvgroup(buf(lbufi1:lbuff1),nlr1,res)
c     
c     output variable: res
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmgsize, fin = 3
      subroutine intspvmgsize(fname)
      character*(*) fname
      include '../stack.h'
c     
      integer size
      logical checkrhs,checklhs,getsmat,checkval,bufstore,cremat
c     
      lbuf = 1

      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      call scipvmgsize(buf(lbufi1:lbuff1),nlr1,size)
c     
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=size
      return
      end
c
c-----SCILAB function : pvmgettid, fin = 4
      subroutine intspvmgettid(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer tid,inum
      logical checkrhs,checklhs,getsmat,checkval,getscalar,bufstore
      logical cremat
c     
      lbuf = 1
c     
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c
c     checking variable inum (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      inum=stk(lr2)
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      call scipvmgettid(buf(lbufi1:lbuff1),nlr1,inum,tid)
c     
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=tid
      return
      end
c
c-----SCILAB function : pvmgetinst, fin = 5
      subroutine intspvmgetinst(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getsmat,checkval,getscalar
      logical bufstore,cremat
      integer tid,inum
c     
      lbuf = 1

      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
c     checking variable tid (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      tid = stk(lr2)
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

      call scipvmgetinst(buf(lbufi1:lbuff1),nlr1,tid,inum)
c     
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=inum
      return
      end
c
c-----SCILAB function : pvmbarrier, fin = 6
      subroutine intspvmbarrier(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getsmat,checkval,getscalar
      logical bufstore,cremat
      integer count,res
c     
      lbuf = 1
c     
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return

c     
c     checking variable count (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      count = stk(lr2)
c     

      call scipvmbarrier(buf(lbufi1:lbuff1),nlr1,count,res)
c     
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmbcast, fin = 7
      subroutine intspvmbcast(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer tag, maxsize, n,res
      integer address
      logical checkrhs,checklhs,getsmat,checkval,getscalar
      logical bufstore,cremat
      integer iadr
c     
      iadr(l)=l+l-1
c     
      lbuf = 1

      if(.not.checkrhs(fname,3,3)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable group (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c     
c     checking variable tag (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
      tag=stk(lr3)

c     construct the pack vector for the variable number 2. 
c     set adress where to put the pack vector and its max
c     allowable size
      ilpack=iadr(lstk(top))
      maxsize=iadr(lstk(bot)) - ilpack
      address = top-rhs+2
      call varpak(address,istk(ilpack),n,maxsize,ierr)
      if(ierr .gt. 0) then 
         buf = fname // ' Unknow type or not yet implemented' 
         call error(999)
         return
      endif
c     
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c
      call scipvmbcast(buf(lbufi1:lbuff1),nlr1,
     $     istk(ilpack),n,stk(lstk(address)),tag,res)
c     
c     output variable res
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c     
c-----SCILAB function : pvmtasks, fin = 8
      subroutine intspvmtasks(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,cremat,getscalar,crepointer,listcremat
      logical lcrestringmatfromC
      integer where, ntask, info
c     following ligne defines memory to contain pointers returned by
c     scipvmtasks
      double precision  ptid, pptid, pdtid, pflag, pname
c     
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
c     checking variable where (number 1)
c     
      if(rhs .le. 0) then
         where=0
      else
         if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
         where = stk(lr1)
      endif

      call scipvmtasks(where, ntask, ptid, pptid, pdtid, pflag,
     $     pname,ne3,info)

c     
      top=top-rhs+1
c     Creation of output list
      call crelist(top,7,lw)
c     
c     Element 1: tid
      if(.not.listcremat(fname,top,1,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,ptid,stk(lrs))
c     
c     Element 2: ptid
      if(.not.listcremat(fname,top,2,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,pptid,stk(lrs))
c     
c     Element 3: dtid
      if(.not.listcremat(fname,top,3,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,pdtid,stk(lrs))
c     
c     Element 4: flag
      if(.not.listcremat(fname,top,4,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,pflag,stk(lrs))
c     
c     Element 5: name
      lw5=lstk(bot)-1
      stk(lw5)=pname
      if(.not.lcrestringmatfromC(fname,top,5,lw,lw5,1,ne3)) return
c     
c     Element 6: ntask
      if(.not.listcremat(fname,top,6,lw,0,1,1,lrs,lcs)) return
      stk(lrs)=ntask
c     
c     Element 7: info
      if(.not.listcremat(fname,top,7,lw,0,1,1,lrs,lcs)) return
      stk(lrs)=info
      return
      end
c
c-----SCILAB function : pvmconfig, fin = 9
      subroutine intspvmconfig(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,cremat,crepointer,listcremat
      logical lcrestringmatfromC
      integer nhost,narch,ne3,info
c     following ligne defines memory to contain pointers returned by
c     scipvmconfig 
      double precision pdtid,pname,parch,pspeed
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
c     
      call scipvmconfig(nhost,narch,pdtid,pname,parch,pspeed,ne3,info)
c     
      top=top-rhs+1
c     Creation of output list
      call crelist(top,7,lw)
c     
c     Element 1: nhost
      if(.not.listcremat(fname,top,1,lw,0,1,1,lrs,lcs)) return
      stk(lrs)=nhost
c     
c     Element 2: narch
      if(.not.listcremat(fname,top,2,lw,0,1,1,lrs,lcs)) return
      stk(lrs)=narch
c     
c     Element 3: dtid
      if(.not.listcremat(fname,top,3,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,pdtid,stk(lrs))
c     
c     Element 4: name
      lw4=lstk(bot)-1
      stk(lw4)=pname
      if(.not.lcrestringmatfromC(fname,top,4,lw,lw4,1,ne3)) return
c     
c     Element 5: arch
      lw5=lstk(bot)-1
      stk(lw5)=parch
      if(.not.lcrestringmatfromC(fname,top,5,lw,lw5,1,ne3)) return
c     
c     Element 6: speed
      if(.not.listcremat(fname,top,6,lw,0,1,ne3,lrs,lcs)) return
      call cintf(ne3,pspeed,stk(lrs))
c     
c     Element 7: info
      if(.not.listcremat(fname,top,7,lw,0,1,1,lrs,lcs)) return
      stk(lrs)=info

      return
      end
c     
c-----SCILAB function : pvmaddhosts, fin = 10
      subroutine intspvmaddhosts(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      logical checkrhs,checklhs,getsmat,checkval,crestringv,cremat
      double precision pstr
c     
      iadr(l)=l+l-1

c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c
c     checking variable hosts (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1,1)) return
c     
c     crestringv returns a pointer on a C structure in stk(lw1)
      if(.not.crestringv(fname,top+1,lr1-5-m1*n1,lw1)) return
      pstr=stk(lw1)
      
      if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
      if(.not.cremat(fname,top+1,0,n1,1,lw3,loc3)) return
      call scipvmaddhosts(pstr,n1,stk(lw3))
c     
c     --------------output variable: infos
      call int2db(n1,istk(iadr(lw3)),-1,stk(lrs),-1)
      return
      end
c
c-----SCILAB function : pvmdelhosts, fin = 11
      subroutine intspvmdelhosts(fname)   
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      logical checkrhs,checklhs,getsmat,checkval,crestringv,cremat
c     
      iadr(l)=l+l-1
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c
c     checking variable hosts (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1,1)) return
c     
c     cross variable size checking
c     
      if(.not.crestringv(fname,top+1,lr1-5-m1*n1,lw1)) return
      
      if(.not.cremat(fname,top,0,1,n1,lrs,lcs)) return
      if(.not.cremat(fname,top+1,0,n1,1,lw3,loc3)) return
      call scipvmdelhosts(stk(lw1),n1,stk(lw3))
c     
c     --------------output variable: infos
      call int2db(n1,istk(iadr(lw3)),-1,stk(lrs),-1)
      return
      end
c
c-----SCILAB function : pvmparent, fin = 12
      subroutine intspvmparent(fname)
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,cremat
      integer res
c     

      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     cross variable size checking
c     
      call scipvmparent(res)
c     
      top=top+1
c     
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmtidtohost, fin = 13
      subroutine intspvmtidtohost(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getscalar,cremat
      integer tid,res
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tid (number 1)
c     
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      tid = stk(lr1)
c     
      call scipvmtidtohost(tid,res)
c     
c     --------------output variable: res
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmsettimer, fin = 14
      subroutine intspvmsettimer(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,cremat
      integer res
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
c     
      call scipvmsettimer(res)

c     return variable res
      top=top+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmgettimer, fin = 15
      subroutine intspvmgettimer(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      double precision t
      logical checkrhs,checklhs,cremat
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
c     
      call scipvmgettimer(t)
c     
c     --------------output variable: t
      top=top+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=t
      return
      end
c     
c-----SCILAB function : pvmmytid, fin = 16
      subroutine intspvmmytid(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer res
      logical checkrhs,checklhs,cremat
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return

      call scipvmmytid(res)
c     
c     --------------output variable: res
      top=top+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmexit, fin = 17
      subroutine intspvmexit(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer res
      logical checkrhs,checklhs,cremat
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
    
      call scipvmexit(res)
c     
c     --------------output variable: res
      top=top+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmkill, fin = 18
      subroutine intspvmkill(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      logical checkrhs,checklhs,getvectrow,cremat
c     
      iadr(l)=l+l-1

      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tids (number 1)
c     
      if(.not.getvectrow(fname,top,top,it1,m1,n1,lr1,lc1)) return
c     
c     cross variable size checking
c     
      if(.not.cremat(fname,top+1,0,n1,1,lw3,loc3)) return
      if(.not.cremat(fname,top+2,0,n1,1,ltids,loc3)) return
      call entier(n1,stk(lr1),istk(iadr(ltids)))

c     
      call scipvmkill(istk(iadr(ltids)),n1,stk(lw3))
c     
c     --------------output variable: res
      if(.not.cremat(fname,top+2,0,1,n1,lrs,lcs)) return
      call int2db(n1,istk(iadr(lw3)),-1,stk(lrs),-1)
      call copyobj(fname,top+2,top)
      return
      end
c
c-----SCILAB function : pvmspawn, fin = 19
      subroutine intspvmspawn(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      integer topk
      logical checkrhs,checklhs,getsmat,checkval,getscalar,cresmat2
      logical bufstore,cremat
      integer ntask,res
c     
      iadr(l)=l+l-1
c     
      lbuf = 1
      topk = top-rhs+1 

      if(.not.checkrhs(fname,2,4)) return
      if(.not.checklhs(fname,1,2)) return
c     
c     checking variable task (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
c     checking variable ntask (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      ntask=stk(lr2)
c     
c     checking variable win (number 3)
c     
      if(rhs .le. 2) then
         top = top+1
         rhs = rhs+1
         nlr3 = 1
         if(.not.cresmat2(fname,top,nlr3,lr3)) return
         call cvstr(nlr3,istk(lr3),'w',0)
      endif
      if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
      if(.not.checkval(fname,m3*n3,1)) return
c     checking variable where (number 4)
c     
      if(rhs .le. 3) then
         top = top+1
         rhs = rhs+1
         nlr4 = 4
         if(.not.cresmat2(fname,top,nlr4,lr4)) return
         call cvstr(nlr4,istk(lr4),'null',0)
      endif
      if(.not.getsmat(fname,top,top-rhs+4,m4,n4,1,1,lr4,nlr4)) return
      if(.not.checkval(fname,m4*n4,1)) return
c     
c     cross variable size checking
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr3,nlr3)) return
      if(.not.bufstore(fname,lbuf,lbufi5,lbuff5,lr4,nlr4)) return

      if(.not.cremat(fname,topk,0,ntask,1,ltids,loc8)) return
      if(.not.cremat(fname,topk+1,0,ntask,1,lw8,loc8)) return

      call scipvmspawn(buf(lbufi1:lbuff1),nlr1,buf(lbufi3:lbuff3),nlr
     $     3,buf(lbufi5:lbuff5),nlr4,ntask,stk(lw8),res)

      top=topk
c     
c     --------------output variable: tids
      call int2db(1*ntask,istk(iadr(lw8)),-1,stk(ltids),-1)
c     
      if(lhs .ge. 2) then
c     --------------output variable: res
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=res
      endif
      return
      end
c
c-----SCILAB function : pvmspawnindependent, fin = 20
      subroutine intspvmspawnindependent(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      integer topk
      logical checkrhs,checklhs,getsmat,checkval,getscalar,cresmat2
      logical bufstore,cremat
      integer ntask,res
c     
      iadr(l)=l+l-1
c     
      lbuf = 1
      topk = top-rhs+1

      if(.not.checkrhs(fname,2,3)) return
      if(.not.checklhs(fname,1,2)) return
c     
c     checking variable task (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
c     
c     checking variable ntask (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      ntask = stk(lr2)
c     
c     checking variable where (number 3)
c     
      if(rhs .le. 2) then
         top = top+1
         rhs = rhs+1
         nlr3 = 4
         if(.not.cresmat2(fname,top,nlr3,lr3)) return
         call cvstr(nlr3,istk(lr3),'null',0)
      endif
      if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
      if(.not.checkval(fname,m3*n3,1)) return
c     
c     cross variable size checking
c     
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
      if(.not.bufstore(fname,lbuf,lbufi4,lbuff4,lr3,nlr3)) return

      if(.not.cremat(fname,topk,0,ntask,1,ltids,loc6)) return
      if(.not.cremat(fname,topk+1,0,ntask,1,lw6,loc6)) return
      

      call scipvmspawnindependent(buf(lbufi1:lbuff1),nlr1,ntask,
     $     buf(lbufi4:lbuff4),nlr3,stk(lw6),res)
      if(err .gt. 0) then 
         buf = fname // ' Internal Error' 
         call error(999)
         return
      endif
c     --------------output variable: tids
      top=topk
      call int2db(ntask,istk(iadr(lw6)),-1,stk(ltids),-1)
c     
      if(lhs .ge. 2) then
c     --------------output variable: res
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=res
      endif
      return
      end
c
c
c-----SCILAB function : pvmrecv, fin = 21
      subroutine intspvmrecv(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer beginvar, tag, tid, iis
      integer topk, rhsk
      logical checkrhs, checklhs, getscalar, cremat
c
      rhs = max(0,rhs)
c     
      topk = top 
      rhsk = rhs 
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,4)) return
c     
c     checking variable tid (number 1)
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      tid=stk(lr1)
c     
c     checking variable tag (number 2)
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      tag=stk(lr2)
c
c     free input variable at the top of the stack
      top=top-rhs+1
c
c     set adress where to put the received variable and its max
c     allowable size
      beginvar=lstk(top)
      maxsize=lstk(bot)-beginvar
c
c     receive the variable
      call scipvmrecv(stk(beginvar),maxsize,n,tid,tag,iis)
c     ATTENTION scipvmrecv ne doit pas ecrire plus de maxsize double
c     dans stk(beginvar)
c
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c
c     set correct size for the received variable
      lstk(top+1)=lstk(top)+n

c     MA MODIF: j incremente top ici et non pas apres la creation de
c      la variable de retour..
c     
      if(lhs .ge. 2) then
      top=top+1
c     create variable status at the top of the returned variables
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
c     set value of the status variable
         stk(lrs)=iis
c     top=top+1
      endif
      if(lhs .ge. 3) then
c     --------------output variable: tid
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=tid
      endif
      if(lhs .ge. 4) then
c       --------------output variable: tag
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=tag
      endif
c     
      return
      end

c
c-----SCILAB function : pvmsend, fin = 22
      subroutine intspvmsend(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr, address, tag, n
      logical checkrhs,checklhs,getvectrow,getscalar,cremat
c     
      iadr(l)=l+l-1
c     
      if(.not.checkrhs(fname,3,3)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tids (number 1)
c     
      if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     
c     checking variable buff (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
      tag=stk(lr3)

c     construct the pack vector for the variable number 2
c     set adress where to put the pack vector and its max
c     allowable size
      ilpack=iadr(lstk(top)+n1)
      maxsize=iadr(lstk(bot)) - ilpack
      address = top-rhs+2
      call varpak(address,istk(ilpack),n,maxsize,ierr)
      if(ierr .gt. 0) then 
         buf = fname // ' Unknow type or not yet implemented' 
         call error(999)
         return
      endif
c     
c     Check if maxsize has been enough
      if(n.gt.maxsize) then
         err=n-maxsize
         call error(17)
         return
      endif 
c     
      call entier(n1,stk(lr1),istk(iadr(lstk(top))))
c     
c     Gestion du passage par @ du buffer a envoyer.
      iiladdress =  lstk(address)
      iladdress=iadr(lstk(address))
      if(istk(iladdress).lt.0) then 
         iiladdress=istk(iladdress+1)
      endif
      
      call scipvmsend(istk(iadr(lstk(top))),n1,
     $     istk(ilpack),n,
     $     stk(iiladdress),
     $     tag,is)
c     
c     --------------output variable: res
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=is
      return
      end
c
c-----SCILAB function : pvmrecvvar, fin = 23
      subroutine intspvmrecvvar(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getscalar,getsmat,checkval,bufstore
      logical cremat
      integer tid, tag,res
c     
      lbuf = 1

      if(.not.checkrhs(fname,3,3)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tid (number 1)
c     
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      tid = stk(lr1)
c     
c     checking variable tag (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      tag = stk(lr2)
c     
c     checking variable buff (number 3)
c     
      if(.not.getsmat(fname,top,top-rhs+3,m3,n3,1,1,lr3,nlr3)) return
      if(.not.checkval(fname,m3*n3,1)) return
c     
      if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr3,nlr3)) return
      call scipvmrecvvar(tid,tag,buf(lbufi3:lbuff3),res)
c
c     --------------output variable: res
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvmsendvar, fin = 24
      subroutine intspvmsendvar(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      integer topk
      logical checkrhs,checklhs,getvectrow,getsmat,checkval,getscalar
      logical bufstore,cremat
      integer tag,res
c     
      iadr(l)=l+l-1
c     
      lbuf = 1
      topk = top-rhs+1

      if(.not.checkrhs(fname,3,3)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tids (number 1)
c     
      if(.not.getvectrow(fname,top,top-rhs+1,it1,m1,n1,lr1,lc1)) return
c     
c     checking variable buff (number 2)
c     
      if(.not.getsmat(fname,top,top-rhs+2,m2,n2,1,1,lr2,nlr2)) return
      if(.not.checkval(fname,m2*n2,1)) return
      if(.not.bufstore(fname,lbuf,lbufi3,lbuff3,lr2,nlr2)) return
c     
c     checking variable tag (number 3)
c     
      if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
      tag = stk(lr3)

      if(.not.cremat(fname,topk+1,0,1,n1,ltids,loc)) return
      call entier(n1,stk(lr1),istk(iadr(ltids)))

      call scipvmsendvar(istk(iadr(ltids)),n1,buf(lbufi3:lbuff3),
     $     tag,res)
c     
      top=topk
c     --------------output variable: res
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c     
c     SCILAB function : pvmreduce, fin = 25
      subroutine intspvmreduce(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getsmat,checkval,getmat,getscalar
      logical bufstore,cremat
      integer msgtag, rootginst, res
      double precision rval
c     
      lbuf = 1

      if(.not.checkrhs(fname,5,5)) return
      if(.not.checklhs(fname,1,2)) return
c     
c     checking variable func (number 1)
c     
      if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
      if(.not.checkval(fname,m1*n1,1)) return
      if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
c     
c     checking variable data (number 2)
c     
      if(.not.getmat(fname,top,top-rhs+2,it2,m2,n2,ldata,lc2)) return
      kdata=top-rhs+2

c     
c     checking variable msgtag (number 3)
c     
      if(.not.getscalar(fname,top,top-rhs+3,lr3)) return
      msgtag = stk(lr3)
c     
c     checking variable group (number 4)
c     
      if(.not.getsmat(fname,top,top-rhs+4,m4,n4,1,1,lr4,nlr4)) return
      if(.not.checkval(fname,m4*n4,1)) return
      if(.not.bufstore(fname,lbuf,lbufi7,lbuff7,lr4,nlr4)) return
c     
c     checking variable rootginst (number 5)
c     
      if(.not.getscalar(fname,top,top-rhs+5,lr5)) return
      rootginst = stk(lr5)

      if(ldata.ge.bot) then
c       buff is passed by reference
         kdata=top-rhs+6
         if(.not.cremat(fname,top-rhs+6,it2,m2,n2,lrs,lcs)) return
         call dcopy(m2*n2*(it2+1),stk(ldata),1,stk(lrs),1)
         ldata=lrs
      endif
c     
      call scipvmreduce(buf(lbufi1:lbuff1),nlr1,stk(ldata),m2,n2,msgtag,
     $     buf(lbufi7:lbuff7),nlr4,rootginst,res)

      if(err .gt. 0) then 
         buf = fname // ' Internal Error' 
         call error(999)
         return
      endif
c     
c     --------------output variable: buff
      call copyobj(fname,kdata,top-rhs+1)
      top=top-rhs+1
c     
      if(lhs .ge. 2) then
c     --------------output variable: res
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=res
      endif
      return
      end
c
c-----SCILAB function : pvm_start, fin = 26
      subroutine intspvmstart(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,cresmat2,getsmat,checkval,cremat
      logical bufstore
      integer res
c     
      lbuf = 1

      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable hostfile (number 1)
c     
      if(rhs .le. 0) then
         buf='null'//char(0)
         lbufi2=1
         lbuff2=5
      else
         if(.not.getsmat(fname,top,top-rhs+1,m1,n1,1,1,lr1,nlr1)) return
         if(.not.checkval(fname,m1*n1,1)) return
         if(.not.bufstore(fname,lbuf,lbufi2,lbuff2,lr1,nlr1)) return
      endif
c
      call scipvmstart(res,buf(lbufi2:lbuff2),nlr1)

c     
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end
c
c-----SCILAB function : pvm_halt, fin = 27
      subroutine intspvmhalt(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer res
      logical checkrhs,checklhs,cremat
c     
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     cross variable size checking
c     
      call scipvmhalt(res)

c     
c     --------------output variable: res
      top=top+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=res
      return
      end

c
c-----SCILAB function : pvm_error, fin = 28
      subroutine intspvmerror(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getscalar,crepointer,crestringmatfromC
      integer num
      double precision pstr
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable err (number 1)
c     
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      num = stk(lr1)

      call scipvmerror(num,pstr)

c     --------------output variable: res
      lw2=lstk(bot)-1
      stk(lw2)=pstr
      if(.not.crestringmatfromC(fname,top,lw2,1,1)) return
      return
      end
c
c-----SCILAB function : pvm_sci2f77, fin = 29
      subroutine intspvmsci2f77(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr
      integer ila, topfrom
      logical checkrhs,checklhs
c     
      iadr(l)=l+l-1
c
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable var (number 1)
      ila=iadr(lstk(top))
      if(istk(ila).lt.0) then
c     the variable is passed by reference, a copy is made
         topfrom=istk(ila+2)
         call copyobj(fname,topfrom,top)
      endif
c     
      call scipvmscitof77(top)
c     
      return
      end
c     
c-----SCILAB function : pvm_f772sci, fin = 30
      subroutine intspvmf772sci(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      integer iadr 
      integer ila,topfrom
      logical checkrhs,checklhs
c
      iadr(l)=l+l-1
c
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable var (number 1)
c     
      ila=iadr(lstk(top))
      if(istk(ila).lt.0) then
c     the variable is passed by reference, a copy is made
         topfrom=istk(ila+2)
         call copyobj(fname,topfrom,top)
      endif
c     
      call scipvmf77tosci(top)
c     
      return
      end
c
c-----SCILAB function : pvm_probe, fin = 31
      subroutine intspvm_probe(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getscalar,cremat
      integer tid, msgtag, res
c     
      if(.not.checkrhs(fname,2,2)) return
      if(.not.checklhs(fname,1,1)) return
c     
c     checking variable tid (number 1)
c     
      if(.not.getscalar(fname,top,top-rhs+1,lr1)) return
      tid = stk(lr1)
c     
c     checking variable msgtag (number 2)
c     
      if(.not.getscalar(fname,top,top-rhs+2,lr2)) return
      msgtag = stk(lr2)

      call scipvmprobe(tid,msgtag,res)
c     
      top=top-rhs+1
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=tid
      return
      end
c
c-----SCILAB function : pvm_bufinfo, fin = 32
      subroutine intspvm_bufinfo(fname)
c     
      character*(*) fname
      include '../stack.h'
c     
      logical checkrhs,checklhs,getscalar,cremat
      integer bufid, bytes, msgtag, tid, res
c     
      if(.not.checkrhs(fname,1,1)) return
      if(.not.checklhs(fname,1,4)) return
c
c     checking variable bufid (number 1)
      if(.not.getscalar(fname,top,top,lr1)) return
      bufid=stk(lr1)

      call scipvmbufinfo(bufid, bytes, msgtag, tid, res)
c     
c     --------------output variable: bytes
      if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
      stk(lrs)=bytes
      if(lhs .ge. 2) then
c     --------------output variable: msgtag
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=msgtag
      endif
      if(lhs .ge. 3) then
c     --------------output variable: tid
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=tid
      endif
      if(lhs .ge. 4) then
c     --------------output variable: res
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lrs,lcs)) return
         stk(lrs)=res
      endif
      return
      end
c
c  interface function 
c   ********************
       subroutine intpvm
       include '../stack.h'
       rhs = max(0,rhs)
c
       goto (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22
     $      ,23
     $      ,24
     $      ,25
     $      ,26,27,28,29,30,31,32) fin
       return
1      call intspvmjoingroup('pvm_joingroup')
       return
2      call intspvmlvgroup('pvm_lvgroup')
       return
3      call intspvmgsize('pvm_gsize')
       return
4      call intspvmgettid('pvm_gettid')
       return
5      call intspvmgetinst('pvm_getinst')
       return
6      call intspvmbarrier('pvm_barrier')
       return
7      call intspvmbcast('pvm_bcast')
       return
8      call intspvmtasks('pvm_tasks')
       return
9      call intspvmconfig('pvm_config')
       return
10      call intspvmaddhosts('pvm_addhosts')
       return
11      call intspvmdelhosts('pvm_delhosts')
       return
12      call intspvmparent('pvm_parent')
       return
13      call intspvmtidtohost('pvm_tidtohost')
       return
14      call intspvmsettimer('pvmset_timer')
       return
15      call intspvmgettimer('pvm_get_timer')
       return
16      call intspvmmytid('pvm_mytid')
       return
17      call intspvmexit('pvm_exit')
       return
18      call intspvmkill('pvm_kill')
       return
19      call intspvmspawn('pvm_spawn')
       return
20      call intspvmspawnindependent('pvm_spawn_independent')
       return
21      call intspvmrecv('pvm_recv')
       return
22      call intspvmsend('pvm_send')
       return
23      call intspvmrecvvar('pvm_recv_var')
       return
24      call intspvmsendvar('pvm_send_var')
       return
25      call intspvmreduce('pvm_reduce')
        return
26      call intspvmstart('pvm_start')
       return
27      call intspvmhalt('pvm_halt')
       return
28      call intspvmerror('pvm_error')
       return
29      call intspvmsci2f77('pvm_sci2f77')
       return
30      call intspvmf772sci('pvm_f772sci')
       return
31      call intspvm_probe('pvm_probe')
       return
32      call intspvm_bufinfo('pvm_bufinfo')
       return
       end
