function xstringb(x,y,str,w,h,option)
//[]=xstringb(x,y,str,w,h,option)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=5, option='no';end
if rhs==0 then
  s_mat=['plot2d(0,0,[1,-1],''012'','' '',[0,0,1,1]);';
      'mat=[''Scilab'',''is'';''not'',''Basile''];';
      'xstringb(0,0.5,mat,1,0.5);xrect(0,1.0,1,0.5);';
      'xstringb(0,0,mat,1,0.5,''fill'');xrect(0,0.5,1,0.5);'];
  write(%io(2),s_mat);execstr(s_mat);
  return;
end;
//on concatene les lignes 
rect=xstringl(0,0,str)
if option=='fill'  then xx=xget('font');
  w_fl=%t;siz=0
  xset('font',xx(1),siz)
  while w_fl , rect=xstringl(0,0,str);
    w_fl=rect(3) < w & rect(4) < h & siz <= 6
    siz=siz+1
    xset('font',xx(1),siz)
  end
  xset('font',xx(1),siz-2)
  rect=xstringl(0,0,str)
end
//centrage 
x=x+abs(w-rect(3))/2;
y=y+abs(h-rect(4))/2;
xstring(x,y,str);
if option=='fill'  then xset('font',xx(1),xx(2));end
endfunction
