// Allan CORNET
// INRIA 2005
//------------------------------------------------------------------------------
function homeret=sethomedirectory(varargin)
nv=size(varargin);
if nv==0 then // no Argument by default
  if (getos()~='Windows') then // Unix
    HomeDirectory=getenv('HOME','ndef');
    if (HomeDirectory == 'ndef') then
      HomeDirectory=unix_g('cd; pwd');
    end
    hometmp=HomeDirectory+'/Scilab/'+getversion();
  else // Windows
    homedrive=getenv('HOMEDRIVE','ndef');
    homepath=getenv('HOMEPATH','ndef');
    if ( (homedrive == 'ndef') | (homedrive == 'ndef') ) then // Windows 9x
      HomeDirectory=SCI;
      hometmp=HomeDirectory+'\Scilab\'+getversion();
      hometmp=pathconvert(hometmp,%f,%f,'w');
    else
      HomeDirectory=homedrive+homepath; // Windows NT
      hometmp=HomeDirectory+'\Scilab\'+getversion();
      hometmp=pathconvert(hometmp,%f,%f,'w');
    end
  end
  
  if ( ExistHomeScilabDirectory(HomeDirectory) == %F ) then
    if (CreateHomeScilabDirectory(HomeDirectory) == %F) then
      disp('No right to write in '+hometmp);
      homeret=SCI;
    else
      homeret= hometmp
    end
  else
    homeret= hometmp
  end
else
 if nv==1 then // to set home with a path
  UserHome=varargin(1);
  if (type(UserHome) == 10) then
   if (fileinfo(UserHome) == []) then
     error(UserHome+' directory doesn''t exist',999);
   else
     homeret=UserHome;
   end
  else
    error('Type incorrect. parameter must a string (path).',999);
  end
 else
   error('Error number of parameters incorrect.',999);
 end
end
  setenv('HOME',homeret); 
endfunction
//------------------------------------------------------------------------------
function bOK=CreateHomeScilabDirectory(HomeDirectory)
bOK=%F;
CurrentDirectory=getcwd();
chdir(HomeDirectory);
if ( mkdir('Scilab') == 1) then
  chdir('Scilab')
  if (mkdir(getversion()) == 1) then
    bOK=%T;
  else
    bOK=%F;
  end
else
  bOK=%F;
end
chdir(CurrentDirectory);
endfunction
//------------------------------------------------------------------------------
function bOK=ExistHomeScilabDirectory(HomeDirectory)
bOK=%F;
if MSDOS then
  HomeScilab=HomeDirectory+'\Scilab\'+getversion();
else
  HomeScilab=HomeDirectory+'/Scilab/'+getversion();
end
if ( fileinfo(HomeScilab) == [] ) then
  bOK=%F;
else
  bOK=%T;
end
endfunction
//------------------------------------------------------------------------------
