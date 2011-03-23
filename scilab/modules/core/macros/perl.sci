// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [result, status] = perl(varargin)
  commandLine = '';
  lhs = argn(1);
  rhs = argn(2);

  if (rhs) then
    // Check that the first param is a file
    [x,ierr]=fileinfo(varargin(1));
    if ~isfile(varargin(1)) then
        error(msprintf(gettext("%s: Unable to find Perl file: %s"), "perl", string(varargin(1))));
    else
      // Check that params are strings
      for i=1:1:rhs,
        if ~(type(varargin(i)) == 10) then
          error(msprintf(gettext("%s: Wrong type for input argument #%d: Strings expected.\n"), "perl", i));
        end

        idx=strindex(varargin(i),' ');
        if ~( idx == 0 ) then
          varargin(i) = """" + varargin(i) + """";
        end
        if (i == 2) then
          commandLine = varargin(1) + ' ' + varargin(i);
        else
          commandLine = commandLine + ' ' + varargin(i);
        end
      end

      if (commandLine == '') then
        error(msprintf(gettext("%s: No perl command specified."),"perl"));
      else
        if getos() == 'Windows' then
          // For Windows
          pathPerl= fullfile(pathconvert(SCI,%f,%f,'w'),'\tools\perl\bin\perl.exe');
          if ~isfile(pathPerl) then
            error(msprintf(gettext("%s: Unable to find Perl in ''%s''"), "perl", pathPerl));
          else
            commandLine = 'perl' + ' ' + commandLine;
            commandPerl = 'set PATH=' + pathPerl + ';%PATH%&' + commandLine + '>'+TMPDIR+'\script';
            status = unix(commandPerl);
            result = mgetl(TMPDIR + '\script');
          end
        else
          // For Linux
          status = unix('which perl' + '>' + TMPDIR + '/pathperl');
          pathperl = mgetl(TMPDIR + '/pathperl');
          if (status == 0) then
            commandLine = 'perl' + ' ' + commandLine + '>' + TMPDIR + '/script';
            status = unix(commandLine);
            result = mgetl(TMPDIR+'\script');
          else
            error(msprintf(gettext("%s: Unable to find Perl.\n"), "perl"));
          end
        end
      end
      if (status~=0) then
        error(msprintf(gettext("%s: System error: Command executed: %s"), "perl", commandLine));
      end
    end
  else
    error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"), "perl", 1));
  end
endfunction
