global tkc_comment tkc_thelist text_button

set ww .tk_choose_menu
catch {destroy $ww}
toplevel $ww
wm title $ww "Tk_Choose"
wm iconname $ww ""
wm protocol $ww WM_DELETE_WINDOW "Return_zero"
wm geometry $ww +100+100

#remove " for tkc_comment
regsub -all "\"" $tkc_comment "" tkc_comment


# caracter string treatment to remove "[" and "]" and replace "," by " "
set tkc_thelist [string trimright $tkc_thelist \] ]
set tkc_thelist [string trimleft $tkc_thelist \[ ]

regsub -all "," $tkc_thelist " " tkc_thelist
regsub -all ";" $tkc_thelist " " tkc_thelist
#regsub -all "\"" $tkc_thelist "" tkc_thelist
set tkc_thelist [list $tkc_thelist]

frame .tk_choose_menu.main_frame
set w .tk_choose_menu.main_frame
pack $w -in  $ww  -side left -expand yes

label $w.label  -height 0 -text $tkc_comment -width 0 
pack $w.label -in $w -side top
set lbf [listbox $w.mylist  -height 0  -width 0]

#if one choice only, different treatment
if { [catch {set titi [expr $tkc_thelist]}] } {
    eval [list $lbf insert end] $tkc_thelist
} else {
    eval [list $lbf insert end] $titi
}

$w.mylist activate 0
bind  $w.mylist  <<ListboxSelect>> {
    Tk_Choose_Select [$w.mylist curselection]
}
pack $w.mylist -in $w -side top -fill x

button $w.cancel_button -text $text_button -command "Return_zero; destroy $ww" -font {Arial 9}
pack  $w.cancel_button -in $w -side top


proc Tk_Choose_Select {sel} {
#    set index [$sel curselection]
    incr sel
    ScilabEval "global tk_choose_num; tk_choose_num = $sel"
}


proc Return_zero { } {
    ScilabEval "global tk_choose_num; tk_choose_num = 0"
}
