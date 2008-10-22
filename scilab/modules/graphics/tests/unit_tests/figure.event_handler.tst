//
//  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
//
//  This file must be used under the terms of the CeCILL.
//  This source file is licensed as described in the file COPYING, which
//  you should have received as part of this distribution.  The terms
//  are also available at
//  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- TEST WITH GRAPHIC -->

// Create a default figure
my_figure = scf();

// check default values
if (my_figure.event_handler <> "") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// setEventhandler("mypersonnaleventhanlder") test
seteventhandler("mypersonnaleventhanlder");
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "on") then pause; end

// setEventhandler("") test
seteventhandler("");
if (my_figure.event_handler <> "") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// my_figure.event_handler = "mypersonnaleventhanlder"
my_figure.event_handler = "mypersonnaleventhanlder";
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// my_figure.event_handler_enable = "on"
my_figure.event_handler_enable = "on";
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "on") then pause; end

// my_figure.event_handler = ""
// Erasing event_handler make enable flag go 'off'
 my_figure.event_handler = "";
if (my_figure.event_handler <> "") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// my_figure.event_handler_enable = "on";
// Forcing event handler enable without event handler raise a warning
if (my_figure.event_handler <> "") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// my_figure.event_handler = "mypersonnaleventhanlder"
// then enable
// then disable
my_figure.event_handler = "mypersonnaleventhanlder";
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end
my_figure.event_handler_enable = "on";
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "on") then pause; end
my_figure.event_handler_enable = "off";
if (my_figure.event_handler <> "mypersonnaleventhanlder") then pause; end
if (my_figure.event_handler_enable <> "off") then pause; end

// Delete
delete(my_figure);
