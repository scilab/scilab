      subroutine readmat(name,m,n,scimat)
c     see creadmat 
c     ==========================================
c     Copyright INRIA/ENPC 
      double precision scimat(m,*)
      integer m,n
      logical creadmat,j
      character*(*) name
      j=creadmat(name,m,n,scimat)
      return
      end

      logical function creadmat(name,m,n,scimat)
c     ==========================================
c!purpose
c     readmat reads vector/matrix in scilab's internal stack
c
c!calling sequence
c     logic=creadmat('matrixname',m,n,scimat)
c
c  matrixname: character string; name of the scilab variable.
c  m: number of rows (output of readmat)
c  n: number of columns (output of readmat)
c  scimat: matrix entries stored columnwise (output of readmat)
c 
c    Example of use:
c    Amat is a real 2 x 3 scilab matrix
c    your subroutine should be as follows:
c
c    subroutine mysubr(...)
c
c    ...
c    call readmat('Amat',m,n,scimat)
c    => m=3 , n=2, and scimat(1)=Amat(1,1)
c                      scimat(2)=Amat(2,1)
c                      scimat(3)=Amat(3,1)
c                      scimat(4)=Amat(1,2) ...
c                      scimat(5)=Amat(3,2)
c!
c     ==========================================
      integer m,n
      logical getrmat
      character*(*) name
      double precision scimat(m,*)
c
      include '../stack.h'
      integer il,it,l,id(nsiz)
      iadr(l)=l+l-1
      creadmat=.false.
      call str2name(name,id)
c     read   : from scilab stack -> fortran variable
c     -------
      fin=-1
      call stackg(id)
      if (err .gt. 0) return
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         return
      endif
      if ( infstk(fin).eq.2) then
         fin = istk(iadr(lstk(fin))+2)
      endif
      if(.not.getrmat('creadmat',fin,fin,m,n,l))return
      call dmcopy(stk(l),m,scimat,m,m,n)
      creadmat=.true.
      return
      end

      logical function cwritemat(name,m,n,mat)
c     =========================================
c!purpose
c     cwritemat writes vector/matrix in scilab's internal stack
c
c!calling sequence
c     logic=cwritemat('matrixname'//char(0),m,n,mat)
c
c  name: character string; name of the scilab variable ( null terminated)
c  m: number of rows
c  n: number of columns 
c  mat: matrix entries stored columnwise in Scilab object 
c    call readmat('Amat',m,n,scimat)
c!
c     =========================================
      character*(*) name
      double precision mat(*)
      include '../stack.h'
      integer id(nsiz),m,n,lr,lc,srhs,l4,topk
      logical cremat
      cwritemat=.false.
      call str2name(name,id)
      topk=top
      top=top+1
      if(.not.cremat('cwritemat',top,0,m,n,lr,lc)) return
      call unsfdcopy(m*n,mat,1,stk(lr),1)
      srhs=rhs
      rhs=0
      l4=lct(4)
      lct(4)=-1
      call stackp(id,0)
      lct(4)=l4
      top=topk
      rhs=srhs
      if(err.gt.0) return
      cwritemat=.true.
      return
      end

      subroutine readchain(name,itslen,chai)
c     see creadchain
c     ==========================================
      character*(*) chai,name
      logical creadchain,j 
      j=creadchain(name,itslen,chai)
      return
      end



      logical function creadchain(name,itslen,chai)
c     ==========================================
c!but
c     this routine reads a string in scilab's  memory
c     and store it into chai 
c!calling sequence
c
c     integer       itslen
c     character*(*) chai,name
c
c     name    : character string = name of scilab variable (input)
c     chai    : chain to be read (output) 
c               null terminated 
c     itslen  : (input) maximum number of character that can ne stored
c               in chain 
c               (output) number of copied characters into chai
c               
c               
c     if Scilab variable x='qwert' exists 
c     character ch*(10)
c     l=10
c     logic= creadchain('x',l,ch) returns l=5 and ch='qwert'
c
c     ==========================================
      character*(*) chai,name
      include '../stack.h'
      integer id(nsiz),m1,n1,lr1,nlr1
      logical getsmat
c
      iadr(l)=l+l-1
c
      err=0
      creadchain=.false.
      call str2name(name,id)
      fin=-1
      call stackg(id)
      if(err.gt.0) return
      if(fin.eq.0)  then 
         call putid(ids(1,pt+1),id)
         call error(4)
         return
      endif
      if ( infstk(fin).eq.2) then
         fin = istk(iadr(lstk(fin))+2)
      endif
      if(.not.getsmat('creadchain',fin,fin,m1,n1,1,1,lr1,nlr1)) 
     $     return
      if( m1*n1.ne.1) then 
         buf = 'creadchain' // ': argument must be a string '
         call error(999)
         return
      endif
      itslen=min(itslen-1,nlr1)
      call cvstr(itslen,istk(lr1),chai,1)
      chai(itslen+1:itslen+1)=char(0)
      creadchain=.true.
      end

      logical function creadchains(name,ir,ic,itslen,chai)
c     ==========================================
c!but
c     this routine reads name(ir,ic) in scilab's  memory
c     and store it into chai 
c     if ir=ic=-1 on entry then the routines returns in ir,ic
c     the size of the matrix 
c!calling sequence
c
c     integer       itslen
c     character*(*) chai,name
c
c     name    : character string = name of scilab variable (input)
c     chai    : chain to be read (output) 
c               null terminated 
c     itslen  : (input) maximum number of character that can be stored
c               in chain 
c               (output) number of copied characters into chai
c               
c               
c     if Scilab variable x='qwert' exists 
c     character ch*(10)
c     l=10
c     logic= creadchain('x',l,ch) returns l=5 and ch='qwert'
c
c     ==========================================
      character*(*) chai,name
      include '../stack.h'
      integer id(nsiz),m1,n1,lr1,nlr1
      logical getsmat
c
      iadr(l)=l+l-1
c
      err=0
      creadchains=.false.
c
      call str2name(name,id)
      fin=-1
      call stackg(id)
      if(err.gt.0) return
      if(fin.eq.0)  then 
         call putid(ids(1,pt+1),id)
         call error(4)
         return
      endif
      if ( infstk(fin).eq.2) then
         fin = istk(iadr(lstk(fin))+2)
      endif
      if ( ir.eq.-1.and.ic.eq.-1) then 
         if(.not.getsmat('creadchain',fin,fin,ir,ic,1,1,lr1,nlr1)) 
     $        return
         creadchains=.true.
         return 
      else
         if(.not.getsmat('creadchain',fin,fin,m1,n1,ir,ic,lr1,nlr1)) 
     $        return
      endif
      itslen=min(itslen-1,nlr1)
      call cvstr(itslen,istk(lr1),chai,1)
      chai(itslen+1:itslen+1)=char(0)
      creadchains=.true.
      end


      logical function cwritechain(name,m,chai)
c     =========================================
c!purpose
c     cwritemat writes vector/matrix in scilab's internal stack
c
c!calling sequence
c     logic=cwritemat('matrixname'//char(0),m,n,mat)
c
c  name: character string; name of the scilab variable ( null terminated)
c  m: number of rows
c  n: number of columns 
c  mat: matrix entries stored columnwise in Scilab object 
c    call readmat('Amat',m,n,scimat)
c!
c     =========================================
      integer m
      character*(*) name,chai
      include '../stack.h'
      integer id(nsiz),nlr,lr,srhs,l4,topk
      logical cresmat2
      cwritechain=.false.
      call str2name(name,id)
      topk=top
      top=top+1
      if(.not.cresmat2('cwritechain',top,m,lr)) return
      call cvstr(m,istk(lr),chai,0)
      srhs=rhs
      rhs=0
      l4=lct(4)
      lct(4)=-1
      call stackp(id,0)
      lct(4)=l4
      top=topk
      rhs=srhs
      if(err.gt.0) return
      cwritechain=.true.
      return
      end

      subroutine matptr(name,m,n,lp)
c     ==========================================
c     see cmatptr 
c     ==========================================
      character*(*) name
      integer m,n,lp
      logical i,cmatptr
      i=cmatptr(name,m,n,lp)
      return
      end

      logical function cmatptr(name,m,n,lp)
c     ==========================================
c!purpose
c     matptr returns the adress of real matrix "name"
c     in scilab's internal stack
c     m=number of rows
c     n=number of columns
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
c     ==========================================
      character*(*) name
      include '../stack.h'
      integer id(nsiz)
      logical getrmat 
      iadr(l)=l+l-1
      cmatptr=.false.
c     ---- get the id 
      call str2name(name,id)
c     ---- get the position in fin 
      fin=-1
      call stackg(id)
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
         call error(4)
         m=-1
         n=-1
         return
      endif
c     ---- get data 
      if ( infstk(fin).eq.2) then
         fin = istk(iadr(lstk(fin))+2)
      endif
      if(.not.getrmat('creadmat',fin,fin,m,n,lp))return
      cmatptr=.true.
      end

      subroutine str2name(name,id)
c     =====================
c     string conversion to Scilab ID 
c     Warning : the character name is null terminated 
c             and len(name) is not used 
c             since it can be wrong (ex when name is transmited 
c             by fort (intfort : function )
c     =====================
      include '../stack.h'
      integer id(nsiz),lon 
      character*(*) name
      lon = 0 
      do 100 i=1,nlgh
         if(name(i:i).eq.char(0)) goto 200
         lon=lon+1
 100  continue
 200  continue
      call cvname(id,name(1:lon),0)
      return
      end

      logical function objptrXXXX(name,lp)
c     ==========================================
c     objptr returns the adress of "name"
c     in scilab's internal stack
*      
c     ==========================================
      character*(*) name
      include '../stack.h'
      integer id(nsiz),iadr
      iadr(l)=l+l-1
      objptr=.false.
      lp=0
c     ---- get the id 
      call str2name(name,id)
c     ---- get the position in fin 
      fin=-1
      call stackg(id)
      if (fin .eq. 0) then
         call putid(ids(1,pt+1),id)
c         we juste return false and lp is set to zero
c         call error(4)
         return
      endif
      lp=lstk(fin)
      if ( infstk(fin).eq.2) then
         lp = lstk(istk(iadr(lp)+2))
      endif
      objptr=.true.
      end






