 
#include <gtk/gtk.h> 

#if  GTK_MAJOR_VERSION == 1 
#include "queue-gtk1.c"
#else 
#include "queue-gtk2.c"
#endif 

