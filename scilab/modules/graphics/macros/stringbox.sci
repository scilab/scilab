// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//=======================================================================
// File   : stringbox.sci
// Desc   : Compute the 4 corners of a displayed string bounding rect
//======================================================================

function [corners] = stringbox( varargin )

// the call is stringBox(handle) or stringBox(text,posX,posY,[angle,[fontId,[fontSize]]]) ;

[lhs,rhs] = argn(0) ;
ListArg = varargin ;

if ( rhs == 1 ) then
  textHandle = ListArg(1) ;
  corners = StringBox( textHandle ) ;
else
  
  
  oldHandle = gce() ;
  axes = gca()  ;
  fig = gcf();
  // initialize values ;
  text     = '' ;
  angle    = 0  ;
  posX     = 0  ;
  posY     = 0  ;
  fontId   = axes.font_style ;
  fontSize = axes.font_size  ;

  select rhs,
    
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
    
  // create an object get is bb and then destroy it
  immediateDrawingMode = fig.immediate_drawing;
  f.immediate_drawing = "off";
  xstring( posX, posY, text, angle ) ;
  textHandle = gce() ;
  textHandle.visible = "off"; // to prevent it from appearing
  textHandle.font_style = fontId ;
  textHandle.font_size = fontSize ;

  corners = StringBox( textHandle ) ;
  
  delete( textHandle ) ;
  set( 'current_entity', oldHandle ) ;
  f.immediate_drawing = immediateDrawingMode;

end;

endfunction
