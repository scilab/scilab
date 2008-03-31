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
# in sync at least with v6.124
# English is the original language of Scipad
# Scipad Tcl/Tk source should use english texts
# for instance [mc "your text in english here"]
# Translations are provided in other msg files
# except for "expanded" text, like preference variables description


#helps.tcl
::msgcat::mcset eng aboutme_message \
                    "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                    Modified by Scilab Consortium.\n\
                    Revised by Enrico Segre 2003-2006.\n\
                    Miscellaneous improvements, particularly debug tools,\
                    by François Vogel 2004-2008.\n\
                    Swedish translation by Gunnar Lindholm 2005.\n\
                    Norwegian translation by Torbjørn Pettersen 2005.\n\
                    German translation by Dirk Reusch 2005.\n\
                    Polish translation by Bartosz Kosiorek 2005.\n\
                    Spanish translation by Juan Nicolás Quesada 2006.\n\
                    Chinese translation by Yung-Jang Lee 2007.\n\
                    Danish translation by Kim B. Hansen 2007."
::msgcat::mcset eng Scipad_is_released_under_GPL "SciPad is released under GPL"

# popupmenus.tcl
::msgcat::mcset eng "AddWatch" "Add %s as watch"
::msgcat::mcset eng "AddGenExp" "Add %s as generic expression"


# scilabexec.tcl
::msgcat::mcset eng impossibleScilabEval_message \
                    "WARNING: A command could not be passed to Scilab and has been ignored.\n\n \
                    Most probably you don't have write access to Scilab's TMPDIR. \
                    Unpredictable results may occur.\n\n \
                    The offending command was:\n\n-->"
::msgcat::mcset eng impossibleScilabEval_message2 \
                    "WARNING: A command could not be passed to Scilab and has been ignored \
                    because it is too long.\n\n \
                    Unpredictable results may occur.\n\n \
                    If you are currently debugging, try to remove watched variables.\n\n \
                    The offending command was:\n\n-->"


#preference variables description
::msgcat::mcset eng "BGCOLOR" "Edit area background"
::msgcat::mcset eng "BGLNMARGCOLOR" "Line numbers background"
::msgcat::mcset eng "FGCOLOR" "Plain text"
::msgcat::mcset eng "FGLNMARGCOLOR" "Line numbers foreground"
::msgcat::mcset eng "CURCOLOR" "Cursor"
::msgcat::mcset eng "PARCOLOR" "Parentheses ()"
::msgcat::mcset eng "BRAKCOLOR" "Brackets \\\[\\\]"
::msgcat::mcset eng "BRACCOLOR" "Braces {}"
::msgcat::mcset eng "PUNCOLOR"  "Punctuation"
::msgcat::mcset eng "INTFCOLOR" "Internal functions"
::msgcat::mcset eng "COMMCOLOR" "Commands"
::msgcat::mcset eng "SCICCOLOR" "Scicos functions"
::msgcat::mcset eng "OPCOLOR"   "Operators"
::msgcat::mcset eng "QTXTCOLOR"  "String text"
::msgcat::mcset eng "REMCOLOR" "Comments"
::msgcat::mcset eng "XMLCOLOR" "XML tags"
::msgcat::mcset eng "NUMCOLOR" "Numbers"
::msgcat::mcset eng "SELCOLOR" "Selection"
::msgcat::mcset eng "BREAKPOINTCOLOR" "Breakpoint background"
::msgcat::mcset eng "LFUNCOLOR" "Library functions"
::msgcat::mcset eng "PDEFCOLOR" "Predefined variables"
::msgcat::mcset eng "FOUNDTEXTCOLOR" "Matched text"
::msgcat::mcset eng "CONTLINECOLOR" "Continued line"
::msgcat::mcset eng "USERFUNCOLOR" "User defined functions"
