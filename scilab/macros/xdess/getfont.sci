function [fontId,fontSize]=getfont(S,v1)
//returns the clicked font id and font size. 
  


[lhs,rhs]=argn(0);

fontId=[];fontSize=[];

if exists('S','local')==0 then S="a";end
if type(S)<>10 then error('Argument must be a character string'),end
S=part(S(1),1)
wins=winsid()
if wins<>[] then
  curwin=xget('window')
  win=max(wins+1)
else
  win=0
end
xset('window',win);
fnts=xlfont();nf=size(find(fnts<>''),'*')-1
//Disabling standard menus
if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'Zoom')
  delmenu(win,'File')
else
  delmenu(win,'3D &Rot.')
  delmenu(win,'&UnZoom')
  delmenu(win,'&Zoom')
  delmenu(win,'&File')
end


x=2;
xset("font size",3);

tsave=xget("thickness");
fsave=xget("font");
curcol=xget('color');
red=addcolor([1 0 0])

W1=1 //first column width
H1=1 // first row height

H=1.5;W=1.5;
drawchart()



done=%f;
addmenu(win,'File',['Ok','Cancel']);
execstr('File_'+string(win)+..
	'=[''done=%t;k=[-fontId,fontSize];'';''done=%t;k=[]'']')

cmdok='execstr(File_'+string(win)+'(1))'
cmdcancel='execstr(File_'+string(win)+'(2))'
if exists('font','local') then
  fontId=max(0,min(nf,font(1)))
  fontSize=max(0,min(5,font(2)))
  xset('color',red), xset("font",fontId,fontSize);
  xstringb(W1+(fontSize)*W,H*fontId,S,W,H),
  xset("font",fsave(1),fsave(2));
  selected=%t;xset('color',curcol)
else
  selected=%f
end

while %t
  [c_i,cx,cy,cw,str]=xclick();
  if c_i==-2 then
    if str==cmdok then break,end
    if str==cmdcancel then break,end
  elseif c_i==-100 then str=cmdcancel ;
    break, 
  elseif c_i>=0&c_i<3 then
    if selected then 
      xset("font",fontId,fontSize);
      xstringb(W1+(fontSize)*W,H*fontId,S,W,H),
      xset("font",fsave(1),fsave(2));
    end
    fontId=max(0,min(nf,floor((cy)/H)));
    fontSize=max(0,min(5,floor((cx-W1)/W)));
    
    xset('color',red), xset("font",fontId,fontSize);
    xstringb(W1+(fontSize)*W,H*fontId,S,W,H),
    xset("font",fsave(1),fsave(2));
    selected=%t;xset('color',curcol)
    
    xinfo('You have chosen (fontId, fontSize) = ( '+..
	  string(fontId)+', '+string(fontSize)+')')
  else
    S=ascii(c_i)
    selected=%f
    drawchart()
  end
end
xdel(win)
if wins<>[] then xset('window',curwin),end
if str==cmdcancel then
  fontId=[];fontSize=[]
end
if lhs<2 then fontId=[fontId,fontSize],end
endfunction

function drawchart()
  xbasc()
  xsetech(frect=[0 0 W1+6*W H1+(nf+1)*H])
  xtitle(["Click to select font Id  and font size";
	 "or press a key to select a character"])
  // Drawing first column
  for k=0:nf
    xstringb(0,H*k,string(k),W1,H)
    xrect(0, H*(k+1), W1, H);
  end
  // Drawing first row
  for l=0:5
    xstringb(W1+l*W,(nf+1)*H ,string(l),W,H1)
    xrect(W1+l*W, H1+(nf+1)*H, W, H1);
  end
  xrect(0,H1+(nf+1)*H,W1,H1);
  xstringb(0, (nf+1)*H,"Id\Sz",H1,W1)
  xset("thickness",2);

  xset('color',xget('foreground'))
  
  for k=-(0:nf)
    for x=1:6
      xset("font",-k,x-1);
      xstringb(W1+(x-1)*W,-H*k,S,W,H)
    end
  end
  xset("font",fsave(1),fsave(2));
  xset("thickness",tsave);

endfunction
