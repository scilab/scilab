c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
Cc (DLL Digital Visual Fortran)     
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /stack/, /vstk/, /recu/, /iop/
cDEC$ ATTRIBUTES DLLIMPORT:: /errgst/, /com/, /adre/
cDEC$ ATTRIBUTES DLLIMPORT:: /intersci/ ,/cha1/, /dbg/
cDEC$ ATTRIBUTES DLLIMPORT:: /ierode/
cDEC$ ENDIF
C     ---------------------------------------------------------------
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
      common /stack/ stk
      integer istk(2*vsiz)
      equivalence (istk(1),stk(1))

      real sstk(2*vsiz)
      equivalence (sstk(1),stk(1))
       
      character cstk*(4*vsiz)
      equivalence (cstk,stk(1))

      complex*16 zstk(vsiz/2)
      equivalence (zstk(1),stk(1))

      integer bot,top,idstk(nsiz,isizt),lstk(isizt),leps,bbot,bot0,isiz
      integer gbot,gtop,infstk(isizt)
      common /vstk/ bot,top,idstk,lstk,leps,bbot,bot0,infstk,
     $     gbot,gtop,isiz

      integer ids(nsiz,psiz),pstk(psiz),rstk(psiz),pt,niv,macr,paus
      integer icall,krec
      common /recu/ ids,pstk,rstk,pt,niv,macr,paus,icall,krec

      integer ddt,err,lct(8),lin(lsiz),lpt(6),rio,rte,wte
      common /iop/ ddt,err,lct,lin,lpt,rio,rte,wte

      integer err1,err2,errct,toperr,errpt,ieee,catch
      common /errgst/ err1,err2,errct,toperr,errpt,ieee,catch

      integer sym,syn(nsiz),char1,fin,fun,lhs,rhs,ran(2),comp(3)
      common /com/ sym,syn,char1,fin,fun,lhs,rhs,ran,comp

      character alfa(csiz)*1,alfb(csiz)*1,buf*(bsiz)
      common /cha1/ alfa,alfb,buf

      integer wmac,lcntr,nmacs,macnms(nsiz,maxdb),lgptrs(maxdb+1)
      integer bptlg(maxbpt)
      common /dbg/ wmac,lcntr,nmacs,macnms,lgptrs,bptlg

      integer lbot,ie,is,ipal,nbarg,ladr(intersiz)
      common/adre/lbot,ie,is,ipal,nbarg,ladr
      integer nbvars,iwhere(intersiz),
     $ nbrows(intersiz),nbcols(intersiz),
     $ itflag(intersiz),ntypes(intersiz),lad(intersiz),
     $ ladc(intersiz),lhsvar(intersiz)
      common/intersci/nbvars,iwhere,nbrows,nbcols,
     $ itflag,ntypes,lad,ladc,lhsvar
      integer ierror
      common/ierode/ierror
c*------------------------------------------------------------------
