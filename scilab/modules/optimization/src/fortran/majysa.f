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
      subroutine majysa(n,nt,np,y,s,ys,lb,g,x,g1,x1,index,ialg,nb)
c
c     mise a jour des vecteurs ({y}(i),{s}(i),ys(i),i=1,np)
      implicit double precision (a-h,o-z)
      dimension     y(nt,n),s(nt,n),ys(nt),g(n),x(n),g1(n),x1(n)
      integer  index(n),ialg(15)
c
c     -----mise a jour de y(lb, ) , s(lb, ) , ys(lb)
      do 100 i=1,n
         y(lb,i)=g(i)-g1(i)
         s(lb,i)=x(i)-x1(i)
100   continue
      ys(lb)=0
      do 200 i=1,n
         ys(lb)=ys(lb)+y(lb,i)*s(lb,i)
200   continue
c
c     accumulation eventuelle
      if(ialg(8).eq.5.and.np.gt.0) then
         do 20 i=1,n
            y(1,i)=y(1,i) + y(lb,i)
            s(1,i)=s(1,i) + s(lb,i)
20       continue
         ys(1)=0
         do 30 i=1,n
30       ys(1)=ys(1)+y(1,i)*s(1,i)
      endif
c
c
c     -----mise a jour de np et index
      if(np.lt.nt) then
         np = np +1
         index(lb)=np
      else
         ij=lb
         do 300 i=nb,nt
            ij=ij+1
            if(ij.gt.nt) ij=nb
            index(i)=ij
300      continue
      endif
c
c     ------chercher la prochaine place libre
      if(lb.eq.nt) then
         lb=nb
      else
         lb=lb+1
      endif
c
c     --------------
      return
      end
