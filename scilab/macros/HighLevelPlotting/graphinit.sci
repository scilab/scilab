function graphinit(varargin)

global myWorld

defaultColormap=graycolormap(64);

//
// Initialisation of graphic state. Must be called once at startup
//

[lhs,rhs]=argn(0);

// Initialisation of graphic state

cmblack=[1 1 0; // We allocate a minimal colormap as a
  1 0.5 1;     // start for new figures
  0 1 1;
  1 .25 .25;
  .25 1 .25;
  .25 .25 1;
  .75 .75 .75;
  0 0 0;
  1 1 1;
  1 0 0;
  1 0 0;
  1 0 0;
  1 0 0;
  1 0 0;
  1 0 0;
  1 0 0;
  0 0 0;   // background color must me number 17 for correct printing in b/w mode
  1 1 1];  // foreground  ------------------- 18 --------------------------------
           // because of the b/w translation of xset('pattern',n). This is a must
           // if you want axes, titles and axes labels to be really black.

cmwhite=[ 0 0 1;
  0 0.5 0;
  1 0 0;
  0 0.75 0.75;
  .75 0 .75;
  .75 .75 0;
  .25 .25 .25;
  0 0 0;
  1 1 1;
  1 0 1;
  1 0 1;
  1 0 1;
  1 0 1;
  1 0 1;
  1 0 1;
  1 0 1;
  1 1 1; // same remark as blackbg mode.
  0 0 0];
  
if rhs==1 // default background is white
   bg=varargin(1);
else
   bg=[1 1 1];
end


if (type(bg)==1)
      if length(bg)==3
         clum = ([.298936021 .58704307445 .114020904255]*bg(:) >= .5) + 1;
	 if clum==1
	    cmap = cmblack;
	    simpletable=tlist(['simpletable';'y';'m';'c';'b';'g';'r';'w';'k'],1,2,3,6,5,4,9,8);
	  else
	    simpletable=tlist(['simpletable';'y';'m';'c';'b';'g';'r';'w';'k'],6,5,4,1,2,3,9,8);
	    cmap = cmwhite;
	  end
      else
        error('graphinit : background color specification must be a 3-vector')
      end
else
      error('graphinit : background color specification must be a 3-vector')
end 

clum = ([.298936021 .58704307445 .114020904255]*bg' >= .5) + 1;
if clum==1
     fg = [1 1 1];
     fbg = 0.7*bg + .3*fg;
else
     fg=[0 0 0];
 	fbg=0.7*bg;
end

cmap(17,:)=bg(:)';

globalGraphicState=tlist(['globalGraphicState';
'bg';
'fg';
'fbg';
'defaultViewport';
'defaultColormap';
'defaultRGBcolormap';
'simpleColorTable'],bg,fg,fbg,[0,0,1,1],cmap,defaultColormap,simpletable);

if length(myWorld)==0 // first call to graphinit()
   graphicState=list();
   myWorld=tlist(['myWorld';'globalGraphicState';'graphicState'],...
   globalGraphicState,graphicState);
else
   myWorld('globalGraphicState')=globalGraphicState;
end

// end of graphinit



