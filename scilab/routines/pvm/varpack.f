      subroutine varpak(k,pack,n,nmax,ierr)
c     Given a scilab variable, stored in the stack at the position k (in
c     lstk) this function returns a "packing" vector pack for pvm.
c     A scilab variable is a consecutive memory region formed by a
c     succession of integer and double precision vectors.
c     pack contains a sequence of pairs of integers. 

c     varpak returns a vector which contains the size of this integer
c     and double precision vectors. pack contains a sequence of pairs of
c     integers:  pack=[i1,r1,i2,r2,...,in,rn]
c     il is a number of integer words
c     rl is a number of double precision word
c
c     n    : is the returned length of pack
c     nmax : is the maximum size allowed for pack
      INCLUDE '../stack.h'
c
      integer k,pack(*)
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      ierr=0

      il=iadr(lstk(k))
      if(istk(il).lt.0) il=iadr(istk(il+1))

c      pack(1) = 2
c      pack(2) = 3
c      pack(3) = 4
c      pack(4) = 5
c      pack(5) = 6
c      pack(6) = 7
c      pack(7) = 8
c      pack(8) = 9
c      n = 8
c      return
c      write(*,*) 'VARPACK:', istk(il), k,nmax
      if(istk(il).eq.1) then
         call spak(il,pack,n,nmax)
c         write(*,*) 'VARPACK2:', istk(il), k,nmax
      elseif(istk(il).eq.2) then
         call ppak(il,pack,n,nmax)
      elseif(istk(il).eq.4) then
         call bpak(il,pack,n,nmax)
      elseif(istk(il).eq.5) then
         call sppak(il,pack,n,nmax)
      elseif(istk(il).eq.6) then
         call bsppak(il,pack,n,nmax)
      elseif(istk(il).eq.8) then
         call ipak(il,pack,n,nmax)
      elseif(istk(il).eq.10) then
         call cpak(il,pack,n,nmax)
      elseif(istk(il).eq.11.or.istk(il).eq.13) then
         call mpak(il,pack,n,nmax)
c      elseif(istk(il).eq.14) then
c         call libpak(il,pack,n,nmax)
      elseif(istk(il).eq.15.or.istk(il).eq.16.or.istk(il).eq.17) then
         call lpak(il,pack,n,nmax)
      else
         ierr=1
         return
      endif
      if(n.gt.nmax) then
         ierr=2
         return
      endif
      return
      end

      integer function odd(n)
      n1=n
      if (2*int(n1/2).ne.n1) n1=n1+1
      odd=n1
      return
      end

      subroutine spak(il,pack,n,nmax)
c     matrix of number case
      INCLUDE '../stack.h'
      integer pack(*)
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      n=2
      if(n.gt.nmax) return
      pack(1)=4
      pack(2)=istk(il+1)*istk(il+2)*(istk(il+3)+1)
      return
      end

      integer function allignf(n,m)
      n1=n
      n2=m*int(n1/m)
      if (n2.ne.n1) n1=n1+(m-(n1-n2))
      allignf=n1
      write(*,*) 'allignf:',n, m, n1,n2
      return
      end

      subroutine ipak(il,pack,n,nmax)
c     matrix of interger
      INCLUDE '../stack.h'
      integer pack(*),allignf,odd
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      n=2
      if(n.gt.nmax) return
      pack(1)=4
      pack(2)=0
c     char
      if (istk(il+3).eq.1) then
         pack(1)=odd(pack(1)+allignf(istk(il+1)*istk(il+2),4)/4)
      endif
c     short
      if (istk(il+3).eq.2) then
         pack(1)=odd(pack(1)+allignf(istk(il+1)*istk(il+2),2)/2)
      endif
c     int
      if (istk(il+3).eq.4) then
         pack(1)=odd(pack(1)+istk(il+1)*istk(il+2))
      endif
c     unsigned char
      if (istk(il+3).eq.11) then
         pack(1)=odd(pack(1)+allignf(istk(il+1)*istk(il+2),4)/4)
      endif
c     unsigned short
      if (istk(il+3).eq.12) then
         pack(1)=odd(pack(1)+allignf(istk(il+1)*istk(il+2),2)/2)
      endif
c     unsigned int
      if (istk(il+3).eq.14) then
         pack(1)=odd(pack(1)+istk(il+1)*istk(il+2))
      endif
      write(*,*) 'VARPACK:', istk(il), istk(il+1)*istk(il+2),
     $     istk(il+3), pack(1)
      return
      end
   

      subroutine ppak(il,pack,np,npmax)
c     matrix of polynomial case
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return

      id=il+8
      mn=istk(il+1)*istk(il+2)
      pack(1)=odd(8+mn+1)
      pack(2)=(istk(id+mn)-1)*(istk(il+3)+1)
      return
      end

      subroutine bpak(il,pack,np,npmax)
c     matrix of boolean case
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return

      id=il+8
      mn=istk(il+1)*istk(il+2)
      pack(1)=odd(3+mn)
      pack(2)=0
      return
      end

      subroutine sppak(il,pack,np,npmax)
c     sparse matrix of numbers  case
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return

      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

      pack(1)=odd(5+m+nel)
      pack(2)=nel*(it+1)
      return
      end

      subroutine bsppak(il,pack,np,npmax)
c     sparse matrix of numbers  case
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return

      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)

      pack(1)=odd(5+m+nel)
      pack(2)=0
      return
      end

      subroutine cpak(il,pack,np,npmax)
c     matrix of strings  case
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd,vol
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return

      mn=istk(il+1)*istk(il+2)
      id=il+4
      vol=istk(il+4+mn)-1
      pack(1)=odd(4+mn+1+vol)
      pack(2)=0
      return
      end

      subroutine mpak(il,pack,np,npmax)
c     function  case (compiled or not)
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,odd,il1,nin,nout,nt
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return
      il1=il+1
      nin=istk(il1)
      il1=il1+nin*nsiz+1
      nout=istk(il1)
      il1=il1+nout*nsiz+1
      nt=istk(il1)
      pack(1)=odd(4+nin*nsiz+nout*nsiz+nt)
      pack(2)=0
      return
      end

      subroutine libpak(il,pack,np,npmax)
c     function  case (compiled or not)
      INCLUDE '../stack.h'
      integer pack(*),np,npmax,il,odd,nf,ilh,nh,iln,nm
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      np=2
      if(np.gt.npmax) return
      nf = istk(il+1)
      ilh = istk(il+2+nf)
      nh = istk(ilh)
      iln = ilh+nh+1
      nm = istk(iln)
c      write(*,*) nf, nh, nm, istk(iln+1)
      pack(1)=odd(4+nf+nh+2*nm+100)
      pack(2)=0
      return
      end
