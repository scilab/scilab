// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function clf(varargin)

nbArg = size(varargin);

if nbArg==0 then
  h=gcf()
  job='clear'
elseif nbArg==1 then
  if type(varargin(1))==1 then // win num given
    num=varargin(1)

    h=[];for k=1:size(num,'*'),h=[h;get_figure_handle(num(k))];end

    job='clear'
  elseif type(varargin(1))==9 then // handle given
    h=varargin(1);job='clear'
  elseif type(varargin(1))==10 then // job given
    h=gcf()
    job=varargin(1)
  end
elseif nbArg==2 then
  if type(varargin(1))==1 then // win num given
    num=varargin(1)

    h=[];for k=1:size(num,'*'),h=[h;get_figure_handle(num(k))];end

  elseif type(varargin(1))==9 then // handle given
    h=varargin(1);
  end
  job=varargin(2);
else
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "clf", 0, 2));
end

if and(job<>['clear','reset']) then
  error(msprintf(gettext("%s: Wrong value for input argument #%d: ''clear'' or ''reset'' expected."), "clf", nbArg));
end

nbHandles = size(h,'*');

// check that all the handles are figures
for k=1:nbHandles
  curFig = h(k);
  if curFig.type <> 'Figure' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A vector of ''Figure'' handle expected."), "clf", 1));
  end
end

if (job == 'reset') then
  for k = 1: nbHandles
    curFig = h(k);

    // delete the figure and recreate a new one with same id.
    // consequently we are sure it is reset to the default values.
    figId = curFig.figure_id;
    delete(curFig);
    scf(figId);
  end 
end


for k=1:nbHandles 
  curFig = h(k)

  // drawlater
  immediateMode = curFig.immediate_drawing;
  curFig.immediate_drawing = 'off';
  
  delete(curFig.children);

  // drawnow
  curFig.immediate_drawing = immediateMode;
end

endfunction

