      function logops(op1)
c ======================================================================
c     effectue les test logiques entre variables
C     This routine is no more used ?
C     it's only here for backward compatibility with old compiled macros
c ================================== ( Inria    ) ======================
c     Copyright INRIA
      include '../stack.h'
c
      logical logops
      double precision e1,e2
      integer equal,less,great,vt1,op,op1
      integer left,right
      integer iadr,sadr
      data equal/50/,less/59/,great/60/
      data left/54/,right/55/
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      logops=.false.
      if(err1.gt.0) return
c
c     les lignes suivantes ont ete rajoutee pour assurer la compatibilite
c     des macro compilee avec l'ancienne gestion des tests logiques
      op=op1
      if(op.eq.left) then 
         op=less
      elseif(op.eq.right)then
         op=great
      elseif(op.eq.left+right) then
         op=less+great
      elseif(op.eq.left+equal) then
         op=less+equal
      elseif(op.eq.right+equal) then
         op=great+equal
      endif

c
      nelt=1
      ielt=1
      nlist=0
c
      il1=iadr(lstk(top-1))
      il2=iadr(lstk(top))
      iecar=il2-il1
c      write(06,*) 'Je suis dans logops : '
   01 vt1=istk(il1)
      if(vt1.ne.1.or.istk(il2).ne.1) goto 20
c
c matrices de scalaires
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      l2=sadr(il2+4)
c
      if(op.ne.equal.and.op.ne.less+great) goto 15
      do 11 i=1,3
      if(istk(il1+i).ne.istk(il2+i) ) goto 99
   11 continue
      do 12 i=1,m1*n1
      if(stk(l1-1+i).ne.stk(l2-1+i)) goto 99
   12 continue
      l1=l1+m1*n1
      goto 70
c
   15 if(istk(il2+1)*istk(il2+2)*(istk(il2+3)+1)*
     1    m1*n1*(it1+1).ne.1) call error(44)
      if(err.gt.0) return
      e1=stk(l1)
      e2=stk(l2)
      if (op.eq.less          .and. e1.lt.e2) goto 98
      if (op.eq.great         .and. e1.gt.e2) goto 98
      if (op.eq.(less+equal)  .and. e1.le.e2) goto 98
      if (op.eq.(great+equal) .and. e1.ge.e2) goto 98
      goto 99
c
   20 if(op.ne.equal.and.op.ne.less+great) call error(57)
      if(err.gt.0) return
      if(istk(il1).ne.istk(il2)) goto 99
c
c matrices de polynomes
      if(vt1.ne.2) goto 30
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      do 22 i=1,m1*n1+8
      if(istk(il1+i).ne.istk(il2+i)) goto 99
   22 continue
      l1=sadr(il1+m1*n1+9)-1
      l2=sadr(il2+m1*n1+9)-1
      m1=(istk(il1+m1*n1+8)-1)*(it1+1)
      do 23 i=1,m1
      if(stk(l1+i).ne.stk(l2+i)) goto 99
   23 continue
      l1=l1+m1+1
      goto 70
c
c chaines de caracteres
   30 if(vt1.ne.10) goto 40
      m1=istk(il1+1)
      n1=istk(il1+2)
      mn1=m1*n1+4
      mn1=mn1+istk(il1+mn1)-1
      do 31 i=1,mn1
      if(istk(il1+i).ne.istk(il2+i)) goto 99
   31 continue
      l1=sadr(il1+mn1+1)
      goto 70
c
   40 if(vt1.ne.11.and.vt1.ne.13) goto 50
c macros compilee ou non
      mlhs=istk(il1+1)
      mrhs=istk(il1+2+nsiz*mlhs)
      n=istk(il1+3+nsiz*(mlhs+mrhs))
      mn1=3+(mlhs+mrhs)*nsiz+n
      do 41 i=1,mn1
      if(istk(il1+i).ne.istk(il2+i)) goto 99
   41 continue
      l1=sadr(il1+mn1+1)
      goto 70
c
   50 continue
      if(vt1.ne.14) goto 60
      nid=istk(il1+1)
      nh=istk(il1+2+nid)
      n=istk(il1+3+nh+nid)
      mn1=3+nid+nh+n*nsiz
      do 51 i=1,mn1
      if(istk(il1+i).ne.istk(il2+i)) goto 99
   51 continue
      l1=sadr(il1+mn1+1)
      goto 70
c
   60 if(vt1.ne.15) goto 90
c listes
      nelt=istk(il1+1)
      do 61 i=1,nelt
      if(istk(il1+2+i).ne.istk(il2+2+i)) goto 99
   61 continue
      nlist=nlist+1
      ill=iadr(lstk(top))
      l1=sadr(il1+3+nelt)
      istk(ill+2*(nlist-1))=l1
      istk(ill+ 2*(nlist-1)+1)=nelt
c
   65 ielt=0
   70 ielt=ielt+1
      if(ielt.gt.nelt) goto 80
      il1=iadr(l1)
      il2=il1+iecar
      goto 01
   80 nlist=nlist-1
      if(nlist.le.0) goto 98
      l1=istk(ill+2*nlist)
      nelt=istk(ill+2*nlist+1)
      goto 65
c
   90 continue
      call error(44)
      return
c
   98 logops=.true.
   99 if(op.eq.less+great) logops=.not.logops
      top=top-2+lhs
      return
      end
