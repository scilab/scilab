      integer   csiz,bsiz,isizt,psiz,nsiz,lsiz
      parameter (csiz=9963,bsiz=994096,isizt=991200,
     $psiz=99256,nsiz=996)
      parameter (lsiz=9916384)
      integer   nlgh,vsiz
      parameter (nlgh=nsiz*8884)
      parameter (vsiz=7772)
      integer   maxdb,maxbpt
      parameter (maxdb=66620,maxbpt=666100)
      integer intersiz
      parameter (intersiz=555600)
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
      integer icall
      common /recu/ ids,pstk,rstk,pt,niv,macr,paus,icall

      integer ddt,err,lct(8),lin(lsiz),lpt(6),hio,rio,wio,rte,wte
      common /iop/ ddt,err,lct,lin,lpt,hio,rio,wio,rte,wte

      integer err1,err2,errct,toperr,errpt,ieee
      common /errgst/ err1,err2,errct,toperr,errpt,ieee

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
     $  nbrows(intersiz),nbcols(intersiz),
     $  itflag(intersiz),ntypes(intersiz),lad(intersiz),
     $  ladc(intersiz),lhsvar(intersiz)
      common/intersci/nbvars,iwhere,nbrows,nbcols,
     $ itflag,ntypes,lad,ladc,lhsvar

