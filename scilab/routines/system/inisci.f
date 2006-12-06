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
c     Copyright INRIA
      integer        ierr,ini1,vsizr
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
c     
c      integer         nunit,unit(50)
c      common /units / nunit,unit
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

      integer graphicsmodels
      logical first
      double precision dlamch
      integer i,k,l,nc,mode(2),vsizg
      integer eps(nsiz),im(nsiz),exp(nsiz),pi(nsiz),bl(nsiz),io(nsiz)
      integer true(nsiz),false(nsiz),dollar(nsiz)
      integer idloc(nsiz)
      integer offset,goffset
      integer iadr,sadr
c     
      double precision  iov(2)
      character*(nlgh) vname 
c     character set
c     0       10       20       30       40       50
c     
c     0      0        a        k        u   colon  :  less   <
c     1      1        b        l        v   plus   +  great  >
c     2      2        c        m        w   minus  -  perc   %
c     3      3        d        n        x   star   *  under  _
c     4      4        e        o        y   slash  /
c     5      5        f        p        z   bslash \
c     6      6        g        q  blank     equal  =
c     7      7        h        r  lparen (  dot    .
c     8      8        i        s  rparen )  comma  ,
c     9      9        j        t  semi   ;  quote  '
c     
      character alpha(csiz)*1,alphb(csiz)*1
      data alpha /'0','1','2','3','4','5','6','7','8','9',
     $     'a','b','c','d','e','f','g','h','i','j',
     $     'k','l','m','n','o','p','q','r','s','t',
     $     'u','v','w','x','y','z','_','#','!','$',
     $     ' ','(',')',';',':','+','-','*','/','\\',
     $     '=','.',',','''','[',']','%','|','&','<','>','~',
     $     '^'/
c     
c     alternate character set
c     
      data alphb /'0','1','2','3','4','5','6','7','8','9',
     $     'A','B','C','D','E','F','G','H','I','J',
     $     'K','L','M','N','O','P','Q','R','S','T',
     $     'U','V','W','X','Y','Z','0','0','?','0',
     $     '0','0','0','0','0','0','0','0','0','$',
     $     '0','0','0','"','{','}','0','0','0','`','0','@',
     $     '0'/
c     
      data im/673714744,nz1*673720360/,exp/673713720,nz1*673720360/
      data pi/672274744,nz1*673720360/,bl/nsiz*673720360/
      data eps/471404088,nz1*673720360/,io/672666168,nz1*673720360/
      data dollar/673720359,nz1*673720360/
      data first/.true./
      data true/673717560,nz1*673720360/,false/673713976,nz1*673720360/
      data nunit/50/
c     
      save /basbrk/,/mprot/
c      save /units/
c     

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      ierr=0
      mode(2)=0
      ieee=0

c     initialization call
c     -------------------

c     .  dynamic linking initialization
c     .  ------------------------------
      nlink=0
c     
c     .  scilab function protection mode
c     .  ------------------------------
      macprt=1
c     
c     .  standard i/o initialization
c     .  ----------------------------
c      call iset(nunit,0,unit,1)
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
      wio = 0
c     .  hio =unit for history output (no more used)
      hio = 0
c     
c     .  banner 
c     .  ------
c     call banier(wte)
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
c     .  Initial values for main window row and column sizes
c     .  ---------------------------------------------------

      if (intexmacs().eq.0) then
         lct(2) = 1
      else
         lct(2) = 0
      endif
      call scilines(28,72)

c     .  en mode fenetre ces valeurs sont remplacees par les dimension effectives

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
c     .  character set
c     .  -------------
      do 20 i = 1, csiz
         alfa(i) = alpha(i)
         alfb(i) = alphb(i)
 20   continue
c     add tab
      alfb(41)=char(9) 

c     .  initial type names
c     .  ------------------
      call settypnames()
      if(err.gt.0) then
         ierr=err
         return
      endif
c     .  initial interface properties
c     .  ----------------------------
      call setippty(0)
      ier= graphicsmodels()
c     
c     .  Stack
c     .  -----
c     
c     . initial values for number of local and global variables
      isiz=isizt-768

c     .  memory allocation
      stk(1)=1.0d0
      offset=0
      call scimem(vsizr,offset)
      lstk(1) =   offset+1
c     . hard predefined variables
      goffset=0
      vsizg=1000
      call scigmem(vsizg,goffset)

      gtop=isiz+1
      lstk(gtop+1)=goffset+1
      gbot=isizt
      lstk(gbot)=lstk(gtop+1)+vsizg-1
c
C     If you want to add more predefined variables, takle care of the
C     following points:
C     - You have to update the number of predefined variable (npredef)
C       ignoring bl
c     - To update the total amount of memory (lpvar) used by all the predefined
C       variables .
c       memory requested for each type of predefined variables 
c       mxn boolean matrix size : sadr(2+m*n+2)        --> sadr(5) for a boolean scalar
c       mxn  matrix size        : sadr(3)+m*n*(it+1))  --> sadr(3)+1 for a real scalar
c       1x1 string matrix       : sadr(6+nchar)+1
c       $                       : sadr(10-1) + 2 
c     - add the new variable at the beginning
      npredef=13

      bot=isiz-npredef
      bbot=bot
      bot0=bot
      call getcomp(buf,nbuf)
c     1 $, 6 booleans , 4 real scalar, 2 2x1 real array, 1 string     
      lpvar = (sadr(10-1) + 2) 
     $     + 6*sadr(5) 
     $     + 4*(sadr(3)+1)
     $     + 2*(sadr(3)+2)
     $     + 1*(sadr(6+nbuf)+1)
      l=vsizr-lpvar
      k=bot
      lstk(k)=lstk(1)-1+l
c     . COMPILER
      vname = ' '
      vname(1:8) = "COMPILER"
      call cvname(idloc,vname,0)
      call cresmatvar(idloc,k,buf,nbuf)
      k=k+1
c     . %gtk 
      vname = ' '
      vname(1:4) = "%gtk"
      call withgtk(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %gui
      vname(1:4) = "%gui"
      call withgui(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %pvm
      vname(1:4) = "%pvm"
      call withpvm(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     . %tk 
      vname(1:4) = "%tk "
      call withtk(irep)
      call cvname(idloc,vname,0)
      call crebmatvar(idloc,k,1,1,irep)
      k=k+1
c     .  $    : formal index
      call putid(idstk(1,k),dollar)
      il=iadr(lstk(k))
      istk(il)=2
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=39
      istk(il+5)=40
      istk(il+6)=40
      istk(il+7)=40
      istk(il+8)=1
      istk(il+9)=3
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
c     . %test 
c      call cvname(idloc,"%test",0)
c      call crematvar(idloc,k,0,1,1,dlamch('p'),0.0d0)
c      k=k+1
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
c     .  blanc 
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

      subroutine crematvar(id,lw,it,m,n,rtab,itab)
C     same as cremat, but without test ( we are below bot)
C     and adding a call to putid 
C     cree une variable de type matrice 
C     de nom id 
C     en lw : sans verification de place 
C     implicit undefined (a-z)
      integer id(*),lw,it,m,n,lr,lc,il,iadr,sadr
      double precision rtab(*),itab(*)
      include '../stack.h'
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      call putid(idstk(1,lw),id)
      il=iadr(lstk(lw))
      istk(il)=1
      istk(il+1)=m
      istk(il+2)=n
      istk(il+3)=it
      lr=sadr(il+4)
      lc=lr+m*n
      if (lw.lt.isiz) lstk(lw+1)=sadr(il+4)+m*n*(it+1)
      call unsfdcopy(m*n,rtab,1,stk(lr),1)
      if (it.eq.1) call unsfdcopy(m*n,itab,1,stk(lc),1)
      return
      end

      subroutine crebmatvar(id,lw,m,n,val)
C     crebmat without check and call to putid 
C     implicit undefined (a-z)
      integer iadr,sadr,lw,m,n,val(*),il,id(*),lr
      include '../stack.h'
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      call putid(idstk(1,lw),id)
      il=iadr(lstk(lw))
      istk(il)=4
      istk(il+1)=m
      istk(il+2)=n
      lr=il+3
      lstk(lw+1)=sadr(il+3+m*n+2)
      call icopy(m*n,val,1,istk(lr),1)
      end


      subroutine cresmatvar(id,lw,str,lstr)
C     crebmat without check and call to putid 
C     implicit undefined (a-z)
      integer iadr,sadr,lw,m,n,il,id(*),lr,nchar
      integer mn
      include '../stack.h'
      character str*(*)
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      call putid(idstk(1,lw),id)
      il = iadr(lstk(lw))
      mn = 1 
      ix1 = il + 4 + (lstr + 1) + (mn + 1)
      istk(il ) = 10
      istk(il +1) = 1
      istk(il +2) = 1
      istk(il +3) = 0
      ilp = il + 4
      istk(ilp) = 1
      istk(ilp+1) = istk(ilp) + lstr
      ilast = ilp + mn
      lr1 = ilast + istk(ilp )
      call cvstr(lstr,istk(lr1),str,0)
      ix1 = ilast + istk(ilast )
      lstk(lw +1) = sadr(ix1)
      return 
      end 


