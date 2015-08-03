                     Changes between version 5.5.1 and 5.6.0
                     =======================================

New Features
=============

* scatter plot with different mark colors is now available:
  TODO : save/load+display+doc of new properties


Xcos
====

* The palette browser has been improved and now is able to :
   * zoom using CTRL(+), CTRL(-) and CTRL(mousewheel).
   * load SVG icons
   * go forward or backward
   * add blocks to the most recent diagram by using the ENTER key.
   * dynamically update the blocks used most recently.

* Bug #10326 fixed - Adds a search engine for the palette browser.

* Bug #12453 fixed - The left panel of the palette browser keeps its size when some category is enabled or disabled.

* Bug #13906 fixed - It is now possible to navigate through the Palette browser with the arrow keys.

* Bug #13942 fixed - Now the palette browser tree is always resized when expanded/collapsed.


Compilation
============

* --without-xcos now only disable Xcos compilation. Xcos Graphic interface is disabled
using --without-gui.
