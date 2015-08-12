c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine typ2cod(il,name,n)
c     returns in name(1:n) the code associated with the type of the
c     variable that began in istk(il)

      INCLUDE 'stack.h'
      integer nmax
      parameter (nmax=8)
c     following common defines the initial database of type names
      integer maxtyp,nmmax
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax),ptmax
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /typnams/
cDEC$ ENDIF
      common /typnams/ tp,ptr,ln,namrec,ptmax
      integer name(*)
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      itype=abs(istk(il))
      if(itype.le.20) then
c     look for itype in predefined types
         if(itype.eq.16.or.itype.eq.17) then
            if(istk(il).lt.0) il=iadr(istk(il+1))
            n1=istk(il+1)
            iltyp=iadr(sadr(il+3+n1))
            nlt=min(nlgh-3,istk(iltyp+5)-1)
            iltyp=iltyp+5+istk(iltyp+1)*istk(iltyp+2)
            n=min(nlt,nmax)
            call icopy(n,istk(iltyp),1,name(1),1)
            return
         elseif(itype.eq.0) then
            n=1
            name(1)=0
         else
            n=ln(itype)
            call icopy(n,namrec(ptr(itype)),1,name,1)
         endif
      else
c     look for itype in dynamically added types
         it=20
 10      it=it+1
         if(it.gt.maxtyp) goto 9000
         if(tp(it).ne.itype) goto 10
         n=ln(it)
         call icopy(n,namrec(ptr(it)),1,name,1)
      endif
c
      return
 9000 continue
      n=0
      return
      end

      subroutine addtypename(typ,nam,ierr)
      integer pos,typ,ierr
      character*(*) nam
c
c     Copyright INRIA
c     following common defines the initial database of type names
      integer maxtyp,nmmax
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax),ptmax
cDEC$ IF DEFINED (FORDLL)
cDEC$ ATTRIBUTES DLLIMPORT:: /typnams/
cDEC$ ENDIF
      common /typnams/ tp,ptr,ln,namrec,ptmax
      character*15 nam1
c
      ierr=0
c
      n=len(nam)
      if(n.gt.0) then
c     add a type
         if(typ.le.20) then
            pos=typ
            if(ln(pos).ne.0) then
c     .     check if new type is the same as old type
               if(ln(pos).eq.len(nam)) then
                  n=ln(pos)
                  call cvstr(ln(pos),namrec(ptr(pos)),nam1,1)
                  if(nam1(1:n).eq.nam(1:n)) return
               endif
               ierr=2
               return
            endif
         else
            pos=20
 10         pos=pos+1
            if(pos.gt.maxtyp) then
               ierr=1
               return
            endif
            if(ln(pos).ne.0) goto 10
         endif
         tp(pos)=typ
         if(ptmax+n.gt.nmmax) then
            ierr=3
            return
         endif

         ln(pos)=n
         ptr(pos)=ptmax
         call cvstr(n,namrec(ptmax),nam,0)
         ptmax=ptmax+n
      else
c     suppress a type
         if(typ.le.20) then
            pos=typ
            if(ln(pos).eq.0) return
         else
            pos=20
 20         pos=pos+1
            if(pos.gt.maxtyp) return
            if(tp(pos).ne.typ) goto 20
         endif
         n=ln(pos)
         ll=ptmax-(ptr(pos)+n)+1
         call icopy(ll,namrec(ptr(pos)+n),1,namrec(ptr(pos)),1)
         ptr(pos)=0
         ln(pos)=0
         ptmax=ptmax-n
      endif
      return
      end
