// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function whos(%_opt,%_sel)

  // get the variables local & global
  [%_nams, %_vol] = who('local');
  [%_nams_global, %_vol_global] = who('global');

//=============================================================================
// getSizeAsString, writeWhosLine, doDisplayWhosLine defined only in whos
// to remove some tmp variables defined on stack
//=============================================================================
function sz = getSizeAsString(_varname, _type)
  sz = ' ';
  if _type == 0 then
    sz = '?';
  elseif _type <= 10 then 
    execstr('sz = size(' + _varname + ');')
  elseif _type == 17 | _type == 16 then
    execstr('%_tt = getfield(1,' + _varname + ')');
    %_tt = %_tt(1);
    ierr = execstr('sz = %' + %_tt + 'size(' + _varname + ')', 'errcatch');
    if ierr <> 0 then 
      sz = '?'
    end
  end
endfunction
//=============================================================================
function writeWhosLine(_name, _namedtype, _sz, _bytes)
  write(%io(2),part(_name, 1:25) + part(_namedtype,1:15) + ..
    part(strcat(string(_sz),' by '), 1:15) + ..
    part(string(_bytes), 1:13));
endfunction
//=============================================================================
function bOK = doDisplayWhosLine(_rhs, _opt, _name, _namedtype, _sel)
  bOK = %t;
  if _rhs == 2 then
    if _opt == '-type' then
      if _namedtype <> _sel then 
        bOK = %f;
      end
    elseif _opt == '-name' then
      if part(_name, 1:length(_sel)) <> _sel then
        bOK = %f;
      end
    end
  else
    bOK = %t;
  end
endfunction
//=============================================================================

  // display defined variable in a long form

  [%_lhs, %_rhs] = argn();
  
  if %_rhs == 0 then
   %_opt = [];
   %_sel = [];
  end
  
  if %_rhs == 1 then
    error(msprintf( ..
      gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), ..
      "whos", 0, 2));
  end
  
  if %_rhs == 2 then 
    %_nams($-1:$) = [];
    if %_opt <> '-type' & %_opt <> '-name' then 
      error(msprintf( ..
        gettext( "%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), ..
        "whos", 1, "-name", "-type"));
    end
  end
  
  //write the display header
  write(%io(2),part('Name',1:25) + ..
    part('Type',1:15) + ..
    part('Size',1:15) + ..
    part('Bytes',1:13))
  
  write(%io(2),' ')
  
  %_typn = '';
  %_typ = 0;
  b_ok = %f;

  // loop on local variables 
  for %_k = 1:size(%_nams, 1)
    %_sz = ' ';
    %_typn = '';
    %_typ = 0;
    b_ok = %f;
    
    execstr('%_typn = typeof(' + %_nams(%_k) + ')');
    if execstr('%_typ = type(' + %_nams(%_k) + ')','errcatch') == 228 then
      %_typ = 0;
      %_typn = '*deleted';
    else
      execstr('%_typn = typeof(' + %_nams(%_k) + ')');
    end
    
    %_sz = getSizeAsString(%_nams(%_k), %_typ);
    b_ok = doDisplayWhosLine(%_rhs, %_opt, %_nams(%_k), %_typn, %_sel);
     
    if b_ok then
      writeWhosLine(%_nams(%_k), %_typn, %_sz, %_vol(%_k)*8);
    end
  end
  
  // loop on global variables
  for %_k = 1:size(%_nams_global, 1)
    %_sz = ' ';
    %_typn = '';
    %_typ = 0;
    b_ok = %f;
    bDoClear = %f;
    
    if isdef(%_nams_global(%_k)) then
      execstr('%_typ = type(' + %_nams_global(%_k) + ')');
      execstr('%_typn = typeof(' + %_nams_global(%_k) + ')');
    else
      execstr('global ' + %_nams_global(%_k));
      execstr('%_typ = type(' + %_nams_global(%_k) + ')');
      execstr('%_typn = typeof(' + %_nams_global(%_k) + ')');
      bDoClear = %t;
    end
    
    %_typn = '*' + %_typn;
    %_sz = getSizeAsString(%_nams_global(%_k), %_typ);
    
    if bDoClear then
      execstr('clear ' + %_nams_global(%_k));
    end
    
    b_ok = doDisplayWhosLine(%_rhs, %_opt, %_nams_global(%_k), %_typn, %_sel);
    
    if b_ok then
      writeWhosLine(%_nams_global(%_k), %_typn, %_sz, %_vol_global(%_k)*8);
    end
  end
endfunction
//=============================================================================
