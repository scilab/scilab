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

# in sync at least with v6.151
# English is the original language of Scipad
# Scipad Tcl/Tk source should use english texts
# for instance [mc "your text in english here"]
# Translations are provided in other msg files
# except for "expanded" text, like preference variables description


# db_bptsgui.tcl
::msgcat::mcset en_us istrue "is true"
::msgcat::mcset en_us haschanged "has changed"
::msgcat::mcset en_us always "always"
::msgcat::mcset en_us equalto "is equal to"
::msgcat::mcset en_us multipleof "is a multiple of"
::msgcat::mcset en_us greaterthan "is greater than"
::msgcat::mcset en_us lessthan "is lower than"


#helps.tcl
::msgcat::mcset en_us aboutme_message \
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
::msgcat::mcset en_us Scipad_is_released_under_GPL "SciPad is released under GPL"

# popupmenus.tcl
::msgcat::mcset en_us "AddWatch" "Add %s as watch"
::msgcat::mcset en_us "AddGenExp" "Add %s as generic expression"


# scilabexec.tcl
::msgcat::mcset en_us impossibleScilabEval_message \
                    "WARNING: A command could not be passed to Scilab and has been ignored.\n\n \
                    Most probably you don't have write access to Scilab's TMPDIR. \
                    Unpredictable results may occur.\n\n \
                    The offending command was:\n\n-->"
::msgcat::mcset en_us impossibleScilabEval_message2 \
                    "WARNING: A command could not be passed to Scilab and has been ignored \
                    because it is too long.\n\n \
                    Unpredictable results may occur.\n\n \
                    If you are currently debugging, try to remove watched variables.\n\n \
                    The offending command was:\n\n-->"


#preference variables description
::msgcat::mcset en_us "BGCOLOR" "Edit area background"
::msgcat::mcset en_us "BGLNMARGCOLOR" "Line numbers background"
::msgcat::mcset en_us "FGCOLOR" "Plain text"
::msgcat::mcset en_us "FGLNMARGCOLOR" "Line numbers foreground"
::msgcat::mcset en_us "CURCOLOR" "Cursor"
::msgcat::mcset en_us "PARCOLOR" "Parentheses ()"
::msgcat::mcset en_us "BRAKCOLOR" "Brackets \\\[\\\]"
::msgcat::mcset en_us "BRACCOLOR" "Braces {}"
::msgcat::mcset en_us "PUNCOLOR"  "Punctuation"
::msgcat::mcset en_us "INTFCOLOR" "Internal functions"
::msgcat::mcset en_us "COMMCOLOR" "Commands"
::msgcat::mcset en_us "SCICCOLOR" "Scicos functions"
::msgcat::mcset en_us "OPCOLOR"   "Operators"
::msgcat::mcset en_us "QTXTCOLOR"  "String text"
::msgcat::mcset en_us "REMCOLOR" "Comments"
::msgcat::mcset en_us "XMLCOLOR" "XML tags"
::msgcat::mcset en_us "NUMCOLOR" "Numbers"
::msgcat::mcset en_us "SELCOLOR" "Selection"
::msgcat::mcset en_us "BREAKPOINTCOLOR" "Breakpoint background"
::msgcat::mcset en_us "LFUNCOLOR" "Library functions"
::msgcat::mcset en_us "PDEFCOLOR" "Predefined variables"
::msgcat::mcset en_us "FOUNDTEXTCOLOR" "Matched text"
::msgcat::mcset en_us "CONTLINECOLOR" "Continued line"
::msgcat::mcset en_us "USERFUNCOLOR" "User defined functions"
