function wpar=do_window(wpar)
// Copyright INRIA
  wd=wpar.wpar;w=wd(1);h=wd(2);

  while %t do
    [ok,h,w]=getvalue('Set parameters',[
	'Window height';
	'Window width'],list('vec',1,'vec',1),string([h;w]))
    if ~ok then break,end
    if or([h,w]<=0) then
      message('Parameters must be positive')
    else
      drawtitle(wpar)
      wpar.wpar(1)=w
      wpar.wpar(2)=h
      break
    end
  end
endfunction
