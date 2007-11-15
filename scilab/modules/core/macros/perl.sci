// Allan CORNET INRIA 2004
// Launch a perl script
function [resultat,status] = perl(varargin)
  Chainecmd = '';
  lhs=argn(1);   
  rhs=argn(2);
  
  if (rhs) then
    // Check that the first param is a file 
    [x,ierr]=fileinfo(varargin(1));
    if (x == []) then
      msgerr=gettext("Unable to find Perl file: ")+string(varargin(1));
    	error(msgerr);
    else
      // Check that params are strings
      for i=1:1:rhs,
        if ~(type(varargin(i)) == 10) then
          error(gettext("All input arguments must be strings."));
        end
        
        idx=strindex(varargin(i),' ');
        if ~( idx == 0 ) then
          varargin(i)=""""+varargin(i)+"""";
        end
        if (i == 2) then
          Chainecmd=varargin(1)+' '+varargin(i);
        else
          Chainecmd=Chainecmd+ ' ' +varargin(i);
        end
      end
      
      if (Chainecmd == '') then
        error(gettext("No perl command specified."));
      else
        if MSDOS then
          // For Windows
          CheminPerl= fullfile(pathconvert(SCI,%f,%f,'w'),'\tools\perl\bin\');
          
          [x,ierr]=fileinfo(CheminPerl+'perl.exe');
          if (x == []) then
            msgerr=gettext("Unable to find Perl in: ")+CheminPerl;
            error(msgerr);
          else
            Chainecmd = 'perl'+' '+Chainecmd;
            CommandePerl = 'set PATH='+CheminPerl+';%PATH%&'+Chainecmd+'>'+TMPDIR+'\script';
            status=unix(CommandePerl);
            resultat=mgetl(TMPDIR+'\script');
          end
        else
          // For Linux
          status = unix('which perl'+'>'+TMPDIR+'/pathperl');
          pathperl=mgetl(TMPDIR+'/pathperl');
          if (status == 0) then
            Chainecmd = 'perl'+' '+Chainecmd+'>'+TMPDIR+'/script';
            status = unix(Chainecmd);
            resultat=mgetl(TMPDIR+'\script');
          else
            error(gettext("Unable to find Perl."));
          end
        end
      end
      if (status~=0) then
        msgerr= gettext("System error: Command executed: ")+ Chainecmd;
        error(msgerr);
      end
    end  
  else
    error(gettext("First input argument must be a Perl File."));
  end
endfunction
