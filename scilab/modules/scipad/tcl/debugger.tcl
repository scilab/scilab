#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
#######################################################################
################## Beginning of breakpoint stuff ######################
#######################################################################
#            FV 13/05/04, breakpoint and debug stuff added            #
#######################################################################
# See scipad.xml for explanations about how this feature should be used
#######################################################################
#
# Just a few words about the variables used:
#
# Highest level:
# --------------
#   funnameargs: Full command sent to Scilab (function name with arguments)
#
# One level down: ($conf and $watch levels)
# ---------------
#  For $conf:
#   Elements of $listboxinput
#   Elements of $listboxinputval
#   Elements of $spin
#   funname: function name shown by $spin
#   funvars($funname): arguments list of function $funname, e.g. {arg1 arg2}
#   funvarsvals($funname,var): content of argument var from function $funname
#   funnames: list of all the different function names found by Obtainall_bp
#   funsinbuffer($textarea): same as funnames, but limited to $textarea
#  For $watch:
#   Elements of $lbvarname
#   Elements of $lbvarval
#   watchvars: list of watch variables, e.g. {var1 var2}
#   watchvarsprops($var,value)    : content of variable $var
#   watchvarsprops($var,prevvalue): content of variable $var at previous debug stop
#   watchvarsprops($var,tysi)     : type and size (in a single string) of variable $var
#   watchvarsprops($var,editable) : editability (true or false) of variable $var
#
# One further level down: ($adda level)
# -----------------------
#   argname: name of currently selected function argument
#   argvalue: content of this argument
#
# Aside of these:
# ---------------
#   Breakpoints are set and managed using tags in the text widget that
#   displays the current buffer. An array also stores the breakpoints properties.
#   Breakpoints are sent or removed in Scilab using setbpt and delbpt.
#   See also head comments in db_bptsmanagement.tcl for furter details.
#
#######################################################################

source [file join $sourcedir db_states.tcl]
source [file join $sourcedir db_bptsmanagement.tcl]
source [file join $sourcedir db_bptsgui.tcl]
source [file join $sourcedir db_actbptmanagement.tcl]
source [file join $sourcedir db_debugsession.tcl]
source [file join $sourcedir db_configureexec.tcl]
source [file join $sourcedir db_watch.tcl]
source [file join $sourcedir db_argsmanagement.tcl]

#######################################################################
##################### End of breakpoint stuff #########################
#######################################################################
