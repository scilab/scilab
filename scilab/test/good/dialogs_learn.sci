//redefine some macros
function [c_i, c_x, c_y, c_w, str]=xclick()
  lhs = argn(1);
  if lhs == 3 then
    [c_i, c_x, c_y] = xclick1();
    c_x=round(10*c_x)/10;c_y=round(10*c_y)/10;
    txt=strcat(string([c_i,c_x,c_y]), ',')
    C=xclick_comment(c_i, c_x, c_y);
  elseif lhs == 4 then
    [c_i, c_x, c_y, c_w] = xclick1();
    c_x=round(10*c_x)/10;c_y=round(10*c_y)/10;
    txt=strcat(string([c_i,c_x,c_y,c_w]), ',')
    C=xclick_comment(c_i, c_x, c_y, c_w)
  else
    [c_i, c_x, c_y, c_w, str] = xclick1();
    if isnan(c_x)|isnan(c_y) then c_x=0,c_y=0,end
    c_x=round(10*c_x)/10;c_y=round(10*c_y)/10;
    txt = strcat([string([c_i,c_x,c_y,c_w]),sci2exp(str)], ',');
    C=xclick_comment(c_i, c_x, c_y, c_w, str)
  end
  mputl( txt + C,uapp);
endfunction



function rep=xgetmouse()
  global MousePos
  rep = xgetmouse1();rep(1:2)=round(10*rep(1:2))/10;
  if rep(3)<0 then
    if or(abs(MousePos(1:2)-rep(1:2))>0) then
      mputl( strcat(string(rep), ',') + comm + 'xgemouse',uapp);
      MousePos=rep
    end
  else
    mputl( strcat(string(rep), ',') + comm + 'xgemouse',uapp);
  end
endfunction

function p=xgetfile(file_mask,dir,title)
  if exists('title','local')==0 then title='',end
  if exists('dir','local')==0 then dir='',end
  if exists('file_mask','local')==0 then file_mask='*',end
  p=xgetfile1(file_mask,dir,title)
  txt=[comm + 'xgetfile: '+title(1);
       p]
  mputl(txt,uapp);
endfunction

function result=x_mdialog(title, labels, default_inputs_vector)
  result = x_mdialog1(title, labels, default_inputs_vector);
  if result ~= [] then
    res=[comm + 'x_mdialog: '+title(1);
	 '  '+result
	 '  '+'o //OK']
  else
    res=[comm + 'x_mdialog: '+title(1);
	'  '+default_inputs_vector(:)
	 '  '+'c //Cancel']

  end,
  mputl(res,uapp);
endfunction


function num=tk_choose(items, title, button)
  if argn(2) == 3 then
    num = tk_choose1(items, title, button);
  else
    num = tk_choose1(items, title);
  end,
  if num<>0 then
    txt=[comm + 'tk_choose:  '+ title(1)
	 '  '+string(num) + '//'+items(num)]
  else
    txt=[comm + 'tk_choose:  '+ title(1)
	 '  '+string(num) + '// Cancel']
  end
  mputl(txt,uapp);
endfunction

function c=getcolor(title, cini)
  colors = string(1:xget('lastpattern'));
  m = prod(size(cini));
  ll = list();
  m = prod(size(cini));
  for k = 1:m,
    ll(k)=list('colors', cini(k), colors);
  end
  c = x_choices(title, ll);
  txt=[comm + 'getcolor: '+title(1)
       '  '+string(c)]
  mputl(txt,uapp);
endfunction

function result=x_dialog(labels, valueini)
  result = x_dialog1(labels, valueini);
  res = [comm + 'x_dialog:' + labels(1)
	 '  '+result
	 '  '+'o //OK']
  mputl( res,uapp);
endfunction

function result=dialog(varargin)
  result = x_dialog(varargin(:))
endfunction

function num=x_message(strings, buttons)
  if argn(2) == 2 then
    num = x_message1(strings, buttons);
    mputl( buttons(num) + comm + 'message',uapp);
  else
    num = 1;
    x_message1(strings);
  end,
endfunction

function num=message(varargin)
  num=x_message(varargin(:))
endfunction

function C=xclick_comment(c_i, c_x, c_y, c_w, str)
  rhs=argn(2)
  if rhs==5 then
    if part(str,1:7)=='execstr' then
      in=evstr(part(str,8:length(str)))
      k=strindex(in,['(',')'])
      M=evstr(part(in,k(1)+1:k(2)-1))
      S=menus(M)(evstr(part(in,k(3)+1:k(4)-1)))
      C=comm+' '+M+'/'+S
      return
    end
  end
  
  N=''
  if rhs>=4 then
    kc=find(c_w==windows(:,2))
    if windows(kc,1)<0 then //click dans une palette
      kpal=-windows(kc,1)
      scs_m=palettes(kpal)
      N=' Palette: '+scs_m.props.title(1)+','
    end
  end
  [k,wh]=getobj(scs_m,[c_x,c_y])
  if k<>[] then
    o=scs_m.objs(k);t=typeof(o)
    if t=='Link' then
      C= ' Link: '+string(k)
    elseif t=='Block' 
      C= N+' Block: '+string(k)+ ' '+o.gui
    else
      C= N+' Text: '+string(k)
    end
  else
    C=' xclick'
  end
  C= comm + C
endfunction

