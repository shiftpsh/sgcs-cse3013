#ifndef __GROWABLE_ARRAY_H__
#define __GROWABLE_ARRAY_H__
#include "array.h"
using namespace std;

template <class T> class GrowableArray : public Array<T> {
  public:
    GrowableArray(int size);
    virtual T &operator[](int i);
};

template <class T> GrowableArray<T>::GrowableArray(int size) {
    if (size < 0) {
        cerr << "Size cannot be less than 0." << endl;
        exit(-1);
    }

    Array<T>::data = new T[size];

    if (Array<T>::data == NULL) {
        cerr << "Memory allocation failed" << endl;
        exit(-1);
    }

    Array<T>::len = size;
}

template <class T> T &GrowableArray<T>::operator[](int i) {
    if (0 <= i && i < Array<T>::len) {
        return Array<T>::data[i];
    }

    int new_length = i << 1;

    T *new_data = new T[new_length];
    if (new_data == NULL) {
        cerr << "Memory allocation failed" << endl;
        exit(-1);
    }

    for (int i = 0; i < Array<T>::len; i++)
        new_data[i] = Array<T>::data[i];
    for (int i = Array<T>::len; i < new_length; i++)
        new_data[i] = 0;

    T *temp = Array<T>::data;
    Array<T>::data = new_data;
    Array<T>::len = new_length;
    delete[] temp;

    return Array<T>::data[i];
}

#endif