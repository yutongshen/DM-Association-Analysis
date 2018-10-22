from ctypes import cdll
CArray = cdll.LoadLibrary('./lib/libCArray.so')
def List2CppIntArray(arr):
    carr = CArray.new_IntArray(len(arr))
    for idx, item in enumerate(arr):
        CArray.IntArray_set(carr, idx, item)
    return carr

def CppIntArray2List(carr):
    length = CArray.IntArray_len(carr)
    arr = []
    for idx in range(length):
        arr += [CArray.IntArray_read(carr, idx)]
    return arr
