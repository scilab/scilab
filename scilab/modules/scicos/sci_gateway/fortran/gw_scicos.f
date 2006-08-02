c     ********************
c     Copyright INRIA
c     ********************
c fin = 1 
c SCILAB function : scicos
c     interface function
c     ********************
      subroutine  gw_scicos
c
      include 'stack.h'
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
c    var2vec  vec2var curblock getblocklabel scicos_debug
c       1          2       3         4            5
c
c   debug_count sctree  sci_tree2  sci_tree3   sci_tree4
c       6          7       8          9           10
c
      goto (1,2,3,4,5,6,7,8,9,10) fin
c
c     var2vec
 1    continue
			call intvar2vec
      goto 999
c
c     vec2var
 2    continue
 			call intvec2var
      goto 999
c
c     curblock
 3    continue
      call intcurblk
      goto 999
c
c     getblocklabel
 4    continue
      call intgetlabel
      goto 999
c
c     scicos_debug(i)
 5    continue
      fname='scicos_debug' 
      call scicosdebug(fname)
      goto 998
c
c     scicos_debug_count
 6    continue
      fname='scicos_debug_count'
      call scicosdebugcount(fname)
      goto 998
c
c     ctree
 7    call intctree
      return
c
c     [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
 8    continue
      fname='sci_tree2'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,4,4)) return
      call intree2(fname)
      goto 998
c
c     [ord,ok]=tree3(vec,dep_ut,typ_l,bexe,boptr,blnk,blptr)
 9    continue
      fname='sci_tree3'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,7,7)) return
      call intree3(fname)
      goto 998
c
c     [r1,r2,ok]=tree4(vec,outoin,outoinptr,dep_ut,typ_r)
 10   continue
      fname='sci_tree4'
c
      if(.not.checklhs(fname,2,2)) return
      if(.not.checkrhs(fname,5,5)) return
      call intree4(fname)
      goto 998

 998  if(.not.putlhsvar())return
 999  return
      end
c     ********************
