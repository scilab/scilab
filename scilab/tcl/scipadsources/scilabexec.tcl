proc execfile {{buf "current"}} {
    global listoffile sciprompt lang

# FV 07/07/04, added capability for exec'ing a buffer which is not the current one
# ES 10/1/05, added return argument 
# (0=success, 1 scilab busy, 2 cancel, -1 fail)
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    set doexec 1
    if [ expr [string compare [getccount $textarea] 1] == 0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of \
             $listoffile("$textarea",filename) may have changed, do you wish \
             to to save your changes?" \
	            -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message \
            "Voulez-vous enregistrer les modifications apportées à \
              $listoffile("$textarea",filename) ?" \
	     -title "Confirmer sauver ?" -type yesnocancel -icon question]
	}
	case $answer {
	    yes { filetosave $textarea; set doexec 1 }
	    no { set doexec 0 }
	    cancel { set doexec 0; return 2 }
	}
    }

    if $doexec {
	if [ expr [string compare $sciprompt -1] == 0 ] {
	    if {$lang == "eng"} {
		tk_messageBox -message \
                 "Scilab is working, wait for the prompt to load file \
                  $listoffile("$textarea",filename)" \
                    -title "Scilab working" -type ok -icon info
	    } else {
		tk_messageBox -message \
                "Scilab est occupé, attendez le prompt pour charger le fichier \
                 $listoffile("$textarea",filename)"\
                    -title "Scilab occupé" -type ok -icon info
                 return 1
	    }
	} else {
	    set f $listoffile("$textarea",filename)
            if {[catch {ScilabEval "exec(\"$f\");" "sync" "seq"}]} {
               scilaberror $listoffile("$textarea",filename)
               return -1
	    } else {
	       return 0 
	    }
	}
    }
}

proc execselection {} {
    global sciprompt lang textareacur tcl_platform
    if [ expr [string compare $sciprompt -1] == 0 ] {
	if {$lang == "eng"} {
	   tk_messageBox -message \
               "Scilab is working, wait for the prompt to execute the selection" \
	       -title "Scilab working" -type ok -icon info
	 } else {
	   tk_messageBox -message \
               "Scilab est occupé, attendez le prompt pour charger la sélection" \
	       -title "Scilab occupé" -type ok -icon info
	 }
     } else {
        set seltexts [selection own]
        if {$seltexts != "" } {
	  if [catch {selection get -selection PRIMARY} sel] {	    
	  } else {
     	    set f [selection get]
#SciEval does not digest multilines, nor comments. The following hacks are 
# not optimal - they can produce very long lines, and get confused about 
# quoted strings containing //.
#strip comments from // to \n (note - \n stays, as the interpreter allows 
#    "...//bla\n rest" ) (NOTE: this way strings like "...//..." are truncated 
#    -- FIXIT -- has to use tag textquoted information)
            regsub -all -line "//.*(\\n|\\Z)" $f "\n" f1
            unset f
# remove trailing white space
            regsub -all -line "^\\s*" $f1 " " f2
            unset f1
#join continued lines
            regsub -all -line "\\.{2,} *\\n" $f2 "" f3
            unset f2
#join multilines with ","
            regsub -all -line "\\n" $f3 "," comm
            unset f3
# last hack - add a final endfunction if there is an unterminated
# function in the selection: TODO (try to involve proc whichfun)
# Things are complicated because the selection may either include
#  the originating "function" or not
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
#TODO ES 9/10/03
#             if { $i2>$i1 } {
#                 set funselstart [lindex [whichfun $i1] 0]
#                 set funselend [lindex [whichfun $i2] 0]
#             } else {
#                 set funselstart [lindex [whichfun $i2] 0]
# 		set funselend [lindex [whichfun $i1] 0]
# 	     }
#             tk_messageBox -message $funselstart"--"$funselend
# 	      if { $funselend !={} && $funselstart == {}} {
# 		  append comm ",endfunction"}
# 	      if { $funselend !={} && $funselstart != $funselend} {
# 		  tk_messageBox -message \
# 		      "How do you pretend Scilab to evaluate the bottom of a function definition without its header?"
# 		  return
# 	    }

# Besides, I'd like to see screen output too.
            regsub -all -line "\"" $comm "\"\"" dispcomm
            regsub -all -line "'" $dispcomm "''" dispcomm1
            unset dispcomm
# Changed by the Scilab Team, 26/06/04
#            if {"$tcl_platform(platform)" == "unix"} {    }
    #           		ScilabEval "disp(\"$dispcomm1\")" # {  }
# Changed for mprintf and with \n on ES request
#
# FV 01/01/05, added test to cope with string length limits in C language using %s
# The hardwired limit in character length is 509-13 since (quote from the MSDN
# Library - Oct 2001):
# ANSI compatibility requires a compiler to accept up to 509 characters in a string
# literal after concatenation. The maximum length of a string literal allowed in
# Microsoft C is approximately 2,048 bytes.
# (end of quote)
# Because I don't know the limit for other compilers, I keep 509 as the maximum
# above which the string is not displayed. Anyway, more than this is very hard
# to read in the Scilab shell.
            if {[string length $dispcomm1] < 496} {
                ScilabEval "mprintf(\"%s\\n\",\"$dispcomm1\")"
            }
	        ScilabEval $comm
          }
        }
     }
}

proc importmatlab {} {
    global pad listoffile sciprompt lang
    if [ expr [string compare $sciprompt -1] == 0 ] {
	if {$lang == "eng"} {
	   tk_messageBox -message \
             "Scilab is working, wait for the prompt to convert a Matlab file" \
	       -title "Scilab working" -type ok -icon info
	 } else {
	   tk_messageBox -message \
             "Scilab est occupé, attendez le prompt pour importer un fichier Matlab"\
	       -title "Scilab occupé" -type ok -icon info
	 }
     } else {
       if {$lang == "eng"} {
	  set types {
	    {"Matlab files" {*.m }} 
	    {"All files" {* *.*}}
	    }
	  set dtitle "Matlab file to convert"
       } else {
	  set types {
	    {"Fichiers Matlab" {*.m }} 
	    {"Tous les fichiers" {*.* *}}
	    }
	    set dtitle "Fichier Matlab à importer"
       }
       set sourcefile [tk_getOpenFile -filetypes $types -parent $pad -title "$dtitle"]
       if {$sourcefile !=""} {
	 set sourcedir [file dirname $sourcefile]
         set destfile [file rootname $sourcefile].sci 
	 set convcomm "execstr(\"res=mfile2sci(\"\"$sourcefile\"\",\
                      \"\"$sourcedir\"\",%f,%f,1,%t)\",\"errcatch\",\"m\")"
	 set impcomm \
	      "if $convcomm==0 then \
                 TK_EvalStr(\"scipad eval {delinfo; openfile \"\"$destfile\"\"} \"); \
               else; \
                 TK_EvalStr(\"scipad eval {failmatlabimp} \");\
               end"
         if {$lang == "eng"} {
	     showinfo "Scilab is converting, please hold on..." }
         if {$lang == "fr"} {
	     showinfo "Scilab est en train de convertir, patientez SVP..." }
         ScilabEval $impcomm "sync" "seq"
       }
     }
}

proc failmatlabimp {} {
  global lang
  if {$lang == "eng"} {
     tk_messageBox -title "Matlab file import"  \
       -message "Conversion of the file failed, see the Scilab window\
                for details -- Perhaps report the error text and the\
                offending Matlab file to \
                http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                -icon error
  }
  if {$lang == "fr"} {
     tk_messageBox -title "Import du fichier Matlab"  \
       -message "La conversion du ficher a échoué, voyez dans la fenêtre\
                Scilab pour plus d'informations -- Veuillez rapporter le\
                message d'erreur et le fichier Matlab incriminé à\
                http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                -icon error
  }
}

proc helpskeleton {} {
     global listoffile lang
# first exec the file in scilab, so that the current function is
#  really defined; then call help_skeleton, and pipe the
# result to a new (unsaved) buffer.
# NB: execing the file can have far-reaching consequences
#  if the file does more than just defining functions. 
# Responsibility left to the user.   
    set indexin [[gettextareacur] index insert]
    scan $indexin "%d.%d" ypos xpos
    set infun [whichfun $indexin]
    set funname [lindex $infun 0]
    if [execfile]==0 {
        if {$lang == "eng"} {
            set pathprompt  "Path for the xml source of the help file" 
        }
        if {$lang == "fr"} {
            set pathprompt "Chemin pour le source xml du fichier d'aide"
        }
        set dir [tk_chooseDirectory -title $pathprompt]
        if {$dir != ""} {
            set xmlfile [file join $dir $funname.xml]
            if {$lang == "eng"} {
              set warntitle "older $xmlfile found!"
              set warnquest \
                "The file $xmlfile alread exists: open the old file instead?"
              set warnold "Existing file" 
              set warnnew "New skeleton" 
            }
            if {$lang == "fr"} {
              set warntitle "Il existe une version plus ancienne de $xmlfile!"
              set warnquest \
                 "Le fichier $xmlfile existe déjà: Ouvrir le fichier existant?"
              set warnold "Fichier existant"
              set warnnew "Nouveau squelette d'aide" 
            }
	    if [file exists $xmlfile] {
		set answer [tk_dialog .existxml $warntitle $warnquest \
                   questhead 0 $warnold $warnnew]
	    } else {
              set answer 1
            }
	    if $answer {
              ScilabEval "help_skeleton(\"$funname\",\"$dir\")" "sync"
	    }
	    openfile $xmlfile
	}
    }
}

proc xmlhelpfile {} {
    global listoffile sciprompt lang
# save the file and call xmlfiletohtml. Catch and popup the error messages.
    filetosavecur
    if [ expr [string compare $sciprompt -1] == 0 ] {
	if {$lang == "eng"} {
	   tk_messageBox -message \
             "Scilab is working, wait for the prompt to compile the help file" \
	       -title "Scilab working" -type ok -icon info
	 } else {
	   tk_messageBox -message \
             "Scilab est occupé, attendez le prompt pour compiler la page d'aide"\
	       -title "Scilab occupé" -type ok -icon info
	 }
     } else {
	 set filetocomp [fullpath $listoffile("[gettextareacur]",filename)]
       ScilabEval "xmlfiletohtml(\"$filetocomp\")"
     }
}
