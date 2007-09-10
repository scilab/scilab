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
#   displays the current buffer.
#   They are sent or removed in Scilab using setbpt and delbpt.
#
#######################################################################

source [file join $sourcedir db_states.tcl]
source [file join $sourcedir db_bptsmanagement.tcl]
source [file join $sourcedir db_actbptmanagement.tcl]
source [file join $sourcedir db_debugsession.tcl]
source [file join $sourcedir db_configureexec.tcl]
source [file join $sourcedir db_watch.tcl]
source [file join $sourcedir db_argsmanagement.tcl]

#######################################################################
##################### End of breakpoint stuff #########################
#######################################################################
