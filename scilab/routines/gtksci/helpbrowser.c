
#include <gtk/gtk.h> 

#if GTK_MAJOR_VERSION == 1 
#include "helpbrowser-gtk1.c"
#else 
#include "helpbrowser-gtk2.c"
#endif 

