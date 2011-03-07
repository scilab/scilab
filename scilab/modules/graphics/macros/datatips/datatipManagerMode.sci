// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipManagerMode(varargin)
//Toggle or sets the interactive mode for datatips edition.;
//  This function is called by the datatips menu callbacks.;
  global datatipAngles; //to be able to detect that a rotation occurred
  old=[] //to preserve cuurent figure
  if winsid()<>[] then  old=gcf();end
  select size(varargin)
  case 0 then //toggle mode
    fig=gcf()
    action='toggle'
  case 1 then
    if type(varargin(1))==10 then
      action=varargin(1)
      fig=gcf()
    elseif type(varargin(1))==9 then
      fig=varargin(1)
      action='toggle'
    elseif type(varargin(1))==1 then
      fig=scf(varargin(1))
      if old<>[] then scf(old),end
      action='toggle'
    end
  case 2 then
    if type(varargin(1))==10 then
      action=varargin(1)
      fig=varargin(2)
    elseif type(varargin(1))==9 then
      fig=varargin(1)
      action=varargin(2)
    elseif type(varargin(1))==1 then
      fig=scf(varargin(1))
      if old<>[] then scf(old),end
      action=varargin(2)
    end
  else
    error(msprintf(_("%s: too many input arguments"),'datatipManagerMode'))
  end
  select action
  case 'on'
    fig.event_handler_enable = "off" //to prevent against bug 7855
    fig.event_handler = "datatipEventhandler"
    fig.event_handler_enable = "on"
    xinfo(_("Left click on a curve to create a datatip, right opens contextual menu"))
    show_window(fig)
  case 'off'
    fig.event_handler_enable = "off"
    clearglobal datatipAngles
    xinfo("")
  case 'toggle'
    if  fig.event_handler_enable == "on" then
      fig.event_handler_enable = "off"
      if fig.event_handler <> "datatipEventhandler" then
        fig.event_handler = "datatipEventhandler"
        fig.event_handler_enable = "on"
        xinfo(_("Left click on a curve to create a datatip, right opens"+...
                " contextual menu"))
      else
        clearglobal datatipAngles
        xinfo("")
      end
      
    else
      fig.event_handler_enable = "off" //to prevent against bug 7855
      fig.event_handler = "datatipEventhandler"
      fig.event_handler_enable = "on"
      xinfo(_("Left click on a curve to create a datatip, right opens contextual menu"))
      show_window(fig)
    end
  end
endfunction
