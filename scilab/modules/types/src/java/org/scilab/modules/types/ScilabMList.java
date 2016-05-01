/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2011-2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.types;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

/**
 * This class provides a representation on the Scilab MList datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<BR>
 * <code>
 * ScilabMList data = new ScilabMList();<BR>
 * data.add(new ScilabString("hello"));<BR>
 * data.add(new ScilabDouble(2));<BR>
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabMList extends ArrayList<ScilabType> implements ScilabType {

    private static final long serialVersionUID = 3224510024213901841L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_mlist;

    private static final int VERSION = 0;

    private String varName = "";

    /**
     * Construct an empty mlist.
     *
     * Note that the first element of this collection is the header used by
     * Scilab to find each field name.
     */
    public ScilabMList() {
        super();
    }

    /**
     * Construct an empty mlist.
     *
     * Note that the first element of this collection is the header used by
     * Scilab to find each field name.
     */
    public ScilabMList(String varName) {
        super();
        this.varName = varName;
    }

    /**
     * Construct an mlist with a specified header.
     *
     * @param types
     *            field names used by the accessors.
     */
    public ScilabMList(String[] types) {
        super();
        String[][] typesData = new String[1][types.length];
        typesData[0] = types;
        add(new ScilabString(typesData));
    }

    /**
     * Construct a tlist containing the elements of the specified collection, in
     * the order that they are returned by the specified collection's iterator.
     *
     * @param names
     *            field names, used by the accessors.
     * @param c
     *            the collection whose elements are to be placed into this
     *            mlist.
     */
    public ScilabMList(String[] names, Collection <? extends ScilabType > c) {
        super(c.size() + 1);

        String[][] namesData = new String[1][names.length];
        namesData[0] = names;
        add(new ScilabString(namesData));

        addAll(c);
    }

    /**
     * @param varName the variable name
     * @param size the initial list size
     */
    public ScilabMList(String varName, int size) {
        super(size);
        this.varName = varName;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isReference() {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    public String getVarName() {
        return varName;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isSwaped() {
        return false;
    }

    /**
     * Get a map between the fields name and the associated ScilabType objects
     *
     * @return the map
     */
    public Map<String, ScilabType> getMListFields() {
        Map<String, ScilabType> map = new HashMap<String, ScilabType>();
        if (isEmpty()) {
            return map;
        }

        ScilabString mat = (ScilabString) get(0);
        if (mat.isEmpty()) {
            return map;
        }

        String[] fields = mat.getData()[0];
        for (int i = 1; i < fields.length; i++) {
            if (i < size()) {
                map.put(fields[i], get(i));
            } else {
                map.put(fields[i], null);
            }
        }

        return map;
    }

    /**
     * Get the mlist type or null if none
     *
     * @return the mlist type
     */
    public String getMListType() {
        if (isEmpty()) {
            return null;
        }

        ScilabString mat = (ScilabString) get(0);
        if (mat.isEmpty()) {
            return null;
        }

        return mat.getData()[0][0];
    }

    /**
     * @return 1 when there is data on the list, 0 otherwise.
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        if (isEmpty()) {
            return 0;
        }
        return 1;
    }

    /**
     * Return the type of Scilab
     *
     * @return the type of Scilab
     * @since 5.4.0
     */
    @Override
    public ScilabTypeEnum getType() {
        return type;
    }

    /**
     * @return 1 when there is data on the list, 0 otherwise.
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (isEmpty()) {
            return 0;
        }
        return size();
    }

    /**
     * Get a serialized list; The format is the following: i) returned[0] is an
     * array of integers containing the Scilab type (ScilabTypeEunm) of the
     * different elements of the list. ii) returned[i] for i&gt;=1 contains the
     * serialized form of each items.
     *
     * @return a serialized ScilabList/
     */
    public Object[] getSerializedObject() {
        int[] types = new int[size()];
        Object[] items = new Object[types.length + 1];

        for (int i = 0; i < types.length; i++) {
            ScilabType var = get(i);
            types[i] = var.getType().swigValue();
            items[i + 1] = var.getSerializedObject();
        }
        items[0] = types;

        return items;
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                int size = in.readInt();
                ensureCapacity(size + 1);
                ArrayList list = (ArrayList) this;
                for (int i = 0; i < size; i++) {
                    list.add(in.readObject());
                }
                varName = (String) in.readObject();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabMList with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(size());
        for (Object var : (ArrayList) this) {
            out.writeObject(var);
        }
        out.writeObject(varName);
    }

    /**
     * Display the representation in the Scilab language of the type<BR>
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty-printed data
     * @see java.util.AbstractCollection#toString()
     */
    @Override
    public String toString() {

        StringBuffer result = new StringBuffer();
        if (isEmpty()) {
            return "mlist()";
        }

        result.append("mlist(");
        for (int i = 0; i < size(); i++) {
            result.append(get(i));
            if (i != size() - 1) {
                result.append(", ");
            }

        }
        result.append(")");

        return result.toString();
    }
}
