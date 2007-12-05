#ifndef __CALLJXGETMOUSE_H__
#define __CALLJXGETMOUSE_H__

/* Copyright INRIA */
/* Vincent COUVERT 2007 */
void CallJxgetmouse();

void CallJxgetmouseWithOptions(int withMotion, int withRelease);

int getJxgetmouseMouseButtonNumber();

double getJxgetmouseXCoordinate();

double getJxgetmouseYCoordinate();
#endif

