ierr=execstr("set_xproperty(1)","errcatch"); 
if ierr <> 999 then pause,end
ierr=execstr("set_xproperty(-1)","errcatch"); 
if ierr <> 999 then pause,end