
if { $tcl_platform(platform) == "windows" } {
    option add *Listbox.background      SystemWindow widgetDefault
    option add *ListBox.background      SystemWindow widgetDefault
    option add *Button.padY             0 widgetDefault
    option add *ButtonBox.padY          0 widgetDefault
    option add *Dialog.padY             0 widgetDefault
    option add *Dialog.anchor           e widgetDefault
} else { 
    option add *Scrollbar.width         12 widgetDefault
    option add *Scrollbar.borderWidth   1  widgetDefault
    option add *Dialog.separator        1  widgetDefault
    option add *MainFrame.relief        raised widgetDefault
    option add *MainFrame.separator     none   widgetDefault
}

option read [file join $::BWIDGET::LIBRARY "lang" "en.rc"]

bind Entry <<TraverseIn>> { %W selection range 0 end; %W icursor end }

bind all <Key-Tab>       { Widget::traverseTo [Widget::focusNext %W] }
bind all <Shift-Key-Tab> { Widget::traverseTo [Widget::focusPrev %W] }
