
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GENINSERT2_H__
#define __GENINSERT2_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param mj
 * @param mi
 * @param indxj
 * @param indxi
 * @param to
 * @param mr
 * @param from
 * @param m3
 * @param inc3
 * @return 
 */
int C2F(geninsert2)(integer *typ,integer *mj,integer *mi,integer *indxj,integer *indxi,integer *to,integer *mr,integer *from,integer *m3,integer *inc3);
#endif /* __GENINSERT2_H__ */
