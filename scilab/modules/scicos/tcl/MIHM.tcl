#  tk_messageBox -icon question -type yesno -title Message  -message "$sure???"

set BWpath [file dirname "$env(SCIPATH)/modules/tclsci/tcl/BWidget-1.8.0"]
 if {[lsearch $auto_path $BWpath]==-1} {             
     set auto_path [linsert $auto_path 0 $BWpath]  
 }
 
set scilabpath $SciPath          
package require BWidget 1.8.0

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget-1.8.0"]
namespace inscope :: package require BWidget

global XML_LIST
global Active_Model
global NRow_last
global labelx
global Xj
global Rowi_last
global Colj_last
global DispMode
global Data_FSel_pm
global Data_FSel
global current_dir
global ztable
global ztree
global SearchedText
global sciGUITable
global last_node
global need_compile 
global scilabpath
global EnableQuit
global der_mode
global IncidenceMatrixSimplified
global IncidenceMatrix
global ParEmbedded
global JacobianEnable
#========================================
#DESSINER la zone pour afficher les textes + ajouter un Entry=hublot
proc MIHM_Create { WindowsID } {
    global sciGUITable
    global NRow_last
    global labelx
    global Xj
    global ztable
    global ztree
    global XML_LIST
    global need_compile 
    global DispMode
    global EnableQuit
    global der_mode
    global ParEmbedded
    global JacobianEnable

    set EnableQuit yes
    set XML_LIST {}
    set need_compile true
    set DispMode "Normal"
    set NRow_last 0
    set ncolumn [llength $labelx ]
    set der_mode "fixed_states"
    set ParEmbedded "0"
    set JacobianEnable "0"

    set NRowMax 20
    set dw  61;#[expr [lindex $bb1 2] - [lindex $bb1 0] + 2] ;# 52 width
    set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
    set dw_saf [expr $ncolumn*$dw ]; #393 ; # width of safhe
    set dh_saf [expr $NRowMax*$dh]; # height of safhe in rows
    set num_char 8 ;# number of characters in Entry

    ################-----------------------------#############
    set w "[sciGUIName $WindowsID].editor"
    set pw    [PanedWindow $w.pw -side top]
    ################---Tree Zone-----------------############
    set pane  [$pw add -weight 1]

    set title [TitleFrame $pane.lf -text "Model tree"]
    set sw    [ScrolledWindow [$title getframe].sw -relief sunken -borderwidth 2]

    set ztree  [Tree $sw.tree -relief ridge -deltay 39 -deltax 30 -padx 15 -width 30 -bg white]

    $ztree  bindText <1> "clickTree $WindowsID "
    $ztree  bindImage <1> "clickTree $WindowsID "
    $ztree  bindImage <Double-1> "clickTreeD $WindowsID "
    $ztree  bindText <Double-1> "clickTreeD $WindowsID "

    $sw setwidget $ztree
    pack $sw    -side top  -expand yes -fill both
    pack $title -fill both -expand yes

    ################---Table Zone-----------------############
    set pane [$pw add -weight [expr 3/2]]
    set lf   [TitleFrame $pane.lf -text "Variables & parameters attributes"]
    set sw   [ScrolledWindow [$lf getframe].sw -scrollbar vertical -auto none -relief sunken -borderwidth 2]

    set safhe [canvas $sw.safhe -width $dw_saf -height $dh_saf ]
    $sw setwidget $safhe

    pack $sw $lf -fill both -expand yes
    pack $pw     -fill both -expand yes

    set ztable $sw
    #---------------------------------------------   
    set hublot [Entry $ztable.safhe.entry -width $num_char  -bd 0 -relief sunken -background yellow -helptext "" -helptype balloon ]
    bind $hublot <Return> "EditHublotEnter $WindowsID 1 0"
    # 	bind $hublot <Right> "EditHublotEnter $WindowsID 0 1"
    # 	bind $hublot <Left> "EditHublotEnter $WindowsID 0 -1"
    bind $hublot <Up> "EditHublotEnter $WindowsID -1 0"
    bind $hublot <Down> "EditHublotEnter $WindowsID 1 0"
    bind $hublot <Tab> "EditHublotEnter $WindowsID 0 1"
    bind $hublot <Shift-Tab> "EditHublotEnter $WindowsID 0 -1"
    
    $ztable.safhe create window -100 -100 -window $ztable.safhe.entry -anchor w -tags tag_safhe
    $ztable.safhe delete tag_safhe
    ############################
    canvas $ztable.y_axes -width $dw_saf -height [expr $dh-6] 
    
    # draw a white page in the editable safhe
    $ztable.y_axes create rectangle 0 0 $dw_saf $dh_saf -fill lightblue -width 0 -tag labels_backgnd
    
    set Xj [list 0 [expr $dw] [expr 2*$dw] [expr 3*$dw] [expr 4*$dw] [expr 5*$dw] [expr 6*$dw] [expr 7*$dw] ]
    for { set j 0 } { $j<$ncolumn } { incr j } {
	$ztable.y_axes create text  [expr [lindex $Xj $j]+5] [expr $dh/2-2] \
            -text [lindex  $labelx $j] -anchor w -tag coltxt$j -fill black
    }
    #defining the scrollable region
    ###########################################################
    canvas $ztable.text1 -width 80 -height [expr $dh-6] 
    canvas $ztable.names -width 80 -height $dh_saf -yscrollcommand "$ztable.y_names set" -xscrollcommand "$ztable.x_names set"
    
    canvas $ztable.text2 -width 40 -height [expr $dh-6] 
    canvas $ztable.kind  -width 40 -height $dh_saf -yscrollcommand "$ztable.y_names set" 
    
    canvas $ztable.text3 -width 40 -height [expr $dh-6] 
    canvas $ztable.fixed -width 40 -height $dh_saf -yscrollcommand "$ztable.y_names set" 
    
    $ztable.text1 create rectangle 0 0 $dw_saf [expr $dh-6] -fill lightblue -width 0 -tag labels_backgnd 
    $ztable.text2 create rectangle 0 0 40 [expr $dh-6] -fill lightblue -width 0 -tag labels_backgnd
    $ztable.text3 create rectangle 0 0 40 [expr $dh-6] -fill lightblue -width 0 -tag labels_backgnd
    
    $ztable.text1 create text 5  [expr $dh/2-2] -text "Name  -    ID"  -anchor w -fill black
    $ztable.text2 create text 5  [expr $dh/2-2] -text "Kind"  -anchor w -fill black
    $ztable.text3 create text 5  [expr $dh/2-2] -text "Fixed" -anchor w -fill black
    
    scrollbar $ztable.x_names   -orient horizontal -command "AjourNames $WindowsID"
    scrollbar $ztable.y_names   -orient vertical   -command "PageDwnUp     $WindowsID"
    
    grid config $ztable.text1    -column 0 -row 0 -sticky "snew"
    grid config $ztable.text2    -column 1 -row 0 -sticky "snew"
    grid config $ztable.text3    -column 2 -row 0 -sticky "snew"
    grid config $ztable.y_axes    -column 3 -row 0 -sticky "snew" 
    
    grid config $ztable.names    -column 0 -row 1 -sticky "snew"
    grid config $ztable.kind     -column 1 -row 1 -sticky "snew"	
    grid config $ztable.fixed    -column 2 -row 1 -sticky "snew"	
    grid config $ztable.safhe -column 3 -row 1 -sticky "snew"
    grid config $ztable.y_names -column 4 -row 1 -sticky "snew"    
    grid config $ztable.x_names -column 0 -row 2 -sticky "snew"

    grid columnconfigure $ztable 0 -weight 1 
    grid columnconfigure $ztable 1 -weight 0
    grid rowconfigure    $ztable 1 -weight 1

    bind $ztable.safhe <1> "ClickCell $WindowsID %x %y"
    bind $ztable.names  <Double-1> "ClickCellName $WindowsID %x %y"

}
##################################################################"
proc MIHM_Init { {WindowsID -1 } } {
    global sciGUITable
    global XML_LIST
    global Active_Model
    global NRow_last
    global labelx
    global Xj
    global Rowi_last
    global Colj_last
    global DispMode
    global Data_FSel_pm
    global Data_FSel
    global current_dir
    global ztable
    global ztree
    global SearchedText
    global last_node
    global need_compile 
    global EnableQuit
    global der_mode
    global IncidenceMatrix
    global IncidenceMatrixSimplified
    global ParEmbedded
    global JacobianEnable

    set XML_LIST {}
    set Active_Model ""
    set NRow_last 0
    set labelx [list Value Weight Max Min Nominal Comment Selection]
    set Xj {}
    set Rowi_last 0
    set Colj_last 0	 
    set DispMode "Normal"
    set Data_FSel_pm [list "Normal" {} ]
    set Data_FSel {}
    set current_dir ""
    set ztable {}
    set ztree {}
    set SearchedText ""
    set last_node ""
    set need_compile true
    set EnableQuit  yes
    set der_mode "fixed_states"
    set IncidenceMatrix ""
    set IncidenceMatrixSimplified ""
    set ParEmbedded "0"
    set JacobianEnable "0"

    #############
    set WindowsID_new [sciGUICreate $WindowsID "GUIM" ]
    set w [sciGUIName $WindowsID_new]
    wm title $w "Xcos Modelica initialization window ($WindowsID_new)"
    wm protocol $w WM_DELETE_WINDOW "button_Quit $WindowsID_new"    
    #--------------------------------------------------
    set root $w
    set base [expr {($root == ".") ? "" : $root}]
    variable ROOT $root
    variable BASE $base
    
    menu $BASE.menu   

    set mx $BASE.menu.file
    menu $mx
    $BASE.menu add cascade -accelerator "F"  -label "File" -menu $mx -underline 0 -command ""
    $mx add command  -label "Open XML"  -underline 0 -accelerator {Alt+o} -command "button_OpenXML  $WindowsID_new"  
    $mx add command  -label "Load simulation result"  -underline 0 -accelerator {Alt+l} -command "button_OpenXMLSim  $WindowsID_new"
    $mx add command  -label "Close XML" -underline 0 -accelerator {Alt+c} -command "button_CloseModel $WindowsID_new"   
    $mx add command  -label "Save XML"  -underline 0 -accelerator {Alt+s} -command "button_SaveXMLs  $WindowsID_new"  
    $mx add command  -label "Save XML as..." -underline 1 -accelerator {Alt+a} -command "button_SaveXML  $WindowsID_new" 
    $mx add separator
    $mx add command  -label "Exit" -underline 1  -accelerator "Alt+x" -command "button_Quit $WindowsID_new"
    
    set mx $BASE.menu.derivatives
    menu $mx
    $BASE.menu add cascade -label "Derivative" -menu $mx -underline 4
    $mx add command -label "Fixed states"   -underline 0  -accelerator {Alt+x} -command "button_der_mode fixed_states  $WindowsID_new"   
    $mx add command -label "Steady states"  -underline 0  -accelerator {Alt+t} -command "button_der_mode steady_states $WindowsID_new"   

    set mx $BASE.menu.show
    menu $mx
    $BASE.menu add cascade -label "Display" -menu $mx -underline 0
    $mx add radio -label "Normal"              -variable DispMode -value "Normal"     -command "DispMode_change  $WindowsID_new"
    $mx add radio -label "Simplified model"    -variable DispMode -value "Simplified" -command "Simplified_mode  $WindowsID_new"
    $mx add radio -label "Only fixed items"    -variable DispMode -value "Onlypars"   -command "DispMode_change  $WindowsID_new"
    $mx add radio -label "Selected"            -variable DispMode -value "Selected"   -command "DispMode_change  $WindowsID_new"
    $mx add radio -label "Selected (show all)" -variable DispMode -value "FSelected"  -command "DispMode_change  $WindowsID_new"
    $mx add radio -label "Changed  (show all)" -variable DispMode -value "FChanged"   -command "DispMode_change  $WindowsID_new"

    set mx $BASE.menu.code
    menu $mx
    $BASE.menu add cascade -label "Code generation" -menu $mx -underline 0
    $mx add checkbutton -label "Parameter Embedding" -variable ParEmbedded -command  "ParameterEmbedding $WindowsID_new"
    $mx add checkbutton -label "Generate Jacobian "  -variable JacobianEnable -command  "JacobianEnabling $WindowsID_new"

    set mx $BASE.menu.helpx
    menu $mx
    $BASE.menu add cascade -label "Help" -menu $mx -underline 0
    $mx add command  -label "About" -underline 0 -accelerator {Alt+h} -command "button_HelpWindow  $WindowsID_new"

    $ROOT configure -menu $BASE.menu
    ###########################################
    frame $w.buttons -bd 1  -relief groove
    frame $w.status  -bd 1  -relief groove
    frame $w.editor  -bd 1 -background LightGray -relief groove
    #--------------------------------------------------    
    #label $w.top.logo  -image sciGUITable(gif,scilab01)
    #-------------------------------------------------
    # radiobutton $w.top.one   -value Normal    -text "Normal"     -variable DispMode -command "DispMode_change $WindowsID_new" 
    # radiobutton $w.top.two   -value Selected  -text "Selected"   -variable DispMode -command "DispMode_change $WindowsID_new"
    # radiobutton $w.top.three -value FSelected -text "Selected (all)" -variable DispMode -command "DispMode_change $WindowsID_new"
    # radiobutton $w.top.four  -value FChanged  -text "Changed (all)"  -variable DispMode -command "DispMode_change $WindowsID_new"
    #    LabelEntry $w.top.time -label "Initial time:" -labelwidth 10  -bd 2 -relief ridge -labelanchor e \
    #                  -text 0 -editable true -entrybg white  -width 8\
    #                  -helptext "Setting initial time"
    # set ST "[sciGUIName $WindowsID].top.time";set SearchedText [ $ST cget -text]    
    # set ST "[sciGUIName $WindowsID].top.combo"; set SearchedText [ $ST get ]
    #--------------------------------------------------
    ############################################################"'
    button $w.buttons.compute  -text "Solve" -width 8 -command "button_Compute $WindowsID_new"
    set cherche [LabelEntry $w.buttons.chercher -label "Search:" -labelwidth 10  -bd 2 -relief ridge -labelanchor e \
                   -text "" -editable true -entrybg  white -width 12\
                   -helptext "Searching matched variables"]
    $cherche bind <Return> "Do_Search $WindowsID_new"
    #$cherche bind <1> "Do_Search $WindowsID_new"

    label $w.buttons.mes02 -text "   Solver:" ;# -font $sciGUITable(font,2)     

    set combo [ComboBox $w.buttons.combo -editable false -text "Kinsol" \
                   -values {"Kinsol" "Ida (init)" "Fsolve" "Optim" } -entrybg white -highlightcolor yellow\
		   -selectbackground white -selectforeground blue -width 14 -helptext "Initial condition computing method"]
    label  $w.buttons.txterror -text "Error:"  -width 5 -anchor w -bd 1 
    label  $w.buttons.error    -text "?"    -width 12 -anchor w -relief groove -bd 2 -fg red

    pack  $w.buttons.mes02 $w.buttons.combo $w.buttons.compute  $w.buttons.txterror \
	$w.buttons.error  $w.buttons.chercher -side left -padx 4 -expand 0 -fill x -pady 2
    ##########################################################
    label $w.status.nequation  -text "Equation=0"      -width 14  -anchor w -relief ridge -bd 0 -fg blue
    label $w.status.nunknown   -text "Unknowns=0"      -width 14  -anchor w -relief ridge -bd 0 -fg blue
    label $w.status.simplf     -text "Reduced=?"       -width 12  -anchor w -relief ridge -bd 0 -fg purple
    label $w.status.nderivats  -text "Diff Ste=0"         -width 12  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.nparfixed  -text "Fixed Par=0"     -width 12  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.nparfree   -text "Relxd Par=0"     -width 14  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.nvarfixed  -text "Fixed Var=0"     -width 14  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.nvarfree   -text "Relxd Var=0"     -width 14  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.ndiscrete  -text "Discret=0"       -width 12  -anchor w -relief ridge -bd 0 -fg darkblue
    label $w.status.ninputs    -text "Input=0"         -width 10  -anchor w -relief ridge -bd 0 -fg darkblue

    pack  $w.status.nequation $w.status.nunknown $w.status.simplf $w.status.nderivats $w.status.nparfixed $w.status.nparfree\
        	$w.status.nvarfixed  $w.status.nvarfree   $w.status.ndiscrete $w.status.ninputs -side left -expand 0  
    ##########################################################
    #pack $w.top     -side top -expand 0 -fill x
    pack $w.buttons -side top -expand 0 -fill x
    pack $w.status  -side top -expand 0 -fill both 
    pack $w.editor  -side top -expand 1 -fill both


    return $WindowsID_new	
}
# ----------------------------------------------------------------------------
proc AjourNames { WindowsID action dx {pages ""} } {
    global ztable

    if { $pages != "" } {
	$ztable.names xview $action $dx $pages
    } else {
	$ztable.names xview $action $dx
    }
}
# ----------------------------------------------------------------------------
proc PageDwnUp { WindowsID action dy {pages ""} } {
    global ztable

    if { $pages != "" } {
	$ztable.safhe  yview $action $dy $pages
	$ztable.names  yview $action $dy $pages
	$ztable.kind   yview $action $dy $pages
	$ztable.fixed  yview $action $dy $pages
    } else {
	$ztable.safhe  yview $action $dy
	$ztable.names  yview $action $dy
	$ztable.kind   yview $action $dy
	$ztable.fixed  yview $action $dy
    }
}
#----------------------------------------------------------------------------
proc  button_Reset { WindowsID } {
    global ztree
    set RootNode "root"
    Reset_variable $WindowsID $ztree $RootNode
    set node [$ztree selection get]
    clickTree $WindowsID $node 
}
#----------------------------------------------------------------------------
proc  Reset_variable { WindowsID tree parent} {
    global labelx
    global Data_FSel
    global Active_Model
                   
	  set kids [$tree nodes $parent]
  
	  foreach  kid  $kids  {    
		  set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
		  if { $MainNode != $Active_Model } { continue }		
		  set data_i [$tree itemcget $kid -data]		
		  set NRow [llength $data_i]	 	
		  for { set i 0 } { $i < $NRow } { incr i } {
		      set rowi  [lindex $data_i $i]	
		      set loctxt [lindex $rowi 13] ; # "value_orig" column
		      set rowi [lreplace $rowi 4 4 $loctxt] 
		      set data_i [lreplace $data_i $i $i $rowi]				        
		  }	   		      		 
		  $tree itemconfigure $kid -data $data_i 
		  Reset_variable    $WindowsID $tree $kid 
	  }
	   return $tree   
	}
#=======================================================================
proc button_Compute { WindowsID } {
    global sciGUITable
    global need_compile
    global Active_Model
    global EnableQuit

    set w "[sciGUIName $WindowsID]"

    set Model_name [string range $Active_Model  4 end] 
    set init "_init"
    set ext ".xml"; 
    set tmpdir "$sciGUITable(win,$WindowsID,data,SCI_TMPDIR)"  
    set sciGUITable(win,$WindowsID,data,Scixmlfile)  "$tmpdir/$Model_name$init$ext"
    set res [button_SaveXML $WindowsID]

    if { $res == "nok" } {
	return
    }
    
    set EnableQuit  no

    $w.buttons.compute configure -state disabled
    $w.buttons.compute configure -text "wait..."

    if  { $need_compile eq "true" } { 	     
       Compile $WindowsID
    } else  {
       Compile_finished ok $WindowsID 
    }
}
#----------------------------------------------------------------
proc Compile { WindowsID } {
    global Active_Model
    global SearchedText
    global Data_FSel
    global DispMode
    global sciGUITable
    global IncidenceMatrix
    global ParEmbedded
    global JacobianEnable

    set w "[sciGUIName $WindowsID]";
    set Model_name [string range $Active_Model  4 end] 

    set is_square [get_model_info $WindowsID ]

    if { $is_square =="false" } {
    	set dlg "Please relax or fix variable(s) or parameter(s) to get a square problem!"
	tk_messageBox -icon error -type ok -title "$Model_name is not square" -message $dlg
	Compute_finished nok $WindowsID 
	return
    }
    
    ScilabEval "fw='$Model_name'; paremb='$ParEmbedded'; jaco='$JacobianEnable'; compile_init_modelica(fw,paremb,jaco); "
    #compile_init_modelica( ) calls Compile_finished ok/nok
}
#=======================================================================
proc Compile_finished { res WindowsID } {
    global need_compile
    global sciGUITable
    global IncidenceMatrixSimplified
    global Active_Model
    set w "[sciGUIName $WindowsID]" 

    if { $res eq "ok" } {
	#----------------- openning/reading the new incidence matrix generated after compile-------------
	set Model_name [string range $Active_Model  4 end]     
	set ext ".xml"; 
	set tmpdir "$sciGUITable(win,$WindowsID,data,SCI_TMPDIR)"  
	set filename "$tmpdir/$Model_name$ext" 
	set filename_incid [string map { ".xml" "i_incidence_matrix.xml" } $filename]    
	set IncidenceMatrixSimplified [read_incidence $WindowsID $filename_incid]	    
	set nimpvar [lindex $IncidenceMatrixSimplified 0]
	$w.status.simplf  configure -text "Reduced=$nimpvar";	
	#----------------- openning/reading the new incidence file -------------
	set need_compile false 
	set Smethod [ $w.buttons.combo get ]
	ScilabEval "method='$Smethod';Nunknowns='$nimpvar';ok=Compute_cic(method,Nunknowns); "	
    } else {
	Compute_finished nok $WindowsID
    }
}
#tk_messageBox -icon error -type ok -title "l_name is not square" -message "need_compile=$need_compile"
#=======================================================================
proc Compute_finished { res  WindowsID } {
    global Active_Model
    global DispMode
    global sciGUITable
    global ztree
    global EnableQuit
    set  EnableQuit yes

    set w "[sciGUIName $WindowsID]";   
    if { $res eq "ok" } {
	set ierror $sciGUITable(win,$WindowsID,data,IERROR)
	$w.buttons.error configure -text "$ierror"
	
	Update_Selected_in_tree  $WindowsID "Write" 	  
	set oldnode [$ztree selection get]
	remove_from_tree $WindowsID  $Active_Model; 
	
	set Model_name [string range $Active_Model  4 end]     
	set init "_init"
	set ext ".xml"; 
	set tmpdir "$sciGUITable(win,$WindowsID,data,SCI_TMPDIR)"  
	set sciGUITable(win,$WindowsID,data,Scixmlfile) "$tmpdir/$Model_name$init$ext" 
	button_OpenXML  $WindowsID
	
	clickTree $WindowsID $oldnode 
	$ztree selection set $oldnode  
	open_parent $oldnode
    }
    $w.buttons.compute configure -state active
    $w.buttons.compute configure -text "Solve"

}
#=======================================================================
proc open_parent { node } {
    global ztree
   
    set parent_node  [$ztree parent $node]
    if {$parent_node eq "root"} { return  }
    $ztree opentree $parent_node false  
    open_parent $parent_node
}

# ----------------------------------------------------------------------------
# Close model button
proc button_CloseModel { WindowsID } {
    global Active_Model
    global XML_LIST
    global DispMode
    global EnableQuit
    if { $EnableQuit == "no" } { return }
    if { $Active_Model eq "" } { return } 

    set Model_name [string range $Active_Model  4 end]  ;# removing "root" from "nodex"

    set answer [tk_messageBox -icon question -type yesno -title "Closing $Model_name" \
		    -message "Are you sure to close the model \n\r '$Model_name'?"]
    if { $answer eq "yes" } {      
	
	Update_Selected_in_tree  $WindowsID "Write" 
	set DispMode "Normal"
	set Active_Model [remove_from_tree $WindowsID  $Active_Model]; 
	clickTree $WindowsID $Active_Model
	get_model_info $WindowsID    
    }
}
#=======================================================================
proc button_OpenXMLSim { WindowsID } {
    global sciGUITable
    global Active_Model
    global DispMode
    set Model_name [string range $Active_Model  4 end-1]  ;# removing "root-f" from "nodex"
    set Sim "Sim"
    set ext ".xml"; 
    set tmpdir "$sciGUITable(win,$WindowsID,data,SCI_TMPDIR)"  
    set simxml "$tmpdir/$Model_name$Sim$ext"
    set sciGUITable(win,$WindowsID,data,Scixmlfile)  $simxml
    if { $DispMode ne "Normal" } {
	set DispMode "Normal"
	DispMode_change $WindowsID ;
    }
    button_OpenXML $WindowsID
}
#=======================================================================
proc button_OpenXML { WindowsID } {
    global XML_LIST
    global Active_Model
    global sciGUITable
    global current_dir
    global need_compile
    global EnableQuit
    global der_mode
    global IncidenceMatrix
    global ztree
    global DispMode

    if { $EnableQuit == "no" } { return }
	     
    set ftypes { {{XML Scripts}    {.xml} }   }
    set filename "$sciGUITable(win,$WindowsID,data,Scixmlfile)"    
    set sciGUITable(win,$WindowsID,data,Scixmlfile) ""
    set init "_init"

    if { $filename=="" } { 
	set Model_name [string range $Active_Model  4 end]  ;# removing "root" from "nodex"
     	set filename [tk_getOpenFile -filetypes $ftypes -initialfile $Model_name$init]
	set need_compile true 
	if { $filename != "" } {	
	    if { $DispMode ne "Normal" } {
		set DispMode "Normal"
		DispMode_change $WindowsID ;
	    }
	}
    }   
    if { $filename != "" } {	
	set filename_tail [file tail  $filename]
	set filename_tail [file root  $filename_tail]
	set current_dir   [file dirname  $filename]			
	
	if { [catch { set fileid [open $filename r] }] } {	      
	    tk_messageBox -icon error -type ok -title "Error Message"  -message "Unable to open $filename"
	    return nok
	}
	set dataxml [read $fileid]  
	close $fileid      

	if { [catch { set xmllist [xml_to_list $dataxml] }] } {	      
	    tk_messageBox -icon error -type ok -title "Error Message"  -message "Unable to read XML $filename"
	    return nok
	}

	set Main_node [fill_tree $WindowsID $xmllist]
	if { $Main_node=="" } { return nok }

	if { $IncidenceMatrix == "" } {
	    set filename_Rel [string map { "f.xml" "f_relations.xml" } $filename]
	    set IncidenceMatrix [read_incidence $WindowsID $filename_Rel]
	}
	#---------------------------------------------
	lappend XML_LIST [list "$Main_node" "$need_compile" $xmllist]  
	set Active_Model $Main_node       

	clickTree $WindowsID $Main_node
	set is_square [get_model_info $WindowsID ]
	if { $is_square == "false"} {

	    set Info [list 0 0 0 0 0 0 0 0]
	    set Info [get_model_info_kids $WindowsID  $ztree root $Info]
	    set NDIF [lindex $Info 7]
	    if  { [expr $NDIF] > 0 } {
		set title "State derivative fixing mode selection"
		set msg "The model contains derivatives, since you have not fixed some of them, you need to fix or relax some variables to have a square system of equations!\nPlease select an option"
		set default 0
		set answer [tk_dialog .dialog1 $title $msg question $default { Fix states } { Fix derivatives } { I will do it later } ]
		switch $answer {
		    0 {  button_der_mode "fixed_states" $WindowsID   }
		    1 {  button_der_mode "steady_states" $WindowsID   }
		}
	    }
	}    
	return ok
    } else {
	return nok
    }
} 
#=======================================================================
proc button_MergeXML { WindowsID newfile} {

    tk_messageBox -icon error -type ok -title "Import module"  -message "Not implemented yet.."

}
#=======================================================================
proc button_SaveXMLs { WindowsID } { ;# Save button
    global Active_Model
    global sciGUITable
    
    set Model_name [string range $Active_Model  4 end]  ;# removing "root" from "nodex"    
    set init "_init"
    set ext ".xml"; 
    set tmpdir "$sciGUITable(win,$WindowsID,data,SCI_TMPDIR)"  
    set savexml "$tmpdir/$Model_name$init$ext"
    set sciGUITable(win,$WindowsID,data,Scixmlfile)  $savexml	
    set res [button_SaveXML $WindowsID]
    if { $res == "nok" } {  return nok	}
    return ok;
}

proc button_SaveXML { WindowsID } { ;# save as button
    global XML_LIST
    global Active_Model
    global current_dir
    global DispMode
    global sciGUITable
    global EnableQuit
   if { $EnableQuit == "no" } { return }

    if { $Active_Model eq "" } { return nok }    
    
    set filename "$sciGUITable(win,$WindowsID,data,Scixmlfile)"  
    set sciGUITable(win,$WindowsID,data,Scixmlfile)  ""
    set init "_init"

    if { $filename=="" } { 
	set ftypes {  {{XML Scripts}   {.xml}    }		}
	set Model_name [string range $Active_Model  4 end]  ;# removing "root" from "nodex"
	set init "_init"
      	set filename [tk_getSaveFile -filetypes $ftypes  -initialfile "$Model_name$init" -initialdir \
			  $current_dir -defaultextension .xml]	
    } 
    if { $filename != "" } {	
	if { [catch { set fileid [open $filename w] }] } {
	    tk_messageBox -icon error -type ok -title "Error Message"  -message "Unable to write to $filename"
	    return nok
	} 
	set DispMode_back $DispMode
	set DispMode "Normal"
	DispMode_change $WindowsID ; # just for updating the data on the tree branches
	set DispMode $DispMode_back 
	DispMode_change $WindowsID ; # just for updating the data on the tree branches
	
	set filename_tail [file tail  $filename]
	set filename_tail [file root  $filename_tail]
 	set read_xmllist [Read_tree $WindowsID $Active_Model]
	if { [catch {set res  [list_to_xml $read_xmllist]}] == 0 } {
	    set res [string map {  "&#93;" ]  "&#91;" [  }  $res]
	    puts $fileid  $res
	    close $fileid
	    return ok
	} else {
	    tk_messageBox -icon error -type ok -title "Error Message"  -message "Error in the XML file\n Please verify your data"
	    return nok
	}
#	    return ok
    }
}
#=======================================================================
#set xin {dddd><"hhhhhhh><hhh"><rrrrrrrr><}
#        dddd}{"hhhhhhh><hhh"}{rrrrrrrr}{
# a single regsub won't work for that kind of parsing,one way is to split your list 
# by the  character, and then do the regsub (or even a [string map]) on every other item,
# then rejoin with quotes.

proc doSub {xin} {
     set new {}
     set old [split $xin \"]
     foreach {outside inside} $old {
        lappend new [string map {< \{ > \}} $outside] $inside
     }
     if {[llength $new] > [llength $old]} {
        # added extra item by iterating 2 at a time so delete last item
        set new [lrange $new 0 end-1]
     }
     return [join $new \"]
}

#=====================================================
proc xml_to_list MyXML {
    regsub -all {>\s*<} [string trim $MyXML " \n\t<>"] "\} \{" MyXML
    set MyXML [filterCS  $MyXML]

    set xmlc ""
    set lstr [string length $MyXML]
    set flagstr off
    
    # set MyXML [string map {> "\} \{#text \{" < "\}\} \{"}  $MyXML]
    for { set i 0 } { $i < $lstr } { incr i } {
	set xss [string index $MyXML $i]
	if { $xss== "\"" } { if { $flagstr== on } {set flagstr off } else { set flagstr on } ; }
	if { $flagstr == off } { 	    
	    if  { $xss == ">" } { set xss "\} \{#text \{"  }
	    if  { $xss == "<" } { set xss "\}\} \{" }
	} 
	append xmlc $xss 
    }
    set MyXML $xmlc

    set Result ""   ;# string to collect the result
    set pile {} ;# track open tags
    set rest {}
    foreach object "{$MyXML}" {
	switch -regexp -- $object {
            ^# {append Result "{[lrange $object 0 end]} " ; #text object}
            ^/ {
                regexp {/(.+)} $object -> Tag_Name ;# end tag
                set expected [lindex $pile end]
                if {$Tag_Name!=$expected} {error "$object != $expected"}
                set pile [lrange $pile 0 end-1]
                append Result "\}\} "
	    }
            /$ { # singleton - start and end in one <> group
		regexp {([^ ]+)( (.+))?/$} $object -> Tag_Name - rest
		#--------------------------------------
		#set restc [string map {= " "} $rest]
		#--------------------------------------
		set flagstr off
		set lstr [string length $rest]
		set restc ""
		for { set i 0 } { $i < $lstr } { incr i } {
		    set xss [string index $rest $i]
		    if { $flagstr == off } { 	    
			if  { $xss == "=" } { set xss " "  }
		    } 
		    if { $xss== "\"" } { if { $flagstr== on } {set flagstr off } else { set flagstr on } ; }
		    append restc $xss
		}
		#-----------------------------
		set rest [lrange $restc 0 end]
		#set rest [lrange [string map {= " "} $rest] 0 end]
		append Result "{$Tag_Name [list $rest] {}} "
            }
            default {
		#set Tag_Name1 [lindex $object 0] ;# start tag		
		set Tag_Name [string range $object 0 [expr [string wordend $object 0]-1]] ;# start tag
		#--------------------------------------
		#set objectc [string map {= " "} $object]
		#--------------------------------------
		set flagstr off
		set lstr [string length $object]
		set objectc ""
		for { set i 0 } { $i < $lstr } { incr i } {
		    set xss [string index $object $i]
		    if { $flagstr == off } { 	    
			if  { $xss == "=" } { set xss " "  }
		    } 
		    if { $xss== "\"" } { if { $flagstr== on } {set flagstr off } else { set flagstr on } ; }
		    append objectc $xss
		}
		#-----------------------------
		set rest [lrange $objectc 1 end]
		#set rest [lrange [string map {= " "} $object] 1 end]
		lappend pile $Tag_Name
		append Result "\{$Tag_Name [list $rest] \{"
            }
	}
	if {[llength $rest]%2} {error "attributes are not paired: $rest"}
    }
    if [llength $pile] {error "unresolved: $pile"}
    string map {"\} \}" "\}\}"} [lindex $Result 0]   
}
#=======================================================================
proc list_to_xml MyList {
    switch -- [llength $MyList] {
        2 {lindex $MyList 1}
        3 {
            foreach {tag attributes children} $MyList break
            set Result <$tag
            foreach {name value} $attributes {
                append Result " $name=\"$value\""
            }
            if [llength $children] {
                append Result >
                foreach child $children {
                    append Result [list_to_xml $child]
                }
                append Result </$tag>\n
            } else {append Result />\n}
        }
        default {error "Unable to parse $MyList"}
    }

}

proc  filterCS  text {
    return [string map { ] "&#93;" [ "&#91;" { "&#123;" } "&#125;"  }  $text]
# & -> &amp;
# " -> &quot;
# ' -> &apos;
# > -> &gt;
# < -> &lt;
}

proc  filterbackCS  text {
    return [string map {  "&#93;" ]  "&#91;" [  "&gt;"  >  "&lt;"  < "&amp;" &   "&#123;" {  "&#125;" } }  $text]
#return [string map {  "&#93;" ]  "&#91;" [  }  $text]
}
#----------------------------------------------------
proc Ceil { input } {	
    set pos [string first "." "$input" ]
    return [string range "$input" 0 [expr $pos-1] ]   
}
##################################################################"

#=======================================================================
proc DispMode_change { WindowsID } {
    global ztree
    set node [$ztree selection get]
    clickTree $WindowsID $node 
}

proc Simplified_mode { WindowsID } {
    global sciGUITable
    global Active_Model
    global IncidenceMatrixSimplified
    global DispMode

    if { $IncidenceMatrixSimplified == "" } { 
	set msg "You first need to compile the model! \rPlease push the Solve button."
	tk_messageBox -icon question -type ok -title "Info"  -message $msg
	set DispMode "Normal"
	return 
    }
    DispMode_change $WindowsID
}

proc Do_Search { WindowsID } {
      global DispMode
      global SearchedText      
     
    set ST "[sciGUIName $WindowsID].buttons.chercher"            
    # set SearchedText [ $ST cget -text ]
    if { [catch {set SearchedText [string map {  [ "&#91;"  ] "&#93;" > "&gt;"  <  "&lt;"  & "&amp;" {  "&#123;" }  "&#125;" }   [ $ST cget -text ]]}]==0 }  {		
	set DispMode "Search"
	DispMode_change $WindowsID
    }
}
# ----------------------------------------------------------------------------
proc button_Quit { WindowsID } {
    global EnableQuit
    if { $EnableQuit == "no" } { tk_messageBox -icon question -type ok -title "Computing..."  -message "Please wait until the end of the simulation:"; return }
    
    set answer [tk_messageBox -icon question -type yesno -title "Exit?"  -message "Are you sure you want to quit? \n\rYou will lose the settings if you do not save the XML file"]
    if { $answer eq "yes" } {      
	ScilabEval "clearglobal('icpr')"
	set need_compile true
#	unset IHMLoc 
	sciGUIDestroy $WindowsID
	
    }
}
#=======================================================================
proc ParameterEmbedding { WindowsID } {
    global ParEmbedded
    global need_compile

    set msg1 "Parameter Embedding option:"
    set msg2 "\nSetting the parameter embedding option lets the user to change the parameters and call the solver without regenerating the code."
    set msg3 "\nIn a code without embedded parameters, parameters are replaced by their numerical values. Hence the code is smaller and is more likely to convert."
    set msg4 "\nSo it is recommended to use the parameter embedded option once you achieve the solution and want to test it with other parameter values."
    tk_messageBox -icon question -type ok -title Help  -message "$msg1$msg2$msg3$msg4"
    set need_compile true

}

proc JacobianEnabling { WindowsID } {
    global JacobianEnable
    global need_compile

    if { $JacobianEnable eq "0" } {      
	set msg1 "Jacobian is disabled"
    }    else {
	set msg1 "Jacobian is enabled"
	set msg1 "$msg1\n\rRemind that the Jacobian is used only with the IDA solver"
    }

    tk_messageBox -icon question -type ok -title Help  -message $msg1
    set need_compile true
}

#=======================================================================
proc button_HelpWindow  { WindowsID }   { 
    set messegedlg "Modelica initialization GUI,\n\rThis version is used to initialize pure static Modelica models.\rREF: Initialization of Modelica Models in Scicos, M. Najafi, R. Nikoukhah.\r2008 Modelica Conference, Bielefeld, Germany.\r"
    tk_messageBox -icon question -type ok -title Help  -message "$messegedlg"
}

proc button_der_mode  { new_mode WindowsID }   { 
    global ztree
    global Active_Model
    global DispMode
    global der_mode
    global need_compile

    if { $new_mode eq "free_fixed_states" }  { return }
    if { $new_mode eq "free_steady_states" }  { return }
    if { "free_$new_mode" != $der_mode } { set need_compile true }
    set der_mode $new_mode

    set DispMode_back $DispMode
    set DispMode "Normal"
    DispMode_change $WindowsID
    #----------------------------------------------
    switch $der_mode {
	"fixed_states" {	
	    set NewDerWeight 0
	    set NewStateWeight 1
	}
	
	"steady_states" {			
	    set NewDerWeight 1
	    set NewStateWeight 0
	}
    }

    set RootNode "root"
    replace_ders_in_tree $WindowsID $ztree  $RootNode $NewDerWeight $NewStateWeight
    #------------------------------------------------
    set DispMode $DispMode_back 
    set der_mode "free_$new_mode"
    DispMode_change $WindowsID
}

proc replace_ders_in_tree { WindowsID tree parent NewDerWeight NewStateWeight } {
    global Active_Model
    set VARS {}
    set VARD {}
    set kids [$tree nodes $parent]

    foreach  kid  $kids  {   
	set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
	if { $MainNode != $Active_Model } { continue }
	set data_i [$tree itemcget $kid -data]		
	set NRow [llength $data_i]	 	
	for { set i 0 } { $i < $NRow } { incr i } {
	    set rowi  [lindex $data_i $i]	
	    set namei [lindex $rowi 0] ; # "NAME" column
	    set idi   [lindex $rowi 2] ; # "ID" column
	    
	    # returns the 11 th element from the list $rowi
	    # boolean fixed_orig
	    set fixed_orig  [lindex $rowi 11] ; # "fixed_orig" column

	    if { [regexp {__der_(\w*)} $idi -> restOfWord] > 0  } {
		lappend  VARS $idi
	    }
	}
	replace_ders_in_tree   $WindowsID $tree $kid $NewDerWeight $NewStateWeight
	$tree itemconfigure $kid -data $data_i;
    }

    # search for VARS to change their attributes
    foreach var $VARS {
	foreach  kid  $kids  {   
	    set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
	    if { $MainNode != $Active_Model } { continue }
	    set data_i [$tree itemcget $kid -data]		
	    set NRow [llength $data_i]	 	
	    for { set i 0 } { $i < $NRow } { incr i } {
		set rowi  [lindex $data_i $i]	
		set idi   [lindex $rowi 2] ; # "ID" column
		set fixed_orig   [lindex $rowi 11] ; # "Fixed_orig" column
		set vard  [string map {__der_ ""} $var]
		if { $idi == $vard } {	
		    if { $fixed_orig == "false" } {
			lappend  VARD $var
			set rowi [lreplace $rowi 5 5 $NewStateWeight ]
			set data_i [lreplace $data_i $i $i $rowi]
		    }
		    break
		}
	    }
	    $tree itemconfigure $kid -data $data_i;
	}
    }

    foreach var $VARD {
	foreach  kid  $kids  {   
	    set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
	    if { $MainNode != $Active_Model } { continue }
	    set data_i [$tree itemcget $kid -data]		
	    set NRow [llength $data_i]	 	
	    for { set i 0 } { $i < $NRow } { incr i } {
		set rowi  [lindex $data_i $i]	
		set idi   [lindex $rowi 2] ; # "ID" column
		if { $idi == $var } {		    	    
		    if { $NewDerWeight ==  1 } {
			set rowi [lreplace $rowi 4 4 0.0 ] ;# set the __der_ variable values to 0.0
		    }
		    set rowi [lreplace $rowi 5 5 $NewDerWeight ]
		    set data_i [lreplace $data_i $i $i $rowi]	
		    break
		}
	    }
	    $tree itemconfigure $kid -data $data_i;
	}
    }

}

# ----------------------------------------------------------------------------
# Description : QUAND on double-click sur une variable, bind with <Double-1> on the safhe
# ----------------------------------------------------------------------------
proc ClickCellName { WindowsID x y } {
    global labelx
    global Xj
    global Data_FSel
    global DispMode
    global ztable
    global ztree
    global AliasVars

    set Tree_node [$ztree selection get]
    if  { $Tree_node=="" } { return }

    if { $DispMode == "Normal" } {
	set data_i [$ztree itemcget $Tree_node -data]
    } else {
	set data_i  $Data_FSel	
    }
    
    set NRow [llength $data_i]
    set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
    set dh_saf [expr $NRow*$dh]; # height of safhe in rows
    set positiony [$ztable.safhe canvasy $y]
    if { $positiony<=$dh_saf } {
	set Rowi [Ceil [expr $positiony/$dh]]
    } else {
	return	
    }

   # set rowi [lindex $data_i $Rowi]
  #  set idi [lindex $rowi 2]
#    set AliasVars [LookForAlias $idi]
  #  #if { $AliasVars == "\{\}" } { set nal 0} else { set nal [llength $AliasVars ] }
   # #set Alias2 [string map {" " "\r"} $AliasVars]
   # # tk_messageBox -icon info -type ok -title "$idi "  -message "$idi \rhas direct connections with $nal variables:\r$Alias2"
#    set DispMode "Alias"
    set DispMode "Normal"
    DispMode_change $WindowsID
}


# ----------------------------------------------------------------------------
#  Quand on click avec la souris sur une case, bind with <1> on the safhe
proc ClickCell { WindowsID x y } {

        global labelx
        global Xj
	global Data_FSel
	global DispMode

	global ztable
	global ztree

        set Tree_node [$ztree selection get]
	if  { $Tree_node=="" } { return }


	if { $DispMode == "Normal" } {
	    set data_i [$ztree itemcget $Tree_node -data]
	} else {
	    set data_i  $Data_FSel
	}
	
        set NRow [llength $data_i]
	set ncolumn [llength $labelx ]

        set dw  61;#[expr [lindex $bb1 2] - [lindex $bb1 0] + 2] ;# 52 width
        set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
        set dw_saf [expr $ncolumn*$dw ]; #393 ; # width of safhe
        set dh_saf [expr $NRow*$dh]; # height of safhe in rows
	set num_char 8 ;# number of characters in Entry

	set positionx [$ztable.safhe canvasx $x]
	set positiony [$ztable.safhe canvasy $y]


	if { $positiony<=$dh_saf } {
	    set Rowi [Ceil [expr $positiony/$dh]]
	    incr Rowi
	} else {
	    set Rowi $NRow
	}
	
	if { $positionx<=$dw_saf } {
	    for { set j 0 } { $j<=$ncolumn } { incr j } {
		set xLj [lindex $Xj $j]
		if { $positionx<=$xLj } {
		    set Colj [expr $j]
		    break
		}
	    }
	} else {
	    set Colj $ncolumn
	}
	
	set new_data_i [EditHublot $WindowsID $Rowi $Colj $data_i]


	if { $DispMode == "Normal" } {
	    $ztree itemconfigure $Tree_node -data $new_data_i; # rewriting edited data into the tree node data space
	} else {
	    set  Data_FSel $new_data_i
	}

}
#=======================================================================
# ----------------------------------------------------------------------------
#editer par le clavier
proc EditHublotEnter { WindowsID dxi dyj } {
    global Rowi_last
    global Colj_last
    global Data_FSel
    global DispMode
    global ztable
    global ztree
    global need_compile
    global ParEmbedded

    set Tree_node [$ztree selection get]

    if { $DispMode == "Normal" } {
	set data_i [$ztree itemcget $Tree_node -data]
    } else {
	set data_i  $Data_FSel
    }
    
    set Rowi [expr $Rowi_last + $dxi]
    set Colj [expr $Colj_last + $dyj]
    set new_data_i [EditHublot $WindowsID $Rowi $Colj $data_i]
    
    if { $DispMode == "Normal" } {
	$ztree itemconfigure $Tree_node -data $new_data_i; # rewriting edited data into the tree node data space
    } else {
	set  Data_FSel $new_data_i
    }

    # Colj est la colonme avant de taper sur l'entrée
    if { $Colj == 2 } { #  updating info-bar only if the second column (weight) is changed
	if { $new_data_i ne $data_i } {
	    set DispMode_back $DispMode
	    set DispMode "Normal"
	    DispMode_change $WindowsID ; # just for updating the data on the tree branches
	    set DispMode $DispMode_back 
	    DispMode_change $WindowsID ; # just for updating the data on the tree branches    
	    get_model_info $WindowsID 
	    set need_compile true
	}
    }
    
    if { $ParEmbedded == 0 } {
	if { $Colj == 1 } { #  compiling the diagram if the value of a fixed parameter changes
	    if { $new_data_i ne $data_i } {
		set rowi [lindex $new_data_i [expr $Rowi-2]]
		set weight [lindex $rowi 5]
		if { [expr $weight] == 1 } { set need_compile true	}
	    }
	}
    }
}
#    tk_messageBox -icon question -type yesno -title Message  -message "Coljl=$Rowi_last  Colj=$Colj "
#=======================================================================
#  EDITER la case séléctionnée
# ----------------------------------------------------------------------------
proc EditHublot { WindowsID Rowi Colj data_i } {
    global Rowi_last
    global Colj_last
    global Xj
    global labelx
    global ztable
    global ztree

    set Tree_node [$ztree selection get]
    set NRow [llength $data_i]
    set ncolumn [llength $labelx ]

    set dw  61;#[expr [lindex $bb1 2] - [lindex $bb1 0] + 2] ;# 52 width
    set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
    set dw_saf [expr $ncolumn*$dw ]; #393 ; # width of safhe
    set dh_saf [expr $NRow*$dh]; # height of safhe in rows
    set num_char 8 ;# number of characters in Entry

    if { $Rowi>$NRow & $Colj==$ncolumn | $Rowi==$NRow & $Colj>$ncolumn } {
	set Rowi 1;
	set Colj 1;
    } else {	
	if { $Rowi>$NRow    } { set Rowi 1;incr Colj }
	if { $Colj>$ncolumn } { set Colj 1;incr Rowi }
    }

     if { $Rowi<1 & $Colj==1 | $Rowi==1 & $Colj<1 } {
	set Rowi $NRow;
	set Colj $ncolumn;
    } else {	
	if { $Rowi<1 } { set Rowi $NRow;   incr Colj -1}
	if { $Colj<1 } { set Colj $ncolumn;incr Rowi -1 }
    }

    if { $Rowi_last>=0 & $Colj_last>=0 } {
	
	set xtarget [expr [lindex $Xj [expr $Colj-1]] + 1] ; #safhe (yellow)
	set ytarget [expr ($Rowi-1)*$dh+1];                  #safhe (yellow)
	$ztable.safhe create window $xtarget $ytarget -window $ztable.safhe.entry -anchor nw -tags tag_safhe
	$ztable.safhe.entry configure -width 0;#$num_char ;# adjustable width size
 
	focus $ztable.safhe ;#crucial for functioning with TAB
	focus $ztable.safhe.entry

	if { ($Rowi_last*$Colj_last)>0 } {; # saving the old cell in the tree as well as in the safhe
	    
	    #Ecriture dans TREE      
	    set ii  [expr $Rowi_last-1]
	    set jj  [expr $Colj_last+3]
	    
	    set getx [ $ztable.safhe.entry get ]
	    set rowi [lindex $data_i $ii]
	    set rowi [lreplace $rowi $jj $jj $getx ]
	    set data_i [lreplace $data_i $ii $ii $rowi]
	    
	    #Ecriture dans Safhe Edit
	    set i  [expr $Rowi_last-1]
	    set j  [expr $Colj_last-1]		
	    set tagi cl$j$i
	    $ztable.safhe delete text $tagi
	    set xold [expr [lindex $Xj $j]+2]
	    set yold [expr ($i+.5)*$dh]
	    dessine_text $ztable.safhe $xold $yold $getx $tagi
	}
    }
    

    set i  [expr $Rowi-1]
    set j  [expr $Colj-1]		
    set tagi cl$j$i
    $ztable.safhe delete text $tagi ;# effacer ce qui était écrit avant dans la cellule
    $ztable.safhe.entry delete 0 end ; #effacer ce que l'on vient d'écrire dans l'entry

    set Row_i [lindex $data_i [expr  $Rowi-1]]
    set Row_Col_ij [lindex $Row_i [expr  $Colj+3]]
    $ztable.safhe.entry insert end  $Row_Col_ij 
    $ztable.safhe.entry configure -helptext "$Row_Col_ij"     

    set Rowi_last $Rowi
    set Colj_last $Colj
    return $data_i
    #data_i [list $TrName $TrKind $TrID $TrValue $TrWeight $TrMax $TrMin $TrNvalue $TrComment $TrSelected]
}
#================================================================
#=======================================================================
proc clickTree { WindowsID node } {
 
   global NRow_last
   global labelx
   global Xj
   global Rowi_last
   global Colj_last
   global Active_Model
   global XML_LIST
   global DispMode
   global Data_FSel
   global ztree


   if { $node == "" } { clear_screen ; return  } ;# if it's already on the root, clear screen and return

#   set msg1 "[sciGUIName $WindowsID].modelname.msg1"	
   set ncolumn [llength $labelx ]

   #-----------------------------------------------
   set MainNode [ find_MainNode  $WindowsID $node $ztree ]
   
   set Active_Model $MainNode; 
   set Model_name [string range $Active_Model  4 end]  ;# removing "root" from "nodex"
 #  $msg1 configure -text $Model_name

   get_model_info $WindowsID ;# when there are several trees open
   #---------------------------------------------------
   if { $MainNode == "" } { clear_screen ; return  } ;# if it's already on the root, clear screen and return
   #-----------------------------------------------------

   set Rowi_last 0
   set Colj_last 0	
   $ztree selection set $node  
   $ztree opentree  $Active_Model false

   #set var [$ztree selection get]
   #set parent_node  [$ztree parent $node]

  
   if { $DispMode == "Normal" } {
       Update_Selected_in_tree  $WindowsID "Write" ; # Just to write the buffer into the tree when there are several branches
       set data_i [$ztree itemcget $node -data]
       draw_data_i $data_i 
   } else {
       Update_Selected_in_tree  $WindowsID "Write" ; # Just to write the buffer into the tree when there are several branches
       Update_Selected_in_tree  $WindowsID "Read" ; 
       draw_data_i $Data_FSel 	    
   } 
}

#=======================================================================
# Double-click on tree nodes
proc clickTreeD { WindowsID node } {
 
   global last_node
   global ztree
   global DispMode


   if { $node == "" } { return  } ;# if it's already on the root, clear screen and return
   set parent [$ztree parent $node]
   set nparent [string length $parent ]
   set Node_name [string range $node  $nparent end]

   set DispMode  "Normal"  ; #ADDEDD to change the dispmode to normal when doubleclicking on the tree
   DispMode_change $WindowsID

   ScilabEval "name='$Node_name';lname='$last_node';Doubleclick(name,lname); "
   set last_node $Node_name

}

#=======================================================================
proc find_MainNode {  WindowsID node tree } {
   # chnage Active_Model when clicking on the tree branches
   set parent $node   
   set nodex $node
   while { $parent != "root" } { 
       set nodex $parent
       set parent [$tree parent $parent]	    
   }
   
   #set MainNode [string range $nodex  4 end]  ;# removing "root" from "nodex"
   set MainNode $nodex

   return $MainNode
 }
#=======================================================================
proc clear_screen {} {
    global NRow_last
    global labelx
    global Xj
    global Rowi_last
    global Colj_last
    global Active_Model
    global XML_LIST
    global DispMode
    global ztable


    set ncolumn [llength $labelx ]
    
    #---------------------------------------------------------- 
    #---delete lines and texts in the safhe
    for { set i 0 } { $i< $NRow_last } { incr i } {
	$ztable.names delete text xx$i 
	$ztable.names delete text idx$i 
	$ztable.kind  delete text yy$i 
	$ztable.fixed delete text zz$i 
	$ztable.safhe delete text cl0$i 
	$ztable.safhe delete text cl1$i 
	$ztable.safhe delete text cl2$i 	 
	$ztable.safhe delete text cl3$i 	 
	$ztable.safhe delete text cl4$i 	 
	$ztable.safhe delete text cl5$i 	 
	$ztable.safhe delete text cl6$i 	 
  }
    for { set i 0 } { $i<= $NRow_last } { incr i } {
	$ztable.safhe delete  row$i
	$ztable.names    delete  rowi$i 
	$ztable.kind     delete  rowik$i 
	$ztable.fixed    delete  rowif$i 
    }	 
    
    for { set j 0 } { $j<=$ncolumn } { incr j } {
	$ztable.safhe delete col$j
	$ztable.y_axes    delete colj$j	
    }
    
    # draw a white page in the editable safhe
    $ztable.safhe  delete rectangle labels_backgnd	
    $ztable.safhe delete tag_safhe 
}
#=======================================================================
proc draw_data_i { data_i } {
    global NRow_last
    global labelx
    global Xj
    global Rowi_last
    global Colj_last
    global Active_Model
    global XML_LIST
    global DispMode
    global ztable
    global ztree

    set ncolumn [llength $labelx ]
    set NRow [llength $data_i]
    
    set dw  61;#[expr [lindex $bb1 2] - [lindex $bb1 0] + 2] ;# 52 width
    set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
    set dw_saf [expr $ncolumn*$dw ]; #393 ; # width of safhe
    set dh_saf [expr $NRow*$dh]; # height of safhe in rows
    set num_char 8 ;# number of characters in Entry
    
    #---------------------------------------------------------- 
    #---delete lines and texts in the edit safhe
    clear_screen
    #---------------------------------------------------------- 
    
    set NRow_last $NRow
    #data_i=[Name Kind ID Value Weight Max Min Nominal Comment Selected]
    $ztable.safhe create rectangle 0 0 $dw_saf $dh_saf -fill white -width 0 -tag labels_backgnd

    # finding the max length of names
    set maxn $num_char; set LL [expr $maxn*10+5]
    for { set i 0 } { $i < $NRow } { incr i } {
	set rowi  [lindex $data_i $i]		
	set Name [lindex $rowi 0]
	if { [string length $Name] > $maxn } { set maxn [string length $Name];set LL [expr $maxn*10] }
    }

    set k 0
    for { set i 0 } { $i < $NRow } { incr i } {
	set rowi  [lindex $data_i $i]		
	set yrowi [expr ($k+0.5)*$dh]

	set Name [lindex $rowi 0]
	set Name [filterbackCS  $Name]

	$ztable.names create text 5 $yrowi -text $Name -anchor w -fill blue -tag xx$i ; # variable name
	set ID   [lindex $rowi 2]
	set ID [filterbackCS  $ID]

	$ztable.names create text $LL $yrowi -text $ID   -anchor w -fill red -tag idx$i ; # variable id
	
	set Kind [lindex $rowi 1]
	if { $Kind == "fixed_parameter"   } { set OKind "parmtr" }
	if { $Kind == "fixed_variable"    } { set OKind "varibl" }
	if { $Kind == "variable"          } { set OKind "varibl" }
	if { $Kind == "input"             } { set OKind "Input" ; set rowi [ lreplace $rowi 4 4 "Scicos_input"] }
	if { $Kind == "discrete_variable" } { set OKind "Discrt" }

	$ztable.kind  create text 5 $yrowi -text "$OKind" -anchor w -tag yy$i;  # Type (var/par)

	set Ofixed [lindex $rowi 11]
	$ztable.fixed create text 5 $yrowi -text "$Ofixed" -anchor w -tag zz$i;  # fixed (true/false)

	for { set j 0 } { $j< $ncolumn} { incr j } {
	    set loctxt [lindex $rowi [expr $j+4]]
	    set loctxt [filterbackCS  $loctxt]
	    set xrowi  [expr [lindex $Xj $j]+2]
	    set tagi cl$j$i
	    dessine_text $ztable.safhe $xrowi $yrowi $loctxt $tagi
	}
	set k [expr $k+1]
    }
    
     
    set newhall [expr $NRow*$dh]
    set qx [$ztable.names bbox all]
    set wix [expr [lindex $qx 2] - [lindex $qx 0] +2] ;
    #$ztable.names config -width $wix  ; # SIZE adaptation  
    #set srf [$ztable.names cget -scrollregion]
    #set srf [lreplace $srf 2 3 $wix $hall];
    #$ztable.names config -scrollregion $srf
    $ztable.names config -scrollregion "0 0 $wix $newhall"
     if { $wix >100 } { set width_names $wix } else { set width_names 100 }

    set qx [$ztable.kind bbox all]
    set wix [expr [lindex $qx 2] - [lindex $qx 0] -2] ; #width
    #set srf [$ztable.kind cget -scrollregion]
    #set srf [lreplace $srf 3 3 $hall]
    #$ztable.kind config -scrollregion $srf
    $ztable.kind config -scrollregion "0 0 $wix $newhall"
    
    set qx [$ztable.fixed bbox all]
    set wix [expr [lindex $qx 2] - [lindex $qx 0] -2] ;
    #set srf [$ztable.fixed cget -scrollregion]
    #set srf [lreplace $srf 3 3 [expr $NRow*$dh_saf]]
    #$ztable.fixed config -scrollregion $srf
    $ztable.fixed config -scrollregion "0 0 $wix $newhall"
    
    set qx [$ztable.safhe bbox all]
    set wix [expr [lindex $qx 2] - [lindex $qx 0] -2] ;
    #set srf [$ztable.safhe cget -scrollregion]
    #set srf [lreplace $srf 3 3 $hall];
    #$ztable.safhe config -scrollregion $srf
    $ztable.safhe config -scrollregion "0 0 $wix $newhall"
    
   # Horzintal lines
    for { set i 0 } { $i<=$NRow } { incr i } {
	$ztable.safhe    create line 0 [expr $i*$dh] $dw_saf [expr $i*$dh] -tag row$i
	$ztable.names    create line 0 [expr $i*$dh] $width_names [expr $i*$dh] -tag rowi$i 
	$ztable.kind     create line 0 [expr $i*$dh] 80   [expr $i*$dh] -tag rowik$i 
	$ztable.fixed    create line 0 [expr $i*$dh] 80   [expr $i*$dh] -tag rowif$i 
    }
        
    # Vertical lines
    for { set j 0 } { $j<=$ncolumn } { incr j } {
	$ztable.safhe create line [expr $j*$dw] 0 [expr $j*$dw] [expr $dh*$NRow] -tag col$j
    }
}
#=======================================================================
proc dessine_text { text_zone xi yi texti tagi} {
    
    set dw  61;#[expr [lindex $bb1 2] - [lindex $bb1 0] + 2] ;# 52 width
    set dh  19;#[expr [lindex $bb1 3] - [lindex $bb1 1] + 2] ;# 18 height
    
    set loctxt $texti
    set loccol black
    set ok 1
    while { $ok==1 } {    
	$text_zone create text $xi $yi -text "$loctxt" -anchor w -width $dw -tag $tagi -fill $loccol
	set bb [$text_zone bbox $tagi]
	set ldh [expr [lindex $bb 3] - [lindex $bb 1]]			 
	if { $ldh>=$dh } {			
	    set lx  [string length "$loctxt"]
	    set loctxt [string range "$loctxt" 0 [expr $lx-2]]
	    $text_zone  delete text $tagi
	    set loccol blue
	} else {
	    set ok 0
	}
    }
}
#=======================================================================
proc remove_from_tree { WindowsID SearchNode } {  
    global XML_LIST
    global ztree

    set nXML [llength $XML_LIST ]

    set index ""
    for { set j 0 } { $j<$nXML } { incr j } {
	set XML_LIST_j  [lindex $XML_LIST $j]
	if { [lindex $XML_LIST_j 0] eq $SearchNode } {
	    set index $j
	    set XML_LIST [lreplace $XML_LIST $index $index]                 
	    $ztree delete  $SearchNode
	    break
	}     
    }


    if { $nXML != "1" } { 
	set XML_LIST_j  [lindex $XML_LIST 0];
	set next_Active_Model [lindex $XML_LIST_j 0] 
    } else { set next_Active_Model "" }
    
    return $next_Active_Model

  }
#=======================================================================
proc fill_tree  { WindowsID xmllist } {  
    global Active_Model   
    global ztree
    
    #$ztree delete [$ztree nodes root];# kill all childrens of root
#
    set Main_node [SearchList $WindowsID $xmllist $ztree root];  

    return $Main_node 
}
#=======================================================================
proc SearchList { WindowsID xlist tree xparent } {
global scilabpath
    set xvalue "value-"    
    set typex [ lindex $xlist 0 ]

    if { $typex == "model" } {
        set model_body [ lindex $xlist 2 ]
        set name_of_model     [ lindex $model_body 0 ] 
        set element_of_model  [ lindex $model_body 1 ]
        set equation_of_model [ lindex $model_body 2 ]     

	# just for extraction of number of equations...
        #set Equations [lindex $equation_of_model 2]
	#set NEQ [llength $Equations]

        #name extraction
        set nm [lindex [lindex $name_of_model 2 ] 0]; 
	set ModelName [string trim [string map {#text ""} $nm] "\{ \}"]
  
        set Main_node "$xparent$ModelName"

        if { [catch {$tree insert end $xparent $Main_node -image [image create photo -file "$scilabpath/modules/scicos/tcl/scicos2.gif"] -text $ModelName }] } {	    
	    set kids [$tree nodes root]
	    set exist "no"
	    foreach  kid   $kids {    
		if { $kid == $Main_node } { set exist "yes";break }
	    }
	    if { $exist == "yes" } {
		set answer [tk_messageBox -icon question -type yesno -title Message  -message "This model is already open!\n\rDo you want to reopen it?"]
		if { $answer == "yes" }  {
		    $tree delete  $Main_node
		    if { [catch {$tree insert end $xparent $Main_node -image [image create photo -file "$scilabpath/modules/scicos/tcl/scicos2.gif"] -text $ModelName }] } {
			return "" 
		    }
		} else { 
		    return "" 
		} 
	    } else {
		tk_messageBox -icon error -type ok -title Message  -message "The XML file is corrupted!"
		return "" 
	    }
	}
	
        set data_i {}
        $tree itemconfigure $Main_node  -data $data_i

        #set Element [lindex [lindex $element_of_model 2] 0]

	set paramnodes [lindex $element_of_model 2]
        set nparamnodes [llength $paramnodes]

        for { set j 0 } { $j<$nparamnodes } { incr j } {
	    set pNode   [ lindex $paramnodes $j ]  
	    SearchList $WindowsID $pNode  $tree  $Main_node
        }

         return $Main_node 
       }
    #.....................       
    if { $typex == "struct" } {
        
        set structure_body [ lindex $xlist 2 ]
        set name_of_struct    [ lindex $structure_body 0 ]
        set Structure         [ lindex $structure_body 1 ]
	
        #name extraction
        set nm [lindex [lindex $name_of_struct 2 ] 0]; 
 	set StructName [string trim [string map {#text ""} $nm] "\{ \}"]

        set SVname "$xparent$StructName" 
	if { [catch {$tree insert end $xparent $SVname  -image [image create photo -file "$scilabpath/modules/scicos/tcl/scicos.gif"] -text $StructName }] } {  return "" }
#      	tk_messageBox -icon question -type yesno -title Message  -message "NEQ=$SVname "

        SearchList $WindowsID  $Structure  $tree $SVname  
        return $tree
       }
    #.....................
    if { $typex == "subnodes" } {
        set allnodes [ lindex $xlist 2 ]
        set nnodes [llength $allnodes]
        
        for { set j 0 } { $j<$nnodes } { incr j } {
	    set Node   [ lindex $allnodes $j ]        
	    SearchList $WindowsID $Node  $tree  $xparent
        }
        return $tree        
    }
    #.....................
    if { $typex == "terminal"} {  set terminal_body [ lindex $xlist 2 ]
        # initialize
        set TrName     "noName"
        set TrKind     ""
        set TrID       "noID"
        set TrFixed    ""
        set TrValue    ""
        set TrWeight   ""
        set TrMax      ""
        set TrMin      ""
        set TrNominal  "1"
        set TrComment  ""
        set TrSelected ""
        set TrFixedOrig ""
        set TrOutput   ""
        set TrWeight_tmp ""
        set TrValue_orig ""

        set nitem [llength $terminal_body]
        for { set j 0 } { $j<$nitem } { incr j } {
        set terj [ lindex $terminal_body $j ]
        set ter_node [lindex $terj 0]
  
	    switch $ter_node  {		
		name { 
		    set ter_node_2  [lindex [lindex $terj 2 ] 0];
		    set TrName [string trim [string map {#text ""} $ter_node_2 ] "\{ \}"]
		}		

		kind { 
		    set ter_node_2  [lindex [lindex $terj 2 ] 0]; 
		    set TrKind [string trim [string map {#text ""} $ter_node_2 ] "\{ \}"]
		}

		fixed   { 
		    set ter_node_1  [lindex $terj 1 ]; 
                    set  ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrFixed  [string range $ter_node_1 [string length $xvalue] end]
		    if { $TrFixed eq "{}" } {set TrFixed ""}
		} 

		id   { 
		    set ter_node_2  [lindex [lindex $terj 2 ] 0]; 
		    set TrID [string trim [string map {#text ""} $ter_node_2 ] "\{ \}"]
		}  		

		comment {
		    set ter_node_1  [lindex $terj 1 ]; 
                    set  ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrComment [string range $ter_node_1 [string length $xvalue] end]
		    set TrComment [string map { \} "" \{ "" } $TrComment]
		}		

		initial_value { 
		    set ter_node_1  [lindex $terj 1 ]; 
		    set TrValue [string range $ter_node_1 [string length $xvalue] end]                      
		    if { $TrValue eq "{}" } {set TrValue ""}
		} 
 			 
		weight {
		    set ter_node_1  [lindex $terj 1 ]; 
                    set ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrWeight [string range $ter_node_1 [string length $xvalue] end]                      
    		    if { $TrWeight eq "{}" } {set TrWeight ""}   
		} 

		max { 
		    set ter_node_1  [lindex $terj 1 ]; 
                    set  ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrMax [string range $ter_node_1 [string length $xvalue] end]                      
		    if { $TrMax eq "{}" } {set TrMax ""}   		    
		}     
		min { 
		    set ter_node_1  [lindex $terj 1 ]; 
                    set  ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrMin [string range $ter_node_1 [string length $xvalue] end]                      
                    if { $TrMin eq "{}" } {set TrMin ""}   
		}     

		nominal_value { 
		    set ter_node_1  [lindex $terj 1 ]; 
		    set TrNominal [string range $ter_node_1 [string length $xvalue] end]                      
   		    if { $TrNominal eq "{}" } {set TrNominal ""}   		    
		}     

		selected { 
		    set ter_node_1  [lindex $terj 1 ]; 
                    set  ter_node_1 [string trim $ter_node_1 "\{ \}"] 
		    set TrSelected [string range $ter_node_1 [string length $xvalue] end]                      
                    if { $TrSelected eq "{}" } {set TrSelected ""}   
		}

		fixed_orig {
		    set ter_node_2  [lindex [lindex $terj 2 ] 0]; 
		    set TrFixedOrig [string trim [string map {#text ""} $ter_node_2 ] "\{ \}"]
		}

		output {
		    set TrOutput  "out"
		}

	    }
        }
  	#$tree insert end $xparent "$xparent$TrName" -image [Bitmap::get file] -text $TrName
	set data_i [$tree itemcget $xparent -data]

        if { $TrFixedOrig== "" } { set TrFixedOrig $TrFixed } 
            if { $TrFixedOrig== "" } {
	      if { $TrKind == "fixed_parameter" } { 
	  	set TrFixed "true" 
	  	set TrFixedOrig "false" 
	    } else { 
	 	set TrFixed "false" 
		set TrFixedOrig "false" 
	    }
        } 

	if { $TrWeight == "" } {  
	    if { $TrFixed == "true" }  { set TrWeight 1 }
	    if { $TrFixed == "false" } { set TrWeight 0 }
	    if { $TrFixed == "-" }     { set TrWeight 0 }	    
	    if { $TrKind == "fixed_parameter" } { set TrWeight 1  }
	}

	if { $TrSelected== "" } { set TrSelected "n"
	    if  {$TrValue ne "" } { set TrSelected "y" } 
	    if  {$TrKind == "discrete_variable" } { set TrSelected "y" } 
	    if  {$TrKind == "input" } { set TrSelected "y" } 
	    if  {$TrKind == "fixed_parameter" } { set TrSelected "y" } 

	} 
	if { $TrValue== "" } { set TrValue $TrNominal }  
	if { $TrValue== "" } { set TrValue 0.0 } 

        set TrValue_orig $TrValue        
	set data_ij [list $TrName $TrKind $TrID $TrFixed $TrValue $TrWeight $TrMax $TrMin $TrNominal $TrComment $TrSelected $TrFixedOrig $TrOutput $TrValue_orig]


	#labels Name Kind ID status Value Weight Max Min Nominal Comment Selection]	
	lappend data_i $data_ij
	
	$tree itemconfigure $xparent -data $data_i 
	
        return $tree          
    }
}
#=======================================================================
 proc Read_tree  { WindowsID active_file } {
     global XML_LIST
     global ztree

     set nXML [llength $XML_LIST ]
     for { set j 0 } { $j<$nXML } { incr j } {
          set XML_LIST_j  [lindex $XML_LIST $j]
          set dd [lsearch $XML_LIST_j $active_file]  
          if {$dd >=0 } { break }     
     }
     set xmllist [lindex $XML_LIST_j 2]    
   # tk_messageBox -icon error -type ok -title "in"  -message "$active_file >$xmllist"
     set read_xmllist [Fill_List $WindowsID $xmllist $ztree root];
    

     return $read_xmllist
 }
#=======================================================================
proc Fill_List { WindowsID xlist tree xparent } {
    global ParEmbedded

    set xvalue "value "
    
    set typex [ lindex $xlist 0 ]
    
    if { $typex == "model" } {
        set model_body [ lindex $xlist 2 ]
        set name_of_model     [ lindex $model_body 0 ] 
        set element_of_model  [ lindex $model_body 1 ]
        set equation_of_model [ lindex $model_body 2 ]     
	
        #name extraction
	set nm [lindex [lindex $name_of_model 2 ] 0]; 
	set ModelName [string trim [string map {#text ""} $nm ] "\{ \}"]

	set Main_node "$xparent$ModelName"
	    
        set paramnodes [lindex $element_of_model 2]
        set nparamnodes [llength $paramnodes]
        for { set j 0 } { $j<$nparamnodes } { incr j } {
	    set pNode   [ lindex $paramnodes $j ]  
	    set new_pNode [Fill_List $WindowsID $pNode $tree $Main_node ]
	    set paramnodes [lreplace $paramnodes $j $j $new_pNode]
        }

        set element_of_model [lreplace $element_of_model 2 2 $paramnodes]

        set model_body [lreplace $model_body 1 1  $element_of_model ]
        
        set xlist [lreplace $xlist 2 2  $model_body ]
        return $xlist
       }
    #.....................       
    if { $typex == "struct" } {
        set structure_body [ lindex $xlist 2 ]
        set name_of_struct [ lindex $structure_body 0 ]
        set Structure      [ lindex $structure_body 1 ]
	
        #name extraction
        set nm [lindex [lindex $name_of_struct 2 ] 0]; 
	set StructName [string trim [string map {#text ""} $nm] "\{ \}"]
        set SVname "$xparent$StructName"
               
        set new_Structure  [Fill_List $WindowsID  $Structure  $tree $SVname]
                
        set structure_body [lreplace $structure_body 1 1  $new_Structure ]    
        set xlist          [lreplace $xlist          2 2  $structure_body]

  
        return $xlist
       }
    #.....................
    if { $typex == "subnodes" } {
        set allnodes [ lindex $xlist 2 ]
        set nnodes [llength $allnodes]
        
        for { set j 0 } { $j<$nnodes } { incr j } {
	        set Node     [lindex $allnodes $j ]        
	        set new_Node [Fill_List $WindowsID $Node $tree $xparent ]
	        set allnodes [lreplace $allnodes $j $j $new_Node]     	        
         }	   
        set xlist [lreplace $xlist 2 2 $allnodes]
        return $xlist        
    }
    #.....................................................................
    if { $typex == "terminal"} {  
        set terminal_body [ lindex $xlist 2 ]
	
	set data_i [$tree itemcget $xparent -data]  
	set nitem [llength $terminal_body]         
        for { set j 0 } { $j<$nitem } { incr j } {
	    set terj [ lindex $terminal_body $j ]
	    set ter_node [lindex $terj 0]
	    if { $ter_node == "id" }   { 
		set ter_node_2  [lindex [lindex $terj 2 ] 0]; 
		set TrID [string trim [string map {#text ""} $ter_node_2] "\{ \}"]
	    } 
          }
	
        for { set j 0 } { $j<[llength $data_i] } { incr j } {
	    set data_ij  [lindex $data_i $j]
	    set dd [lsearch $data_ij $TrID]  
	    if {$dd >=0 } { break }
        }
  
  	 # data_i [$TrName $TrKind $TrID $TrFixed $TrValue $TrWeight $TrMax $TrMin $TrNominal $TrComment $TrSelected]

	set TrName [lindex $data_ij 0]
	set TrKind [lindex $data_ij 1]
	set TrID [lindex $data_ij 2]
	set TrFixed [lindex $data_ij 3]
	set TrValue [lindex $data_ij 4]
	set TrWeight [lindex $data_ij 5]
	set TrMax [lindex $data_ij 6]
	set TrMin [lindex $data_ij 7]
	set TrNominal [lindex $data_ij 8]
	set TrComment [lindex $data_ij 9]
	set TrSelected [lindex $data_ij 10]
	set TrFixedOrig [lindex $data_ij 11]
	set TrOutput   [lindex $data_ij 12]
	set TrValue_orig $TrValue

	set TrComment \"[string map { \} "" \{ "" \" "" } $TrComment]\"
	if { $TrComment eq "" } {set TrComment "{}"} 
			  
	if { $TrKind == "fixed_parameter" |  $TrKind == "variable" } {
	    if { [Ceil $TrWeight] == "1" |  $TrWeight == "1" } { 
		set TrFixed "true";
		if { $ParEmbedded == "1" } { 
		    #set TrKind "parameter";
		} else {
		    #set TrKind "variable"
		}
	    } else { 		
		set TrFixed "false";
		#set TrKind "variable"		
	    }
	};# else {
	   # if { $TrKind == "input" } { set TrKind $TrKindOrig }
	#}
        set new_xlist     "terminal {} {"
        append new_xlist  " {name {} {{#text $TrName}}}"
        append new_xlist  " {kind {} {{#text $TrKind}}}"
        append new_xlist  " {id {} {{#text $TrID}}}"
	append new_xlist  " {fixed {value $TrFixed} {}}" 

	if { [regexp -nocase  -all {[^0-9 . e \- \+]} $TrValue ]== 0 } {
           if [catch {set TrValue  [expr double($TrValue)]}]==1 {
	       tk_messageBox -icon error -type ok -title "error in TrName"  -message "The value of \"$TrID\" ($TrValue) is not a valid number"
	       return "";
	   } 
	} 

        append new_xlist  " {initial_value {value $TrValue} {}}"

        if [catch {set TrWeight  [expr double($TrWeight)]}]==1 {
	    tk_messageBox -icon error -type ok -title "Error in $TrName"  -message "The weight value of \"$TrID\" ($TrWeight) is not valid number"
	    return "";
	}
        append new_xlist  " {weight {value $TrWeight} {}}"

        if { $TrMax ne "" } { 
         if [catch {set TrMax  [expr double($TrMax)]}]==1 {
       	    tk_messageBox -icon error -type ok -title "Error in $TrName"  -message "The max value of \"$TrID\" ($TrMax) is not valid number"
	     return "";
	 }
        }
        append new_xlist  " {max {value $TrMax} {}}"

        if { $TrMin ne "" } { 
         if [catch {set TrMin  [expr double($TrMin)]}]==1 {
       	    tk_messageBox -icon error -type ok -title "Error in $TrName"  -message "The min value of \"$TrID\" ($TrMin) is not valid number"
       	    return "";
	 } 
        }
        append new_xlist  " {min {value $TrMin} {}}"

        if { $TrNominal ne "" } { 
         if [catch {set TrNominal  [expr double($TrNominal)]}]==1 {
       	    tk_messageBox -icon error -type ok -title "Error in $TrName"  -message "The nominal value of \"$TrID\" ($TrNominal) is not valid number"
       	    return "";
	 } 
        }
        append new_xlist  " {nominal_value {value $TrNominal} {}}"


       if { $TrComment ne "" } { 
	   if { [regexp {\\}  $TrComment match)]==1 } {
       	    tk_messageBox -icon error -type ok -title "Error in $TrName"  -message "The comment of \"$TrID\" ($TrComment) is not valid (\\)."
       	    return "";
	   }
       }
       append new_xlist  " {comment {value $TrComment} {}}"

       if { $TrSelected ne "y" } { 	   
	   if { $TrSelected ne "n" } { 
	       set TrSelected "n"
	   }
       }
       append new_xlist  " {selected {value $TrSelected} {}}"
       # append new_xlist  " {fixed_orig {} {{#text $TrFixedOrig}}}"
       #append new_xlist  " {fixed_orig {value $TrFixedOrig} {}}" 

       if { $TrOutput == "out" } { append new_xlist  " {output {} {}}" }

       append new_xlist  "}"

       return $new_xlist          
    }
}
#=======================================================================
proc Update_Selected_in_tree { WindowsID RW } {
    global Data_FSel
    global DispMode 
    global Data_FSel_pm 
    global ztree
    global SearchedText
    global Active_Model
    
    set RootNode "root"
    set SelectedNode [$ztree selection get]
  
    switch $RW {

	"Read" {
	    set Data_FSel {}
	    Read_Selected_from_tree_kids $WindowsID $ztree $RootNode
	    set Data_FSel_pm [list $DispMode $SelectedNode $SearchedText]
	}

	"Write" {
	    if { [llength $Data_FSel] != 0 } {
		Write_Selected_to_tree_kids $WindowsID $ztree $RootNode 
		set Data_FSel_pm [list "Normal" $SelectedNode $SearchedText]  

	    }
	}
    }

}
#=======================================================================
proc  Read_Selected_from_tree_kids { WindowsID tree parent} {
    global labelx
    global Data_FSel
    global Active_Model
    global DispMode
    global SearchedText
    global IncidenceMatrixSimplified

    set ncolumn [llength $labelx ]

    switch $DispMode {
	
	"Selected" {
	    set node [$tree selection get]
	    set data_i [$tree itemcget $node -data]  
	    set NRow [llength $data_i]
	    for { set i 0 } { $i < $NRow } { incr i } {
		set rowi  [lindex $data_i $i]	
		set loctxt [lindex $rowi [expr $ncolumn+3]]
		if { $loctxt == "y" } { lappend Data_FSel $rowi }
	    } 
	}
	
	"FSelected" {
	    set kids [$tree nodes $parent]
	    foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
		if { $MainNode != $Active_Model } { continue }
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set loctxt [lindex $rowi [expr $ncolumn+3]] ; # "Selected" column
		    if { $loctxt == "y" } { lappend Data_FSel $rowi }
		} 		
		Read_Selected_from_tree_kids  $WindowsID $tree $kid 
	    }
	}
	
	"FChanged" {

	  set kids [$tree nodes $parent]

	  foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]
		if { $MainNode != $Active_Model } { continue }

		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]

		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set weight [lindex $rowi 5]
		    set kind  [lindex $rowi 1] 
		    set changed 0
		    if { [Ceil $weight] == "1" |  $weight == "1" } { 
		   	  if { $kind == "fixed_parameter" } { set changed 0}
		   	  if { $kind == "variable" } { set changed 1}
		   	  if { $kind == "input" } { set changed 0}		   	  
		    } else { 
			        if { $kind == "fixed_parameter" } { set changed 1}
			        if { $kind == "variable" } { set changed 0}
			        if { $kind == "input" } { set changed 0}
		    }
		    if { $changed == 1 } { lappend Data_FSel $rowi }
		}
		Read_Selected_from_tree_kids   $WindowsID  $tree $kid 
	 }
	}
	
	"Search" {
	    
	    set kids [$tree nodes $parent]
  
	    foreach  kid  $kids  {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
		if { $MainNode != $Active_Model } { continue }
		
		set data_i [$tree itemcget $kid -data]		
		set NRow [llength $data_i]	 	
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set loctxt [lindex $rowi 2] ; # "idi" column
		    #set inx [string last "." $loctxt]
		    #set inx2  [string last "." [string range  $loctxt 0 [expr $inx-1]]]
		    #set inx3 [string length $loctxt]
		    #set loctxt [string range $loctxt $inx2 $inx3] ;# searching in name+its parent
		    # set loctxt "aaaa.bbb.ccc.ddd"
		    #  => search  .ccc.ddd
		    if { [regexp ($SearchedText) $loctxt match x] > 0  } { lappend Data_FSel $rowi }
		}	   		      
		Read_Selected_from_tree_kids    $WindowsID $tree $kid 
	    }
	 }

        "Onlypars" {
	   
  	      set kids [$tree nodes $parent]
   	     foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]
		if { $MainNode != $Active_Model } { continue }

		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]

		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set weight [lindex $rowi 5] 
		    if { [Ceil $weight] == "1" |  $weight == "1" } { lappend Data_FSel $rowi }
		}
		Read_Selected_from_tree_kids   $WindowsID  $tree $kid 
	      }

	  }

	"Simplified" {
	    set Svariables [lindex $IncidenceMatrixSimplified 2]
	    set kids [$tree nodes $parent]	    
	    foreach  kid  $kids  {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
		if { $MainNode != $Active_Model } { continue }		
		set data_i [$tree itemcget $kid -data]		
		set NRow [llength $data_i]	 	
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set loctxt [lindex $rowi 2] ; # "id" column
		    if { [lsearch -exact $Svariables $loctxt] >=0 } { lappend Data_FSel $rowi }
		}
		Read_Selected_from_tree_kids    $WindowsID $tree $kid 
	    }
	}

 	    
    }
}
#=======================================================================
proc  Write_Selected_to_tree_kids { WindowsID tree parent} {
    global labelx
    global Active_Model
    global Data_FSel
    global Data_FSel_pm 
    global IncidenceMatrixSimplified

    set ncolumn [llength $labelx ]
    
    set P_DispMode     [lindex $Data_FSel_pm 0]
    set P_node         [lindex $Data_FSel_pm 1]
    set P_SearchedText [lindex $Data_FSel_pm 2]
    
    switch $P_DispMode {
	
	"Selected" {
	    set data_i [$tree itemcget $P_node -data]  
	    set NRow [llength $data_i]
	    for { set i 0 } { $i < $NRow } { incr i } {
		set rowi  [lindex $data_i $i]	
		set loctxt [lindex $rowi [expr $ncolumn+3]]
		if { $loctxt == "y" } {
		    set new_rowi  [lindex $Data_FSel 0]
		    set data_i [lreplace $data_i $i $i $new_rowi]
		    $tree itemconfigure $P_node -data $data_i 
		    set Data_FSel [lreplace $Data_FSel 0 0]
		}
	    }
	}

	"FSelected" {
	    
	    set kids [$tree nodes $parent]
	    set MainNode_P [ find_MainNode $WindowsID $P_node $tree ]	  
	    foreach  kid   $kids {    
		set MainNode [ find_MainNode $WindowsID $kid $tree ]	   			        
		if { $MainNode != $MainNode_P } { continue }
		
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set loctxt [lindex $rowi [expr 3+$ncolumn]]; # "Selected" column (the last column)
		    if { $loctxt == "y" } { 
			set new_rowi  [lindex $Data_FSel 0]
			set data_i [lreplace $data_i $i $i $new_rowi]
			$tree itemconfigure $kid -data $data_i 
			set Data_FSel [lreplace $Data_FSel 0 0]
		    }
		} 
		
		Write_Selected_to_tree_kids  $WindowsID $tree $kid 
	    }
	}
	
	"FChanged" {

	    set kids [$tree nodes $parent]
	    set MainNode_P [ find_MainNode $WindowsID $P_node $tree ]	  
	    foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	   			        
		if { $MainNode != $MainNode_P } { continue }
		
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set weight [lindex $rowi 5] 
		    set kind [lindex $rowi 1]  
		    set changed 0
		    if { [Ceil $weight] == "1" |  $weight == "1" } { 
		   	  if { $kind == "fixed_parameter" } { set changed 0}
		   	  if { $kind == "variable" } { set changed 1}
		   	  if { $kind == "input" } { set changed 0}		   	  
		    } else { 
			        if { $kind == "fixed_parameter" } { set changed 1}
			        if { $kind == "variable" } { set changed 0}
			        if { $kind == "input" } { set changed 0}
		    }
		    if { $changed == 1 } {
		     set new_rowi  [lindex $Data_FSel 0]
			   set data_i [lreplace $data_i $i $i $new_rowi]
			  $tree itemconfigure $kid -data $data_i 
			set Data_FSel [lreplace $Data_FSel 0 0]
		    }
		} 
		
		Write_Selected_to_tree_kids   $WindowsID $tree $kid 
	    }
	}
	
	
	"Search" {
	    
	    set kids [$tree nodes $parent]
	    set MainNode_P [ find_MainNode  $WindowsID $P_node $tree ]	  
	    foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	   			        
		if { $MainNode != $MainNode_P } { continue }
		
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]			    
		    set loctxt [lindex $rowi 2] ; # "id" column
		    if {  [regexp ($P_SearchedText) $loctxt match x] > 0  } {
			set new_rowi  [lindex $Data_FSel 0]
			set data_i [lreplace $data_i $i $i $new_rowi]
			$tree itemconfigure $kid -data $data_i 
			set Data_FSel [lreplace $Data_FSel 0 0]
		    }
		} 
		
		Write_Selected_to_tree_kids    $WindowsID $tree $kid 
	    }
	}

        "Onlypars" {
 	   	    set kids [$tree nodes $parent]
	            set MainNode_P [ find_MainNode $WindowsID $P_node $tree ]	  
       	    foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	   			        
		if { $MainNode != $MainNode_P } { continue }
		
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]	
		    set weight [lindex $rowi 5] 
		    if { [Ceil $weight] == "1" |  $weight == "1" } { 
     		          set new_rowi  [lindex $Data_FSel 0]
			  set data_i [lreplace $data_i $i $i $new_rowi]
			  $tree itemconfigure $kid -data $data_i 
			  set Data_FSel [lreplace $Data_FSel 0 0]
		    }
		}
		Write_Selected_to_tree_kids   $WindowsID $tree $kid 
	    }
	  }

	"Simplified" { 
	    set Svariables [lindex $IncidenceMatrixSimplified 2]
	    set kids [$tree nodes $parent]
	    set MainNode_P [ find_MainNode  $WindowsID $P_node $tree ]	  
	    foreach  kid   $kids {    
		set MainNode [ find_MainNode  $WindowsID $kid $tree ]	   			        
		if { $MainNode != $MainNode_P } { continue }
		
		set data_i [$tree itemcget $kid -data]  
		set NRow [llength $data_i]
		for { set i 0 } { $i < $NRow } { incr i } {
		    set rowi  [lindex $data_i $i]			    
		    set loctxt [lindex $rowi 2] ; # "id" column
		    if { [lsearch -exact $Svariables $loctxt] >=0 } { 
			set new_rowi  [lindex $Data_FSel 0]
			set data_i [lreplace $data_i $i $i $new_rowi]
			$tree itemconfigure $kid -data $data_i 
			set Data_FSel [lreplace $Data_FSel 0 0]
		    }
		} 		
		Write_Selected_to_tree_kids    $WindowsID $tree $kid 
	    }
	}
	
    }
    return $tree
}
#------------------------------------------------
#-------------------------------------------
#  tk_messageBox -icon question -type yesno -title Message  -message "$sure???"
#============================================
proc get_model_info { WindowsID } {
    global Active_Model
    global ztree
    global XML_LIST
    global DispMode
   
    if { $Active_Model == "" } {     
	set w "[sciGUIName $WindowsID].status";
	$w.nequation configure -text "Equation=0" ;
	$w.nunknown  configure -text "Unknowns=0";
	$w.nparfixed configure -text "Fixed Par=0";
	$w.nparfree  configure -text "Relxd Par=0";
	$w.nvarfixed configure -text "Fixed Var=0" ;
	$w.nvarfree  configure -text "Relxd Var=0" 
	$w.ndiscrete configure -text "Discrete=0"
	$w.ninputs   configure -text "Input=0"
	$w.nderivats configure -text "Diff Ste=0" 
	return;  }
    #----------------- reading Number of equation in the xml list of the current Model
    set nXML [llength $XML_LIST ]
    set index -1
    for { set j 0 } { $j<$nXML } { incr j } {
	set XML_LIST_j  [lindex $XML_LIST $j]
	if { [lindex $XML_LIST_j 0] eq  $Active_Model } {
	    set index $j
	    break
	}     
    }

    set xmllist [lindex $XML_LIST_j 2]
    set typex [ lindex $xmllist 0 ]
    set NEQ "error"
    if { $typex == "model" } {
        set model_body [ lindex $xmllist 2 ]
        set equation_of_model [ lindex $model_body 2 ]
        set Equations [lindex $equation_of_model 2]
	set NEQ [llength $Equations]	
    }
    #-----------------

    set Info [list 0 0 0 0 0 0 0 0]
    set Info [get_model_info_kids $WindowsID  $ztree root $Info]

    set NPF  [lindex $Info 1]
    set NPL  [lindex $Info 2]
    set NVF  [lindex $Info 3]
    set NVL  [lindex $Info 4]
    set NDIS [lindex $Info 5]
    set NIN  [lindex $Info 6]
    set NDIF [lindex $Info 7]

    set w "[sciGUIName $WindowsID].status"
    set NVAR [expr $NPL+$NVL+$NDIS]    
    if  { $NEQ!= $NVAR } {
	$w.nequation configure -text "Equation=$NEQ" -fg red
	$w.nunknown configure -text "Unknowns=[expr $NPL+$NVL]" -fg red 
	set square false
    } else {
	$w.nequation configure -text "Equation=$NEQ" -fg blue
	$w.nunknown configure -text "Unknowns=[expr $NPL+$NVL]" -fg blue
	set square true
    }

    $w.nparfixed configure -text "Fixed Par=$NPF"
    $w.nparfree  configure -text "Relxd Par=$NPL"
    $w.nvarfixed configure -text "Fixed Var=$NVF" 
    $w.nvarfree  configure -text "Relxd Var=$NVL" 
    $w.ndiscrete configure -text "Discrete=$NDIS" 
    $w.ninputs   configure -text "Input=$NIN" 
    $w.nderivats configure -text "Diff St=$NDIF" 

    return $square

}
#	tk_messageBox -icon question -type yesno -title Message  -message "NEQ=$NEQ "

proc get_model_info_kids {  WindowsID tree parent Info } {
    global Active_Model
    
    set kids [$tree nodes $parent]
    foreach  kid   $kids {   

	set NEQ  [lindex $Info 0]
	set NPF  [lindex $Info 1]
	set NPL  [lindex $Info 2]
	set NVF  [lindex $Info 3]
	set NVL  [lindex $Info 4]
	set NDIS [lindex $Info 5]
	set NIN  [lindex $Info 6]
	set NDIF [lindex $Info 7]

	set MainNode [ find_MainNode  $WindowsID $kid $tree ]	       
	if { $MainNode != $Active_Model } { continue }
	set data_i [$tree itemcget $kid -data]  
	set NRow [llength $data_i]

	for { set i 0 } { $i < $NRow } { incr i } {
	    set rowi  [lindex $data_i $i]
	    incr NEQ
	    set kind   [lindex $rowi 1 ]
	    set weight [lindex $rowi 5 ] ; # "weight" column
	    set idi    [lindex $rowi 2] ; # "ID" column

	    if { $kind == "fixed_parameter" & $weight == "1" } { incr NPF }
	    if { $kind == "fixed_parameter" & $weight != "1" } { incr NPL }
	    if { $kind == "variable" & $weight == "1" } { incr NVF }
	    if { $kind == "variable" & $weight != "1" } { incr NVL }
	    if { $kind == "discrete_variable" } { incr NDIS }
	    if { $kind == "input" } { incr NIN }
	    
	    if { [regexp {__der_(\w*)} $idi -> restOfWord] > 0  } {
		incr NDIF
	    }

	} 	
	set Info [lreplace $Info 0 0  $NEQ]
	set Info [lreplace $Info 1 1  $NPF]
	set Info [lreplace $Info 2 2  $NPL]
	set Info [lreplace $Info 3 3  $NVF]
	set Info [lreplace $Info 4 4  $NVL]
	set Info [lreplace $Info 5 5  $NDIS]
	set Info [lreplace $Info 6 6  $NIN]
	set Info [lreplace $Info 7 7  $NDIF]

	set Info [get_model_info_kids $WindowsID  $tree $kid $Info]

    }
    return $Info

}
#___________________________________________________________

proc read_incidence {WindowsID filename  } {

    if { [catch { set fileid [open $filename r] }] } {	      
	tk_messageBox -icon error -type ok -title "Error Message"  -message "Unable to open $filename"
	return nok
    }
    set dataxml [read $fileid]  
    close $fileid      

    if { [catch { set incidxmllist [xml_to_list $dataxml] }] } {	      
	tk_messageBox -icon error -type ok -title "Error Message"  -message "Unable to read iXML $filename"
	return nok
    }  

    set IncidenceMatrix [fill_incidence $incidxmllist]
    return $IncidenceMatrix
}
#-----------------------------------
proc fill_incidence { incidxmllist } {    
    set typex [ lindex $incidxmllist 0 ]    
    if { $typex != "model" } {
    	tk_messageBox -icon error -type ok -title "XML error"  -message "Error in the incidence XML file x1 ($typex)"
      return []
    }
    
    set all_model [ lindex $incidxmllist 2 ]
    set nfield [llength $all_model]

    set nimpvar 0 
    set nrels 0
    set impVariables {}
    set expVariables {}
    set Parameters {}
    set impRelations {}
    set expRelations {}
    
    for { set K 0 } { $K<$nfield } { incr K } {
	set Field  [ lindex $all_model $K ] 	
	set typeK [ lindex $Field 0 ]
	#-------------------------------------    
	if { $typeK == "model_info" } { 	    
	}
	#-------------------------------------    
	if { $typeK == "identifiers" } { 	    
	    set  Variable_list $Field 
	    set vars [lindex $Variable_list 2 ]
	    set nvars [llength $vars]
	    
	    for { set j 0 } { $j<$nvars } { incr j } {
		set var_j  [ lindex $vars $j ] 
		set pvtype [lindex $var_j 0]
		set Var_j  [lindex [lindex [lindex $var_j 2] 0] 1]
		if { $pvtype == "parameter"         } { set Parameters   [linsert $Parameters   end $Var_j] }
		if { $pvtype == "explicit_variable" } { set expVariables [linsert $expVariables end $Var_j] }
		if { $pvtype == "implicit_variable" } { set impVariables [linsert $impVariables end $Var_j] 
		    incr nimpvar}
	    }        
	}
	#-------------------------------------
	if { $typeK == "identifiers" } { 	    
	    set rels [lindex $Field 2 ]
	    set nrels [llength $rels]
	    for { set j 0 } { $j<$nrels } { incr j } {
		if { [lindex [ lindex $rels $j ] 0] == "implicit_relation" } {
		    set rel_j  [lindex [ lindex $rels $j ] 2] 
		    set nrel_j [llength $rel_j]
		    set Relation_j {}
		    for { set i 0 } { $i<$nrel_j } { incr i } {
			set v_i_of_rel_j_l [lindex $rel_j $i]
			set v_i_of_rel_j [lindex [lindex [lindex $v_i_of_rel_j_l 2] 0] 1]
			set Relation_j [linsert $Relation_j end $v_i_of_rel_j]         
		    }
		    set impRelations [linsert $impRelations end $Relation_j]
		}
	    }
	}
	#-------------------------------------
	if { $typeK == "outputs" } { 	    
	}
	#-------------------------------------    		
    }
    #-----------------------------------    
    set capacityP ""
    return [list $nimpvar $nrels $impVariables $Parameters $capacityP ]
}
#-----------------------------------
proc Capacity { incidxmllist } {
    set NN  [expr $nvars+$nrels]
    set inf  [expr $NN*2]
    set GVars $Variables
    for { set i 0 } { $i<$nrels } { incr i } {
	set vr  [expr $i+1]
	set GVars [linsert $GVars end $vr]
    }
    
    for { set i 0 } { $i<$NN } { incr i } {
	set v_i [lindex $GVars $i]
	for { set j 0 } { $j<$NN } { incr j } {
	    set v_j [lindex $GVars $j]
	    set capacity($v_i,$v_j) 0
	}
    }
    for { set k 0 } { $k<$nrels } { incr k } {
	set Relation_k [lindex $Relations $k]
	set vr  [expr $k+1]
	set nR_k [llength $Relation_k]
	for { set i 0 } { $i<$nR_k } { incr i } {
	    set v_i [lindex $Relation_k $i]
	    if { [lsearch -exact $Parameters $v_i] >=0 } {
		set capacity($v_i,$vr) "-$inf"
		set capacity($vr,$v_i) "-$inf"
	    } else {
		set capacity($v_i,$vr) $inf      
		set capacity($vr,$v_i) $inf
	    }
	}
    }
    set capacityP [array get capacity ]
    return [list $nvars $nrels $Variables $Parameters $capacityP ]
}
