      logical function lcompa(a,b,op)
c     compare two boolean coded integer numbers a and b
c! calling sequence
c     logical function dcompa(a,b,op)
c     a,b  : boolean coded integer
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=119: a~=b
c
c!origin : Serge Steer INRIA 1995
c!
c     Copyright INRIA
      integer a,b
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.equal) then
         l=a.eq.b
      elseif(op.eq.less+great) then
         l=a.ne.b
      endif
      lcompa=l
      end
