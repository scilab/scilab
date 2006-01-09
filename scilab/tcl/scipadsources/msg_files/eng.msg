# in sync at least with v5.62
# English is the original language of Scipad
# Scipad tcl/tk source should use english texts
# for instance [mc "your text in english here"]
# Translations are provided in other msg files
# except for "expanded" text, like preference variables description


#helps.tcl
::msgcat::mcset eng aboutme_message \
                    "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                    Modified by Scilab Consortium.\n\
                    Revised by Enrico Segre 2003-2005.\n\
                    Miscellaneous improvements, particularly debug tools,\
                    by François Vogel 2004-2005.\n\
                    Swedish translation by Gunnar Lindholm 2005.\n\
                    Norwegian translation by Torbjørn Pettersen 2005.\n\
                    German translation by Dirk Reusch 2005.\n\
                    Polish translation by Bartosz Kosiorek 2005.\n\
                    Spanish translation by Juan Nicolás Quesada 2006"

# menues.tcl 
  # locale names (common)
        ::msgcat::mcset eng "de locale (de)" "Deutsch (de)"
        ::msgcat::mcset eng "fr locale (fr)" "Français (fr)"
        ::msgcat::mcset eng "eng locale (eng)" "English (eng)"
        ::msgcat::mcset eng "es locale (es)" "Español (es)"
        ::msgcat::mcset eng "it locale (it)" "Italiano (it)"
        ::msgcat::mcset eng "no locale (no)" "Norsk (no)"
        ::msgcat::mcset eng "pl locale (pl)" "Polski (pl)"
        ::msgcat::mcset eng "se locale (se)" "Svenska (se)"


# popupmenus.tcl
::msgcat::mcset eng "AddWatch" "Add %s as watch"


# scilabexec.tcl
::msgcat::mcset eng impossibleScilabEval_message \
                    "WARNING: A command could not be passed to Scilab and has been ignored.\n\n \
                    Most probably you don't have write access to Scilab's TMPDIR. \
                    Unpredictable results may occur.\n\n \
                    The offending command was:\n\n-->"


#preference variables description
::msgcat::mcset eng "BGCOLOR" "Background"
::msgcat::mcset eng "FGCOLOR" "Plain text"
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
