      logical  function getimat(fname,topk,lw,m,n,it,lr,ind)
C     ==================================================
C     verifie qu'il y a une matrice boolenne [m,n]
C     a la position lw en renvoyant .true. ou .false.
C	  suivant la reponse
C     Entree :
C       lw : position (entier)
C       m, n dimensions
C     Sortie :
C       lr : pointe sur le premier booleen  a(1,1)=istk(lr)
C     ==================================================
C      implicit undefined (a-z)
      integer topk,lw,m,n,lr,it
      logical ind
      logical getimatI
      character fname*(*)
      include '../stack.h'
      getimat= getimatI(fname,topk,lw,lstk(lw),m,n,it,lr,.false.,0,ind)
      return
      end 

      logical  function getimatI(fname,topk,spos,lw,m,n,lr,
     $     inList,nel,ind)
C      implicit undefined (a-z)
      logical inList,ind
      integer topk,lw,m,n,lr,nel,spos,it
      character fname*(*)
      integer il
      integer iadr, sadr
      include '../stack.h'
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      il=iadr(lw)
C     test particulier decouvert ds logic.f
      if(istk(il).lt.0) then
         ind=.true.
         il=iadr(istk(il+1))
      else
         ind=.false.
      endif
      if(istk(il).ne.8) then
         getimatI=.false.
         write(buf,*) fname 
         write(buf(nlgh+1:nlgh+3),'(i3)') rhs+(spos-topk)
         write(buf(nlgh+4:nlgh+6),'(i3)') nel
         if (inList) then 
            call basout(io,wte,buf(1:nlgh)// ': argument ' 
     $           // buf(nlgh+1:nlgh+3) // '>('// buf(nlgh+4:nlgh+6)
     $           //') '// 'should be an integer matrix ')
            buf = ' '
            call error(999)
            return 
         else
            err=rhs+(spos-topk)
            call cvname(ids(1,pt+1),fname,0)
            getimatI=.false.
            call error(208)
            return 
         endif
      else
         m=istk(il+1)
         n=istk(il+2)
         it=istk(il+3)
         lr=il+4
         getimatI=.true.
      endif
      return
      end
