c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine inisci( ini1, vsizr, ierr)
c!Purpose 
c     scilab initialisation
c!Parameters
c     ini1 :
c     = -1  for silent initialization
c     = -3  for special io initialization 
c     vsizr: initial stack size
c     ierr : return error flag
c!   
c====================================================================
      integer        ierr,ini1,vsizr
      include 'stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
c     
c
c     common for scicos debug mode

      common /cosdebug/ cosd
      integer cosd

c     common for Control-C interruptions
      logical iflag,interruptible
      common /basbrk/ iflag,interruptible
c     scilab function protection mode
      integer macprt
      common /mprot/ macprt
c     mmode : matlab ops compatibilty mode
      common /mtlbc/ mmode
c     
c     simpmd : rational fraction simplification mode
      integer simpmd
      common /csimp/  simpmd
c     ippty: interfaces properties
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
      common /ippty/ byptr,nbyptr

      logical first
      double precision dlamch
      integer k,l,mode(2),vsizg,stacksize
      integer eps(nsiz),im(nsiz),exp(nsiz),pi(nsiz),bl(nsiz),io(nsiz)
      integer true(nsiz),false(nsiz),dollar(nsiz)
      integer idloc(nsiz)
      integer offset,goffset
      integer iadr,sadr
      
      character bufcomp*(bsiz)
      character bufsci*(bsiz)
      character bufscihome*(bsiz)
      character buftmp*(bsiz)

c     
      double precision  iov(2)
      character*(nlgh) vname 
c  
      data im/673714744,nz1*673720360/,exp/673713720,nz1*673720360/
      data pi/672274744,nz1*673720360/,bl/nsiz*673720360/
      data eps/471404088,nz1*673720360/,io/672666168,nz1*673720360/
      data dollar/673720359,nz1*673720360/
      data first/.true./
      data true/673717560,nz1*673720360/,false/673713976,nz1*673720360/
      
c     
c     mprot used to protect function see funcprot.c
      save /basbrk/,/mprot/

c     

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      ierr=0
      mode(2)=0
      ieee=0

c     initialization call
c     -------------------

c     initialization C environment
      call initscilab
c     
c     .  scilab function protection mode
c     .  ------------------------------
      call initfuncprot
c     
c     .  standard i/o initialization
c     .  ----------------------------
c     .  rte = unit number for terminal input
      err=0
      if(ini1.ne.-3) then
         rte = 5
         mode(1)=0
         call clunit(rte,buf,mode)
         if(err.gt.0) then
            call error(241)
            ierr=err
            return
         endif
      else
         rte=9999
      endif
      rio = rte
c     .  wte = unit number for terminal output
      if(ini1.ne.-3) then
         wte = 6
         mode(1)=1
         call clunit(wte,buf,mode)
         if(err.gt.0) then
            call error(240)
            ierr=err
            return
         endif
      else
         wte=9999
      endif
c     
      rio=rte
c     
c     .  Control-C recovery
c     .  ------------------
      if (first) then
         call inibrk
         interruptible=.true.
         first=.false.
      endif
c     
c     .  random number seed
c     .  ------------------
      ran(1) =  0
      ran(2) =  0
c     
c     .  initial format for number display
c     .  ---------------------------------
      lct(6) =  1
      lct(7) = 10
c     
c     .  initial debug mode
c     .  ------------------
      ddt = 0

c
c     .  scicos initial debug mode
      cosd = 0
c      
c     .  initial type names
c     .  ------------------
      call inittypenames()
      if(err.gt.0) then
         ierr=err
         return
      endif
c     .  initial interface properties
c     .  ----------------------------
      call setippty(0)
c      ier= graphicsmodels()
c     
c     .  Stack
c     .  -----
c     
c     . initial values for number of local and global variables
c @TODO : What is 768 (512 + 256) ?
      isiz=isizt-768

c     .  memory allocation
c     get default stackize from c
      call getdefaultstacksize(stacksize)
      offset=0
      call scimem(vsizr,offset)
      stk(1) = stacksize
      lstk(1) =   offset+1
c     . hard predefined variables
      goffset=0
c     get default global stackize from c      
      call getdefaultgstacksize(vsizg)
      call scigmem(vsizg,goffset)

      gtop=isiz+1
      lstk(gtop+1)=goffset+1
      gbot=isizt
      lstk(gbot)=lstk(gtop+1)+vsizg-1
c
c     17 is the number of predefined variables 
c     18 - 1 blank not include
      bot=isiz-17
      bbot=bot
      bot0=bot
c     memory requested for predefined variables 
c     mxn bmat -> size : sadr(2+m*n+2)
c     $        -> size : sadr(10-1) + 2 
c     mxn mat  -> size : sadr(3)+m*n*(it+1)
c     string   -> size : sadr(6+nchar)+1
c     4 strings
c     7 booleans
      call getcomp(bufcomp,nbcomp)
      call getsci(bufsci,nbsci)
      call getscihome(bufscihome,nbscihome)
      call gettmpdir(buftmp,nbtmpdir)
      lpvar = (sadr(10-1) + 2) 
     $     + 7*sadr(5) 
     $     + 4*(sadr(3)+1)
     $     + 2*(sadr(3)+2)
     $     + 1*(sadr(6+nbcomp)+1)
     $     + 1*(sadr(6+nbsci)+1)
     $     + 1*(sadr(6+nbscihome)+1)
     $     + 1*(sadr(6+nbtmpdir)+1)

      l=vsizr-lpvar
      k=bot
      lstk(k)=lstk(1)-1+l
C     . COMPILER
      vname = ' '
      vname(1:8) =  "COMPILER"
      call cvname(idloc,vname,0)
      call cresmatvar(idloc,k,bufcomp,nbcomp)
      k=k+1
c     . SCI
      vname = ' '
      vname(1:3) =  "SCI"
      call cvname(idloc,vname,0)
      call cresmatvar(idloc,k,bufsci,nbsci)
      k=k+1
c     . SCIHOME
      vname = ' '
      vname(1:7) =  "SCIHOME"
      call cvname(idloc,vname,0)
      call cresmatvar(idloc,k,bufscihome,nbscihome)
      k=k+1      
c     . TMPDIR
      vname = ' '
      vname(1:6) = "TMPDIR"
      call cvname(idloc,vname,0)
      call cresmatvar(idloc,k,buftmp,nbtmpdir)
      k=k+1
c     . MSDOS
      vname = ' '
      vname(1:5) = "MSDOS"
      call withmsdos(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1 
c     . %gui
      vname = ' '
      vname(1:4) = "%gui"
      call withgui(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %pvm
      vname = ' '
      vname(1:4) = "%pvm"
      call withpvm(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %tk
      vname = ' ' 
      vname(1:3) = "%tk"
      call withtk(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %fftw 
      vname = ' '
      vname(1:5) = "%fftw"
      call withfftw(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     .  $    : formal index
      call putid(idstk(1,k),dollar)
      il=iadr(lstk(k))
c @TODO: What is 2 ?
      istk(il)=2
c @TODO: What is 1 ?
      istk(il+1)=1
c @TODO: What is 1 ?
      istk(il+2)=1
c @TODO: Why is 0 ?
      istk(il+3)=0
c @TODO: What is 39 ?
      istk(il+4)=39
c @TODO: What is 40 ?
      istk(il+5)=40
c @TODO: What is 40 ?
      istk(il+6)=40
c @TODO: What is 40 ?
      istk(il+7)=40
c @TODO: What is 1 ?
      istk(il+8)=1
c @TODO: What is 3 ?
      istk(il+9)=3
c @TODO: Why 10 ?
      lw=sadr(il+10)
      stk(lw)=0.0d0
      stk(lw+1)=1.0d0
      lstk(k+1)=lw+2
      k=k+1
c     .  %t   : True boolean
      call crebmatvar(true,k,1,1,1)
      k=k+1
c     .  %f   : False boolean
      call crebmatvar(false,k,1,1,0)
      k=k+1
c     .  %eps : machine precision 
      call crematvar(eps,k,0,1,1,dlamch('p'),0.0d0)
      leps=sadr( iadr(lstk(k)) +4)
      k=k+1
c     .  %io : standard input&output
      iov(1)=dble(rte)
      iov(2)=dble(wte)
      call crematvar(io,k,0,1,2,iov,0.0d0)
      k=k+1
c     .  %i : sqrt(-1)
      call crematvar(im,k,1,1,1,0.0d0,1.0d0)
      k=k+1
c     .  %e : exp(1)
      call crematvar(exp,k,0,1,1,2.71828182845904530d+0,0.0d0)
      k=k+1
c     .  %pi 
      call crematvar(pi,k,0,1,1,3.14159265358979320d+0,0.0d0)
      k=k+1
c     .  blanc. Memory used by getsym to store parsed number
C     .  then by getnum.f and macro.f to retreive it (stk(lstk(isiz) -))

      call crematvar(bl,k,0,1,1,0.0d0,0.0d0)
      k=k+1
c     
c
c     --------------
c     initialize
c     --------------
c
c     compilation flag
      comp(1) =   0
      comp(2) =   0
      comp(3) =   0
c     error indicators
      err     =   0
      errct   =   -1
      err1    =   0
      err2    =   0
      catch   =   0 
c     recursion
      fun     =   0
      macr    =   0
      niv     =   0
      paus    =   0
      pt      =   0
c     stack variable
      top     =   0
c     debug mode
      lcntr   =   0
      nmacs   =   0
      lgptrs(1)=  1
      wmac    =   0
      mmode   =   0
      simpmd  =   1

      return
      end

