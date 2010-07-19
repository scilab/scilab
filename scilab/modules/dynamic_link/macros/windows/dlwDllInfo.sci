// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function dllinfolist = dlwDllInfo(dllname, options)
//=============================================================================
  function symbolslist = dllinfoimports(dllname)
    symbolslist = list();
    [result,bOK] = dos('dumpbin /IMPORTS ""' + dllname +'""');
    if bOK == %T then
      result(5) = '';
      indicedotdll = grep(result, getdynlibext());

      if (indicedotdll <> []) then
        dlllist = result(indicedotdll);

        symbolslist = list();

        for i = 1:size(dlllist,'*')-1
          symbolsdll = [];
          symbolsdllstr = result(indicedotdll(i) + 6:indicedotdll(i + 1) - 2);
          for j = 1: size(symbolsdllstr, '*')
            tok = tokens(symbolsdllstr(j), ' ');
            if size(tok, '*') >=2 then
              symbolsdll(j) = tok(2);
            end
          end
          symbolslist(i)= list(dlllist(i), symbolsdll);
        end

        symbolsdllstr = result(indicedotdll(size(dlllist, '*')) + 6:grep(result, '.data') - 4);
        for j = 1: size(symbolsdllstr, '*')
          symbolsdll = [];
          tok = tokens(symbolsdllstr(j), ' ');
          if size(tok,'*') >=2 then
            symbolsdll(j) = tok(2);
          end
        end
        symbolslist(size(dlllist, '*'))= list(dlllist(size(dlllist, '*')), symbolsdll);
      end
    end
  endfunction
//=============================================================================
  function symbolslist = dllinfoexports(dllname)
    symbolslist = list();
    symbolsdll = [];
    [result,bOK] = dos('dumpbin /EXPORTS ""' + dllname +'""');
    if bOK == %T then
      if size(result,'*') > 20 then
        indicenumberfunctstr = 15;
        numberfunctstr = result(indicenumberfunctstr);
        numberfunct = sscanf(numberfunctstr, '%d');

        indicefirstsymbolstr = 20;
        firstsymbolstr = result(indicefirstsymbolstr);
        for i = 0:numberfunct
          tok = tokens(result(indicefirstsymbolstr + i), ' ');
          if size(tok,'*') >=4 then
            symbolsdll = [symbolsdll; tok(4)];
          end
        end
        [pat, nam, ext] = fileparts(dllname);
        symbolslist = list(nam + ext, symbolsdll);
      end
    end
  endfunction
//=============================================================================
  function dllinfolist = dllinfomachine(dllname)
    dllinfolist = list();
    machine = '';
    [result,bOK] = dos('dumpbin /HEADERS ""' + dllname +'""');
    if bOK == %T then
      if size(result, '*') > 20 then
        indiceinfomachine = 12;
        infomachinestr = result(indiceinfomachine);
        tok = tokens(infomachinestr, ' ');
        if size(tok, '*') == 3 then
          machine = strsubst(tok(3), ')', '');
          machine = strsubst(machine, '(', '');
        end
        [pat, nam, ext] = fileparts(dllname);
        dllinfolist = list(nam + ext, machine);
      end
    end
  endfunction
//=============================================================================

  [lhs,rhs]=argn(0);

  if rhs == 1 then
    options = 'exports';
  end

  if type(dllname) <> 10 | size(dllname) <> 1  then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), 'dllinfo', 1));
  end

  if fileinfo(dllname) == [] then
    error(msprintf(gettext("%s: The file %s does not exist.\n"), 'dllinfo', dllname));
  end

  fext = fileext(dllname);
  if ~(strcmpi(fext, '.exe') == 0 | strcmpi(fext, '.dll')== 0) then
    error(msprintf(gettext("%s: Cannot open file %s.\n"), 'dllinfo', dllname));
  end

  if type(options) <> 10 | size(options) <> 1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"), 'dllinfo', 2));
  end

  if ~(strcmpi(options, 'imports')== 0 | strcmpi(options, 'exports')== 0 | strcmpi(options, 'machine')== 0) then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), 'dllinfo', 2, '''imports'', ''exports'', ''machine'''));
  end

  dllinfolist = list();

  if findmsvccompiler() <> 'unknown' then
    if strcmpi(options, 'imports')== 0 then
      dllinfolist = dllinfoimports(dllname);
    end

    if strcmpi(options, 'exports')== 0 then
      dllinfolist = dllinfoexports(dllname);
    end

    if strcmpi(options, 'machine')== 0 then
      dllinfolist = dllinfomachine(dllname);
    end

  else
    warning(msprintf(gettext("%s: This feature required Microsoft visual studio C compiler.\n"), 'dllinfo'));
  end

endfunction
//=============================================================================
