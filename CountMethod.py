from ArrayConvert import *
from ctypes import cdll

Hash = cdll.LoadLibrary('./lib/libHashTree.so')
HASH_NUM = 3
def HashTreeCnt(Transaction, C_k):
    L_k = dict.fromkeys(C_k, 0)
    set_len = len(list(C_k)[0])
    # Build hash tree call by C++ function
    HashTree = Hash.new_HashTree(HASH_NUM)
    for itemset in C_k:
        Hash.AddData(HashTree, List2CppIntArray(itemset))
    # Count
    for t in Transaction:
        trans = List2CppIntArray(sorted(t))
        for i in range(Hash.Travel(HashTree, trans)):
            cond = CppIntArray2List(Hash.Output(HashTree, i))
            L_k[tuple(cond)] += 1
    return L_k

def BruteForceCnt(Transaction, C_k):
    L_k = dict.fromkeys(C_k, 0)
    for t in Transaction:
        for iset in C_k:
            hit = True
            for item in iset:
                if not item in t:
                    hit = False
                    break
            if hit:
                L_k[iset] += 1
    return L_k
