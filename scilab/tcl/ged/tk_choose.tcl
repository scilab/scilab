global tkc_comment tkc_thelist text_button

set tk_choose_ww .tk_choose_menu
catch {destroy $tk_choose_ww}
toplevel $tk_choose_ww
wm title $tk_choose_ww "Tk_Choose"
wm iconname $tk_choose_ww ""
wm protocol $tk_choose_ww WM_DELETE_WINDOW "Return_zero"
wm geometry $tk_choose_ww +100+100

#remove some special caracters for tkc_comment
#character string treatment to remove "[" and "]" and replace "," and ";" by " "
set tkc_comment [string trimright $tkc_comment \] ]
set tkc_comment [string trimleft $tkc_comment \[ ]
regsub -all \" $tkc_comment "\n" tkc_comment
regsub -all "," $tkc_comment " " tkc_comment
regsub -all ";" $tkc_comment " " tkc_comment

#remove some special caracters for text_button
#character string treatment to remove "[" and "]" and replace "," and ";" by " "
set text_button [string trimright $text_button \] ]
set text_button [string trimleft $text_button \[ ]
regsub -all \" $text_button "\n" text_button
regsub -all "," $text_button " " text_button
regsub -all ";" $text_button " " text_button

# character string treatment to remove "[" and "]" and replace "," and ";" by " "
set tkc_thelist [string trimright $tkc_thelist \] ]
set tkc_thelist [string trimleft $tkc_thelist \[ ]

regsub -all "," $tkc_thelist " " tkc_thelist
regsub -all ";" $tkc_thelist " " tkc_thelist
set tkc_thelist [list $tkc_thelist]

frame .tk_choose_menu.main_frame
set tk_choose_w .tk_choose_menu.main_frame
pack $tk_choose_w -in  $tk_choose_ww  -side left -expand yes

label $tk_choose_w.label  -height 0 -text $tkc_comment -width 0 
pack $tk_choose_w.label -in $tk_choose_w -side top
set lbf [listbox $tk_choose_w.mylist  -height 0  -width 0]

#if one choice only, different treatment
if { [catch {set titi [expr $tkc_thelist]}] } {
    eval [list $lbf insert end] $tkc_thelist
} else {
    eval [list $lbf insert end] $titi
}

$tk_choose_w.mylist activate 0
bind  $tk_choose_w.mylist  <<ListboxSelect>> {
    Tk_Choose_Select [$tk_choose_w.mylist curselection]
}
pack $tk_choose_w.mylist -in $tk_choose_w -side top -fill x

button $tk_choose_w.cancel_button -text $text_button -command "Return_zero; destroy $tk_choose_ww" -font {Arial 9}
pack  $tk_choose_w.cancel_button -in $tk_choose_w -side top


proc Tk_Choose_Select {sel} {
#    set index [$sel curselection]
    incr sel
    ScilabEval "global tk_choose_num; tk_choose_num = $sel" "seq"
}


proc Return_zero { } {
    ScilabEval "global tk_choose_num; tk_choose_num = 0"
}
