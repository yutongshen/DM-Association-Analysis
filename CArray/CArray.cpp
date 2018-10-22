#include "IntList.hpp"

extern "C" {
    IntList* new_IntArray(int len) {
        return new IntList(len);
    }
    void IntArray_set(IntList arr, int idx, int value) {
        arr[idx] = value;
    }
    int IntArray_len(IntList arr) {
        return arr.length();
    }
    int IntArray_read(IntList arr, int idx) {
        return arr[idx];
    }
}
