      logical function getspmat(fname,topk,lw,type,m,n,nnz,lr)
C     ==================================================
C     verifie qu'il y a une matrice creuse scalaire 
C     a la position lw en renvoyant .true. ou .false.
C	  suivant la reponse
C     Entree :
C       lw : position (entier)
C     Sortie :
C       m, n dimensions
C       lr : pointe sur le element de la matrice creuse
C     ==================================================
C!
C      implicit undefined (a-z)
      character fname*(*)
      integer topk,lw,m,n,lr
      integer il
      integer iadr
      include '../stack.h'
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      il=iadr(lstk(lw))
C     test particulier decouvert ds logic.f
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).ne.5) then
         getspmat=.false.
         call cvname(ids(1,pt+1),fname,0)
         err=rhs+(lw-topk)
         call error(217)
      else
         m=istk(il+1)
         n=istk(il+2)
         type=istk(il+3)
         nnz=istk(il+4)
         lr=il+5+m+nnz
         getspmat=.true.
      endif
      return
      end


      logical function mycspmatptr(name,m,n,type,nnz,lp)
c!purpose
c     mycspmatptr returns the adress of real/complex sparse matrix "name"
c     in scilab's internal stack
c     m=number of rows
c     n=number of columns
c     type= 0 for double and 1 for complex
c     stk(lp),stk(lp+1),...,stk(lp+nnz-1)= entries 
c     If matrix "name" not in Scilab stack, returns m=n=-1.
c
      character*(*) name
      integer type
c
      include '../stack.h'
      integer iadr,sadr
      integer id(nsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      mycspmatptr=.false.
      lon=0
      do 100 i=1,csiz
         if(name(i:i).eq.char(0)) goto 200
         lon=lon+1
 100  continue
 200  continue
      lon1=len(name)
      if((lon1.gt.0).and.(lon1.lt.lon)) then
         ln=lon1
      else
         ln=lon
      endif
      if(lon.eq.lon1) ln=lon
      ln=min(nlgh,ln)
      call cvname(id,name(1:ln),0)
c
      fin=-1
      call stackg(id)
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         m=-1
         n=-1
         return
      endif
      il=iadr(lstk(fin))
      if(istk(il).ne.5)
     $     call error(44)
      if(err.gt.0) return
      m=istk(il+1)
      n=istk(il+2)
      type=istk(il+3)
      nnz=istk(il+4)
      lp=sadr(il+5+m+nnz)
      mycspmatptr=.true.
      end


      logical function mycspbmatptr(name,m,n,type,nnz,lp)
c!purpose
c     mycspmatptr returns the adress of real/complex sparse matrix "name"
c     in scilab's internal stack
c     m=number of rows
c     n=number of columns
c     type= 0 for double and 1 for complex
c     stk(lp),stk(lp+1),...,stk(lp+nnz-1)= entries 
c     If matrix "name" not in Scilab stack, returns m=n=-1.
c
      character*(*) name
      integer type
c
      include '../stack.h'
      integer iadr,sadr
      integer id(nsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      mycspbmatptr=.false.
      lon=0
      do 100 i=1,csiz
         if(name(i:i).eq.char(0)) goto 200
         lon=lon+1
 100  continue
 200  continue
      lon1=len(name)
      if((lon1.gt.0).and.(lon1.lt.lon)) then
         ln=lon1
      else
         ln=lon
      endif
      if(lon.eq.lon1) ln=lon
      ln=min(nlgh,ln)
      call cvname(id,name(1:ln),0)
c
      fin=-1
      call stackg(id)
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         m=-1
         n=-1
         return
      endif
      il=iadr(lstk(fin))
      if(istk(il).ne.6)
     $     call error(44)
      if(err.gt.0) return
      m=istk(il+1)
      n=istk(il+2)
      type=istk(il+3)
      nnz=istk(il+4)
      lp=sadr(il+5+m+nnz)
      mycspbmatptr=.true.
      end


      logical function mycmatptr(name,m,n,type,lp)
c!purpose
c     matptr returns the adress of real matrix "name"
c     in scilab's internal stack
c     m=number of rows
c     n=number of columns
c     type= 0 for double and 1 for complex
c     stk(lp),stk(lp+1),...,stk(lp+m*n-1)= entries (columnwise)
c     If matrix "name" not in Scilab stack, returns m=n=-1.
c
c    Example of use:
c    Amat is a real 2 x 3 scilab matrix
c    your subroutine should be as follows:
c
c    subroutine mysubr(...)
c    include '../stack.h'
c    ...
c    logic= cmatptr('Amat',m,n,lp)
c    => m=3 , n=2, and stk(lp)=Amat(1,1)
c                      stk(lp+1)=Amat(2,1)
c                      stk(lp+2)=Amat(3,1)
c                      stk(lp+3)=Amat(1,2) ...
c                      stk(lp+5)=Amat(3,2)
c   see example in fydot.f file
c   see also  readmat.f, matz.f
      character*(*) name
      integer type
c
      include '../stack.h'
      integer iadr,sadr
      integer id(nsiz)
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      mycmatptr=.false.
      lon=0
      do 100 i=1,csiz
         if(name(i:i).eq.char(0)) goto 200
         lon=lon+1
 100  continue
 200  continue
      lon1=len(name)
      if((lon1.gt.0).and.(lon1.lt.lon)) then
         ln=lon1
      else
         ln=lon
      endif
      if(lon.eq.lon1) ln=lon
      ln=min(nlgh,ln)
      call cvname(id,name(1:ln),0)
c
      fin=-1
      call stackg(id)
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         m=-1
         n=-1
         return
      endif
      il=iadr(lstk(fin))
      if(istk(il).ne.1.or.(istk(il+3).ne.0.and.istk(il+3).ne.1))
     $     call error(44)
      if(err.gt.0) return
      m=istk(il+1)
      n=istk(il+2)
      type = istk(il+3)
      lp=sadr(il+4)
      mycmatptr=.true.
      end
