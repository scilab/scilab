      subroutine misops
c     ================================== ( Inria    ) =============
c     operations sur les types secondaires (macros librairies...)
c     =============================================================
c     Copyright INRIA
      include '../stack.h'
c     
      integer top0,iadr,sadr,op,rhs1
      integer equal,less,great,insert,extrac
      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' macops '//buf(1:4))
      endif
c     
      fun=0
c     
      top0=top
c     
      lw=lstk(top+1)
      rhs1=rhs
      if(op.eq.insert) rhs=2
      if(op.eq.extrac) rhs=1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
c     
c     
      ityp=0
      do 01 i=top+1-rhs1,top
        ityp=max(ityp,abs(istk(iadr(lstk(i)))))
 01   continue
      if(ityp.eq.11.or.ityp.eq.13) goto 100
      if(ityp.eq.14) goto 50
      call error(43)
      return
c
c
 50   continue
c     librairies
      if(op.eq.equal.or.op.eq.less+great) goto 60
c     
c     operations non implantees
      top=top0
      fin=-fin
      return
c     
c     comparaisons
 60   continue
      top = top-1
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      itrue=1
      if(op.eq.less+great) itrue=0
      ilog=1-itrue
c     
      if(istk(il1).ne.istk(il2)) goto 65
      if(istk(il1+1).ne.istk(il2+1)) goto 65
      nf=istk(il1+1)
      do 61 i=1,nf
         if(istk(il1+1+i).ne.istk(il2+1+i)) goto 65
 61   continue
      ln=2+nf
      if(istk(il1+ln).ne.istk(il2+ln)) goto 65
      nh=istk(il1+ln)
      if(nh.gt.0) then
         do 62 i=1,nh
            if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 65
 62      continue
      endif
      ln=ln+nh+1
      if(istk(il1+ln).ne.istk(il2+ln)) goto 65
      long=istk(il1+ln)
      do 63 i=1,long*nsiz
         if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 65
 63   continue
      ilog=itrue
      
 65   istk(ilr)=4
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=ilog
      lstk(top+1)=sadr(ilr+4)
      goto 999
      
      
c     
 100  continue
c     macros
      if(op.eq.equal.or.op.eq.less+great) goto 180
c     
c     operations non implantees
      top=top0
      fin=-fin
      return
c     
c     comparaisons
 180  continue
      top = top-1
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      itrue=1
      if(op.eq.less+great) itrue=0
      ilog=1-itrue
c     
      If(istk(il1).ne.istk(il2)) goto 185
      if(istk(il1+1).ne.istk(il2+1)) goto 185
      mrhs=istk(il1+1)
      do 181 i=1,nsiz*mrhs
         if(istk(il1+1+i).ne.istk(il2+1+i)) goto 185
 181  continue
      ln=2+nsiz*mrhs
      if(istk(il1+ln).ne.istk(il2+ln)) goto 185
      mlhs=istk(il1+ln)
      do 182 i=1,nsiz*mlhs
         if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 185
 182  continue
      ln=ln+nsiz*mlhs+1
      if(istk(il1+ln).ne.istk(il2+ln)) goto 185
      long=istk(il1+ln)
      do 183 i=1,long
         if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 185
 183  continue
      ilog=itrue
      
 185  istk(ilr)=4
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=ilog
      lstk(top+1)=sadr(ilr+4)
      goto 999
      
c     
      
 999  return
      end
