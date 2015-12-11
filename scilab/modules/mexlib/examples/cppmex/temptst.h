
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
