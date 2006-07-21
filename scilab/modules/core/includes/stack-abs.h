c Copyright INRIA 

c*------------------------------------------------------------------
c vsiz  size of internal scilab stack 
c
c bsiz  size of internal chain buf 
c
c isizt  maximum number of scilab variables global and local 
c isiz maximum number of scilab local variables
c psiz  defines recursion size
c lsiz  dim. of vector containing the command line
c nlgh  length of variable names
c csiz  used for character coding
c intersiz used in interfaces
c*-------------------------------------------------------------------
      integer   csiz,bsiz,isizt,psiz,nsiz,lsiz
      parameter (csiz=63,bsiz=4096,isizt=10000,psiz=4096,nsiz=6)
      parameter (lsiz=65536)
      integer   nlgh,vsiz
      parameter (nlgh=nsiz*4,vsiz=2)
      integer   maxdb,maxbpt
      parameter (maxdb=100,maxbpt=1000)
      integer intersiz
      parameter (intersiz=1024)
c
      double precision stk(vsiz)
$IF DEFINED(SCI_IMPORTDLL)
      global /stack/ stk
$ELSE
      common /stack/ stk 	
$ENDIF
      integer istk(2*vsiz)
      equivalence (istk(1),stk(1))

      real sstk(2*vsiz)
      equivalence (sstk(1),stk(1))
       
      character cstk*(4*vsiz)
      equivalence (cstk,stk(1))

      integer bot,top,idstk(nsiz,isizt),lstk(isizt),leps,bbot,bot0,isiz
      integer gbot,gtop,infstk(isizt)
$IF DEFINED(SCI_IMPORTDLL)
      global /vstk/ bot,top,idstk,lstk,leps,bbot,bot0,infstk,
     $     gbot,gtop,isiz
$ELSE
      common /vstk/ bot,top,idstk,lstk,leps,bbot,bot0,infstk,
     $     gbot,gtop,isiz
$ENDIF

      integer ids(nsiz,psiz),pstk(psiz),rstk(psiz),pt,niv,macr,paus
      integer icall
$IF DEFINED(SCI_IMPORTDLL)      
      global /recu/ ids,pstk,rstk,pt,niv,macr,paus,icall
$ELSE
      common /recu/ ids,pstk,rstk,pt,niv,macr,paus,icall
$ENDIF      

      integer ddt,err,lct(8),lin(lsiz),lpt(6),hio,rio,wio,rte,wte
$IF DEFINED(SCI_IMPORTDLL)      
      global /iop/ ddt,err,lct,lin,lpt,hio,rio,wio,rte,wte
$ELSE      
      common /iop/ ddt,err,lct,lin,lpt,hio,rio,wio,rte,wte
$ENDIF      

      integer err1,err2,errct,toperr,errpt,ieee
$IF DEFINED(SCI_IMPORTDLL)      
      global /errgst/ err1,err2,errct,toperr,errpt,ieee
$ELSE 
      common /errgst/ err1,err2,errct,toperr,errpt,ieee     
$ENDIF
      integer sym,syn(nsiz),char1,fin,fun,lhs,rhs,ran(2),comp(3)
$IF DEFINED(SCI_IMPORTDLL)            
      global /com/ sym,syn,char1,fin,fun,lhs,rhs,ran,comp
$ELSE
      common /com/ sym,syn,char1,fin,fun,lhs,rhs,ran,comp
$ENDIF      

      character alfa(csiz)*1,alfb(csiz)*1,buf*(bsiz)
$IF DEFINED(SCI_IMPORTDLL)                  
      global /cha1/ alfa,alfb,buf
$ELSE
      common /cha1/ alfa,alfb,buf
$ENDIF      

      integer wmac,lcntr,nmacs,macnms(nsiz,maxdb),lgptrs(maxdb+1)
      integer bptlg(maxbpt)
$IF DEFINED(SCI_IMPORTDLL)                        
      global /dbg/ wmac,lcntr,nmacs,macnms,lgptrs,bptlg
$ELSE 
      common /dbg/ wmac,lcntr,nmacs,macnms,lgptrs,bptlg
$ENDIF      

      integer lbot,ie,is,ipal,nbarg,ladr(intersiz)
$IF DEFINED(SCI_IMPORTDLL)                              
      global /adre/lbot,ie,is,ipal,nbarg,ladr 
$ELSE
      common /adre/lbot,ie,is,ipal,nbarg,ladr
$ENDIF      
      integer nbvars,nbrows(intersiz),nbcols(intersiz),
     $  itflag(intersiz),ntypes(intersiz),lad(intersiz),
     $  ladc(intersiz),lhsvar(intersiz)
$IF DEFINED(SCI_IMPORTDLL)                                   
      global /intersci/nbvars,nbrows,nbcols,
     $ itflag,ntypes,lad,ladc,lhsvar
$ELSE
      common /intersci/nbvars,nbrows,nbcols,
     $ itflag,ntypes,lad,ladc,lhsvar
$ENDIF     
c*------------------------------------------------------------------

