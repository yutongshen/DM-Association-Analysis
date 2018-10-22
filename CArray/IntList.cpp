#include <iostream>
#include "IntList.hpp"

IntList::IntList(int len): len(len) {
    if (len)  data = new int[len];
    else      data = NULL;
}
IntList::IntList(const IntList& copy): len(copy.len) {
    if (!len) {
        data = NULL;
        return;
    }
    data = new int[len];
    for (int i(0); i < len; ++i)
        data[i] = copy.data[i];
}
IntList::~IntList() {
    if (data) delete [] data;
}
int IntList::length() const {
    return len;
}
int& IntList::operator[](const int& idx) {
    return data[idx];
}
int IntList::operator[](const int& idx) const {
    return data[idx];
}
IntList& IntList::operator=(const IntList& list) {
    for (int i(0); i < len; ++i)
        data[i] = list.data[i];
    return *this;
}
bool IntList::operator<(const IntList& list) const {
    int i(0), len_min, len_a(len), len_b(list.len);
    len_min = len_a < len_b ? len_a : len_b;
    for (; i < len_min; ++i)
        if (data[i] < list.data[i])      return 1;
        else if (data[i] > list.data[i]) return 0;
    return len_a < len_b;

}

