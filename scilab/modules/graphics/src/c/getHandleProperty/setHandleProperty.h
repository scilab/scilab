/*------------------------------------------------------------------------*/
/* file: setHandleProperty.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : contains declarations of functions needed by sci_set            */
/*        Each function has the same signature and :                      */
/*        int (sciPointobj *, int, int, int)                              */
/*        parameters stands for respectively, the object to modify,       */
/*        the pointer of the object on the scilab stack, and the number   */
/*        of rows and columns of the new value                            */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _SET_HANDLE_PROPERTY_H_
#define _SET_HANDLE_PROPERTY_H_

#include "ObjectStructure.h"

int set_color_map_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_old_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_pixel_drawing_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_default_values_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_visible_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_auto_resize_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_entity_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_axes_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_current_figure_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_axes_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_position_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_name_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_figure_id_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_rotation_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_immediate_drawing_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_pixmap_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_background_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;
int set_interp_color_vector_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol ) ;

#endif /* _SET_HANDLE_PROPERTY_H_ */
