// Allan CORNET
// INRIA 2007
// start a command or a PowerShell script
function [resultat,status] = powershell(varargin)
  Chainecmd = '';
  Chainecmdbegin = 'powershell.exe -nologo -inputformat text -outputformat text -Noninteractive ';
	resultat = [gettext("Error(s) : see help powershell'';''verify your script without scilab.")];
	status = %f;

	[lhs,rhs]=argn(0);
	
	if (~MSDOS) then
	  resultat = [gettext("only for Microsoft Windows.")];
	  return;
	end
	
	try
	  winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\PowerShell\1','Install');
	catch
	  resultat = [gettext("Powershell not found.")];
	  return;
	end
 
  if (rhs == 1) then
    if ( (type(varargin(1)) == 10) & and(size(varargin(1)) == [1 1]) ) then
      // Check that the first input argument is a file
      [x,ierr]=fileinfo(varargin(1));
      if ( x == [] ) then // it is a command
        Chainecmd = Chainecmdbegin + '-command ""' + varargin(1) + '""';
      else //it is a file script
        Chainecmd = Chainecmdbegin + '-command ""Set-ExecutionPolicy RemoteSigned; &{' + varargin(1) + '}""';
      end
      [resultat,status]=dos(Chainecmd);
    else
      error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"),"powershell"));
    end
  else
    error(msprintf(gettext("%s: Wrong type for input argument: String expected.\n"),"powershell"));
  end
  
endfunction
