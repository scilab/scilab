c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
	subroutine cuproi(n,wr,wi)
c      Utility fct: cumulated product, complex argument
c
	double precision wr(*),wi(*),tr,ti,wwr
	tr=1.0d0
        ti=0.0d0
	do 1 k=1,n
c	w(k)=t*w(k)
           wwr=wr(k)
           wr(k)=tr*wwr-ti*wi(k)
           wi(k)=tr*wi(k)+ti*wwr
	tr=wr(k)
        ti=wi(k)
 1	continue
	end
