      subroutine wdiv(ar,ai,br,bi,cr,ci)
c!but
c
c     cette subroutine wdiv calcule la division c=a/b de
c     nombres complexes de double precision. les parties
c     reelles de a, b et c sont rangees, respectivement dans
c     ar, br et cr. de facon analogue les parties imaginaires
c     de a, b et c son rangees dans ai, bi et ci.
c
c!liste d'appel
c
c     subroutine wdiv(ar,ai,br,bi,cr,ci)
c
c     ar, ai: double precision, parties reelle et imaginaire de a.
c
c     br, bi: double precision, parties reelle et imaginaire de b.
c
c     cr, ci: double precision, parties reelle et imaginaire de c.
c
c!auteur
c
c     cleve moler.
c
c!
c     Copyright INRIA
      double precision ar,ai,br,bi,cr,ci
c     c = a/b
      double precision s,d,ars,ais,brs,bis
      s = abs(br) + abs(bi)
      if (s .eq. 0.0d+0) return
      ars = ar/s
      ais = ai/s
      brs = br/s
      bis = bi/s
      d = brs**2 + bis**2
      cr = (ars*brs + ais*bis)/d
      ci = (ais*brs - ars*bis)/d
      return
      end
