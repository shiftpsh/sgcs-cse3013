#ifndef __ARRAY__
#define __ARRAY__
#include <cstdlib>
#include <iostream>
using namespace std;

template <class T> class Array {
  protected:
    T *data;
    int len;

  public:
    Array(){};
    Array(int size);
    ~Array();

    int length() const;

    virtual T &operator[](int i);
    virtual T operator[](int i) const;

    void print();
};

template <class T> Array<T>::Array(int size) {
    if (size < 0) {
        cerr << "Size cannot be less than 0." << endl;
        exit(-1);
    }

    data = new T[size];

    if (data == NULL) {
        cerr << "Memory allocation failed" << endl;
        exit(-1);
    }

    len = size;
}

template <class T> Array<T>::~Array() { delete[] data; }

template <class T> int Array<T>::length() const { return len; }

template <class T> T &Array<T>::operator[](int i) {
    static T __undefined__;
    if (0 <= i && i < len) {
        return data[i];
    } else {
        cerr << "Array index out of bounds" << endl;
        return __undefined__;
    }
}

template <class T> T Array<T>::operator[](int i) const {
    if (0 <= i && i < len) {
        return data[i];
    } else {
        cerr << "Array index out of bounds" << endl;
        return 0;
    }
}

template <class T> void Array<T>::print() {
    cout << '[';
    for (int i = 0; i < len; i++) {
        cout << ' ' << data[i];
    }
    cout << " ]\n";
}

#endif