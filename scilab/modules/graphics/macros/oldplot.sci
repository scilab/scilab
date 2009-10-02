// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=oldplot(x,y,legx,legy,leg)
// 
// For backward compatibility with some very old stuff
//! 
  [lhs,rhs]=argn(0);
  if rhs >=2,
    if type(y)==1, 
      if prod(size(x))<>prod(size(y)),
	write(%io(2),'x and y must have the same size');
	return;end
    end;
  end;

  select rhs,
  case 1 then 
    [nl,nk]=size(x);
    if nl==1|nk==1 then
      plot2d1("enn",1,matrix(x,prod(size(x)),1));
    else
      clf();plot2d((ones(nl,1)*(1:nk))',x')
    end
  case 2 then clf();
    if type(y)==10,
      plot2d1("enn",1,matrix(x,prod(size(x)),1));
      xtitle(' ',y,' ');
    else
      plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
    end;
  case 3 then clf();
    if type(y)==10,
      plot2d1("enn",1,matrix(x,prod(size(x)),-1));
      xtitle(' ',y,legx);
    else 
      plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
      xtitle(' ',legx,' ');
    end;
  case 4 then clf();
    if type(y)==10,
      plot2d1("enn",1,matrix(x,prod(size(x)),1));
      xtitle(legy,y,legx);
    else 
      plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
      xtitle(' ',legx,legy);
    end;
  case 5 then clf();
    plot2d(matrix(x,prod(size(x)),1),matrix(y,prod(size(y)),1));
    xtitle(leg,legx,legy);
  end
endfunction



