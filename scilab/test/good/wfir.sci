// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [wft,wfm,fr]=wfir(ftype,forder,cfreq,wtype,fpar)
//[wft,wfm,fr]=wfir(ftype,forder,cfreq,wtype,fpar)
//Macro which makes linear-phase, FIR low-pass, band-pass,
//high-pass, and stop-band filters
//using the windowing technique.
//Works interactively if called with no arguments.
//  ftype  :Filter type ('lp','hp','bp','sb')
//  forder :Filter order (pos integer)(odd for ftype='hp' or 'sb')
//  cfreq  :2-vector of cutoff frequencies (0<cfreq(1),cfreq(2)<.5)
//         :only cfreq(1) is used when ftype='lp' or 'hp'
//  wtype  :Window type ('re','tr','hm','hn','kr','ch')
//  fpar   :2-vector of window parameters
//         :     Kaiser window: fpar(1)>0 fpar(2)=0
//         :     Chebyshev window: fpar(1)>0 fpar(2)<0 or
//         :                       fpar(1)<0 0<fpar(2)<.5
//  wft    :Time domain filter coefficients
//  wfm    :Frequency domain filter response on the grid fr
//  fr     :Frequency grid
//!
wft=[];wfm=[];fr=[]
//check arguments of macro call
 
[lhs,rhs]=argn(0);
 
//if macro called with no arguments query user for values
 
if rhs<=0 then,
 
  //Query user for filter type and filter length
  nc=x_choose([gettext('low pass');
               gettext('high pass');
	       gettext('band pass');
	       gettext('stop band')],gettext('Choose type of filter to be designed'))
  //Select filter type and Query user for cut-off frequencies
  flag=0;
  select nc
  case 1 then //low pass
    [ok,fl,forder]=getvalue(gettext('Input filter characteristics'),..
                    [gettext('cut-off frequency (0.<frequ<.5):');
		     gettext('filter length')],..
		    list('vec',1,'vec',1),[' ';' '])
    ftype='lp'
    fh=0;
  case 2 then //high pass
    [ok,fl,forder]=getvalue(gettext('Input filter characteristics'),..
                    [gettext('cut-off frequency (0.<frequ<.5):');
		     gettext('filter length (odd value)')],..
		    list('vec',1,'vec',1),[' ';' '])
    fh=0;
    flag=1;
    ftype='hp'
  case 3 then //band pass
    [ok,fl,fh,forder]=getvalue(gettext('Input filter characteristics'),..
                    [gettext('low cut-off frequency (0.<flow<.5):');
		     gettext('high cut-off frequency (0.<flow<fhi<.5):');
		     gettext('filter length')],..
		    list('vec',1,'vec',1,'vec',1),[' ';' ';' '])
    ftype='bp'	
  case 4 then //stop band
    [ok,fl,fh,forder]=getvalue(gettext('Input filter characteristics'),..
                    [gettext('low cut-off frequency (0.<flow<.5):');
		     gettext('high cut-off frequency (0.<flow<fhi<.5):');
		     gettext('filter length (odd value)')],..
		    list('vec',1,'vec',1,'vec',1),[' ';' ';' '])
    flag=1;
    ftype='sb'	
  else
    return
  end
  
  if flag==1 then
    if forder-2*int(forder/2)==0 then
      messagebox([gettext('Even length high pass and stop band filters not allowed');
	    gettext('---Filter order is being incremented by 1')],"modal","error");
      forder=forder+1;
    end
  end
 
  //Query user for window type and window parameters
  nc=x_choose([gettext('Kaiser');
             gettext('Chebyshev');
	     gettext('Rectangular');
	     gettext('Triangular');
	     gettext('Hamming') ],gettext('Input window type'))
  select nc
  case 1 then
    wtype='kr'
    [ok,Beta]=getvalue(gettext('Input window characteristics'),..
                     ['beta>0'],list('vec',1),' ')
    fpar(1)=Beta
    fpar(2)=0;
  case 2 then
    wtype='ch' 
    [ok,name,value]=getvalue([gettext('Input window characteristics:');
             ' ';
	     gettext('dp (dp>0)   : the maximum value of the window side-lobe height');
	     gettext('df (0<df<.5): the width of the window main lobe')
	     ' ';
	     gettext('only one of this two values is to be defined,')
	     gettext('the other one is automatically deduced')],..
	     [gettext('name of specified value');
	     gettext('value')],list('str',-1,'vec',1),['dp','0.3'])
    if part(name,1:2)=='dp' then
      fpar=[value,-1]
    elseif part(name,1:2)=='df' then
      fpar=[-1,value]
    else
      messagebox(gettext('Incorrect parameter name entered'),"modal","error");
      return
    end
  case 3 then
    wtype='re'
    fpar=[0 0];
  case 4 then
    wtype='tr'
    fpar=[0 0];
  case  5 then
    wtype='hm'
    fpar=[0 0];
  case  6 then
    wtype='hn'
    fpar=[0 0];
  else
    return
  end
else
  fl=cfreq(1);
  fh=cfreq(2);
end,
 
//Calculate window coefficients
 
   [win_l,cwp]=window(wtype,forder,fpar);
   [dummy,forder]=size(win_l);
 
//Get forder samples of the appropriate filter type
 
   hfilt=ffilt(ftype,forder,fl,fh);
 
//Multiply window with sinc function
 
   wft=win_l.*hfilt;
 
//Calculate frequency response of the windowed filter
 
   [wfm,fr]=frmag(wft,256);
endfunction
