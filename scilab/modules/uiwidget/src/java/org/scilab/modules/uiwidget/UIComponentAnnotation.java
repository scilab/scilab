/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.lang.annotation.Annotation;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
public @interface UIComponentAnnotation {

    /**
     * Map method arguments with attributes name
     * For example,
     * <code>
     * @UIComponentAnnotation(attributes={"a", "b"})
     * void foo(String one, int two) { ... }
     * </code>
     * The value of attribute "a" is converted into a String and passed as "one" argument,...
     */
public String[] attributes() default {
        ""
    };
}
