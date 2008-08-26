// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plotout(zoom)
  global count
  if count==[] then count=0,end
  if argn(2)<1 then
    zoom=1/2
  end

  count=count+1
  DATA_BEGIN=ascii(2);
  DATA_END=ascii(5)
  win=xget('window')
  output='/tmp/foo'+string(count)
  opts='""'+[output+'.eps' '*'+string(zoom) '*'+string(zoom) '' '' '' '']+'""';

  xs2ps(win,output) 

  // not needed any more
  //unix_s(SCI+'/bin/BEpsf -portrait '+output)

  //[DATA_BEGIN]scheme:(postscript "/tmp/foo.eps" "*1/2" "*1/2" "" "" "" "")[DATA_END]

  write(%io(2),DATA_BEGIN+'scheme:(postscript '+strcat(opts,' ')+')'+ ...
	DATA_END,'(a)')

endfunction
