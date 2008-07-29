function an=remezb(nc,fg,ds,wt)
//an=remezb(nc,fg,ds,wt)
//Minimax approximation of a frequency domain
//magnitude response.  The approximation takes
//the form
//
//          h = sum[a(n)cos(wn)]
//
//for n=0,1,...,nc.  An FIR, linear-phase filter
//can be obtained from the the output of the macro
//by using the following Scilab commands
//
//          hn(1:nc-1)=an(nc:-1:2)/2;
//          hn(nc)=an(1);
//          hn(nc+1:2*nc-1)=an(2:nc)/2;
//
//  nc :Number of cosine functions
//  fg :Grid of frequency points in [0,.5)
//  ds :Desired magnitude on grid fg
//  wt :Weighting function on error on grid fg
//  an :Cosine filter coefficients
//!
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//get frequency grid size
  
  ngrid=maxi(size(fg));
  
  //check for errors in input
  
  if min(fg)<0|max(fg)>0.5 then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"),'remezb',2,"0","0.5"));
  end,
  dsize=maxi(size(ds));
  wsize=maxi(size(wt));
  if dsize<>ngrid then
    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),'remezb',2,3));
  end,
  if wsize<>ngrid then
    error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),'remezb',2,4));
  end,
  
  //set up the initial guess for the extremal frequencies
  
  iext=round(1:ngrid/nc:ngrid);
  iext(nc+1)=ngrid;
  iext(nc+2)=ngrid;
  
  //call remez.f
  
  an=remez(iext,ds,fg,wt);
  an=an(1:nc)
endfunction
