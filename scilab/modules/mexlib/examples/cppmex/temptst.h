
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#ifndef TEMPTST_H

template<class T>
class N1
{
private:
    T data_;
public:
    ~N1() { }
    T data() const
    {
        return data_;
    }
    T sqr() const;
    const N1<T>& operator=(const N1<T>& v);
    N1(const N1<T>& v)
    {
        operator=(v);
    }
    N1(T d1):  data_(d1) { }
};


template<class T>
T N1<T>::sqr() const
{
    return data_ * data_;
}

template<class T>
const N1<T>& N1<T>::operator=(const N1<T>& v)
{
    data_ = v.data();
    return *this;
}

#endif

#define TEMPTST_H
