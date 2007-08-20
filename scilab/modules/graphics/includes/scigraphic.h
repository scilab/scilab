
#include "bcg.h"

/**
 * Sets the dimension of the scroll bars
 * @attention Do not call SetScrollInfo windows function,
 * sciSetScrollInfo do that and more things !
 * @param[in] Scilabgc
 * @param[in] sb_ctl
 * @param[in] si
 * @param[in] bRedraw
 *@author Matthieu PHILIPPE 
 *@date Dec 1999
 **/
int sciSetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si, BOOLEAN bRedraw);

/**
 * Returns the dimension of the scroll bars
 * @attention Do not call GetScrollInfo windows function,
 * sciGetScrollInfo do that and more things !
 * @param[in] Scilabgc
 * @param[in] sb_ctl
 * @param[in] si
 *@author Matthieu PHILIPPE 
 *@date Dec 1999
 **/
int sciGetScrollInfo(struct BCG *Scilabgc, int sb_ctl, SCROLLINFO *si);

/**
 * Returns the wresize status.
 * 0: it's in scroll bars mode
 * 1: it's in wresize mode
 *@author Matthieu PHILIPPE 
 *@date Dec 1999
 **/
integer sciGetwresize();

/**
 * Returns the pixmap status.
 * 0: it's drawn directly on screen
 * 1: it's drawn by a pixmap first
 *@author Matthieu PHILIPPE 
 *@date Dec 1999
 **/
integer sciGetPixmapStatus();

/**
 * used to move the panner and the viewport interactively 
 * through scilab command.
 * @param[in] struct BCG *ScilabGC : structure associated to a Scilab Graphic window
 * @param[in] int x,y : the x,y point of the graphic window to be moved at the up-left position of the viewport
 **/
void SciViewportMove __PARAMS((struct BCG *ScilabXgc,int x,int y));

/**
 * A little beat different to windowdim. GPopupResize sets the visible window (parents dimension)
 * @param[in] struct BCG *ScilabXgc
 * @param[in] int * x,y , where x,y are the new dimension
 * @see setwindowdim
 **/
void GPopupResize __PARAMS((struct BCG *ScilabXgc,int *x,int *y));

