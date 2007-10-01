//------------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//------------------------------------------------------------------------------
function [HomeDirectory,ScilabHomeDirectory]=sethomedirectory(varargin)
  nv=size(varargin);

  if (getos()~='Windows') then // Unix
  	HomeDirectory=getenv('HOME','ndef');
    if (HomeDirectory == 'ndef') then
      HomeDirectory=unix_g('cd; pwd');
    end
  else // Windows
    homedrive=getenv('HOMEDRIVE','ndef');
    homepath=getenv('HOMEPATH','ndef');
    if ( (homedrive == 'ndef') | (homedrive == 'ndef') ) then // Windows 9x
      HomeDirectory=pathconvert(SCI,%f,%f,'w');
    else
      HomeDirectory=homedrive+homepath; // Windows NT
      HomeDirectory=pathconvert(homedrive+homepath,%f,%f,'w');
    end
    setenv('HOME',HomeDirectory);
  end

  if nv==0 then // no Argument by default
    if MSDOS then
      ScilabHomeDirectory=HomeDirectory+'\Scilab\'+getversion();
    else
      ScilabHomeDirectory=HomeDirectory+'/.Scilab/'+getversion();
    end

    if (ExistScilabHomeDirectory(ScilabHomeDirectory)==%F) then
      ScilabHomeDirectory=CreateScilabHomeDir(HomeDirectory,ScilabHomeDirectory);
    end

  else

    if nv==1 then // to set Scilab home with a path
  		UserScilabHome=varargin(1);
  		if (type(UserScilabHome) == 10) then
   			if (fileinfo(UserScilabHome) == []) then
     			error(UserScilabHome+' directory doesn''t exist',999);
   			else
     		  ScilabHomeDirectory=UserScilabHome;
        end
      else
        error('Type incorrect. parameter must a string (path).',999);
      end
    else
      error('Error number of parameters incorrect.',999);
    end

  end

  setenv('SCIHOME',ScilabHomeDirectory);
endfunction
//------------------------------------------------------------------------------
function bOK=ExistScilabHomeDirectory(SciHome)
	if ( fileinfo(SciHome) == [] ) then
    bOK=%F;
  else
    bOK=%T;
  end
endfunction
//------------------------------------------------------------------------------
function ScilabHome=CreateScilabHomeDir(Home,SciHome)
  ScilabHome=SciHome
  if MSDOS then
    HomeScilabLvl1=HomeDirectory+'\Scilab';
    HomeScilabLvl2=HomeScilabLvl1+'\'+getversion();
  else
    HomeScilabLvl1=HomeDirectory+'/.Scilab';
    HomeScilabLvl2=HomeScilabLvl1+'/'+getversion();
  end

  nameScilab='';
  if MSDOS then
    nameScilab='Scilab'
  else
    nameScilab='.Scilab'
  end

  if ( fileinfo(HomeScilabLvl1) == [] ) then
    if ( mkdir(HomeDirectory,nameScilab)==0 ) then
      disp('No right to write in '+HomeDirectory);
      ScilabHome=SCI;
    else
      if ( fileinfo(HomeScilabLvl2) == [] ) then
        if ( mkdir(HomeScilabLvl1,getversion())==0 ) then
          disp('No right to write in '+HomeScilabLvl1);
          ScilabHome=HomeScilabLvl1;
        end
      end
    end
  else
    if ( fileinfo(HomeScilabLvl2) == [] ) then
      if ( mkdir(HomeScilabLvl1,getversion())==0 ) then
        disp('No right to write in '+HomeScilabLvl1);
        ScilabHome=SCI;
      end
    end
  end

endfunction
//------------------------------------------------------------------------------
