c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1986-2008 - INRIA - Serge STEER
c
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
C/MEMBR ADD NAME=HORNER,SSI=0
      subroutine horner(p,dp,xr,xi,vr,vi)
c!but
c     cette subroutine calcule la valeur du polynome p au point x
c     suivant la formule de horner
c!liste d'appel
c     subroutine horner(p,dp,xr,xi,vr,vi)
c
c     double precision p(dp+1),xr,xi,vr,vi
c     integer dp
c
c     p : tableau contenant les coefficients du polynome ranges
c         consecutivement et par puissance croissante
c     dp : degre du polynome
c     xr,xi : parties reelle et imaginaire de l'argument
c     vr,vi : parties reelle et imaginaire du resultat
c!
      integer dp,ip,i
      double precision p(dp+1),xr,xi,vr,vi,t
c
      ip=dp+1
      vr=p(ip)
      vi=0.0d+0
      if(dp.eq.0) return
      if(xi.ne.0.0d+0) goto 20
c x reel
      do 10 i=1,dp
      vr=vr*xr+p(ip-i)
   10 continue
      return
c
c x complexe
   20 continue
      do 21 i=1,dp
      t=vr*xr-vi*xi+p(ip-i)
      vi=vi*xr+vr*xi
      vr=t
   21 continue
      return
c
      end
