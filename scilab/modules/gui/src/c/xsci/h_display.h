/*
 * display.h : Definitions of device-independent interface functions
 *
 * This header file is used for both the X and Curses display routines.
 * Not all functions are needed for both types of display.
 *
 * George Ferguson, ferguson@cs.rochester.edu, 23 Apr 1993.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* Text output routines */
extern void setTitleText(char *str), setStatusText(char *str);
extern void setSearchText(char *str), setHostText(char *str);
extern void setLocationText(char *str), setFileText(char *str);
extern void setSizeText(char *str), setModesText(char *str);
extern void setDateText(char *str);

/* Button sensitivity routines */
extern void setQuerySensitive(int state), setAbortSensitive(int state);


#endif /* DISPLAY_H */
