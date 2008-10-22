      subroutine desi00(ityp,om,norma,edeg,ndeg,adeg,
     *vsn,vd,a)
c!purpose
c transform tolerance scheme
c!
c
      implicit double precision (a-h,o-z)
      double precision  om(*)
c
      if (ityp.ge.3) ndeg = (ndeg+1)/2
      if (ndeg.ne.0) adeg = dble(real(ndeg))/(1.0d+0+edeg)
      call transn (ityp,om,norma,vsn,vd,a)
      return
      end
