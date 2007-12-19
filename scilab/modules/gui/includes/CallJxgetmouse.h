#ifndef __CALLJXGETMOUSE_H__
#define __CALLJXGETMOUSE_H__

/* Copyright INRIA */
/* Vincent COUVERT 2007 */
void CallJxgetmouse(void);

void CallJxgetmouseWithOptions(int withMotion, int withRelease);

int getJxgetmouseMouseButtonNumber(void);

double getJxgetmouseXCoordinate(void);

double getJxgetmouseYCoordinate(void);
#endif

