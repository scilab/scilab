


/** Structure to keep the graphic state  **/
struct BCG 
{ 
  Drawable Cdrawable ; /** The drawable = CWindow or a Pixmap */
  Widget CinfoW ;  /** info widget of graphic window **/
  Widget Viewport; /** Viewport for graphic window **/
  Widget Panner ;  /** Panner widget **/
  Widget drawbox ; /** the widget associated to CWindow **/
  Widget popup   ; /** graphic window popupc **/
  Window CWindow ; /** the graphic window **/
  Window CBGWindow ; /** window of the top level graphic popup widget **/
  int CurWindow ;   /** Id of window **/
  int CWindowWidth ; /** graphic window width **/
  int CWindowHeight ; /** graphic window height **/
  int FontSize;
  int FontId;
  XID FontXID;
  int CurHardSymb;
  int CurHardSymbSize;
  int CurLineWidth;
  int CurPattern;
  int CurColor;
  int CurPixmapStatus;
  int CurResizeStatus;
  int CurVectorStyle;
  int CurDrawFunction;
  int ClipRegionSet;
  int CurClipRegion[4];
  int CurDashStyle;
  char CurNumberDispFormat[20];
  int CurColorStatus;

  int IDLastPattern; /* number of last pattern or color 
		      in color mode = Numcolors - 1 */
  Colormap Cmap; /* color map of current graphic window */
  int CmapFlag ; /* set to 1 if the Cmap has default colors */
  int Numcolors; /* number of colors */
  Pixel *Colors; /* vector of colors 
		    Note that there are 2 colors more than Numcolors,
		    ie black and white at the end of this vector */
  float *Red; /* vector of red value: between 0 and 1 */
  float *Green; /* vector of green value: between 0 and 1 */
  float *Blue; /* vector of blue value: between 0 and 1 */
  int NumBackground;  /* number of Background in the color table */
  int NumForeground; /* number of Foreground in the color table */
  int NumHidden3d;  /* color for hidden 3d facets **/
};

/** jpc_SGraph.c **/

extern void ChangeBandF __PARAMS((int win_num,Pixel fg, Pixel bg));
extern int CheckClickQueue   __PARAMS((integer *,integer *x,integer *y,integer *ibut));
extern int ClearClickQueue  __PARAMS((integer));
void CreatePopupWindow  __PARAMS((integer WinNum,Widget top,struct BCG *, Pixel *fg,Pixel *bg));
extern void GViewportResize __PARAMS((struct BCG *ScilabXgc, int *width,int *height));

/** jpc_Xloop.c **/

extern integer C2F(ismenu) __PARAMS((void));
extern int C2F(getmen) __PARAMS((char *btn_cmd,integer *lb,integer *entry));

extern void DisplayInit __PARAMS((char *string,Display **dpy,Widget *toplevel));
extern void MenuFixCurrentWin __PARAMS(( int ivalue));
