/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.Collection;
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
public class Stack<E> {
    private ArrayList<E> stack = new ArrayList<>();

    public E pop() {
        return stack.remove(stack.size() - 1);
    }

    public void push(E e) {
        stack.add(e);
    }

    public void addAll(Collection<E> c) {
        stack.addAll(c);
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

    /**
     * Stream in a reversed order (traditional stack usage)
     * @return a stream object
     */
    public Stream<E> stream() {
        return IntStream.range(0, stack.size()).mapToObj(i -> stack.get(stack.size() - 1 - i));
    }

    @Override
    public String toString() {
        return stack.toString();
    }
}