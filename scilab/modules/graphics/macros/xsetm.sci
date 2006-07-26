function []=xsetm() 
// Changing graphic context with x_choices 
// Copyright INRIA
if driver()<>"Rec" & driver()<>"X11" then 
	write(%io(2),"xsetm works with X11 or Rec driver only")
	return;
end

AlusIds=["0","src & dst","src & ~dst","src","~src & dst","dst",...
	"src XOR dst","src OR dst","~src & ~dst","~src XOR dst",...
	"~dst","src OR ~dst","~src","~src OR dst",...
	"~src OR ~dst","1"];

num_cols=xget("lastpattern");
colors=string(1:num_cols+2);

patterns=['black','pat 1','pat 2','pat 3','pat 4','pat 5','pat 6','pat 7',...
	'pat 8','pat 9','pat 10','pat 11','pat 12','pat 13','pat 14','pat 15','white'];

fontsSiz=['08' ,'10','12','14','18','24'];

fontsIds=[ 'Courrier',  'Symbol',  'Times ',   'Times Italic',  'Times Bold',  'Times B. It.'];

marksIds=['.','+','x','*','diamond fill.','diamond','triangle up','triangle down','trefle','circle'];

DashesIds=['Solid','-2-  -2-','-5-  -5-','-5-  -2-','-8-  -2-','-11- -2-','-11- -5-'];

ff=xget('font');
mm=xget('mark');
col=xget('use color');
alu=xget('alufunction');
dsh=xget('dashes');
pat=xget('pattern');
th=xget('thickness');
pix_on=xget('pixmap');


lfid=list('fontId',ff(1)+1,fontsIds);
lfiz=list('fontsize',ff(2)+1,fontsSiz);
lmid=list('markId',mm(1)+1,marksIds);
lmsiz=list('marksize',mm(2)+1,fontsSiz);
lth=list('Thickness',th+1,string(1:20));
lpix=list('pixmap/flag',pix_on+1,['Off','On']);
lcol=list('use color',col+1,['No','Yes']);
ldash=list('dashes',dsh(1)+1,DashesIds); 
lpat=list('patterns',pat,patterns);
lcols=list('colors',pat,colors);
lalu=list('alufunction',alu+1,AlusIds);

winid=xget("window");

if col==1 then 
  // On est en couleur 
  xch_l = list(lfid,lfiz,lmid,lmsiz,lth,lpix,lcol,lcols,lalu);
  entval=[lfid(2),lfiz(2),lmid(2),lmsiz(2),lth(2),lpix(2),lcol(2),lcols(2),lalu(2)];
  rep=x_choices('Graphic context of graphic window '+string(winid),xch_l);
  if rep<>[] then
    if or(rep<>entval) then 
      rep=rep-ones(rep);
      if rep(7)==0 then 
	// On passe de couleur a n&b le changement de use color
	// doir remettre pattern et dash a z'ero
	xset("use color",rep(7));
      else 
	// On reste en couleur ( attention au + 1) 
	xset("pattern",rep(8)+1);
      end
      xset("alufunction",rep(9));
      xset("font",rep(1),rep(2));
      xset("mark",rep(3),rep(4));
      xset("thickness",rep(5));
      xset("pixmap",rep(6));
    end
  end
else 
  // On est en n&b 
  xch_l = list(lfid,lfiz,lmid,lmsiz,lth,lpix,lcol,ldash,lpat,lalu);
  entval=[lfid(2),lfiz(2),lmid(2),lmsiz(2),lth(2),lpix(2),lcol(2),ldash(2),lpat(2),lalu(2)];
  rep=x_choices('Graphic context of graphic window '+string(winid),xch_l);
  if rep<>[] then
    if or(rep<>entval) then 
      rep=rep-ones(rep);
      if rep(7)==1 then 
	// On passe de n&b a couleur le changement de use color
	// doir remettre pattern et dash a z'ero
	xset("use color",rep(7));
      else 
	// On reste en n& b 
	xset("pattern",rep(9)+1);
	xset("dashes",rep(8));
      end
      xset("alufunction",rep(10));
      xset("font",rep(1),rep(2));
      xset("mark",rep(3),rep(4));
      xset("thickness",rep(5));
      xset("pixmap",rep(6));
    end
  end
end
endfunction
