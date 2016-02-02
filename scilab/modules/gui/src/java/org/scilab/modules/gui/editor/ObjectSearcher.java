/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.editor;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;


import java.util.*;

/**
* Serach for objects in the given object child tree.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class ObjectSearcher {

    private List<Integer> objects = new ArrayList<Integer>();
    private Integer type;
    private Integer[] types;

    /**
    * Search for the given object type.
    * @param rootUid    Root object to search.
    * @param objType    Object type.
    * @return             A vector with the uid of the objects found.
    */
    public Integer[] search( Integer rootUid, int objType) {
        return search(rootUid, objType, false);
    }

    public Integer[] search( Integer rootUid, int objType, boolean isDatatip) {

        type = objType;
        objects.clear();
        if (isDatatip) {
            getDatatipsObjects(rootUid);
        } else {
            getObjects(rootUid);
        }
        if (objects.size() != 0) {
            Integer[] ret = new Integer[objects.size()];
            for (int i = 0; i < objects.size(); ++i) {
                ret[i] = objects.get(i);
            }
            return ret;
        }

        return null;
    }

    /**
    * Search for the given object type in object parent path.
    *
    * @param uid Object unique identifier.
    * @param type Object type.
    * @return The first parent found with the given type, or null if none is found.
    */
    public Integer searchParent(Integer uid, Integer type) {

        if (uid != null) {
            Integer parent = (Integer)GraphicController.getController().getProperty( uid, GraphicObjectProperties.__GO_PARENT__);
            if (type == (Integer)GraphicController.getController().getProperty(parent, GraphicObjectProperties.__GO_TYPE__)) {
                return parent;
            } else {
                return searchParent(parent, type);
            }
        }
        return null;
    }

    private void getDatatipsObjects(Integer uid) {

        Integer parentType = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        if (parentType == GraphicObjectProperties.__GO_POLYLINE__) {
            Integer tipCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_DATATIPS_COUNT__);
            Integer[] tipUid = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_DATATIPS__);

            for (Integer i = 0; i < tipCount; ++i ) {
                objects.add(tipUid[i]);
            }
        } else {
            Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
            Integer[] childUid = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);

            for (Integer i = 0; i < childCount; ++i ) {
                getDatatipsObjects(childUid[i]);
            }
        }
    }

    private void getObjects(Integer uid) {

        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        Integer[] childUid = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);

        for (Integer i = 0; i < childCount; ++i ) {
            Integer objType = (Integer)GraphicController.getController().getProperty(childUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType.equals(type)) {
                objects.add( childUid[i] );
            } else {
                getObjects(childUid[i]);
            }
        }
    }

    private void getMultipleObjects(Integer uid) {

        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        Integer[] childUid = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);

        for (Integer i = 0; i < childCount; ++i ) {
            Integer objType = (Integer)GraphicController.getController().getProperty(childUid[i], GraphicObjectProperties.__GO_TYPE__);

            boolean found = false;

            for (Integer j = 0; j < types.length; ++j) {
                if (objType.equals(types[j])) {
                    objects.add(childUid[i]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                getMultipleObjects(childUid[i]);
            }
        }
    }

    public Integer[] searchMultiple(Integer root, Integer[] objTypes) {

        types = objTypes;
        objects.clear();
        getMultipleObjects(root);
        if (objects.size() != 0) {
            Integer[] ret = new Integer[objects.size()];
            for (int i = 0; i < objects.size(); ++i) {
                ret[i] = objects.get(i);
            }
            return ret;
        }
        return null;
    }
}

