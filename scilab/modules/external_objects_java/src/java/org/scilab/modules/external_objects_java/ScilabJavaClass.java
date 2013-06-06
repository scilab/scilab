/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.net.URL;
import java.util.logging.Level;

/**
 * A Java Class wrapper
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaClass extends ScilabJavaObject {

    protected ScilabJavaConstructor sjc = null;
    protected URL classURL;

    /**
     * Constructor
     * @param cls the class to wrap
     */
    public ScilabJavaClass(Class cls) {
        super(cls, cls);
        sjc = new ScilabJavaConstructor(cls);
    }

    /**
     * @param url the URL of this class
     */
    public void setURL(URL url) {
        classURL = url;
    }

    /**
     * @return the URL of this class
     */
    public URL getURL() {
        return classURL;
    }

    /**
     * @param id the id of an Scilab Object
     * @param args an array containing the arguments id
     * @param the id of the resulting object
     */
    public static int newInstance(int id, int[] args) throws ScilabJavaException {
        if (ScilabJavaObject.debug) {
            StringBuffer buf = new StringBuffer();
            buf.append("(");
            if (args.length > 0) {
                int i = 0;
                for (; i < args.length - 1; i++) {
                    buf.append(Integer.toString(args[i]));
                    buf.append(",");
                }
                buf.append(Integer.toString(args[i]));
            }
            buf.append(")");
            ScilabJavaObject.logger.log(Level.INFO, "New instance of class id=" + id + " with args=" + buf.toString());
        }

        if (id == 0) {
            throw new ScilabJavaException("null cannot be instantiated");
        }
        if ((arraySJO[id] instanceof ScilabJavaClass) && ((ScilabJavaClass) arraySJO[id]).sjc != null) {
            return new ScilabJavaObject(((ScilabJavaClass) arraySJO[id]).sjc.invoke(args)).id;
        }
        throw new ScilabJavaException("The object " + id + " is not a valid Class object");
    }

    /**
     * Create a new identical reference to a Java class
     * @return A deep copy of this {@link ScilabJavaClass}
     */
    @Override
    protected ScilabJavaObject clone() {
        ScilabJavaClass	sjcazz = new ScilabJavaClass(clazz);
        sjcazz.setURL(getURL());
        return sjcazz;
    }
}
