                      Scilab 5.2 Release_Notes
                      ========================

Known incompatibilities:

- Fonction "figure": when setting the size of the figure using the "Position" 
  property with a matrix such as [x y width height], the values of width and 
  height no more set the window size itself but the size of the virtual graphics
  window (the part of the figure which contains uicontrols and graphics) as if
  the user sets the axes_size property of a figure.
