/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.graphicView;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;

public class LogView implements GraphicView {

    private static LogView me = null;
    
    private LogView() {}
   
    public static LogView createLogView() {
        if(me == null) {
            me = new LogView();
        }
        
        return me;
    }
    
    public void createObject(String id) {
       int objectType = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
       System.out.println("+++"+id+" Object Created type is "+objectType+".");
    }

    public void deleteObject(String id) {
        System.out.println("---"+id+" Object Deleted.");

    }

    public void updateObject(String id, int property) {
        System.out.println("==="+id+" Object Property " + property + " Updated.");
    }

}
