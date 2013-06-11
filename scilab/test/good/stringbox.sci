//=======================================================================
// Copyrigth INRIA 2005
// File   : stringbox.sci
// Author : Jean-Baptiste Silvy
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
        // initialize values ;
        text     = "" ;
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
        xstring( posX, posY, text, angle ) ;
        textHandle = gce() ;
        textHandle.font_style = fontId ;
        textHandle.font_size = fontSize ;

        corners = StringBox( textHandle ) ;

        delete( textHandle ) ;
        set( "current_entity", oldHandle ) ;


    end;
endfunction
