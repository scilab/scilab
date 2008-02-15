
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_do_settings(GraphList)
// Dialog for default parameters selection

//nodes parameters current values
  diam         = GraphList.nodes.graphics.defaults.diam;
  border       = GraphList.nodes.graphics.defaults.border;
  colors       = GraphList.nodes.graphics.defaults.colors ;
  nodefont     = GraphList.nodes.graphics.defaults.font;

  //edges parameters current values
  width        = GraphList.edges.graphics.defaults.width;
  foreground   = GraphList.edges.graphics.defaults.foreground;
  edgefont     = GraphList.edges.graphics.defaults.font;
  profile_index= GraphList.edges.graphics.defaults.profile_index;
  //number of available profiles (profile 1 is for loops)
  np=size(GraphList.edges.graphics.profiles); 
  
  //Graph parameters  current values
  if GraphList.directed==0 then directed='no',else directed='yes',end
  
  nc=size(get(gcf(),'color_map'),1) //le nombre de couleurs de la fenetre d'edition
  
  //Build the dialog parameters
  labels=['Node diameter';
	  'Border node width';
	  'Node colors index [foreground, background]'
	  'Node label font [size,style,foreground]'
	  'Edge width',
	  'Edge color index '
	  'Edge label font [size,style,foreground]'
	  'Edge profile index min=2,max='+string(np)
	  'Is graph directed (yes/no)']
  tit=['Set default parameters'
       'Color indices should be in [-2 -1 1:'+string(nc)+']'
       'Font styles should be in [1 9]'
       'Font sizes should be in [1 9]']
  typ=list('vec',1,'vec',1,'vec',2,'vec',3, ..
	   'vec',1,'vec',1,'vec',3,'vec',1,'str',-1)
  
  while %t 
    ini=[sci2exp(diam),sci2exp(border),sci2exp(colors),sci2exp(nodefont),..
	 sci2exp(width),sci2exp(foreground),sci2exp(edgefont),sci2exp(profile_index),..
	 directed]
    [ok,diam,border,colors,nodefont,..
     width,foreground,edgefont,profile_index,directed]=getvalue(tit,labels,typ,ini)
    if ~ok then return,end //user cancels
    
    //Validate the answers
    mess=[]
    if diam <=0 then 
      mess=[mess;'Default Node diameter must be positive']
    end
    if border <=0 then 
      mess=[mess;'Default Node border must be positive']
    end
    if width <=0 then 
      mess=[mess;'Default edge width must be positive']
    end
    if nodefont(1) <=0 then 
      mess=[mess;'Default node font size must be positive']
    end
    if nodefont(2) <=0 |nodefont(2)>9 then 
      mess=[mess;'Default font style must be in [1 9]']
    end
    if and(nodefont(3)<>[-2 -1 1:nc]) then
      mess=[mess;'Default node  font style must be in [-2 -1 1:'+string(nc)+']']
    end
    if edgefont(1) <=0 then 
      mess=[mess;'Default edge font size must be positive']
    end
    if edgefont(2) <=0 |nodefont(2)>9 then 
      mess=[mess;'Default edge style must be in [1 9]']
    end
    if and(edgefont(3)<>[-2 -1 1:nc]) then
      mess=[mess;'Default edge  font style must be in [-2 -1 1:'+string(nc)+']']
    end
    
    directed=convstr(stripblanks(directed))
    if and(directed<>['yes','no']) then
      mess=[mess;'The answer for ""Is graph directed"" must be ""yes"" or ""no""']
    end
    if mess<>[] then
      x_message(mess)
    else
      break
    end
  end

  drawlater()
  ge_axes_handle=gca();
  //Check if complete redraw is necessary
  kcn=[];kce=[]
  if GraphList.nodes.graphics.defaults.diam <>diam then
    kcn=find(GraphList.nodes.graphics.diam==0);//nodes with defaut diameter
    GraphList.nodes.graphics.defaults.diam          = diam;
  end
  if GraphList.edges.graphics.defaults.profile_index <>profile_index then
    kce=find(GraphList.edges.graphics.profile_index==0);//nodes with defaut diameter
    GraphList.edges.graphics.defaults.profile_index          = profile_index;
  end
  if kcn<>[]|kce<>[] then //  node diameter or profile change,
    
    //change all parameters and redraw
    //Set nodes parameters
    GraphList.nodes.graphics.defaults.diam          = diam;
    GraphList.nodes.graphics.defaults.border        = border;
    GraphList.nodes.graphics.defaults.colors        = colors;
    GraphList.nodes.graphics.defaults.font          = nodefont;

    //Set edges parameters
    GraphList.edges.graphics.defaults.width         = width;
    GraphList.edges.graphics.defaults.foreground    = foreground;
    GraphList.edges.graphics.defaults.font          = edgefont;
    GraphList.edges.graphics.defaults.profile_index = profile_index;
    
    //Set graph parameters
    GraphList.directed=find(directed==['no','yes'])-1

    set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))
    ge_do_replot(GraphList)
  else
    
    gindex=ge_axes_handle.user_data;
    hnodes=gindex.node
    
    if GraphList.nodes.graphics.defaults.border <>border then
      for k=find(GraphList.nodes.graphics.border==0);
	e=hnodes(k).children(2)
	ge_update_gprops(e,'thickness',border)
      end
    end
    GraphList.nodes.graphics.defaults.border        = border;
    
    if GraphList.nodes.graphics.defaults.colors(1)<> colors(1) then 
      for k=find(GraphList.nodes.graphics.colors(1,:)==0)
	e=hnodes(k).children(2)
	ge_update_gprops(e,'foreground',colors(1))
      end
    end
    if GraphList.nodes.graphics.defaults.colors(2)<> colors(2) then  
      for k=find(GraphList.nodes.graphics.colors(2,:)==0)
	e=hnodes(k).children(2)
	ge_update_gprops(e,'background',colors(2))
      end
    end
    GraphList.nodes.graphics.defaults.colors        = colors;
    
    if GraphList.nodes.graphics.defaults.font(1)<>nodefont(1) then 
      for k=find(GraphList.nodes.graphics.font(1,:)==0)
	e=hnodes(k).children(1)
	ge_update_gprops(e,'font_size',nodefont(1))
      end
    end
    if GraphList.nodes.graphics.defaults.font(2)<>nodefont(2) then 
      for k=find(GraphList.nodes.graphics.font(2,:)==0)
	e=hnodes(k).children(1)
	ge_update_gprops(e,'font_style',nodefont(2))
      end
    end
    if GraphList.nodes.graphics.defaults.font(3,:)<>nodefont(3)  then 
      for k=find(GraphList.nodes.graphics.font(3)==0)
	e=hnodes(k).children(1)
	ge_update_gprops(e,'font_foreground',nodefont(3))
      end
    end
    GraphList.nodes.graphics.defaults.font        = nodefont;
    //Set edges parameters
    hedges=gindex.edge
    if GraphList.edges.graphics.defaults.width<>width then 
      for k=find(GraphList.edges.graphics.width==0)
	e=hedges(k).children(2:3)
	ge_update_gprops(e,'thickness',width)
      end
      GraphList.edges.graphics.defaults.width         = width;
    end
    
    if GraphList.edges.graphics.defaults.foreground<>foreground then 
      for k=find(GraphList.edges.graphics.foreground==0)
	e=hedges(k).children(2:3)
	ge_update_gprops(e,'foreground',foreground)
      end
      GraphList.edges.graphics.defaults.foreground    = foreground;
    end
    
    if GraphList.edges.graphics.defaults.font(1)<>edgefont(1) then 
      for k=find(GraphList.edges.graphics.font(1,:)==0)
	e=hedges(k).children(1)
	ge_update_gprops(e,'font_size',edgefont(1))
      end
    end
    if GraphList.edges.graphics.defaults.font(2)<>edgefont(2) then 
      for k=find(GraphList.edges.graphics.font(2,:)==0)
	e=hedges(k).children(1)
	ge_update_gprops(e,'font_style',edgefont(2))
      end
    end
    if GraphList.edges.graphics.defaults.font(3)<>edgefont(3)  then 
      for k=find(GraphList.edges.graphics.font(3,:)==0)
	e=hedges(k).children(1)
	ge_update_gprops(e,'font_foreground',edgefont(3))
      end
    end
    GraphList.edges.graphics.defaults.font        = edgefont;

    if GraphList.edges.graphics.defaults.profile_index<>profile_index then 
      for k=find(GraphList.edges.graphics.profile_index==0)
	e=hedges(k).children(2:3)
	ge_update_gprops(e,'font_style',profile_index)
      end
      GraphList.edges.graphics.defaults.profile_index         = profile_index;
    end
    directed=find(directed==['no','yes'])-1
    vis='off',if directed==1 then vis='on',end
    if GraphList.directed<>directed then 
      for k=1:size(hedges,'*')
	e=hedges(k).children(2)
	ge_update_gprops(e,'visible',vis)
      end
      GraphList.directed=directed
    end
  end
  drawnow(),show_pixmap()
  
endfunction
