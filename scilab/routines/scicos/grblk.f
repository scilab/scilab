      subroutine grblk(neq,t,xc,ng1,g)
c!purpose
c     interface to grbl1 at the lsodar format
c!calling sequence
c     neq   : integer vector whose first element is the size of the
c             continuous state. next elements store integer arrays used by
c             simbl1 and grbl1
c     t     : current time 
c     xc    : double precision vector whose first neq(1) elements contain
c             the continuous state. Next elements store double precision
c             arrays used by simbl1 and grbl1
c     g     : computed zero crossing surface (see lsodar)
c     ng1   : size ng
c!
c     Copyright INRIA
      integer neq(*)
      double precision t
      double precision xc(*)
      integer ng1
      double precision g(ng1)
c
      external grblk1
c 

c     adress of differents array stored in neq and xc
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
      call grblk1(neq(lfunpt),neq(lfuntyp),t,xc,neq(lxptr),xc(lz),
     $     neq(lzptr),neq(liz),neq(lizptr),xc(lrpar),neq(lrpptr),
     $     neq(lipar),neq(lipptr),neq(linpptr),neq(linplnk),
     $     neq(loutptr),neq(loutlnk),neq(llnkptr),xc(louttb),nblk,
     $     ncblk+ndblk+1,neq(lzord),nzord,g,ng1) 
      end
C
      subroutine grblk1(funptr,funtyp,told,x,xptr,z,zptr,iz,
     $        izptr,rpar,rpptr,ipar,ipptr,inpptr,inplnk,
     $        outptr,outlnk,lnkptr,outtb,nblk,kzblk,zord,nzord,g,ng)
c!purpose
c     compute state derivative of the continuous part
c!calling sequence
c     funptr : table of block simulation functions adresses
c     told   : current rtime value
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
c     nblk   : number of blocks
c     kzblk  : first zero crossing block address
c     outtb  : vector containing the concatenation of link (vector)
c              values
c     zord   : block evaluation order necessary to compute zero crossing inputs
c     nzord  : zord vector size
c     g      : concatenation of zero crossing blocks inputs
c     ng     : g vector size
c!
      double precision told,x(*),z(*),rpar(*),outtb(*),g(*)
      integer funptr(*),funtyp(*),xptr(*),zptr(*),iz(*),izptr(*)
      integer rpptr(*),ipar(*),ipptr(*),inpptr(*),inplnk(*)
      integer outptr(*),outlnk(*),lnkptr(*),nblk,kzblk,zord(*),nzord,ng
c
      integer ig,kport,klink,n,flag
      double precision tvec(1)
c
      integer iero
      common /ierode/ iero
c     
      integer kfun
      common /curblk/ kfun
c 
c     compute threshold inputs 
c
      iero = 0
      nclock = 0
      tvec(1)=0.0d0
      ntvec=0
      if(nzord.gt.0) then
         do 10 jj=1,nzord
            kfun=zord(jj)
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
c     form z vector (concatenation of threshold inputs)
c
      ig=1
c     loop on zero crossing blocks
      do 30 kfun=kzblk,nblk
c     .  loop on block input ports
         do 25 kport=inpptr(kfun),inpptr(kfun+1)-1
c     .     get corresponding link pointer 
            klink=inplnk(kport)
            n=lnkptr(klink+1)-lnkptr(klink)
c     .     copy vector valued link in g
            call dcopy(n,outtb(lnkptr(klink)),1,g(ig),1)
            ig=ig+n
 25      continue
 30   continue
      end

