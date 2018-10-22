from CountMethod import *

class Apriori:
    def __init__(self, min_support, Method):
        self.min_support = min_support
        self.Method   = Method.upper()
        self.FreqItemset = None

    def _FPGrowth(self, trans):
        from ctypes import cdll
        FP = cdll.LoadLibrary('./lib/libFPTree.so')
        # Sort transaction
        from functools import cmp_to_key
        def _cmp(a, b):
            if   self.FreqItemset[0][(a,)] < self.FreqItemset[0][(b,)]: return 1
            elif self.FreqItemset[0][(b,)] < self.FreqItemset[0][(a,)]: return -1
            else:
                if a < b: return 1
                else:     return -1

        # Construct tree
        FPTree = FP.new_FPTree()
        for t in trans:
            t = sorted([item for item in t if (item,) in self.FreqItemset[0]], key=cmp_to_key(_cmp))
            FP.AddTrans(FPTree, List2CppIntArray(t))
        
        n = FP.FindPattern(FPTree)
        for idx in range(n):
            itemset = CppIntArray2List(FP.OutputPattern(FPTree, idx))
            if (itemset[-1] < self.min_support): continue;
            try:
                self.FreqItemset[len(itemset) - 2]
            except:
                self.FreqItemset[len(itemset) - 2] = {}
            self.FreqItemset[len(itemset) - 2][tuple(sorted(itemset[:-1]))] = itemset[-1]
        # Release tree        
        FP.del_FPTree(FPTree)
        
        return self.FreqItemset

    def Run(self, trans):
        # itemsets  = []
        self.FreqItemset = {0:{}}
        for t in trans:
            for item in t:
                item = (item, )
                try:
                    self.FreqItemset[0][item]
                except:
                    self.FreqItemset[0][item] = 0
                self.FreqItemset[0][item] += 1
        for item in list(self.FreqItemset[0]):
            if self.FreqItemset[0][item] < self.min_support:
                del self.FreqItemset[0][item]
        
        if self.Method == 'FP':
            return self._FPGrowth(trans)
        else:
            L_k_1 = list(self.FreqItemset[0])
            k = 1
            while True:
                C_k = set()
                # Join L_k
                for idx1 in range(len(L_k_1)):
                    for idx2 in range(idx1 + 1, len(L_k_1)):
                        join_set = set(L_k_1[idx1]) | set(L_k_1[idx2])
                        if len(join_set) == len(L_k_1[idx1]) + 1:
                            C_k |= {tuple(sorted(join_set))}
                
                # Prune
                for iset in list(C_k):
                    for item in iset:
                        if not tuple(sorted(set(iset) - {item})) in L_k_1:
                            C_k -= {iset}
            
                # If no new join item, return
                if len(C_k) == 0: break
            
                # Count C_k
                if self.Method == 'HASH':
                    L_k = HashTreeCnt(trans, C_k)
                else:
                    L_k = BruteForceCnt(trans, C_k)
            
                # Check min support
                for item in list(L_k):
                    if L_k[item] < self.min_support:
                        del L_k[item]

                # If no condidate get min support, return
                if len(L_k) == 0: break
                L_k_1 = list(L_k)
                self.FreqItemset[k] = L_k
                k += 1

            return self.FreqItemset
