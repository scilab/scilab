      logical function wcompa(ar,ai,br,bi,op)
c     compare two double precision complex numbers a and b
c! calling sequence
c     logical function dcompa(ar,ai,br,bi,op)
c     ar,ai : real and imaginary part of a
c     br,bi : real and imaginary part of b
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=119: a~=b
c
c!origin : Serge Steer INRIA 1995
c!
c     Copyright INRIA
      double precision ar,ai,br,bi
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.equal) then
         l=ar.eq.br.and.ai.eq.bi
      elseif(op.eq.less+great) then
         l=ar.ne.br.or.ai.ne.bi
      endif
      wcompa=l
      end
