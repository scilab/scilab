// Please be sure that current scilab is associated with .sce & .sci extensions
//
if MSDOS then
  fullscilabname = WSCI+filesep()+'wscilex.exe';
  
  r = findfileassociation('.sce');
  if r <> fullscilabname then pause,end
  
  r = findfileassociation('.sci');
  if r <> fullscilabname then pause,end
  
  r = findfileassociation('.tst');
  if (r <> fullscilabname) & (r <> []) then pause,end
  
  r = findfileassociation('.dem');
  if (r <> fullscilabname) &  (r <> []) then pause,end
  
  if with_module('metanet') then
    r = findfileassociation('.graph');
    if (r <> fullscilabname) &  (r <> []) then pause,end
    
    r = findfileassociation('.graphb');
    if (r <> fullscilabname) &  (r <> []) then pause,end
  end
  
  if with_module('scicos') then
    r = findfileassociation('.cos');
    if (r <> fullscilabname) &  (r <> []) then pause,end
  
    r = findfileassociation('.cosf');
    if (r <> fullscilabname) &  (r <> []) then pause,end
  end
  
  r = findfileassociation('.sav');
  if (r <> fullscilabname) &  (r <> []) then pause,end
  
  r = findfileassociation('.bin');
  if (r <> fullscilabname) &  (r <> []) then pause,end
end  
