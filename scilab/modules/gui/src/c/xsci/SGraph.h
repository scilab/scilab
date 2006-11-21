

int IswmDeleteWindow(XEvent *event);

int IsCloseSGWindow(XEvent *event);


void btn_pressed(Widget widget, XEvent*	event, String* params, Cardinal* num_params);

void btn_released(Widget widget, XEvent* event,String* params, Cardinal* num_params);

void ctrl_key(Widget widget, XEvent* event, String*	params, Cardinal* num_params);

void key_pressed(Widget	widget, XEvent*	event, String* params, Cardinal* num_params);

void key_released(Widget widget,XEvent*	event, String* params, Cardinal* num_params);

void mouse_moved(Widget	widget, XEvent*	event, String* params, Cardinal* num_params);

void SciViewportMove(struct BCG *ScilabXgc, int x, int y);

void SciViewportGet(struct BCG *ScilabXgc, int *x, int *y);

void SciViewportClipGetSize(struct BCG *ScilabXgc,int *w,int *h);

void SetBar(Widget w, Position top, Dimension length, Dimension total);

void RedrawThumbs(ViewportWidget w);

void GViewportResize(struct BCG *ScilabXgc, int *width,int *height);

void GPopupResize(struct BCG *ScilabXgc, int *width,int *height);

void CreatePopupWindow(integer WinNum, Widget button, struct BCG *ScilabXgc, Pixel *fg, Pixel *bg);

int getNbSubMenus(int winNumber, char * menuName );

void refreshMenus( struct BCG * ScilabGC );

void scig_print(integer number);

void scig_export(integer number);
