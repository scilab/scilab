#ifndef TEMPTST_H

template<class T>
class N1 {
private:
  T data_;
public:
  ~N1() { }
  T data() const {return data_;}
  T sqr() const;
  const N1<T>& operator=(const N1<T>& v);
  N1(const N1<T>& v) {
    operator=(v);
  }
  N1(T d1):  data_(d1) { }
};


template<class T>
T N1<T>::sqr() const {
  return data_ * data_;
}

template<class T>
const N1<T>& N1<T>::operator=(const N1<T>& v) {
  data_ = v.data();
  return *this;
}

#endif

#define TEMPTST_H
