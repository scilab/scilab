
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function lnk=editgraph_arc(v1,v2,v3,v4,v5,v6,v7,v8)

//initialisation de link mlist
  if exists('xx','local')==0 then xx=[],end
  if exists('yy','local')==0 then yy=[],end
  if exists('thick','local')==0 then thick=[0,0],end
  if exists('ct','local')==0 then ct=[1],end
  if exists('from','local')==0 then from=[],end
  if exists('to','local')==0 then to=[],end
  lnk=mlist(['Arc','xx','yy','thick','ct','from','to'],..
	    xx,yy,thick,ct,from,to)

endfunction
