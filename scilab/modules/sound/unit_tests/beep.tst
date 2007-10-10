old = beep();  
if beep('on') <> 'on' then pause,end;
if beep('off') <> 'off' then pause,end;
if beep() <> 'off' then pause,end;
beep(old);
