proc execfile {} {
    global listoffile sciprompt lang

    set textarea [gettextareacur]

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
	    cancel { set doexec 0 }
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
	    }
	} else {
	    set f $listoffile("$textarea",filename)
	    ScilabEval "exec(\"$f\");"
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
            ScilabEval "disp(\"$dispcomm1\")" 
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
         ScilabEval $impcomm
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
