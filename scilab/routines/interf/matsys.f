      subroutine matsys
c     ====================================================================
c     
c     evaluate system functions
c     
c     ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
c     
      common /mprot/ macprt
c
      
      integer eol,comma,p,r
      integer id(nsiz),fe,fv,semi
      integer double,reel,ent,sort,errn,orts,rtso,tsor,out
      integer uto,tou
      integer top2,tops,pt0,count,fptr,bbots,cmode
      integer local
      double precision x
      double precision ar,ai,dx,dy
      complex*16 zabs
      logical flag,eqid
      integer offset
      integer iadr
c     
      character*(nlgh)    name
c
      common /mtlbc/ mmode
c
      integer resume(nsiz),sel(nsiz)
      data resume/505155099,673713686,nz2*673720360/
      data sel/236260892,673717516,nz2*673720360/
      data eol/99/,semi/43/
      data comma/52/

      data fe/14/,fv/31/
      data double/13/,reel/27/,ent/18/,sort/28/
      data orts/24/,rtso/27/,tsor/29/,out/24/,uto/30/,tou/29/
      data local/21/
c    
      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
c     
c     fonction/fin
c     debu who        line argn comp   fort  mode type error
c     1    2    3    4    5    6      7     8    9    10
c     resu form  link   exists errcatch errclear iserror predef
c     11   12     XXX    14      15       16       17      18
c     newfun clearfun  funptr  macr2lst setbpt delbpt dispbpt
c     19      20       21       22       23     24      25
c     funcprot whereis where   timer         havewindow stacksize
c     26         27    28      29       30       31       32
c     mtlb_mode  link     ulink  c_link addinter <free>   <free>
c     33         34        35     36    37       38        39
c     fclear    what    sciargs  chdir getwd ieee typename
c     40         41       42     43     44     45    46
c     global   clearglobal isglobal gstacksize getdate intppty
c     47         48          49        50        51       52
c     lasterror version loadhistory savehistory gethistory resethistory sendtobrowser macr2tree
c     53         54        55          56         57          58        59            60
c     hidetoolbar use_as_command    
c     61             62
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matsys '//buf(1:4))
      endif
c     
      if(rstk(pt).eq.901) goto 70

      if(rhs.gt.0) il=iadr(lstk(top))
      goto (10 ,20 ,998 ,55,60 ,70 ,80 ,120,130,140,
     +      150,160,190,180,200,210,220,230,240,250,
     +      251,300,320,350,370,380,390,400,410,420,
     +      450,500,510,600,610,620,630,640,650,660,
     +      670,680,681,682,683,684,690,691,692,693,
     +      694,695,697,698,699,700,701,702,703,704,
     +      705,706),fin
c     
c     debug
 10   call intdebug()
      go to 999
c  
c     who
 20   continue
      call intwho()
      goto 999
c     
c     lines
 55   call intlines()
      goto 999
c     
c     argn
 60   call intargn()
      goto 999
c     
c     compilation
 70   call intcomp()
      goto 999
c     
c     fort
c     ----
 80   call intfort('fort')
      goto 999
c     
c     mode
 120  call intmode()
      goto 999
c     
c     type
 130  call inttype()
      goto 999
c     
c     error
 140  call interror()
      return
c     
c     resume/return
 150  call intresume()
      return
c     
c     format
 160  call intformat()
      goto 999
c     
c     exists
 180  call intexists()
      goto 999
c     
c     link dynamique
 190  continue
      return
c
c     errcatch
 200  call interrcatch()
      goto 999
c     
c     errclear
 210  call interrclear()
      return
c     
c     iserror
 220  call intiserror()
      goto 999
c     
c     predef
 230  call intpredef()
      goto 999
c     
c     newfun
c     
 240  call intnewfun
      goto 999
c     
c     clearfun 
 250  call intclearfun
      goto 999
c     
c     funptr
 251  call intfunptr
      goto 999
c
c     macrovar 
 300  call intmacrovar
      goto 999
c
c     setbpt 
 320  call intsetbpt
      goto 999
c
c     delbpt 
 350  call intdelbpt
      goto 999
c
c     dispbpt
 370  call intdispbpt
      goto 999
c
c     funcprot
 380  call intfuncprot()
      goto 999
c
c     whereis
 390  call intwhereis()
      return
c
c     where
 400  continue
      call where
      return
c
c     timer
 410  call inttimer
      return
c
c     notify
 420  continue
      return
c
c     havewindow
 450  call inthavewindow
      return
c
c     stacksize
 500  call intstacksize
      goto 999
c
c     mtlb_mode
 510  call intmtlbmode
      goto 999
 600  call scilink("link")
      goto 999
 610  call sciulink("unlink")
      goto 999
 620  call sciislink("c_link")
      goto 999
 630  call scidint("addinter")
      goto 999
 640  return
      goto 999
 650  return
      goto 999
 660  call intclear("clear")
      goto 999
 670  call intwhat("what")
      goto 999
 680  call intsciargs("sciargs")
      goto 999
 681  call intschdir("chdir")
      goto 999
 682  call intsgetdir("getcwd")
      goto 999
 683  call intsieee("ieee")
      goto 999
 684  call inttypnam("typnam")
      goto 999
 690  call intglobal()
      goto 999
 691  call intclearglobal("clearglob")
      goto 999
 692  call intisglobal("isglobal")
      goto 999
 693  call intgstacksize()
      goto 999
 694  call intgetdate()
      goto 999
 695  call intintppty()
      goto 999
 697  call lasterror('lasterror')
      goto 999
 698  call intversion('version')
      goto 999
 699  call loadhistory('loadhistory')
      goto 999
 700  call savehistory('savehistory')
      goto 999
 701  call gethistory('gethistory')
      goto 999
 702  call resethistory('resethistory')
      goto 999
 703  call openbrowser('openbrowser')
      goto 999    
 704  call macr2tree('macr2tree')
      goto 999
 705  call hidetoolbar('hidetoolbar')
      goto 999             
 706  call useascommand('use_as_command')
      goto 999             

 998  continue
c     fake calls : only to force the 
c     linker to load the following functions
c     in scilab executable 
      job=0
      call rcopy(1,ar,1,ar,1)
      call ccopy(1,'tutu',1,buf,1)
      call matz(ar,ai,1,1,1,name,job)
      call readmat(' ',1,1,ar)
      i=creadmat(' ',1,1,w)
      i=creadchain(' ',1,w)
      call matptr(' ',1,1,1)
      i=cmatptr(' ',1,1,1)
      call matc(' ',1,1,1,buf,job)
      call readchain(' ',1,buf)
      call matzs(ar,ai,1,1,1,' ',job)
      call rea2db(0,dx,1,dy,1)
      call freeptr(i)
      call csparsef(ar,0,1,1,0,0,icol,ar,ar)
      zabs=abs(33)
      zabs=abs(cos(zabs)/sin(exp(zabs*zabs+zabs)))
      zabs=sqrt(zabs)
 999  return
      end

      subroutine scilink(fname) 
C     ================================================================
C    link function  
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr
      integer m3,n3,lr3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      integer ilib,gettype,iv ,l1
      logical getwsmat,checkrhs,getsmat,getscalar,cremat
      character*(2) strf
      iadr(l)=l+l-1

      call ref2val
      strf='f'//char(0)
      if (.not.checkrhs(fname,1,3)) return
      topk=top
C     third argument if present is a char 
      if (rhs.ge.3) then
         if(.not.getsmat(fname,topk,top,m3,n3,1,1,lr3,nlr3))return
         if (nlr3.ne.1) then
            buf=fname //' : flag has a wrong size, 1 expected'
            call error(999)
            return
         endif
         call cvstr(nlr3,istk(lr3),strf,1)
         top=top-1
      endif
C     second argument 
      if (rhs.ge.2) then
         if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
         top=top-1
      endif
C     first argument 
      itype=gettype(top) 
      if ( itype.eq.1) then 
         if (.not.getscalar(fname,topk,top,l1)) return
         if (rhs.eq.1) then 
            buf = fname // ': must have two arguments when '
     $           // 'first arg is an integer'
            call error(999)
            return
         endif
         ilib=int(stk(l1))
         iflag = 1
         call iscilink(iv,iv,iv,
     $        istk(il2),istk(ild2),m2*n2,strf,ilib,iflag,rhs)
      else 
         if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
         iflag = 0
         if ( rhs.eq.1) then 
            call iscilink(istk(il1),istk(ild1),m1*n1,
     $           iv,iv,iv,strf,ilib,iflag,rhs)
         else
            call iscilink(istk(il1),istk(ild1),m1*n1,
     $           istk(il2),istk(ild2),m2*n2,strf,ilib,iflag,rhs)
         endif
         if ( ilib.lt.0 ) then 
            if (ilib.eq.-1) then
               call error(236)
            elseif (ilib.eq.-2) then
               call error(239)
            elseif (ilib.eq.-3) then
               call error(238)
            elseif (ilib.eq.-4) then
               call error(237)
            elseif (ilib.eq.-5) then
               call error(235)
            elseif (ilib.eq.-6) then
               call error(235)
            else
               buf= fname // ': Error'
               call error(999)
            endif
            return
         endif
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr) = ilib
         return
      endif
      return
      end



      subroutine scidint(fname) 
C     ================================================================
C     addinter 
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr,gettype
      logical getwsmat,checkrhs,getsmat,lib_cpp,getscalar
      character strf*25, c_cpp*10

      iadr(l)=l+l-1
c
      call ref2val
c
      if (.not.checkrhs(fname,3,4)) return
      topk=top

      lib_cpp=.false.
      strf=' '
      len=0
      if (rhs .eq. 4) then
        if(.not.getsmat(fname,topk,top,m4,n4,1,1,lr4,nlr4))return
        if (n4 .ne. 1) then
          buf=fname//' : option name has a wrong size, string expected'
          call error(999)
          return
        endif

        call cvstr(nlr4,istk(lr4),strf,1)
        if ((strf(1:3) .eq. 'c++') .or. 
     $      (strf(1:3) .eq. 'C++')) then
           lib_cpp=.true.
           c_cpp='none'
           len=4
           if ((nlr4 .gt. 3) .and. (strf(4:4) .eq. '=')) then
              if (nlr4 .eq. 4) then
                 buf=fname//' : none C++ compiler name !!'
                 call error(999)
                 return
              endif
              c_cpp=strf(5:nlr4) 
              len=nlr4-4
           endif
        else
           buf=fname//' : bad option name'
           call error(999)
           return
        endif
        top=top-1
      endif
      c_cpp(len+1:len+1)=char(0)

      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top=top-1
      if(.not.getsmat(fname,topk,top,m2,n2,1,1,lr2,nlr2))return
      if ( m2*n2.ne.1) then
         buf=fname //' : ename has a wrong size, 1x1 expected'
         call error(999)
         return
      endif
      if ( nlr2.gt.24) then 
         buf=fname //' : ename max size 24'
         call error(999)
         return
      endif
      call cvstr(nlr2,istk(lr2),strf,1)
      strf(nlr2+1:nlr2+1)=char(0)
      top=top-1
C     first argument 
C     jpc on accepte un entier 
      itop = gettype(top)
      if ( itop .eq. 1 ) then 
         if (.not.getscalar(fname,topk,top,il1)) return
         ilib=int(stk(il1))
         call addinter(ilib,iv,iv,iv,strf,
     $        istk(il3),istk(ild3),m3*n3,c_cpp,lib_cpp,ierr)
      else
         if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
         ilib=-1
         call addinter(ilib,istk(il1),istk(ild1),m1*n1,strf,
     $        istk(il3),istk(ild3),m3*n3,c_cpp,lib_cpp,ierr)
      endif
      if(ierr.ne.0) then
         if (ierr.eq.-1) then
            call error(236)
         elseif (ierr.eq.-2) then
            call error(239)
         elseif (ierr.eq.-3) then
            call error(238)
         elseif (ierr.eq.-4) then
            call error(237)
         elseif (ierr.eq.-5) then
            call error(231)
         elseif (ierr.eq.-6) then
            call error(234)
         elseif (ierr.eq.1) then
            call error(233)
         elseif (ierr.eq.2) then
            call error(232)
         else
            buf = fname // ': Error '
            call error(999)
         endif
         return
      endif
      call objvide(fname,top)
      return
      end

      subroutine sciulink(fname) 
C     ================================================================
C     unlink function  (unlik a whole shared lib ) 
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include '../stack.h'
      integer ilib,l1,topk
      logical getscalar,checkrhs
      character*(2) strf
      topk=top
      if (.not.checkrhs(fname,1,1)) return
      if (.not.getscalar(fname,topk,top,l1)) return
      ilib=int(stk(l1))
      call isciulink(ilib)
      call objvide(fname,top)
      return
      end

      subroutine sciislink(fname) 
C     ================================================================
C     [%t|%false,number]=c_link(name [,ilib]) 
C     checks if name is linked and optionaly linked form lib number ilib
C     ================================================================
      character*(*) fname
cc    implicit undefined (a-z)
      include '../stack.h'
      integer topk,iadr
c      integer m3,n3,lr3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      integer ilib,iv ,l1
      logical checkrhs,checklhs,crebmat,getscalar,getsmat,cremat
      iadr(l)=l+l-1
      if (.not.checkrhs(fname,1,2)) return
      if (.not.checklhs(fname,1,2)) return
      topk=top
      if (rhs.eq.2) then 
         if(.not.getscalar(fname,topk,top,lr))return
         ilib = int(stk(lr))
         top=top-1
      else
         ilib=-1
      endif
      if(.not.getsmat(fname,topk,top,m3,n3,1,1,lr3,nlr3))return
      call cvstr(nlr3,istk(lr3),buf,1)
      buf(nlr3+1:nlr3+1)=char(0)
      call iislink(buf,ilib)
      if (.not.crebmat(fname,top,1,1,lr)) return
      if (ilib.eq.-1)  then 
         istk(lr) = 0
      else
         istk(lr) = 1
      endif
      if ( lhs.eq.2) then 
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)= ilib
      endif
      return
      end



      subroutine intfort(fname)
c     =====================================
c     interface for the scilab fort command 
c     =====================================
      include '../stack.h'
cc      implicit undefined (a-z)
      character*(*) fname
      character*1   type
      logical checkrhs,checklhs,getsmat,checkval,cresmat2,bufstore
      logical flag,getscalar ,getmat,getrhsvar,cremat,lcres
      logical createvar
      integer gettype,sadr,iadr,top2,tops,topl,topk

      parameter (fortname=24)
      character  name*25
      common /cinter/ name
      common /ibfu/ ibuf(intersiz*6)
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      call ref2val
      if (rhs .eq. 0) then
         call error(39)
         return
      endif
      nbvars = 0
      flag=.false.
c     maximum number of variables ( see stack.h) 
c     
      namax=intersiz
      call iset(namax,0,ladr,1)
c     get a scilab string the fort function name 
      top2=top-rhs+1
      if(.not.getsmat(fname,top,top2,m1,n1,1,1,lrc,nc))return
      name=' '
      if ( nc.gt.fortname ) then 
         buf = fname // ' first argument must be of length < 24'
         call error(999)
         return
      endif
      call cvstr(nc,istk(lrc),name,1)
      name(nc+1:nc+1)=char(0)
C     Check the name in the <<fort>> table 
      call setinterf(name,irep)
      if ( irep.eq.1) then 
         buf = name
         call error(50)
         return
      endif
C     test the argument list : search for 'out' or 'sort'
c     ----------------------------------------------------
      do 81 i=1,rhs-1
         tops=top2+i
         itype = gettype(tops) 
         if ( itype.eq.10 ) then 
            if(.not.getsmat(fname,top,tops,m1,n1,1,1,lr1,nlr1)) return
            if(.not.checkval(fname,m1*n1,1)) return
            lbuf=1
            if(.not.bufstore(fname,lbuf,lbufi1,lbuff1,lr1,nlr1)) return
            if (buf(lbufi1:lbuff1).eq.'sort'.or.
     $           buf(lbufi1:lbuff1).eq.'out') goto 82 
         endif
 81   continue
c     'sort' or 'out' are not in the calling list we use the brief form 
C     we call the routine directly 
C     the routine must deal itself with the stack 
c     ----------------------------------------------------
      call interf1(name(1:nc))
      return
c     Now the long form 
c     -----------------
 82   continue
c     for the long form the call to interf is hardcoded 
c     with a limit of 30 arguments
      namax=min(intersiz,30)
c     ie : number of input arguments 
      ie=(tops-1-top2)/3
c     computing iss : 
c     number of output variables described after the 'out'  string 
c     -----------------
      jjj=tops+1
      iss=0
 86   if ( jjj.gt.top) goto 861
      itype = gettype(jjj)
      if (itype.eq.1) then 
         if(.not.getmat(fname,top,jjj,it1,m,n,lr1,lc1)) return
         if( m*n.eq.2 ) then 
            jjj = jjj+3 
         else
            jjj = jjj+1 
         endif
         iss = iss+1
      else
         err= jjj-top2+1
         call cvname(ids(1,pt+1),fname,0)
         call error(81) 
         return
      endif
      goto 86 
 861  continue
      if( iss.gt.0 .and. lhs.gt.iss) then
         call error(41)
         return
      endif
      if( err.gt.0) return
      ieis=ie+iss
      if(ie.ne.0) then 
c     input arguments 
c     storing information in ibuf for each input variable
c     ibuf(6*(i-1)+ j) 
c       j=1,2,3,4,5,6 ==>[call position,adress of data,type, nrow,ncol
c         number-in-the-caling-stack]
c     -------------------------------------------------------------
         do 87 i=1,ie
            ir1=top2+1+3*(i-1)
            if(.not.getsmat(fname,top,ir1+2,m1,n1,1,1,lr1,nlr1)) return
            if( m1*n1.ne.1.or.nlr1.ne.1) then 
               buf = fname // ': argument must be "c","d","i",or "r"'
               call error(999)
               return
            endif
c           stack-position 
            ibuf(6+6*(i-1)) = ir1-top2+1
            call cvstr(nlr1,istk(lr1),type,1)
            ibuf(3+6*(i-1)) = ichar(type)
            if(.not.getscalar(fname,top,ir1+1,lr1))return
            ibuf(1+6*(i-1)) = int(stk(lr1))
            if(.not.getrhsvar(2+3*(i-1),type,m1,n1,lr1)) return
            ibuf(2+6*(i-1)) = lr1 
            ibuf(4+6*(i-1)) = m1
            ibuf(5+6*(i-1)) = n1
            if (type.eq.'d' ) then 
               ladr(ibuf(1+6*(i-1))) = lr1 
            else if (type.eq.'r') then 
               ladr(ibuf(1+6*(i-1))) = sadr(lr1)
            else if (type.eq.'i') then 
               ladr(ibuf(1+6*(i-1))) = sadr(lr1)
            else if (type.eq.'c') then 
               call cs2st(lr1,lr2)
               ladr(ibuf(1+6*(i-1))) = lr2
            endif

 87      continue
      endif
c     Output variables 
c     -------------------------------------------------------------
c     icre will output variables which need to be created
      icre=0
      jjj=tops+1
      if(iss.eq.0) goto 95
      do 94 i=1,iss
         ilm=iadr(lstk(jjj))
         if(istk(ilm+1)*istk(ilm+2).eq.1) then 
c           output variable is described by position only 
            ipla=int(stk(sadr(ilm+4)))
c           get the data of the associated input var 
            do 91 j=1,ie
               jj=6*(j-1)
               if(ibuf(jj+1).eq.ipla) then 
                  call icopy(6,ibuf(jj+1),1,ibuf(6*(ie+i-1)+1),1)
                  jjj=jjj+1
                  goto 94 
               endif
 91         continue
            buf = fname // ': output variable described by position'
     $          // ' must be an input variable'
            call error(999)
         else
c           explicit dimension of the output var 
            if(.not.getsmat(fname,top,jjj+2,m1,n1,1,1,lr1,nlr1)) return
            if( m1*n1.ne.1.or.nlr1.ne.1) then 
               buf = fname // ': argument must be "c","d","i",or "r"'
               call error(999)
               return
            endif
            call cvstr(nlr1,istk(lr1),type,1)
            if(.not.getscalar(fname,top,jjj+1,lr1))return
            ipla = int(stk(lr1))
            if(.not.getmat(fname,top,jjj,it1,m1,n1,lr1,lc1)) return
            if (m1*n1.ne.2) then 
               buf = fname 
     $              // 'Output argument dimension must be [nr,nc]'
               call error(999)
               return
            endif
            m1 = int(stk(lr1))
            n1 = int(stk(lr1+1))
c           maybe this output variable was also an input variable
c           we must check that dimensions and type are compatible 
            iecor=0
            do 910 j=1,ie
               jj=6*(j-1)
               if(ibuf(jj+1).eq.ipla) then 
                  iecor= j
                  goto 911
               endif
 910        continue
 911        continue 
            if ( iecor.eq.0 ) then 
c           we must create a new entry 
c           for an output variable 
               icre=icre+1
               if (.not.createvar(rhs+icre,type,m1,n1,lr1)) return
               ipos=6*(ie+i-1)
               ibuf(ipos+1) = ipla  
               ibuf(ipos+2) = lr1
               ibuf(ipos+3) = ichar(type)
               ibuf(ipos+4) = m1
               ibuf(ipos+5) = n1 
               ibuf(ipos+6) = rhs + icre 
               narg = ibuf(6*(ie+i-1)+1)
               if(narg.gt.namax) then
                  call error(70)
                  return
               endif            
               if (type.eq.'d' ) then 
                  ladr(ipla) = lr1 
               else if (type.eq.'r') then 
                  ladr(ipla) = sadr(lr1)
               else if (type.eq.'i') then 
                  ladr(ipla) = sadr(lr1)
               else if (type.eq.'c') then 
                  ladr(ipla) = sadr((lr1/4)+1)
               endif
            else
c           we must check input-output consistency 
               ii=6*(iecor-1)
               if (m1*n1.gt.ibuf(ii+4)*ibuf(ii+5)
     $              .or.ichar(type).ne.ibuf(ii+3)) then
                  buf = fname // ': incompatibility between '
     $                 //'input and output variable'
                  call error(999)
                  return
               endif
               ibuf(ii+4)=m1
               ibuf(ii+5)=n1
               call icopy(6,ibuf(ii+1),1,ibuf(6*(ie+i-1)+1),1)
            endif
            jjj=jjj+3
         endif
 94   continue
 95   continue
      call interf(stk(ladr(1)),stk(ladr(2)),
     $     stk(ladr(3)),stk(ladr(4)),stk(ladr(5)),stk(ladr(6)),
     $     stk(ladr(7)),stk(ladr(8)),stk(ladr(9)),stk(ladr(10)),
     $     stk(ladr(11)),stk(ladr(12)),stk(ladr(13)),stk(ladr(14)),
     $     stk(ladr(15)),stk(ladr(16)),stk(ladr(17)),stk(ladr(18)),
     $     stk(ladr(19)),stk(ladr(20)),stk(ladr(21)),stk(ladr(22)),
     $     stk(ladr(23)),stk(ladr(24)),stk(ladr(25)),stk(ladr(26)),
     $     stk(ladr(27)),stk(ladr(28)),stk(ladr(29)),stk(ladr(30)))
      if(iss.le.0) then
         top=top2
         call objvide(fname,top)
      else
c        check if output variabe are in increasing order in the stack 
         lcres=.true.
         ibufprec=0
         do 105 i=1,lhs 
            ir1= 6*(ie+1-1)
            if ( ibuf(ir1+1).lt.ibufprec) then 
               lcres=.false.
               goto 106 
            else
               ibufprec = ibuf(ir1+1)
            endif
 105     continue
 106     continue
         if ( lcres) then 
            top=top2-1
         else
            topk=top2-1
            topl=top+icre
            top=topl
         endif
         ir1= 6*(ie+1-1)
         do 104 i=1,lhs
            top=top+1
            type=char(ibuf(ir1+3)) 
            m = ibuf(ir1+4)
            n = ibuf(ir1+5)
c     Warning : in the following code copy for 
c     overlapping object is necessary 
            if (type.eq.'d') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call unsfdcopy(m*n,stk(ibuf(ir1+2)),1,stk(lr1),1)
            else if (type.eq.'i') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call stacki2d(m*n,ibuf(ir1+2),lr1)
            else if (type.eq.'r') then 
               if (.not.cremat(fname,top,0,m,n,lr1,lc1)) return
               call stackr2d(m*n,ibuf(ir1+2),lr1)
            else if (type.eq.'c') then 
               if (.not.cresmat2(fname,top,m*n,lr1)) return
               l1=ibuf(ir1+2)
               call stackc2i(m*n,l1,lr1)
            endif
            ir1=ir1+6
 104     continue
         if (.not.lcres) then 
            do 107 i=1,lhs 
               call copyobj(fname,topl+i,topk+i)
 107        continue
            top=topk+lhs
         endif
      endif
      return
      end

      subroutine intsciargs(fname)
c     sciargs()
      character*(*) fname
      logical checkrhs,checklhs
      integer iadr,sadr,sciiargc
      include '../stack.h'
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs = max(0,rhs)
      lbuf = 1

      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return

      nargs = sciiargc()
      top=top+1
      il=iadr(lstk(top))
      l=il+5+nargs+1
      err=sadr(l)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      istk(il)=10
      istk(il+1)=nargs+1
      istk(il+2)=1
      istk(il+3)=0
      istk(il+4)=1

      do 20 k=0,nargs
         call scigetarg(k,buf)
         l1=len(buf)+1
 10      l1=l1-1
         if(buf(l1:l1).eq.' ') goto 10
         err=sadr(l+l1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call cvstr(l1,istk(l),buf,0)
         istk(il+5+k)=istk(il+4+k)+l1
         l=l+l1
 20   continue
      lstk(top+1)=sadr(l)
      return
      end


      subroutine intschdir(fname)
      character*(*) fname
      logical createvar,  putlhsvar
      logical checkrhs,checklhs,getrhsvar
      include '../stack.h'
      nbvars=0
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
      if(rhs.eq.0) then
         if(.not.createvar(1,'i',1,1,l2)) return
         istk(l2)=0
         lhsvar(1)=1
         if(.not.putlhsvar()) return
         return 
      endif
 
      if(.not.getrhsvar(1,'c',m1,n1,lr)) return 
      call cluni0(cstk(lr:lr+m1*n1), buf,lp)
      buf(lp+1:lp+1)=char(0)
      if(.not.createvar(2,'i',1,1,l2)) return
      call scichdir(buf(1:lp+1),istk(l2))
      if(istk(l2) .gt. 0) then 
         buf = fname // ': Internal Error' 
         call error(998)
         return
      endif
      lhsvar(1)=2
      if(.not.putlhsvar()) return
      return 
      end

      subroutine intsgetdir(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include '../stack.h'
      logical putlhsvar, createvarfromptr
      double precision l1
      nbvars = 0
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,0)) return
      if(.not.checklhs(fname,1,1)) return
      call scigetcwd(l1,m,err)
      if(err .gt. 0) then 
         buf = fname // ': Internal Error' 
         call error(998)
         return
      endif
      if(.not.createvarfromptr(1,'c',m,1,l1)) return
      lhsvar(1)=1
      if(.not.putlhsvar()) return
      end
      
      subroutine intsieee(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include '../stack.h'
      logical cremat, getscalar

      integer iadr
c
      iadr(l)=l+l-1
C
      nbvars = 0
      rhs = max(0,rhs)
      if(.not.checkrhs(fname,0,1)) return
      if(.not.checklhs(fname,1,1)) return
      if(rhs.le.0) then
         top=top+1
         if(.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)=ieee
      else
         if(.not.getscalar(fname,top,top,lr)) return
         i=stk(lr)
         if(i.lt.0.or.i.gt.2) then
            err=1
            call error(116)
            return
         endif
         ieee=i
         il=iadr(lstk(top))
         istk(il)=0
      endif
      end
      
      subroutine inttypnam(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include '../stack.h'
      logical cremat, getscalar
c     following common defines the initial database of type names
      integer maxtyp,nmmax,ptmax
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax)
      common /typnams/ tp,ptr,ln,namrec,ptmax
C
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
C
      nbvars = 0
      rhs = max(0,rhs)
      
      if(rhs.eq.0) then
         if(.not.checklhs(fname,1,2)) return
c     compute number of defined types
         nt=0
         do 01 it=1,maxtyp
            if(ln(it).ne.0) nt=nt+1
 01      continue
c     allocate results
         top=top+1
c     .  vector of type numbers
         il=iadr(lstk(top))
         l=sadr(il+4)
         iln=iadr(l+nt)
         lw1=sadr(iln+5+nt+ptmax)
         err=lw1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c
         istk(il)=1
         istk(il+1)=nt
         istk(il+2)=1
         istk(il+3)=0
         lstk(top+1)=l+nt
c
         top=top+1
c     .  vector of type names
         iln=iadr(lstk(top))
         istk(iln)=10
         istk(iln+1)=nt
         istk(iln+2)=1
         istk(iln+3)=0
         istk(iln+4)=1
         ilc=iln+5+nt

         i1=0

         do 02 it=1,maxtyp
            if(ln(it).ne.0) then
               stk(l+i1)=tp(it)
               istk(iln+5+i1)=istk(iln+4+i1)+ln(it)
               call icopy(ln(it),namrec(ptr(it)),1,istk(ilc),1)
               ilc=ilc+ln(it)
               i1=i1+1
            endif
 02      continue
         lstk(top+1)=sadr(ilc)
         if(lhs.eq.1) top=top-1
         return
      endif
c
      if(.not.checkrhs(fname,0,2)) return
      if(.not.checklhs(fname,0,1)) return
      if(.not.getscalar(fname,top,top,lr)) return
      itype=stk(lr)
      if(itype.le.0) then
         err=1
         call error(116)
         return
      endif
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.10) then
         err=1
         call error(55)
         return
      endif
      if(istk(il+1).ne.1.or.istk(il+2).ne.1) then 
         err=1
         call error(60)
         return
      endif
      n=istk(il+5)-1
      call cvstr(n,istk(il+6),buf,1)

      call addtypename(itype,buf(1:n),ierr)
      if(ierr.eq.1) then
         call error(224)
         return
      elseif(ierr.eq.2) then
         call error(225)
         return
      elseif(ierr.eq.3) then
         call error(224)
         return
      endif
      istk(il)=0
      return
      end

      subroutine intargn
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,checkrhs,cremat,getscalar
      integer nlhs,nrhs,p,choix
c 
      rhs=max(0,rhs)
      if(.not.checkrhs('argn',0,1)) return
      if(.not.checklhs('argn',1,2)) return
c
      if(rhs.eq.1)  then 
         if(.not.getscalar('argn',top,top,lr)) return

         choix=stk(lr)
         if(choix.lt.0.or.choix.gt.2) then
            err=1
            call error(36)
            return
         endif
         top=top-1
      else
         choix=0
      endif
      if(choix.ne.0.and.lhs.eq.2) then
         call error(78)
         return
      endif

c     computes then actual number of lhs ans rhs
      if(macr.eq.0) then
         nlhs=0
         nrhs=0
      else
         p=pt+1
 10      p=p-1
         if(rstk(p).ne.501.and.rstk(p).ne.502) goto 10
         nlhs=ids(2,p)
         nrhs=max(ids(1,p),0)
      endif

      if (choix.ne.2) then
c     number of lhs 
         top=top+1
         if(.not.cremat('argn',top,0,1,1,l,lc)) return
         stk(l)=nlhs
      else
         top=top+1
         if(.not.cremat('argn',top,0,1,1,l,lc)) return
         stk(l)=nrhs
      endif

      if(lhs.eq.1) return

c     number of rhs
      top=top+1
      if(.not.cremat('argn',top,0,1,1,l,lc)) return
      stk(l)=nrhs

      return
      end

      subroutine intclear(fname)
c     
c     Copyright INRIA
      include '../stack.h'
c     
      parameter (nz3=nsiz-3,nz2=nsiz-2)
      character*(*) fname
      integer top0,id(nsiz)
      logical getsmat,checkval,checklhs
      integer iadr, sadr
      integer a, blank,percen,sfin,helps(nsiz),scspal(nsiz)

      logical ok
      data a/10/,blank/40/,percen/56/
      data helps /353243448,673717273,nz2*673720360/
      data scspal/202509340,421796888,673715466,nz3*673720360/

      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
c
      if(.not.checklhs(fname,1,1)) return

      if(rhs.le.0) then
         if (macr.ne.0 .or. paus.ne.0) then
            if(rstk(pt-2).eq.909) then
c     .     clear within an exec'd macro
               goto 01
            endif
c     .     clear within a macro, an execstr, an exec or a pause
            k = lpt(1) - (13+nsiz)
            if(lin(k+7).ne.0.and.istk(lin(k+6)).eq.10) goto 02
c     .     clear within a macro, an exec or a pause
            bot = lin(k+5)
            goto 02
         endif

 01      continue
c     .  clear all variable

c     .  preserve %help and scicos_pal variables
         i1=bbot
         fin=-1
         call stackg(helps)
         if(err.gt.0) return
         if (fin.gt.0) i1=min(fin,i1)

         fin=-1
         call stackg(scspal)
         if(err.gt.0) return
         if (fin.gt.0) i1=min(fin,i1)
         bot = i1
         if(bot.eq.bbot) goto 02
         
         fin=0
         call stackg(helps)
         if(err.gt.0) return
         ih=fin
         fin=0
         call stackg(scspal)
         if(err.gt.0) return
         is=fin
         bot = bbot 
         if(is.eq.-1) then
            call stackp(scspal,0)
            if(err.gt.0) return
         endif
         if(ih.eq.-1) then
            call stackp(helps,0)
            if(err.gt.0) return
         endif
 02      top=top+1
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         return
      endif
     
      top0=top
      do 10 k=1,rhs
         if(.not.getsmat(fname,top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval(fname,m*n,1)) return
         if(nlr.eq.0) then
            top=top-1
            goto 10
         endif
c        . check for valid variable name
         do 05 i=0,nlr-1
            ic=abs(istk(lr+i))
            if((ic.gt.blank.and.(i.gt.0.and.ic.eq.percen)).or.
     $           (i.eq.0.and.ic.lt.a)) then
               err=rhs+1-k
               call error(248)
               return
            endif
 05      continue
         call namstr(id,istk(lr),nlr,0)
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         rhs = 0
         call stackp(id,0)
         if (err .gt. 0.or.err1.gt.0) return
         fin = 1
 10   continue
      top=top+1
      il = iadr(lstk(top))
      istk(il) = 0
      lstk(top+1) = lstk(top) + 1
      end

      subroutine intclearfun
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('clearfun',1,1)) return
      if(.not.checkrhs('clearfun',1,1)) return

      if(.not.getsmat('clearfun',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('clearfun',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call funtab(id,fptr,4)
      call objvide('clearfun',top)
      return
      end
      subroutine intclearglobal(fname)
c     Copyright INRIA
c     clear variables in the global area
      include '../stack.h'
c     
      character*(*) fname
      integer top0,id(nsiz),vk
      logical getsmat,checkval,checklhs,eqid
      integer iadr, sadr
      integer a, blank,percen
      data a/10/,blank/40/,percen/56/
c
      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
c
      if(.not.checklhs(fname,1,1)) return

      if(rhs.le.0) then
c     clear all variable but those created by the startup
         nprotect=6
         nv=gtop-(isiz+1)-nprotect
         if(nv.gt.0) then 
            call iset(nv*nsiz,0,idstk(1,isiz+2+nprotect),1)
            gtop=isiz+1+nprotect
         endif
         goto 100
      endif
     
      top0=top
      do 10 k=1,rhs
         if(.not.getsmat(fname,top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval(fname,m*n,1)) return
         top=top-1
         if(nlr.eq.0) then
c     .     blank string, ignored
            goto 10
         endif

c     .  check if variable name is valid 
         do 05 i=0,nlr-1
            ic=abs(istk(lr+i))
            if((ic.gt.blank.and.(i.gt.0.and.ic.eq.percen)).or.
     $           (i.eq.0.and.ic.lt.a)) then
               err=rhs+1-k
               call error(248)
               return
            endif
 05      continue
         call namstr(id,istk(lr),nlr,0)
c
c     .  look for this variable in the global area
         kg=isiz+1
 06      continue
         kg=kg+1
         if(kg.gt.gtop) then
c     .     no such variable exists, ignored
            goto 10
         endif
         if (.not.eqid(idstk(1,kg),id)) goto 06
c     . destroy this variable and pack storage if necessary
         if (kg.ne.gtop) then
c     .     pack
            ls = lstk(kg+1)
            ll = lstk(kg)
            vk=ls-ll
c     .     translate "values" up
            call unsfdcopy(lstk(gtop+1)-lstk(kg+1),stk(ls),1,stk(ll),1)
            do 09 i = kg, gtop-1
c     .        translate names up
               call putid(idstk(1,i),idstk(1,i+1))
c     .        translate property up
               infstk(i)=infstk(i+1)
c     .        update pointers 
               lstk(i) = lstk(i+1)-vk
c     .        update pointers in variables which refer this global var
               do 07 j=bot,isiz-1
                  if(infstk(j).eq.2) then
                     if(eqid(idstk(1,j),idstk(1,i))) then
c     .                 variable j refers this global var
                        ilj=iadr(lstk(j))
                        istk(ilj+1)=lstk(i)
                        istk(ilj+2)=i
                     endif
                  endif
 07            continue
 09         continue
         endif
         gtop=gtop-1
 10   continue
c
c     return a null matrix
 100  top=top+1
      il = iadr(lstk(top))
      istk(il) = 0
      lstk(top+1) = lstk(top) + 1
      end

      subroutine intcomp
c     Copyright INRIA
      include '../stack.h'
      integer cmode,topk,p
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr,sadr
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      call ref2val
      if(rstk(pt).eq.901) goto 61
c
      rhs=max(0,rhs)
      if(.not.checklhs('comp',1,1)) return
      if(.not.checkrhs('comp',1,2)) return
      topk=top
c
      if(rhs.eq.2) then
c     .  get compilation option
         if(.not.getscalar('debug',topk,top,l)) return
         cmode=stk(l)
         if(cmode.ne.0.and.cmode.ne.1.and.cmode.ne.2) then
            err=2
            call error(42)
            return
         endif
         top=top-1
         rhs=1
      else
         cmode=0
      endif

      il=iadr(lstk(top))

c

      if(istk(il).eq.13) then
c     .  function is already compiled
         call msgs(80,0)
         goto 72
      endif
      if(istk(il).ne.11) then
         err=1
         call error(44)
         return
      endif
c
      l=il+1
      mlhs=istk(l)
      l=l+nsiz*mlhs+1
      mrhs=istk(l)
      l = l + nsiz*mrhs + 2
      pt=pt+1
      ids(1,pt)=l
      pstk(pt)=fin
      fin=lstk(top)

      comp(1)=iadr(lstk(top+1))
      comp(2)=0
      comp(3)=cmode
      rstk(pt)=901
      icall=5
c     *call* parse  macro
      return
 61   l=ids(1,pt)
      pt=pt-1
      if(err1.ne.0) then
         comp(3)=0
         comp(2)=0
         comp(1)=0
         if (rstk(pt).eq.904) then
            top=toperr
            return
         endif
         il=iadr(lstk(top))
         istk(il)=0
         lhs=0
         err2=err1
         if(errct.eq.0) then
            err1=0
         else
            top=top-1
         endif
         return
      endif
      il=iadr(lstk(top))
      il1=iadr(lstk(top+1))
      n=comp(2)-il1
      comp(2)=0
      comp(3)=0
      call icopy(n,istk(il1),1,istk(l),1)
      istk(l-1)=n
      lstk(top+1)=sadr(l+n)
      istk(il)=13
 72   rhs=0
      call stackp(idstk(1,top),0)
      if(err.gt.0.or.err1.gt.0) goto 999
      top=top+1
      call objvide('comp',top)
c
      lhs=0
c     
 999  return
      end
      subroutine intdebug()
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)
      if(.not.checklhs('debug',1,1)) return
      if(.not.checkrhs('debug',0,1)) return
      if (rhs.eq.1) then
c     .  set debug mode
         if(.not.getscalar('debug',top,top,l)) return
         ddt = int(stk(l))
         write(buf(1:4),'(i4)') ddt
         call basout(io,wte,' debug '//buf(1:4))
         call objvide('debug',top)
      else
c     .  get debug mode
         top=top+1
         if(.not.cremat('debug',top,0,1,1,lr,lc)) return
         stk(lr)=ddt
      endif 
      return
      end
      subroutine intdelbpt
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,getscalar,checkval,eqid
      integer topk
c
      topk=top

      if(.not.checklhs('delbpt',1,1)) return
      if(.not.checkrhs('delbpt',1,2)) return

      if(rhs.eq.2) then
         if(.not.getscalar('delbpt',topk,top,l)) return
         lnb=int(stk(l))
         top=top-1
      endif
      if(.not.getsmat('delbpt',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('delbpt',m*n,1) ) return
      call namstr(id,istk(l),n1,0)

      if(nmacs.eq.0) goto 360
c
      do 353 kmac=1,nmacs
         if(eqid(macnms(1,kmac),id)) goto 355
 353  continue
      goto 360
      
 355  continue
      if(rhs.eq.1) then
c     on supprime tous les points d'arret de la macro
         if(kmac.lt.nmacs) then
            l0=lgptrs(kmac+1)
            call icopy(lgptrs(nmacs+1)-l0 ,bptlg(l0),1,
     $           bptlg(lgptrs(kmac)),1)
            do 356 kk=kmac,nmacs-1
               call icopy(nsiz,macnms(1,kk+1),1,macnms(1,kk),1)
c Francois VOGEL, May 2004
c the following statement was wrong - replaced with the next line
c               lgptrs(kk)=lgptrs(kk+1)
               lgptrs(kk+1)=lgptrs(kk)+lgptrs(kk+2)-lgptrs(kk+1)
 356        continue
c            lgptrs(nmacs)=lgptrs(nmacs+1)
            lgptrs(nmacs+1)=0
c FV, May 2004
c else part (just these two lines) added to cure bugzilla #718
         else
            lgptrs(nmacs+1)=0
         endif
         nmacs=nmacs-1
      else
c     on supprime le point d'arret specifie
         kk1=lgptrs(kmac)-1
         do 357 kk=lgptrs(kmac),lgptrs(kmac+1)-1
            if(bptlg(kk).ne.lnb) then
               kk1=kk1+1
               bptlg(kk1)=bptlg(kk)
            endif
 357     continue
         if(kk.eq.kk1) goto 360

         kk2=kk-kk1-1
         if(kmac.lt.nmacs) then
            l0=lgptrs(kmac+1)
            do 358 kk=kmac+1,nmacs
c FV, May 2004
c changed to remove at once multiple breakpoints on a single line
c               call icopy(lgptrs(kk+1)-l0,bptlg(l0),1,bptlg(l0-1),1)
               call icopy(lgptrs(kk+1)-l0,bptlg(l0),1,bptlg(l0-kk2),1)
               l0=lgptrs(kk+1)
c               lgptrs(kk+1)=lgptrs(kk+1)-1
               lgptrs(kk)=lgptrs(kk)-kk2
 358        continue
         endif
c         lgptrs(kmac+1)=lgptrs(kmac+1)-1
         lgptrs(nmacs+1)=lgptrs(nmacs+1)-kk2
         lgptrs(nmacs+2)=0
         if(lgptrs(kmac+1).eq.lgptrs(kmac)) then
            if(kmac.lt.nmacs) then
               do 359 kk=kmac,nmacs-1
                  call icopy(nsiz,macnms(1,kk+1),1,macnms(1,kk),1)
                  lgptrs(kk)=lgptrs(kk+1)
 359           continue
            endif
            lgptrs(nmacs)=lgptrs(nmacs+1)
            lgptrs(nmacs+1)=0
            nmacs=nmacs-1
         endif
      endif

 360  continue
      call objvide('delbpt',top)
      return
      end

      subroutine intdispbpt
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs
c
      rhs=max(rhs,0)
      if(.not.checklhs('dispbpt',1,1)) return
      if(.not.checkrhs('dispbpt',0,0)) return

      if(nmacs.gt.0) then
         do 375 kk=1,nmacs
            call cvname(macnms(1,kk),buf(1:nlgh),1)
            call msgs(27,0)
            do 373 kl=lgptrs(kk),lgptrs(kk+1)-1
               write(buf(1:10),'(5x,i5)') bptlg(kl)
               call basout(io,wte,buf(1:10))
 373        continue
 375     continue
      endif
      top=top+1
      call objvide('dispbpt',top)
      return
      end
      subroutine interrcatch
c     Copyright INRIA
      include '../stack.h'
      integer topk,p
      logical checkrhs,checklhs,getscalar,cremat,getsmat,checkval
      integer iadr
c
      data local/21/
c    
      iadr(l)=l+l-1
c
      topk=top
      rhs=max(0,rhs)
      if(.not.checklhs('errcatch',1,1)) return
      if(.not.checkrhs('errcatch',0,3)) return

      if(rhs.eq.0) then
         errct=0
         errpt=0
         top=top+1
         call objvide('errcatch',top)
         return
      endif
      num=0
      imode=0
      imess=0
      do 201 i=1,rhs
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))

         if(istk(il).eq.1) then
            if(.not.getscalar('errcatch',topk,top,l)) return
            num=nint(stk(l))
         else if(istk(il).eq.10) then
            if(.not.getsmat('errcatch',topk,top,m,n,1,1,il1,n1)) return
            if(.not.checkval('errcatch',m*n,1) ) return
            l=abs(istk(il1))
            if(l.eq.12) imode=1
            if(l.eq.25) imode=2
            if(l.eq.23) imess=1
            if(l.eq.28) imode=3
         else
            err=rhs-1+i
            call error(44)
            return
         endif
         top=top-1
 201  continue
      errct=(8*imess+imode)*100000+abs(num)
      if(num.lt.0) errct=-errct
      p=pt+1
 202  p=p-1
      if(p.eq.0) goto 203
      if(int(rstk(p)/100).ne.5) goto 202
 203  errpt=pt
      top=top+1
      call objvide('errcatch',top)
      return
      end

      subroutine interrclear
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs
c
      rhs=max(0,rhs)
      if(.not.checklhs('errclear',1,1)) return
      if(.not.checkrhs('errclear',0,1)) return

      err2=0
      if(rhs.eq.1) top=top-1
      top=top+1
      call objvide('errclear',top)
      return
      end

      subroutine interror
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,checkrhs,getscalar,getsmat
      integer topk,errn
      integer iadr
c 
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
c
      if(.not.checkrhs('error',1,2)) return
      if(.not.checklhs('error',1,1)) return

      topk=top

      il=iadr(lstk(top+1-rhs))
      if(istk(il).eq.1) then
c     .  error(errn [,numero_d'argument])
         if(rhs.eq.2) then
c     .     get arg number if given
            if(.not.getscalar('error',topk,top,l)) return
            nb=int(stk(l))
            top=top-1
         else
            nb=0
         endif
c     .  get error number 
         if(.not.getscalar('error',topk,top,l)) return
         errn=int(stk(l))
         err=nb
      else
c     .  error(str [,errn])
         if(rhs.eq.2) then
c     .  get error number  if given
            if(.not.getscalar('error',topk,top,l)) return
            errn=int(stk(l))
            if(errn.ge.100000) then
               err=2
               call error(116)
               return
            endif
            top=top-1
         else
            errn=9999
         endif
         if(.not.getsmat('error',topk,top,m,n,1,1,il,n1)) return
         buf=' '
         call cvstr(n1,istk(il),buf,1)
      endif
      call error(errn)
      return
      end

      subroutine intexists
c     Copyright INRIA
      include '../stack.h'
      integer topk
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      logical flag
      integer local
      integer iadr,sadr
c
      data local/21/
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      topk=top

      if(.not.checklhs('exists',1,1)) return
      if(.not.checkrhs('exists',1,2)) return

      flag=.false.
c
      if(rhs.eq.2) then
         if(.not.getsmat('exists',topk,top,m,n,1,1,il,n1)) return
         if(.not.checkval('exists',m*n,1) ) return
         if(istk(il).eq.local) flag=.true.
         top=top-1
      endif


      if(.not.getsmat('exists',topk,top,m1,n1,1,1,il,n)) return
      if(.not.checkval('exists',m1*n1,1) ) return
      call namstr(id,istk(il),n,0)

c     look for  variables in the stack
      if(flag) then
c      just in local environnement
         fin=-3
         call stackg(id)
         ilw=iadr(lstk(top))
         top=top-1
         if(istk(ilw).lt.0) then
            fin=1
         else
            fin=0
         endif
      else
c     in all the stack
         fin=-1
         call stackg(id)
         if (fin.le.0) then
c     look for libraries functions
            fin=-3
            kfun=fun
            call funs(id)
            fun=kfun
         endif
      endif
c
      if(.not.cremat('exists',top,0,1,1,l,lc)) return
      if (fin.gt.0) then
         stk(l)=1.0d0
      else
         stk(l)=0.0d0
      endif
      fin=1
      return
      end

      subroutine intformat
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar,getsmat,cremat
      integer topk,fe,fv
      integer iadr
c
      data fe/14/,fv/31/
c
      iadr(l)=l+l-1
c

      rhs=max(0,rhs)
      topk=top
c
      if(.not.checklhs('format',1,1)) return
      if(.not.checkrhs('format',0,2)) return
 
      if(rhs.eq.0) then
c     .  return current format definition
         top=top+1
         if(.not.cremat('format',top,0,1,2,l,lc)) return
         stk(l)=lct(6)
         stk(l+1)=lct(7)
         return
      endif

 160  il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))

      if(istk(il).eq.1) then
c     .  nbre de digits
         if(.not.getscalar('format',topk,top,l)) return
         nd=max(2,int(stk(l)))
         if(lct(6).eq.0) nd=max(nd,8)
         lct(7)=nd
      elseif(istk(il).eq.10) then
c     .  type du format
         if(.not.getsmat('format',topk,top,m,n,1,1,l,n1)) return
         if(abs(istk(l)).eq.fe) then
            lct(6)=0
            lct(7)=max(8,lct(7))
         else
            if(abs(istk(l)).ne.fv) then
               err=1
               call error(36)
               return
            endif
            lct(6)=1
         endif
      else
         err=1
         call error(55)
         return
      endif

      if(rhs.eq.2) then
         top=top-1
         rhs=rhs-1
         goto 160
      endif
      call objvide('format',top)

      return
      end

      subroutine intfuncprot
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,cremat,getscalar
      integer topk
      common /mprot/ macprt
c
      rhs=max(rhs,0)
      if(.not.checklhs('funcprot',1,1)) return
      if(.not.checkrhs('funcprot',0,1)) return

      if (rhs .eq. 0) then
c     .  get value
         top=top+1
         if(.not.cremat('funcprot',top,0,1,1,l,lc)) return
         stk(l)=macprt
         return
      else
c     .  set value
         if(.not.getscalar('funcprot',top,top,l)) return
         m=int(stk(l))
         if(m.lt.0.or.m.gt.2) then
            err=1
            call error(116)
            return
         endif
         macprt=m
         call objvide('funcprot',top)
      endif
      return
      end
      subroutine intfunptr
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('funptr',1,1)) return
      if(.not.checkrhs('funptr',1,1)) return

      if(.not.getsmat('funptr',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('funptr',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call funtab(id,fptr,1)
      if(.not.cremat('funptr',top,0,1,1,l,lc)) return
      stk(l)=dble(fptr)
      return
      end

      subroutine intglobal()
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,getsmat,checkval,cremat
      integer top0,topk,typ,vol
      integer id(nsiz),blank,percen,ch
      integer iadr,sadr
      data blank/40/,percen/56/
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(.not.checklhs('global',1,1)) return
      topk=top
      do 10 i=1,rhs
c     .  get the variable name
         if(.not.getsmat('global',topk,top,m,n,1,1,lr,nn)) return
         if(.not.checkval('global',m*n,1)) return
c     .  check if it is a correct variable name
         ch=abs(istk(lr))
         if(.not.(ch.eq.percen.or.(ch.gt.9.and.ch.lt.blank))) then
            err=rhs+1-i
            call error(116)
            return
         endif
         if(nn.gt.1) then
            do 01 k=1,nn-1
               ch=abs(istk(lr+k))
               if(ch.ge.blank) then
                  err=rhs+1-i
                  call error(116)
                  return
               endif
 01         continue
         endif
         call namstr(id,istk(lr),nn,0)
c     .  check if the variable is already defined in global area
         fin=-5
         call stackg(id)
         if(err.gt.0) return
         if (gtop+2.gt.isizt) then
            call error(262)
            return
         endif
         if(fin.eq.0) then
c     .     the variable is not defined yet in global area 
c     .     check if it is defined in local area
            fin=-6
            call stackg(id)
            if(err.gt.0) return
            if(fin.eq.0) then
c     .        no, create an empty variable in the global area
               vol=5
               if (lstk(gtop+1)+vol.gt.lstk(gbot)) then
c     .           not enought memory, realloc
                  mem=lstk(gbot)-lstk(isiz+2)+max(vol+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
               endif
               gtop=gtop+1
               call putid(idstk(1,gtop),id)
               infstk(gtop)=0
               il=iadr(lstk(gtop))
               istk(il)=1
               istk(il+1)=0
               istk(il+2)=0
               istk(il+3)=0
               lstk(gtop+1)=sadr(il+4)
            else
c     .        yes, move it to the global area
               vol=lstk(fin+1)-lstk(fin)
               if (lstk(gtop+1)+vol+10.ge.lstk(gbot)) then
c     .           max(vol+1,100000) to avoid too many reallocation
                  mem=lstk(gbot)-lstk(isiz+2)+max(vol+1,10000)
                  call reallocglobal(mem)
                  if(err.gt.0) return
               endif

               gtop=gtop+1

               call putid(idstk(1,gtop),id)
               infstk(gtop)=0
               lstk(gtop+1)=lstk(gtop)+vol
               call unsfdcopy(vol,stk(lstk(fin)),1,stk(lstk(gtop)),1)
            endif
            kg=gtop 
         else
c     .    the variable is  already defined  in the global area
            kg=fin
         endif
c     .  create a link to the created or moved variable with the same
c     .  name in the local area
         top=top-1
         call createref(iadr(lstk(kg)),kg,lstk(kg+1)-lstk(kg))
         call stackp(id,0)
         if(err.gt.0) return
         infstk(fin)=2
 10   continue
c     return a null matrix
      top=top+1
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end

      subroutine intgstacksize
c     Copyright INRIA
      include '../stack.h'
      integer offset
      logical checkrhs,checklhs,cremat,getscalar,eqid
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('gstacksize',1,1)) return
      if(.not.checkrhs('gstacksize',0,1)) return

      if (rhs.eq.0) then
         top=top+1
         if(.not.cremat('gstacksize',top,0,1,2,l,lc)) return
         stk(l)=lstk(gbot)-lstk(isiz+2)+1
         if(gtop.ge.isiz+2) then
            stk(l+1)=lstk(gtop+1)-lstk(isiz+2)+1
         else 
            stk(l+1)=0.0d0
         endif
         return
      endif

      if(.not.getscalar('gstacksize',top,top,l)) return
      mem=stk(l)
      memold=lstk(gbot)-lstk(isiz+2)
      if (mem.eq.memold) goto 10
      l=lstk(gtop+1)-lstk(isiz+2)
      if (mem.lt.l) then
         buf='Required memory too small for defined data'
         call error(1503)
         return
      endif
      call scigmem(mem+1,offset)
      if(offset.eq.0) then
         call error(112)
         return
      endif
      offset=offset+1
      call unsfdcopy(l,stk(lstk(isiz+2)),1,stk(offset),1)
      kd=offset-lstk(isiz+2)
      do 05 k=isiz+2,gtop+1
         lstk(k)=lstk(k)+kd
 05   continue 
      call freegmem()
      lstk(gbot)=lstk(isiz+2)+mem
c     following lines are necessary because of use of il=iadr(istk(il+1)) 
c     to get variable pointed by an indirect variable.
c     it should be no more useful with il=iadr(lstk(istk(il+2)))
      do 09 i = isiz+2, gtop
c     update pointers in variables which refer this global var
      do 07 j=bot,isiz-1
         if(infstk(j).eq.2) then
            if(eqid(idstk(1,j),idstk(1,i))) then
c     .        variable j refers this global var
               ilj=iadr(lstk(j))
               istk(ilj+1)=lstk(i)
               istk(ilj+2)=i
            endif
         endif
 07   continue
 09   continue
 10   call objvide('gstacksize',top)
      return
      end
      subroutine inthavewindow
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,crebmat
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('havewindow',1,1)) return
      if(.not.checkrhs('havewindow',0,0)) return

      call xscion(iflag)

      top=top+1
      if(.not.crebmat('havewindow',top,1,1,l)) return
      istk(l)=iflag
      return
      end

      subroutine intiserror
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar,cremat
c
      rhs=max(0,rhs)

      if(.not.checklhs('iserror',1,1)) return
      if(.not.checkrhs('iserror',0,1)) return

      if(rhs.eq.1) then
         if(.not.getscalar('iserror',top,top,l)) return
         num=nint(stk(l))
      else
         num=0
         top=top+1
      endif
      if(.not.cremat('iserror',top,0,1,1,l,lc)) return
      if(num.ge.1) then
         if(err2.eq.num) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      else
         if(err2.ne.0) then
            stk(l)=1.0d+0
         else
            stk(l)=0.0d+0
         endif
      endif
      return
      end

      subroutine intisglobal(fname)
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,crebmat
      character*(*) fname
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      if(.not.checklhs('isglobal',1,1)) return
      if(.not.checkrhs('isglobal',1,1)) return

      if(.not.crebmat(fname,top,1,1,ilr)) return
      if(infstk(top).eq.2) then
         istk(ilr)=1
      else
         istk(ilr)=0
      endif
      return
      end


   
      subroutine intlines()
c     Copyright INRIA
      include '../stack.h'
      logical checklhs,checkrhs,getscalar,cremat
      integer topk
      integer iadr
c 
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
      topk=top
c
      if(.not.checkrhs('lines',0,2)) return
      if(.not.checklhs('lines',1,1)) return
c

      if(rhs.eq.0) then
c     .  get lines properties
         top=top+1
         if(.not.cremat('lines',top,0,1,2,l,lc)) return
         stk(l)=lct(5)
         stk(l+1)=lct(2)
      else
c     .  set lines properties
         if(rhs.eq.2) then
c     .     number of columns
            if(.not.getscalar('lines',topk,top,l)) return
            lct(5)=max(10,int(stk(l)))
            top=top-1
         endif
c     .  number of lines
         if(.not.getscalar('lines',topk,top,l)) return
         lct(2) = max(0,int(stk(l)))

c     .  return a null matrix
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
      endif
      return
      end

      subroutine intmacrovar
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,ref
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(.not.checklhs('macrovar',1,1)) return
      if(.not.checkrhs('macrovar',1,1)) return


      ilfun=iadr(lstk(top))
      if (istk(ilfun).lt.0) then
         illist=ilfun
         ilfun=iadr(istk(ilfun+1))
         ref=.true.
      else
         illist=iadr(lstk(top+1))
         ref=.false.
      endif

      if(istk(ilfun).ne.13) then
         err=1
         call error(44)
         return
      endif


      call tradsl(ilfun,idstk(1,top),illist,nlist)
      if(err.gt.0) return
c     store result
      if(.not.ref) then
         il=iadr(lstk(top))
         call icopy(nlist,istk(illist),1,istk(il),1)
         lstk(top+1)=sadr(il+nlist)
      else
         lstk(top+1)=sadr(illist+nlist)
      endif
      return
      end

      subroutine intmode
c     Copyright INRIA
      include '../stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr,sadr
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
      if(.not.checklhs('mode',1,1)) return
      if(.not.checkrhs('mode',0,1)) return

      if (rhs.eq.1) then
c     .  set  line mode value
         if(.not.getscalar('mode',top,top,l)) return
         lct(4)=int(stk(l))
         if(lct(4).eq.7.or.lct(4).eq.4) call msgs(26,0)
         call objvide('mode',top)
      else
c     .  get line  mode value
         top=top+1
         if(.not.cremat('mode',top,0,1,1,lr,lc)) return
         stk(lr)=lct(4)
      endif
      return
      end
      subroutine intmtlbmode
c     Copyright INRIA
      include '../stack.h'
      integer offset
      logical checkrhs,checklhs,crebmat,getbmat,checkval
      common /mtlbc/ mmode
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('mtlb_mode',1,1)) return
      if(.not.checkrhs('mtlb_mode',0,1)) return

      if(rhs.eq.0) then
         top=top+1
         if(.not.crebmat('mtlb_mode',top,1,1,l)) return
         istk(l)=mmode
      else
         if(.not.getbmat('mtlb_mode',top,top,m,n,l)) return
         if(.not.checkval('mtlb_mode',m*n,1) ) return
         mmode=istk(l)
         call objvide('mtlb_mode',top)
      endif
      return
      end
      subroutine intnewfun 
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getscalar,getsmat,checkval
      integer topk,fptr
c
      topk=top

      if(.not.checklhs('newfun',1,1)) return
      if(.not.checkrhs('newfun',2,2)) return

      if(.not.getscalar('newfun',topk,top,l)) return
      fptr=int(stk(l))
c     
      top=top-1
      if(.not.getsmat('newfun',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('newfun',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
c     
      call funtab(id,fptr,3)
      if(err.gt.0) return
      call objvide('newfun',top)
      return
      end

      subroutine intnotify
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('notify',1,1)) return
      if(.not.checkrhs('notify',1,1)) return

      if(.not.getscalar('notify',top,top,l)) return
      ntfy=stk(l)
      call objvide('notify',top)
      return
      end
      subroutine intpredef
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,getscalar,cremat,getsmat,checkval
      integer bbots
      integer iadr
c
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)

      if(.not.checklhs('predef',1,1)) return
      if(.not.checkrhs('predef',0,1)) return

      bbots=bbot
      if(rhs.le.0) then
c     .  get number of protected variables
         top=top+1
         if(.not.cremat('predef',top,0,1,1,l,lc)) return
         stk(l)=isiz-bbots
      else
         ityp=abs(istk(iadr(lstk(top))))
         if(ityp.eq.1) then
c     .     mark n oldest current variables as protected
            if(.not.getscalar('predef',top,top,l)) return
            is=isiz
            bbot=max(bot,min(isiz-nint(stk(l)),bot0))
         elseif(ityp.eq.10) then
            if(.not.getsmat('predef',top,top,m,n,1,1,l,n1)) return
            if(.not.checkval('predef',m*n,1) ) return
            if(abs(istk(l)).eq.10) then
c     .        mark all current variables as protected
               bbot=bot
            elseif(abs(istk(l)).eq.12) then
c     .        unmark all 
               bbot=bot0
            else
               buf='Unknown option'
               call error(901)
            endif
         endif
         if(.not.cremat('predef',top,0,1,2,l,lc)) return
         stk(l)=isiz-bbots
         stk(l+1)=isiz-bbot
      endif
      return
      end

      subroutine intresume
c     Copyright INRIA
      include '../stack.h'
      parameter (nz1=nsiz-1,nz2=nsiz-2)
      logical eqid
      integer pt0,count,r
      integer semi,comma,eol
      integer resume(nsiz),sel(nsiz)
c
      data resume/505155099,673713686,nz2*673720360/
      data sel/236260892,673717516,nz2*673720360/
      data eol/99/,semi/43/
      data comma/52/
c
      call ref2val
      if(rhs.ne.lhs) then
         call error(41)
         return
      endif
c
      count=0
      pt0=pt+1
 151  pt0=pt0-1
      if(pt0.le.0) return
         
      if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &     (rstk(pt0).eq.805.and.eqid(ids(1,pt0),sel)).or.
     &     (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1
      if(int(rstk(pt0)/100).ne.5) goto 151
c
 152  if(rstk(pt0).eq.501) then
c     resume in a compiled macro
         lc=pstk(pt)
         ids(1,pt0+1)=lc
         pstk(pt0+2)=count
      elseif(rstk(pt0).eq.502) then
c     resume in an uncompiled macro or an exec or an execstr
         if(rstk(pt0-1).eq.903.or.rstk(pt0-1).eq.706) then
c     .  in an execstr, check execstr calling context
            ip0=pt0+1
            pt0=pt0-2
 153        pt0=pt0-1
            if(pt0.le.0) return
            if(rstk(pt0).eq.802.or.rstk(pt0).eq.612 .or.
     &           (rstk(pt0).eq.805.and.eqid(ids(1,pt0),sel)).or.
     &           (rstk(pt0).eq.616.and.pstk(pt0).eq.10)) count=count+1
            if(rstk(pt0).lt.501.or.rstk(pt0).gt.503) goto 153
            if(rstk(pt0).eq.503.and.rio.eq.rte.and.pause.ne.0) then
c     .       resume appele dans par un execstr sous pause
               k=lpt(1)-(13+nsiz)
               lpt(1)=lin(k+1)
               macr=macr-1

               k=lpt(1)-(13+nsiz)
               bot=lin(k+5)
               mrhs=rhs
               rhs=0
               paus=paus-1
               do 154 i=1,mrhs
                  call stackp(ids(1,ip0),0)
                  ip0=ip0-1
 154           continue
               paus=paus+1
               lin(k+5)=bot
               top=top-count
               pt=pt0
               goto 999
            elseif(paus.ne.0.and.rstk(pt0).eq.201) then
c     .        ???
               r=rstk(pt0-4)
               if (r.eq.701.or.r.eq.604) goto 156
            endif
c    .      resume in an execstr, simulate a resume in the calling macro
c    .      see macro.f code for details 
            k = lpt(1) - (13+nsiz)
            lpt(1)=lin(k+1)
            macr=macr-1

c     .     get location of lhs var names
            lvar=pt-3
            rstk(pt0)=502
            pstk(pt0+1)=lvar
            pstk(pt0+2)=count+1
         else
c     .     resume in an uncompiled macro
            if(rstk(pt-1).ne.201
     &           .or.rstk(pt-2).ne.101
     &           .or.rstk(pt-3).ne.703
     &           .or.(sym.ne.semi.and.sym.ne.comma.and.sym.ne.eol)) 
     &           goto 156
            pt=pt-3
            pstk(pt0+1)=pt
            pstk(pt0+2)=count
         endif
      elseif(rio.eq.rte) then
c     resume in a pause
         if(rstk(pt-1).ne.201
     &        .or.rstk(pt-2).ne.101
     &        .or.rstk(pt-3).ne.703
     &        .or.(sym.ne.semi.and.sym.ne.comma.and.sym.ne.eol)) 
     &         goto 156
         pt=pt-3
         k=lpt(1)-(13+nsiz)
         bot=lin(k+5)
         if(macr.ne.0.or.paus.ne.0) then
            lpts=lpt(1)
            lpt(1)=lin(k+1)
         endif
         mrhs=rhs
         rhs=0
         paus=paus-1
         do 155 i=1,mrhs
            call stackp(ids(1,pt),0)
            pt=pt-1
 155     continue
         paus=paus+1
         if(macr.ne.0.or.paus.ne.0) then
            lpt(1)=lpts
         endif
         lin(k+5)=bot
         top=top-count
      else
         goto 156
      endif
      pt=pt0
      goto 999
 156  continue
      call putid(ids(1,pt),resume)
      call error(72)
 999  return
      end


      subroutine intsetbpt
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,getsmat,getscalar,checkval,eqid
      integer topk
c
      topk=top

      if(.not.checklhs('setbpt',1,1)) return
      if(.not.checkrhs('setbpt',1,2)) return

      if(rhs.eq.2) then
         if(.not.getscalar('setbpt',topk,top,l)) return
         lnb=int(stk(l))
         top=top-1
      else
         lnb=1
      endif
      if(.not.getsmat('setbpt',topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval('setbpt',m*n,1) ) return
      call namstr(id,istk(l),n1,0)
cx      if(fin.eq.24) goto 350
c
c      if(rhs.eq.1) lnb=1
      if(nmacs.gt.0) then
         do 323 kmac=1,nmacs
c Francois VOGEL, May 2004
c            if(eqid(macnms(1,kmac),id)) goto 325
            if(eqid(macnms(1,kmac),id)) goto 324
 323     continue
      endif
C Serge Steer May 2004
      if (nmacs.ge.maxdb) then
         buf='Too many functions contain breakpoints'
         call error(9999)
         return
      endif

      nmacs=nmacs+1
      call putid(macnms(1,nmacs),id)
C Serge Steer May 2004
      if (lgptrs(nmacs)+1.gt.maxbpt) then
         buf='Too many defined  breakpoints'
         call error(9998)
         return
      endif
  
      lgptrs(nmacs+1)=lgptrs(nmacs)+1
      bptlg(lgptrs(nmacs))=lnb
      goto 330
c FV, May 2004
c do statement added to avoid definition of duplicate bpts
 324  do 3241 kk=lgptrs(kmac),lgptrs(kmac+1)-1
          if (bptlg(kk).eq.lnb) goto 330
 3241 continue
 325  if(kmac.eq.nmacs) then
         lgptrs(nmacs+1)=lgptrs(nmacs+1)+1
C Serge Steer May 2004
         if (lgptrs(nmacs+1)-1.gt.maxbpt) then
            buf='Too many defined  breakpoints'
            call error(9998)
            return
         endif
         bptlg(lgptrs(nmacs+1)-1)=lnb
      else
C Serge Steer May 2004
        if (lgptrs(nmacs+1)-1.gt.maxbpt) then
            buf='Too many defined  breakpoints'
            call error(9998)
            return
         endif
  
         do 326 kk=nmacs,kmac,-1
            l0=lgptrs(kk)
            call icopy(lgptrs(kk+1)-l0,bptlg(l0),-1,bptlg(l0+1),-1)
            lgptrs(kk+1)=lgptrs(kk+1)+1
 326     continue
c FV, May 2004
c this statement was wrong - replaced with next line
c         bptlg(lgptrs(kmac+1)-1)=lnb
         bptlg(lgptrs(kmac))=lnb
      endif
 330  continue
      call objvide('setbpt',top)
      return
      end

      subroutine intstacksize
c     Copyright INRIA
      include '../stack.h'
      integer offset,p
      logical checkrhs,checklhs,cremat,getscalar
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('stacksize',1,1)) return
      if(.not.checkrhs('stacksize',0,1)) return

      if (rhs.eq.0) then
         top=top+1
         if(.not.cremat('stacksize',top,0,1,2,l,lc)) return
         stk(l)=lstk(isiz)-lstk(1)
         stk(l+1)=lstk(isiz)-lstk(bot)+1
         return
      endif

      if(.not.getscalar('stacksize',top,top,l)) return
      top=top-1
c
      if (stk(l).lt.1000.or.stk(l).gt.2.0d0**31) then
         buf='Out of bounds value for stacksize argument'
         call error(1504)
         return
      endif
      mem=stk(l)
      memold=lstk(isiz)-lstk(1)
      if (mem.eq.memold) goto 50
      lbot=lstk(isiz)-lstk(bot)
      ltop=0
      if (top.gt.0) ltop=lstk(top+1)-lstk(1)
      if (mem.lt.lbot+ltop) then
         buf='Required memory too small for defined data'
         call error(1503)
         return
      endif
      call scimem(mem+1,offset)
      if(offset.eq.0) then
         call error(112)
         return
      endif
      offset=offset+1
      l1=lstk(1)
      l2=lstk(bot)
c     copy the top of the stack
      if (top.gt.0) call unsfdcopy(ltop,stk(l1),1,stk(offset),1)
      kd1=offset-l1
      do 30 k=1,top+1
         lstk(k)=lstk(k)+kd1
 30   continue

c     copy the bottom of the stack
      call unsfdcopy(lbot,stk(l2),1,stk(offset+mem-lbot),1)
      kd2=offset+mem-lbot-l2
      do 40 k=bot,isiz
         lstk(k)=lstk(k)+kd2
 40   continue

      call freemem()
c     update reference variables
      do 41 i=1,top
         if(infstk(i).ne.2) then
            il=iadr(lstk(i))
            if(istk(il).lt.0) istk(il+1)=istk(il+1)+kd1
         endif
 41   continue
      do 42 i=bot,isiz-1
         if(infstk(i).ne.2) then
            il=iadr(lstk(i))
            if(istk(il).lt.0) istk(il+1)=istk(il+1)+kd2
         endif
 42   continue

c     rebuild asolute pointers if necessary see macro.f  and run.f
      if(macr.gt.0) then
         lpt1=lpt(1)
c     Check if a compiled macro is running
         p=pt+1
 45      p=p-1
         if((rstk(p).ge.601.and.rstk(p).le.603).or.rstk(p).eq.605) then
            call adjuststkptr(pstk(p),kd1,kd2,l2)
         elseif(rstk(p).eq.604.or.rstk(p).eq.606) then
            call adjuststkptr(ids(1,p),kd1,kd2,l2)
            if(rstk(p).le.606) call adjuststkptr(ids(2,p),kd1,kd2,l2)
         elseif(rstk(p).ge.611.and.rstk(p).le.616) then
             call adjuststkptr(ids(1,p),kd1,kd2,l2)
             if(rstk(p).ge.614) call adjuststkptr(pstk(p),kd1,kd2,l2)
         elseif(rstk(p).eq.501.or.rstk(p).eq.502.or.rstk(p).eq.503) then
            k = lpt1 - (13+nsiz)
            lpt1 = lin(k+1)
            if(rstk(p).eq.501.or.rstk(p).eq.502) then
               call adjuststkptr(lin(k+6),kd1,kd2,l2)
               call adjuststkptr(lin(k+7),kd1,kd2,l2)
               call adjuststkptr(ids(3,p),kd1,kd2,l2)
            endif
         endif
         if(p.gt.0) goto 45
      endif

      leps=sadr(iadr(lstk(isiz-5)) +4)

 50   top=top+1
      call objvide('stacksize',top)
      return
      end
      subroutine adjuststkptr(p,topoffset,botoffset,lbot)
c     routine for intstacksize
      integer p,topoffset,botoffset,lbot
      if(p.ge.2*lbot) then
         p=p+2*botoffset
      else
         p=p+2*topoffset
      endif
      end

      subroutine inttimer
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,cremat
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('timer',1,1)) return
      if(.not.checkrhs('timer',0,0)) return

      top=top+1
      if(.not.cremat('timer',top,0,1,1,l,lc)) return
      call timer(stk(l))
      return
      end


      subroutine inttype
c     Copyright INRIA
      include '../stack.h'
      integer cmode,topk
      logical checkrhs,checklhs,getscalar,cremat
      integer iadr
c    
      iadr(l)=l+l-1
c
      if(.not.checklhs('type',1,1)) return
      if(.not.checkrhs('type',1,1)) return


      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      k=istk(il)
      if(.not.cremat('type',top,0,1,1,l,lc)) return
      stk(l)=dble(k)
      return
      end
      subroutine intwhat(fname)
c     
c     Copyright INRIA
      include '../stack.h'
c     
      character*(*) fname
      logical checklhs,checkrhs
      integer iadr

      integer cmdl,id(nsiz)
      parameter (cmdl = 22)
      integer cmd(nsiz,cmdl)
      common/cmds/cmd


      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
c
      if(.not.checklhs(fname,1,1)) return
      if(.not.checkrhs(fname,0,0)) return
c
      call funtab(id,0,0)
c     comandes
      fin = 1
      call msgs(41,0)
      call prntid(cmd,cmdl,wte)
c     
      top=top+1
      il = iadr(lstk(top))
      istk(il) = 0
      lstk(top+1) = lstk(top) + 1
      end
      subroutine intwhereis
c     Copyright INRIA
      include '../stack.h'
      integer id(nsiz)
      logical checkrhs,checklhs,cremat,getsmat,checkval
      integer topk
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
      topk=top
c
      if(.not.checklhs('whereis',1,1)) return
      if(.not.checkrhs('whereis',1,1)) return


      il=iadr(lstk(top))
      if (istk(il).lt.0) il=iadr(istk(il+1))
c
      if(istk(il).eq.11.or.istk(il).eq.13) then
         call putid(id,idstk(1,top))
      elseif(istk(il).eq.10) then
         if(.not.getsmat('whereis',topk,top,m,n,1,1,l,n1)) return
         if(.not.checkval('whereis',m*n,1) ) return
         call namstr(id,istk(l),n1,0)
      else
         err=1
         call error(44)
         return
      endif
      fin=-3
      call funs(id)
      if(err.gt.0) return
      if(fun.eq.0) then
         if(.not.cremat('whereis',top,0,0,0,lr,lc)) return
      else
         call putid(id,idstk(1,fun))
         il=iadr(lstk(top))
         istk(il)=10
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         istk(il+4)=1
         l=il+6
         fun=0
         call namstr(id,istk(l),n,1)
         istk(il+5)=n+1
         lstk(top+1)=sadr(l+n)
      endif
      return
      end
      subroutine intwho()
c     Copyright INRIA
      include '../stack.h'
      integer sz,szm,nv,nvm,top0
      character*6 what
      logical checkrhs,checklhs,getsmat,checkval,cremat
      integer iadr,sadr

c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c
      rhs=max(0,rhs)
      if(.not.checkrhs('who',0,1)) return

c
      if(rhs.eq.0) then
c     .  Display stored variables
         if(.not.checklhs('who',1,1)) return
c
c     .  local variables
         call msgs(38,0)
         sz=lstk(isiz) - lstk(bot) + 1
         szm=lstk(isiz)-lstk(1)
         nv=isiz-bot
         nvm=isiz-1
         call prntid(idstk(1,bot),nv-1,wte)
         write (buf,'(4i10)') sz,szm,nv,nvm
         call msgs(39,0)
c
c     .  global variables
         if(gtop.ge.isiz+2) then
            call msgs(44,0)
            sz=lstk(gtop+1) - lstk(isiz+2) + 1
            szm=lstk(gbot) - lstk(isiz+2) + 1
            nv=gtop-isiz-1
            nvm=isizt-isiz-1
            call prntid(idstk(1,isiz+2),nv,wte)
            write (buf,'(4i10)') sz,szm,nv,nvm
            call msgs(39,0)
         endif
c     .  return null matrix
         top=top+1
         il = iadr(lstk(top))
         istk(il) = 0
         lstk(top+1) = lstk(top) + 1
         return
      else
c     .  get stored variables
         if(.not.checklhs('who',1,2)) return
         top0=top
         if(.not.getsmat('who',top0,top,m,n,1,1,lr,nlr)) return
         if(.not.checkval('who',m*n,1)) return
         call codetoascii(max(5,nlr),istk(lr),what)
         if (what(1:3).eq.'get'.or.what(1:5).eq.'local') then
            n=isiz-bot
            il=iadr(lstk(top))
            lw=sadr(il+5+n+n*nlgh)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ip=il+4
            l=ip+n+1
            istk(il)=10
            istk(il+1)=n
            istk(il+2)=1
            istk(il+3)=0
            istk(ip)=1
            do 21 i=0,n-1
               call namstr(idstk(1,bot+i),istk(l),nl,1)
               istk(ip+1+i)=istk(ip+i)+nl
               l=l+nl
 21         continue
            lstk(top+1)=sadr(l)
            if(lhs.eq.1) goto 999
            top=top+1
            if(.not.cremat('who',top,0,n,1,l,lc)) return
            do 22 i=0,n-1
               stk(l+i)=dble(lstk(bot+i+1)-lstk(bot+i))
 22         continue
         elseif (what.eq.'global') then
            n=gtop-(isiz+1)
            if(n.eq.0) then
               if(.not.cremat('who',top,0,0,0,l,lc)) return
               if(lhs.eq.1) goto 999
               top=top+1
               if(.not.cremat('who',top,0,0,0,l,lc)) return
               goto 999
            endif
            il=iadr(lstk(top))
            lw=sadr(il+5+n+n*nlgh)
            err=lw-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            ip=il+4
            l=ip+n+1
            istk(il)=10
            istk(il+1)=n
            istk(il+2)=1
            istk(il+3)=0
            istk(ip)=1
            do 31 i=0,n-1
               call namstr(idstk(1,isiz+2+i),istk(l),nl,1)
               istk(ip+1+i)=istk(ip+i)+nl
               l=l+nl
 31         continue
            lstk(top+1)=sadr(l)
            if(lhs.eq.1) goto 999
            top=top+1
            if(.not.cremat('who',top,0,n,1,l,lc)) return
            do 32 i=0,n-1
               stk(l+i)=dble(lstk(isiz+2+i+1)-lstk(isiz+2+i))
 32         continue
         else
            err=1
            call error(116)
         endif
      endif
 999  return
      end

      subroutine intgetdate()
c     Copyright INRIA
      include '../stack.h'
      parameter (nf=10)
      integer w(nf),dt
      logical checkrhs,checklhs,cremat,getscalar,getrmat
      integer gettype
c
      rhs=max(0,rhs)
      if(.not.checkrhs('getdate',0,1)) return
      if(.not.checklhs('getdate',1,1)) return
      if(rhs.eq.1) then
         ityp=gettype(top)
         if(ityp.eq.10) then
            job=0
            top=top-1
            n=1
         else 
            if(.not.getrmat('getdate', top, top, m1, n1, lr)) return
            top=top+1
            if(.not.cremat('getdate',top,0,m1*n1,nf,l,lc)) return
            do 05 i=0,m1*n1-1
               dt=stk(lr+i)
               call convertdate(dt,w)
c     .         dt contains a number of seconds, number of milliseconds
C     .         w(10) must be 0
               w(10)=0
               call int2db(nf,w,1,stk(l+i),m1*n1)
 05         continue
            call copyobj('getdate',top,top-1)
            top=top-1
            return
         endif
      else
         job=1
         n=nf
      endif
      
      call scigetdate(dt,ierr)
      if(ierr.ne.0) then
         buf='Impossible to get the date...'
         call error(999)
         return
      endif
      if(job.eq.1) then
         call convertdate(dt,w)
      else
         w(1)=dt
      endif

 10   top=top+1
      if(.not.cremat('getdate',top,0,1,n,l,lc)) return
      call int2db(n,w,1,stk(l),1)
      return
      end

      subroutine intintppty()
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,cremat,getscalar
      parameter (mxbyptr=40)
      integer byptr(mxbyptr),nbyptr
      common /ippty/ byptr,nbyptr
      integer iadr,sadr
c
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(0,rhs)
      if(.not.checkrhs('intppty',0,1)) return
      if(.not.checklhs('intppty',1,1)) return
      
      if(rhs.eq.0) then
c     query
         top=top+1
         if(.not.cremat('intppty',top,0,1,nbyptr,l,lc)) return
         call int2db(nbyptr,byptr,1,stk(l),1)
      else
c     set
         if(.not.getscalar('intppty',top,top,l)) return
         num=stk(l)
         call setippty(num)
         il=iadr(lstk(top))
         istk(il)=0
         lstk(top+1)=lstk(top)+1
      endif
      return
      end

      subroutine useascommand(fname)
c     Copyright INRIA
      include '../stack.h'
      logical checkrhs,checklhs,cremat,getscalar,getsmat,checkval
      integer topk,id(nsiz)
      integer iadr,sadr
      character*1 opt
      character*(*) fname
c
c    
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(0,rhs)
      if(.not.checkrhs(fname,1,2)) return
      if(.not.checklhs(fname,1,1)) return

      job=1
      topk=top
      
      if(rhs.eq.2) then
         if(.not.getsmat(fname,topk,top,m,n,1,1,l,nl))return
         if (nl.ne.1) then
            err=2
            call error(36)
            return
         endif
         call cvstr(nl,istk(l),opt,1)
         top=top-1

         if(opt.eq.'a') then
            job=1
         elseif (opt.eq.'d') then
            job=2
         else
            err=2
            call error(36)
            return
         endif
      endif
      if(.not.getsmat(fname,topk,top,m,n,1,1,l,n1)) return
      if(.not.checkval(fname,m*n,1) ) return
      call namstr(id,istk(l),n1,0)
      call comand(id,job)
      if(err.gt.0) return
      il=iadr(lstk(top))
      istk(il)=0
      lstk(top+1)=lstk(top)+1
      return
      end
