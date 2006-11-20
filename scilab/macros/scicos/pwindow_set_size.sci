function pwindow_set_size()
  rect=dig_bound(scs_m);
  
  if rect<>[] then 
    %wsiz=[rect(3)-rect(1),rect(4)-rect(2)];
  else
    %wsiz=[600/%zoom,400/%zoom]
  end
  %wdd=min(1200,%zoom*%wsiz(1))+30;
  %hdd=min(1000,%zoom*%wsiz(2))+30
  if ~MSDOS then
    %hdd=%hdd+50
  end
  xset('wpdim',max(400,min(1200,%wdd)),max(300,min(1000,%hdd)))
  //xset('wpdim',min(1200,%wdd),min(1000,%hdd))
endfunction
