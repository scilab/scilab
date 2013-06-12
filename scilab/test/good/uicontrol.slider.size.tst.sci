// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function change_value(slider_num)
    value = get(findobj("tag","slider_"+string(slider_num)),"value");
    set(findobj("tag","value_"+string(slider_num)),"string",string(value));
endfunction

function create_slider(witdh,height)

    global num_slider;

    global slider_min;
    global slider_max;
    global slider_step;
    global slider_ini_value;

    num_slider = num_slider + 1;

    text_value = msprintf("width = %+5d - height = %+5d",witdh,height);

    text_w     = 250;
    text_h     = 20;
    text_x     = padding_x;
    text_y     = axes_h - 50 - num_slider*(60+padding_y);

    slider_w   = witdh;
    slider_h   = height;
    slider_x   = 2*padding_x + text_w;
    slider_y   = text_y;

    value_w    = 100;
    value_h    = text_h;
    value_x    = 3*padding_x + text_w + slider_w;
    value_y    = text_y;

    uicontrol(my_figure_handle                                            , ...
    "position"          , [text_x text_y text_w text_h]               , ...
    "Style"             , "text"                                      , ...
    "String"            , text_value                                  , ...
    "FontName"          , "monospaced"                                , ...
    "BackgroundColor"   , [1 1 1]                                     );


    uicontrol(my_figure_handle                                       , ...
    "position"          , [slider_x slider_y slider_w slider_h]  , ...
    "Style"             , "slider"                               , ...
    "Value"             , slider_ini_value                       , ...
    "SliderStep"        , slider_step                            , ...
    "Min"               , slider_min                             , ...
    "Max"               , slider_max                             , ...
    "callback"          , "change_value("+string(num_slider)+")" , ...
    "tag"               , "slider_"+string(num_slider)           );


    uicontrol(my_figure_handle                                       , ...
    "position"          , [value_x value_y value_w value_h]      , ...
    "Style"             , "text"                                 , ...
    "String"            , string(slider_ini_value)               , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "value_"+string(num_slider)            );

endfunction
