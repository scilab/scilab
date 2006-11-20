
#ifndef _GET_DATA_PROPERTY_H_
#define _GET_DATA_PROPERTY_H_
#include "ObjectStructure.h"

/* the grayplot data is now given as a tlist (like for surface and champ objects) */

int getgrayplotdata (sciPointObj *);

/* the champ data is now given as a tlist (like for surface objects) */
int getchampdata(sciPointObj *pobj);

int get3ddata(sciPointObj *pobj);

#endif /* _GET_DATA_PROPERTY_H_ */
