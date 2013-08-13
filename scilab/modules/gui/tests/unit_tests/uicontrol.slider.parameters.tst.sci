// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function change_value(slider_num)
    value = get(findobj("tag","slider_"+string(slider_num)),"value");
    set(findobj("tag","value_"+string(slider_num)),"string",string(value));
endfunction


function create_slider(sld_min,sld_max,sld_sliderstep,sld_ini_value)

    global axes_w;
    global axes_h;

    global padding_x;
    global padding_y;

    global text_w;
    global text_h;
    global text_x;

    global slider_w;
    global slider_h;
    global slider_x;

    global value_w;
    global value_h;
    global value_x;

    global num_slider;

    num_slider = num_slider + 1;

    text_value = msprintf("Min = %+5d - Max = %+5d - SliderStep = [%+5d - %+5d]",sld_min,sld_max,sld_sliderstep(1),sld_sliderstep(2));

    text_y   = axes_h - 100 - text_h - num_slider*(text_h+padding_y);
    slider_y = text_y;
    value_y  = text_y;

    uicontrol(my_figure_handle                                            , ...
    "position"          , [text_x text_y text_w text_h]               , ...
    "Style"             , "text"                                      , ...
    "String"            , text_value                                  , ...
    "FontName"          , "monospaced"                                , ...
    "BackgroundColor"   , [1 1 1]                                     );


    uicontrol(my_figure_handle                                       , ...
    "position"          , [slider_x slider_y slider_w slider_h]  , ...
    "Style"             , "slider"                               , ...
    "Value"             , sld_ini_value                          , ...
    "SliderStep"        , sld_sliderstep                         , ...
    "Min"               , sld_min                                , ...
    "Max"               , sld_max                                , ...
    "callback"          , "change_value("+string(num_slider)+")" , ...
    "tag"               , "slider_"+string(num_slider)           );


    uicontrol(my_figure_handle                                       , ...
    "position"          , [value_x value_y value_w value_h]      , ...
    "Style"             , "text"                                 , ...
    "String"            , string(sld_ini_value)                  , ...
    "BackgroundColor"   , [1 1 1]                                , ...
    "tag"               , "value_"+string(num_slider)            );

endfunction
