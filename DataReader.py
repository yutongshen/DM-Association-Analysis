def DataReader(path):
    with open(path, 'r') as f:
        train_data = f.read().split('\n')
    
    transaction = {}
    itemID = {}
    _id = 0
    ID2item = {}
    if path[-4:] == '.csv':
        for d in train_data[1:]:
            d = d.split(',')
            if len(d) < 2: break
            try:
                itemID[d[-1]]
            except:
                itemID[d[-1]] = _id
                ID2item[_id] = d[-1]
                _id += 1
            try:
                transaction[int(d[-2])]
            except:
                transaction[int(d[-2])] = []
            if itemID[d[-1]] in transaction[int(d[-2])]:
                continue
            transaction[int(d[-2])] += [itemID[d[-1]]]
    else:
        for d in train_data:
            d = d.split()
            if len(d) < 2: break
            try:
                itemID[d[-1]]
            except:
                itemID[d[-1]] = _id
                ID2item[_id] = d[-1]
                _id += 1
            try:
                transaction[int(d[-2])]
            except:
                transaction[int(d[-2])] = []
            if itemID[d[-1]] in transaction[int(d[-2])]:
                continue
            transaction[int(d[-2])] += [itemID[d[-1]]]
    return transaction.values(), ID2item
