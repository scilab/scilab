      subroutine simblk(neq,t,xc,xcdot)
c!purpose
c     interface to simbl1 at the lsodar format
c!calling sequence
c     neq   : integer vector whose first element is the size of the
c             continuous state. next elements store integer arrays used by
c             simbl1 and grbl1
c     t     : current time 
c     xc    : double precision vector whose first neq(1) elements contain
c             the continuous state. Next elements store double precision
c             arrays used by simbl1 and grbl1
c     xcdot : double precision vector, contain the computed derivative
c             of the state 
c!
c     Copyright INRIA
      integer neq(*)
      double precision t
      double precision xc(*)
      double precision xcdot(*)
c
      external simbl1
c 
      integer lfunpt,lxptr,lz,lzptr,liz,lizptr,lrpar,lrpptr,lipar
      integer lipptr,linpptr,linplnk,loutptr,loutlnk,llnkptr
      integer louttb,loord,lzord,lfuntyp

      common /cosptr/ lfunpt,lxptr,lz,lzptr,liz,lizptr,lrpar,lrpptr,
     &     lipar,lipptr,linpptr,linplnk,loutptr,loutlnk,llnkptr,
     &     louttb,loord,lzord,lfuntyp
c
      integer         nblk,nxblk,ncblk,ndblk,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
      common /cossiz/ nblk,nxblk,ncblk,ndblk,nout,ng,nrwp,niwp,ncord,
     &     noord,nzord
c
      call simbl1(neq(lfunpt),neq(lfuntyp),t,xcdot,xc,neq(lxptr),
     $     xc(lz),neq(lzptr),
     $     neq(liz),neq(lizptr),xc(lrpar),neq(lrpptr),neq(lipar),
     $     neq(lipptr),neq(linpptr),neq(linplnk),neq(loutptr),
     $     neq(loutlnk),neq(llnkptr),xc(louttb),nxblk,neq(loord),
     $     noord) 

      end
c
      subroutine simbl1(funptr,funtyp,told,xd,x,xptr,z,zptr,iz,
     $        izptr,rpar,rpptr,ipar,ipptr,inpptr,inplnk,
     $        outptr,outlnk,lnkptr,outtb,nxblk,oord,noord)
c!purpose
c     compute state derivative of the continuous part
c!calling sequence
c     funptr : table of block simulation functions adresses
c     told   : current rtime value
c     xd     : vector of full continuous state derivatives
c     x      : vector of full continuous state
c     xptr   : x,xd splitting array on individual blocks
c     z      : floatting point full discrete state
c     zptr   : z splitting array on individual blocks
c     iz     : integer full discrete state
c     izptr  : iz splitting array on individual blocks
c     rpar   : vector resulting of the concatenation of blocks floatting
c              point parameters
c     rpptr  : rpar splitting array on individual blocks
c     ipar   : vector resulting of the concatenation of blocks integer
c              parameters
c     ipptr  : ipar splitting array on individual blocks
c     inpptr : input  splitting array
c     inplnk : input indirection table
c     outptr : output  splitting array
c     outlnk : output indirection table
c     lnkptr : outtb splitting array by link
c     outtb  : vector containing the concatenation of link (vector) values
c!
      double precision told,xd(*),x(*),z(*),rpar(*),outtb(*)
      integer funptr(*),funtyp(*),xptr(*),zptr(*),iz(*),izptr(*)
      integer rpptr(*),ipar(*),ipptr(*),inpptr(*),inplnk(*),outptr(*)
      integer outlnk(*),lnkptr(*),nxblk,oord(*),noord
c
      integer iero
      common /ierode/ iero

c     
      integer kfun
      common /curblk/ kfun
c 
      integer jj,flag,nclock,ntvec
      double precision tvec(1)
c 
c     compute outputs necessary for computing xdot
      iero = 0
      nclock = 0
      tvec(1)=0.0d0
      ntvec=0
      if (noord.gt.0) then
         do 10 jj=1,noord
            kfun=oord(jj)
            flag=1
            call callf(kfun,nclock,funptr,funtyp,told,x,x,xptr,z,zptr,iz
     $           ,izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk
     $           ,outptr,outlnk,lnkptr,outtb,flag) 
            if (flag .lt. 0) then
               iero = 5 - flag
               return
            endif

 10      continue
      endif
c
c     compute  xdot
c
      nclock = 0
      do 20 kfun = 1,nxblk
        flag = 2
        call callf(kfun,nclock,funptr,funtyp,told,xd,x,xptr,z,zptr,iz,
     $       izptr,rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk,
     $       outptr,outlnk,lnkptr,outtb,flag) 
        if (flag .lt. 0) then
          iero = 5 - flag
          return
        endif
 20   continue
      end



