      logical function dcompa(a,b,op)
c     compare two double precision numbers a and b
c! calling sequence
c     logical function dcompa(a,b,op)
c     a,b : double precision numbers
c     op is an integer indicator of the comparison to perform
c     op=50 : a==b
c     op=59 : a<b
c     op=60 : a>b
c     op=119: a~=b
c     op=109: a<=b
c     op=110: a>=b
c
c!origin : Serge Steer INRIA 1995
c!
c     Copyright INRIA
      double precision a,b
      integer op
      integer less,great,equal
      logical l
      data less/59/,great/60/,equal/50/
      if(op.eq.less) then
         l=a.lt.b
      elseif(op.eq.great) then
         l=a.gt.b
      elseif(op.eq.equal) then
         l=a.eq.b
      elseif(op.eq.less+great) then
         l=a.ne.b
      elseif(op.eq.less+equal) then
         l=a.le.b
      elseif(op.eq.great+equal) then
         l=a.ge.b
      endif
      dcompa=l
      end
