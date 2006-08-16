      subroutine sigelm
c     Copyright INRIA
c ================================== ( Inria    ) =============
c     basic signal processing routines
      include 'stack.h'
c
      double precision adelp,adels,fact,zzi(16),zzr(16),zpi(16),zpr(16)
      double precision alpha,beta,u,y,eps,eps1,v
      double precision kappa,lambda,mu,nu
      integer ordre,lw,lt,li,lr,lo,lf,lg
      integer i,top2
      integer iadr,sadr
c
      logical vect,arma
c
c functions
c
c         1      2      3      4      5      6      7      8      9
c       ffir   fft   fiir   corr   rpem   amell  delip  remez  syredi
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' sigelm '//buf(1:4))
      endif
c
      lbot=lstk(bot)
      goto(10,20,30,40,50,60,70,80,90)fin
c
c     filtres a reponse impulsionnelle finie  c
 10   continue
      call sciffir('ffir')
      return
c
c fft : transformee de  fourier rapide
 20   continue
      call scifft('fft')
      return
 30   continue
      call scifiir('fiir')
      return
c corr correlation      
 40   continue
      call scicorr("corr")
      return
c      
c rpem
 50   continue
      call scirpem('rpem')
      return
 60   continue
c
c     amell: calculation of the jacobi's elliptic function am(u,k)
      call sciamell('amell')
      return
c
c delip
 70   continue
      call scidelip('delip')
      return
c
c remez      
 80   continue
      call sciremez('remez')
      return
c
c     syredi goes here      
 90   continue
      call scisyredi('syredi')
      return
      end

 