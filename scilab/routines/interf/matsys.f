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
c     resu form  isdef   exists errcatch errclear iserror predef
c     11   12     13    14      15       16       17      18
c     newfun clearfun  funptr  macr2lst setbpt delbpt dispbpt
c     19      20       21       22       23     24      25
c     funcprot whereis where   <free>  <free>   havewindow stacksize
c     26         27    28      29       30       31       32
c     mtlb_mode  link     ulink  c_link addinter <free>   <free>
c     33         34        35     36    37       38        39
c     fclear    what    sciargs  chdir getwd ieee typename
c     40         41       42     43     44     45    46
c     global   clearglobal isglobal gstacksize intppty
c     47         48          49        50      52
c     lasterror version loadhistory savehistory gethistory resethistory macr2tree
c     53         54        55          56         57          58        59
c     <free> getos
c     60    61 
c     banner fromjava  getmemory fromc getmd5 warning
c     62     63        64        65       66   
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matsys '//buf(1:4))
      endif
c     
      if(rstk(pt).eq.901) goto 70

      if(rhs.gt.0) il=iadr(lstk(top))
      goto (10 ,20 ,998 ,55,60 ,70 ,80 ,120,130,140,
     +      150,160,190,180,200,210,220,230,240,250,
     +      251,300,320,350,370,380,390,400,998,998,
     +      450,500,510,600,610,620,630,640,650,660,
     +      670,680,681,682,683,684,690,691,692,693,
     +      695,697,698,699,700,701,702,703,
     +      998,706,707,708,709,710,711,712),fin
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
 180  call intexists(0)
      goto 999
c     
c     isdef
 190  call intexists(1)
      goto 999
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
c     havewindow
 450  call inthavewindow
      return
c
c     stacksize
 500  call intstacksize("stacksize")
      goto 999
c
c     mtlb_mode
 510  call intmtlbmode
      goto 999
 600  call scilink("link")
      goto 999
 610  call intulink("unlink")
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
 703  call macr2tree('macr2tree')
      goto 999
 706  call intgetos('getos')
      goto 999 
 707  call intbanner('banner')
      goto 999     
 708  call intfromjava('fromjava')
      goto 999
 709  call intgetmemory('getmemory')
      goto 999
 710  call intfromc('fromc')
      goto 999   
 711  call intgetmd5('getmd5')
      goto 999
 712  call intwarning('warning')
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
