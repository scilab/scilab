      subroutine misops
c     ================================== ( Inria    ) =============
c     operations sur les types secondaires (macros librairies...)
c     =============================================================
c     Copyright INRIA
      include '../stack.h'
c     
      integer iadr,sadr,op,rhs1,r
      integer equal,less,great,insert,extrac
      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      r=0
      if(pt.gt.0) r=rstk(pt)
      if (r.eq.408) goto 50
      op=fin
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' misops '//buf(1:4))
      endif
c     
      fun=0
c     
      lw=lstk(top+1)
      if(op.eq.insert) then
         rhs1=2
      elseif(op.eq.extrac) then
         rhs1=1
      else
         rhs1=rhs
      endif
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
      call libops
      return
c     
 100  continue
c     macros
      call macroops
      return
c     
      end

      subroutine libops
c     Copyright INRIA
      include '../stack.h'
c     
      integer iadr,sadr,op,r
      integer id(nsiz)
      integer equal,less,great,insert,extrac,blank
      logical cresmat,ptover

      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
      data blank/40/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      r=0
      if(pt.gt.0) r=rstk(pt)
      if (r.eq.408) goto 32

      op=fin
      
      if(op.eq.equal.or.op.eq.less+great) then
         goto 10
      elseif (op.eq.extrac) then
         goto 30
      else
c     .  operations non implantees
         fin=-fin
         return
      endif

 10   il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))

      top = top-1
      il1=iadr(lstk(top))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      itrue=1
      if(op.eq.less+great) itrue=0
      ilog=1-itrue
c     
      if(istk(il1).ne.istk(il2)) goto 15
      if(istk(il1+1).ne.istk(il2+1)) goto 15
      nf=istk(il1+1)
      do 11 i=1,nf
         if(istk(il1+1+i).ne.istk(il2+1+i)) goto 15
 11   continue
      ln=2+nf
      if(istk(il1+ln).ne.istk(il2+ln)) goto 15
      nh=istk(il1+ln)
      if(nh.gt.0) then
         do 12 i=1,nh
            if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 15
 12      continue
      endif
      ln=ln+nh+1
      if(istk(il1+ln).ne.istk(il2+ln)) goto 15
      long=istk(il1+ln)
      do 13 i=1,long*nsiz
         if(istk(il1+ln+i).ne.istk(il2+ln+i)) goto 15
 13   continue
      ilog=itrue
      
 15   istk(ilr)=4
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=ilog
      lstk(top+1)=sadr(ilr+4)
      return
c     
c     extraction
 30   continue
c     .  the library
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))

c     . the index
      il1=iadr(lstk(top-1))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).eq.10) then
c     .    only extract the function
         if(istk(il1+1)*istk(il1+2).ne.1) then
            call error(44)
            return
         endif
c     .  get the macro name and its length
         nn=istk(il1+5)-1
         ilnam=il1+6
      elseif(istk(il1).eq.15) then
c     .  extract and call
         n1=istk(il1+1)
         l=sadr(il1+3+n1)
         iln=iadr(l)
         if(istk(iln).ne.10) then
            call error(44)
            return
         endif
         if(istk(iln+1)*istk(iln+2).ne.1) then
            call error(44)
            return
         endif
         nn=istk(iln+5)-1
         ilnam=iln+6
      else
         call error(44)
         return
      endif
c     extract the function out of the library
c     get the lib path
      np=istk(il2+1)
      ilpath=il2+2
c     load the bin file
c     create a variable with the bin file path
      top=top+1
      if(.not.cresmat(' ',top,1,1,np+4+nn)) return
      call getsimat('load',top,top,mp,mp,1,1,ilp,nlp)
c     path
      call icopy(np,istk(ilpath),1,istk(ilp),1)
c     name
      call icopy(nn,istk(ilnam),1,istk(ilp+np),1) 
c     extension
      call cvstr(4,istk(ilp+np+nn),'.bin',0)
c     load variables stored in the given file
      srhs=rhs
      slhs=lhs
      fun=0
      rhs=1
      lhs=1
      id(1)=blank

      if(istk(il1).eq.15) then
c     simulate a macro to preserve old function definition
         k = lpt(6)
         if(k+13+nsiz.gt.lsiz) then
            call error(108)
            return
         endif
         lin(k+1) = lpt(1)
         lin(k+5) = bot
         
         lpt(1) = k + 13+nsiz
         lpt(6)=lpt(1)
         macr=macr+1
      endif

      call intload(id,k)
      if(err.gt.0) return
      rhs=srhs
      lhs=slhs
c     
      if(istk(il1).eq.10) then
         top=top-2
         istk(il1)=0
         lstk(top+1)=sadr(il1+1)
         return
      endif

      top=top-2
c     form individual variables with the 2..n1 entries of index
      l1=lstk(top)
      do 31 i=2,n1
         lstk(top+1)=lstk(top)+istk(il1+2+i)-istk(il1+1+i)
         top=top+1
 31   continue
      l=sadr(il1+3+n1)+istk(il1+3)-1
      call scidcopy(istk(il1+2+n1)-istk(il1+3),stk(l),1,stk(l1),1)
      top=top-1
      rhs=n1-1

c     evaluate loaded macro for given arguments
      fin=lstk(fin)
      if (ptover(1,psiz)) return
      rstk(pt)=408
      icall=5
c     *call* macro
      return
 32   macr=macr-1
      pt=pt-1
      k = lpt(1) - (13+nsiz)
      bot=lin(k+5)
      lpt(1)=lin(k+1)
      fin=1

      return
      end

      subroutine macroops
c     Copyright INRIA
      include '../stack.h'
c     
      integer iadr,sadr,op,rhs1
      integer equal,less,great,insert,extrac
      data equal/50/,less/59/,great/60/,insert/2/,extrac/3/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      op=fin
c
      if(op.eq.equal.or.op.eq.less+great) goto 180
c     
c     operations non implantees
      fin=-fin
      return
c     
c     comparaisons of macros
 180  continue
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))

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
      return
      end
