/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.sax;

import java.util.ArrayList;
import java.util.Vector;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Local implementation of a Stack as the java default one inherit from
 * {@link Vector}.
 *
 * @param <E>
 *            the contained item klass
 */
class Stack<E> {
    private ArrayList<E> stack = new ArrayList<>();

    public void pop() {
        stack.remove(stack.size() - 1);
    }

    public void push(E e) {
        stack.add(e);
    }

    public E peek() {
        return peek(0);
    }

    public E peek(int depth) {
        return stack.get(stack.size() - 1 - depth);
    }

    public int size() {
        return stack.size();
    }

    public Stream<E> stream() {
        // stream in a reversed order (traditional stack usage)
        return IntStream.range(0, stack.size()).mapToObj(i -> stack.get(stack.size() - 1 - i));
    }

    @Override
    public String toString() {
        return stack.toString();
    }
}