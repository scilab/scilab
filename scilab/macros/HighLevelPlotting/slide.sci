function slide(text,color)

   winNum=xget('window');
   state=loadGraphicState(winNum);
   globalState=loadGlobalGraphicState();  
   
   viewport = state('viewport');
   xsetech(viewport);
   plot2d(0,0,[-1,1],'010',' ',[0,0,1,1]);
//   fontname='-adobe-helvetica-medium-r-normal--%s-*-75-75-p-*-iso8859-1'
//   xlfont(fontname,1);
//   xset('font',1);
   xset('pattern',addcolor(color));
   xstringb(0,0.5,text,1,.5,'fill');   

   saveGraphicState(state,winNum); // save the state of the current window

