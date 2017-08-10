c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine ffinf1 (n,nv,jc,xpr,p,s)
      implicit double precision (a-h,o-z)
      dimension jc(nv),p(*),s(n),xpr(nv)
c
c          cette subroutine calcule s = sigma xpr(.)*p(.)
c          sachant que les xpr ont ete calcules par fprf2
c
      do 920 i=1,n
      ps=0.
      do 910 k=1,nv
      j=jc(k)-1
      if(j.eq.0) go to 910
      nij=(j-1)*n+i
      ps=ps+xpr(k)*p(nij)
  910 continue
  920 s(i)=ps
      return
      end
