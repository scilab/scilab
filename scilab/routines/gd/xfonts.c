#include <X11/X.h>
#include <X11/Xlib.h>
/* Get the metrics for a character. */
typedef struct gs_point_s {
    double x, y;
} gs_point;
typedef struct gs_int_point_s {
    int x, y;
} gs_int_point;
typedef struct gs_int_rect_s {
    gs_int_point p, q;
} gs_int_rect;

int x_char_metrics(XFontStruct* x11font, char chr, gs_point* pwidth, gs_int_rect* pbbox)
{
  if (x11font->per_char == NULL) {
    pwidth->x = x11font->max_bounds.width;
    pwidth->y = 0;
    pbbox->p.x = x11font->max_bounds.lbearing;
    pbbox->q.x = x11font->max_bounds.rbearing;
    pbbox->p.y = -x11font->max_bounds.ascent;
    pbbox->q.y = x11font->max_bounds.descent;
  } else {
	int i = (int)chr - x11font->min_char_or_byte2;

	pwidth->x = x11font->per_char[i].width;
	pwidth->y = 0;

	pbbox->p.x = x11font->per_char[i].lbearing;
	pbbox->q.x = x11font->per_char[i].rbearing;
	pbbox->p.y = -x11font->per_char[i].ascent;
	pbbox->q.y = x11font->per_char[i].descent;
    }
    return 0;
}

/* Render a character. */
int
x_render_char(Display* dpy, XFontStruct* x11font, char chr, int xo, int yo)
{
  gs_int_rect bbox;
  int x, y, w, h;


  /* Display on an intermediate bitmap, then copy the bits. */
  int i;
  XImage *xim;
  Pixmap xpm;
  GC fgc;
  gs_point wxy;
  int code;

  code = x_char_metrics(x11font, chr, &wxy, &bbox);

  w = bbox.q.x - bbox.p.x;
  h = bbox.q.y - bbox.p.y;


  xpm = XCreatePixmap(dpy, InputOnly, w, h, 1);
  fgc = XCreateGC(dpy, xpm, None, NULL);
  XSetForeground(dpy, fgc, 0);
  XFillRectangle(dpy, xpm, fgc, 0, 0, w, h);
  XSetForeground(dpy, fgc, 1);
  XSetFont(dpy, fgc, x11font->fid);
  XDrawString(dpy, xpm, fgc, -bbox.p.x, -bbox.p.y, &chr, 1);
  xim = XGetImage(dpy, xpm, 0, 0, w, h, 1, ZPixmap);
  i = 0;
  for (y = 0; y < h; y++) {
    char b;

    for (x = 0; x < w; x++) {
	b = XGetPixel(xim, x, y);
      /*      if ((x & 7) == 7)
	      bits[i++] = b;*/
    }
  }
  XFreePixmap(dpy, xpm);
  XFreeGC(dpy, fgc);
  XDestroyImage(xim);
  return;
    
}
