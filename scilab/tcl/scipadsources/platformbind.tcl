# We have found that some key sequences are given a variety of names
#  on different platforms. E.g. Shift-F12 can be called XF86_Switch_VT_12
#  on most unices, or even Shift-SunF37 on Suns. Therefore, when we need to
#  bind these sequences, we try and catch all the known possibilities,
#  using pbind instead of bind. The lists of possible names are defined in 
#  bindings.tcl.
proc pbind {w events action} {
    foreach e $events {
        catch {bind $w <$e> $action} 
    }
} 
