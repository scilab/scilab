function hilite_path(path,mess,with_intermediates)
//hilite a block included in a superblock hierarchy and displays a
//message.   If required all the intermediate superblocs are displayed
//first entry of the path must be a bloc of currently opened diagram  
  if argn(2)<3 then with_intermediates=%f,end
  if argn(2)<2 then mess=' ',end
  scs_m;
  mxwin=maxi(winsid()),opened_windows=[]
  hilite_obj(scs_m.objs(path(1)))
  
  if with_intermediates then
    for k=2:size(path,'*')
      scs_m=scs_m.objs(path(k)).model.rpar;
      scs_show(scs_m,mxwin+k);opened_windows=[mxwin+k opened_windows]
      hilite_obj(scs_m.objs(path(k)))
    end
  else
    if size(path,'*')==1 then
      hilite_obj(scs_m.objs(path))
    else
      for k=1:size(path,'*')-1;scs_m=scs_m.objs(path(k)).model.rpar;end
      scs_show(scs_m,mxwin+1);opened_windows=[mxwin+1 opened_windows]
      hilite_obj(scs_m.objs(path($)))
    end
  end
  message(mess)
  xdel(opened_windows)
  scs_m=null()
  unhilite_obj(scs_m.objs(path(1)))
endfunction
