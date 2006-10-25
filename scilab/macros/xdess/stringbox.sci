//=======================================================================
// Copyrigth INRIA 2005
// File   : stringbox.sci
// Author : Jean-Baptiste Silvy
// Desc   : Compute the 4 corners of a displayed string bounding rect
//======================================================================

function [corners] = stringbox( varargin )

// the call is stringBox(handle) or stringBox(text,posX,posY,[angle,[fontId,[fontSize]]]) ;

[lhs,rhs] = argn(0) ;

// initialize values ;
text     = '' ;
angle    = 0  ;
posX     = 0  ;
posY     = 0  ;
font     = xget('font') ;
fontId   = font( 1 )    ;
fontSize = font( 2 )    ;

curFig     = gcf() ;
curAxes    = gca() ;
curFigAxes = gca() ; // selected axes of the parent figure

ListArg = varargin ;

select rhs,
  
case 1 then
  textHandle = ListArg(1) ;
  
  // works only on text handles
  if type(textHandle) ~= 9 then
    error("If only a single argument is specified, it must be a handle.");
    return ;
  end
  
  
  // works with text handle and label handle (axes label and titles)
  if (textHandle.type == "Text" ) then
    // get the properties
    text     = textHandle.text       ;
    posX     = textHandle.data( 1 )  ;
    posY     = textHandle.data( 2 )  ;
    angle    = textHandle.font_angle ;
    fontId   = textHandle.font_style ;
    fontSize = textHandle.font_size  ;
  elseif (textHandle.type == "Label" ) then
    // get the properties
    // they use jpc syntax which must be converted to normal syntax
    text     = arobasestring2strings( textHandle.text ) ;
    posX     = textHandle.position( 1 ) ;
    posY     = textHandle.position( 2 ) ;
    angle    = textHandle.font_angle    ;
    fontId   = textHandle.font_style    ;
    fontSize = textHandle.font_size  ;
  else
    error("Handle should be a Text or Label handle.") ;
    return ;
  end
  
  // set parent subwindow as current
  parentSubWin = textHandle.parent ;
  while parentSubWin.type ~= "Axes",
    parentSubWin = parentSubWin.parent ;
  end
  
  set( "current_figure", parentSubWin.parent ) ; // we must set the axes and figure
  curFigAxes = gca() ; // may have changed
  set( "current_axes"  , parentSubWin        ) ;
  
  
  
case 3 then
  text = ListArg( 1 ) ;
  posX = ListArg( 2 ) ;
  posY = ListArg( 3 ) ;
    
case 4 then
  text  = ListArg( 1 ) ;
  posX  = ListArg( 2 ) ;
  posY  = ListArg( 3 ) ;
  angle = ListArg( 4 ) ;
  
case 5 then
  text   = ListArg( 1 ) ;
  posX   = ListArg( 2 ) ;
  posY   = ListArg( 3 ) ;
  angle  = ListArg( 4 ) ;
  fontId = ListArg( 5 ) ;
  
case 6 then
  text     = ListArg( 1 ) ;
  posX     = ListArg( 2 ) ;
  posY     = ListArg( 3 ) ;
  angle    = ListArg( 4 ) ;
  fontId   = ListArg( 5 ) ; 
  fontSize = ListArg( 6 ) ;
  
else
  error(39);
  return ;
end

// check the type of inputs
if type( text ) ~= 10 | type( posX ) ~= 1 | type( posY ) ~= 1 | type( angle ) ~= 1 | type( fontId ) ~= 1 | type( fontSize ) ~= 1 then
  error(246) ;
  return ;
end

corners = StringBox( text, posX, posY, angle, fontId, fontSize ) ;

set( "current_axes"  , curFigAxes ) ;
set( "current_figure", curFig     ) ;
set( "current_axes"  , curAxes    ) ;

endfunction
