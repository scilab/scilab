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
	subroutine cusum(n,w)
c    Utility fct: cumulated sum
	double precision w(*),t
	t=0.0d0
	do 1 k=1,n
	w(k)=t+w(k)
	t=w(k) 
 1	continue
	end

