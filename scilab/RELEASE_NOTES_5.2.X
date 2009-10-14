                      Scilab 5.2 Release_Notes
                      ========================

Known incompatibilities:

- Function "figure": when setting the size of the figure using the "Position" 
  property with a matrix such as [x y width height], the values of width and 
  height no more set the window size itself but the size of the virtual graphics
  window (the part of the figure which contains uicontrols and graphics) as if
  the user sets the axes_size property of a figure.

- 'sort' is obsolete. see 'gsort'.

- 'getf' is obsolete. see 'exec'.

- Removed functions, previously declared obsoletes:
   'mtlb_eig' to replace by 'spec'.
   'set_posfig_dim' to replace by 'figure_size' property.
   'xclip' to replace by 'clip_state' and 'clip_box' properties.
   'xgetfile' to replace by 'uigetfile'.
   'tk_getfile' to replace by 'uigetfile'.
   'x_message' to replace by 'messagebox'.