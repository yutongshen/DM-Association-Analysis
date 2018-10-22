from Apriori import *
from DataReader import *
import argparse
import sys

def ID2Item(itemset):
    global id_table
    res = ()
    for i in itemset:
        res += (id_table[i],)
    return res
    

if __name__ == '__main__':
    
    parser = argparse.ArgumentParser()

    parser.add_argument('-m',
                        default='fp',
                        dest='METHOD',
                        help='Algorithm select. {brute|hash|fp}(default:fp) ')
    parser.add_argument('-t',
                        default='./data/BreadBasket_DMS.csv',
                        dest='TRANSATION_FILE',
                        help='Import transaction file')
    parser.add_argument('-ms',
                        default='3',
                        dest='MIN_SUPPORT',
                        help='Set minimum support (default:3)')
    parser.add_argument('-mc',
                        default='0.5',
                        dest='MIN_CONFIDENCE',
                        help='Set minimum confidence (default:0.5)')
    parser.add_argument('-a',
                        default='false',
                        dest='PRINT_ASSOCIATE',
                        help='Show association rule {true|false}')


    args = parser.parse_args()

    try:
        min_support = int(args.MIN_SUPPORT)
    except ValueError:
        print('error: Minimum support must be an integer number')
        sys.exit()

    try:
        min_confid = float(args.MIN_CONFIDENCE)
    except ValueError:
        print('error: Minimum confidence must be a number')
        sys.exit()

    assoc_print = False if args.PRINT_ASSOCIATE.upper() != 'TRUE' else True
    method = args.METHOD


    trans, id_table = DataReader(args.TRANSATION_FILE)
    
    apriori = Apriori(min_support, method)
    itemsets = apriori.Run(trans)
    
    for i in itemsets:
        print('Frequent itemset L('+str(i+1)+'):')
        for key in itemsets[i]:
            print(ID2Item(key), ':', itemsets[i][key])
        print()

    close_freq_n_1 = []
    close_freq   = []
    maximal_freq = []
    all_freq     = []
    
    for i in reversed(range(len(itemsets))):
        close_freq_n = []
        tmp = list(itemsets[i])
        for item in tmp:
            close = 1
            maximal = 1
            for cls in close_freq_n_1:
                if (set(item) - set(cls)) == set():
                    maximal = 0
                    if itemsets[i][item] == itemsets[i+1][cls]:
                        close = 0
                        break
            if close:
                close_freq_n.append(item)
            if maximal:
                maximal_freq.append(item)
        close_freq_n_1 = close_freq_n
        close_freq += close_freq_n
        all_freq   += tmp
    print('Close frequent itemset:')
    for s in close_freq:
        print(ID2Item(s))
    print()
    print('Maximal frequent itemset:')
    for s in maximal_freq:
        print(ID2Item(s))
    if assoc_print:
        assoc = []
        for large in reversed(range(len(itemsets))):
            for small in range(large):
                for maxset in itemsets[large]:
                    for subset in itemsets[small]:
                        if set(subset).issubset(maxset):
                            if itemsets[large][maxset] / itemsets[small][subset] >= min_confid:
                                remain = set(maxset) - set(subset)
                                assoc.append((subset, tuple(remain)))
        
        print('\nFind', len(assoc), 'rules:')
        for i, (a, b) in enumerate(assoc):
            print(i+1, ':', ID2Item(a), '=>', ID2Item(b))
    # for i in itemsets:
    #     print(i,len(itemsets[i]))
