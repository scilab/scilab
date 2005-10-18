      subroutine mpwmul(ar,ai,br,bi,cr,ci)
      double precision ar,ai,br,bi,cr,ci
      include 'mpstack.h'
c     multiplication of max-plus symmetrized numbers/SG
c     c = a*b
      cr=PCZERO
      ci=PCZERO
      write(*,*)'ar=',ar,'br=',br,'ai=',ai,'bi=',bi,'PCZERO=',PCZERO
      if(ar.ne.PCZERO)then
         write(*,*)'ici'
         cr = ar+br
         ci = ar+bi
         write(*,*)'cr=',cr,'ci=',ci
      endif
      if(ai.ne.PCZERO)then
         write(*,*)'la'
         cr=max(cr,ai+bi)
         ci=max(ci,ai+br)
         write(*,*)'cr=',cr,'ci=',ci
      endif
      write(*,*)'cr=',cr,'ci=',ci
      return
      end
