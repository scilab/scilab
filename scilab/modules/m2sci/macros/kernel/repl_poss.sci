// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=repl_poss(expr,varargin)
// Display informations for replacement for a statement in converted file

txt=[]
nposs=size(varargin)

if nposs==0 | fix(nposs/3)*3~=nposs then
  error(gettext("Wrong number of inputs."))
end

txt=[expression2code(expr)+gettext(" may be replaced by:")]
for k=1:3:nposs
  vars=""
  if typeof(varargin(k+1))<>"list" then
    vars=expression2code(varargin(k+1))
  else
    for l=1:size(varargin(k+1))
      vars=vars+expression2code(varargin(k+1)(l))
      if l<size(varargin(k+1)) then
	vars=vars+" and "
      end
    end
  end
  txt=[
      txt
      "   --> "+expression2code(varargin(k))+" "+gettext("if")+" "+vars+" "+varargin(k+2)]
end

set_infos(txt,1)

endfunction
