//  Scipad - programmer's editor and debugger for Scilab
//
//  Copyright (C) 2002 -      INRIA, Matthieu Philippe
//  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
//  Copyright (C) 2004-2008 - Francois Vogel
//
//  Localization files ( in tcl/msg_files/) are copyright of the 
//  individual authors, listed in the header of each file
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file scipad/license.txt
//
function locstr = LocalizeForScipad(strtag)
// Make use of msgcat from Scipad to localize as locstr the input
// string strtag
// This function is intended to be used only from Scipad .sci or
// .sce ancillaries, therefore no argument checks nor checks that
// Tcl is installed nor checks that Scipad interpreter exists, etc.
// Author: François Vogel, 2006-2007
// Note: this doesn't make use of request 319 implementation (TCL_EvalStr
//       returns the result of the evaluation) to ease backporting work in
//       the 4.x environment
// <TODO>: Checked again (19/09/08) in scilab-gtk-4.2 and in 4.1.2
//         Request 319 is now implemented in both. So let's get rid of this
//         old syntax creating a temporary variable in Scilab

    TCL_EvalStr("set locstring_scipad [mc """+strtag+"""]","scipad")
    locstr = TCL_GetVar("locstring_scipad","scipad")
endfunction

