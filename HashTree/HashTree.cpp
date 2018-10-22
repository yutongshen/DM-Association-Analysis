#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include "IntList.hpp"
#include "HashTree.hpp"

Node::Node(const int& num): node_ptr(new Node*[num]) {
    for (int i(0); i < num; ++i)
        node_ptr[i] = NULL;
}

Node::~Node() {
    delete [] node_ptr;
}

void HashTree::_travel(IntList patten, int patten_pos, const IntList& trans, int trans_pos) {
    if (trans.length() < trans_pos) return;
    else if (patten_pos == patten_len) {
        int hash;
        Node* trace(root);
        for (int i(0); i < patten_len; ++i) {
            hash = patten[i] % hash_num;
            trace = trace->node_ptr[hash];
            if (!trace) return;
        }
        if (trace->data_set.find(patten) != trace->data_set.end())
             buff.push_back(patten);
        return;
    }
    _travel(patten, patten_pos, trans, trans_pos+1);
    patten[patten_pos] = trans[trans_pos];
    _travel(patten, patten_pos+1, trans, trans_pos+1);
}

HashTree::HashTree(const int& hash_num): 
    hash_num(hash_num),
    root(new Node(hash_num)) {}

HashTree::~HashTree() {}

void HashTree::buildTree(const IntList& data) {
    int i, hash;
    patten_len = data.length();
    Node* trace(root);
    for (i = 0; i < patten_len; ++i) {
        hash = data[i] % hash_num;
        if (!trace->node_ptr[hash])
            trace->node_ptr[hash] = new Node(hash_num);
        trace = trace->node_ptr[hash];
    }
    trace->data_set.insert(data);
}

int HashTree::Travel(const IntList& trans) {
    buff.clear();
    _travel(IntList(patten_len), 0, trans, 0);
    return buff.size();
}

IntList HashTree::Output(const int& idx) {
    return buff[idx];
}


extern "C" {
    HashTree* new_HashTree(int hash_num) { 
        return new HashTree(hash_num); 
    }
    void AddData(HashTree* tree, IntList data) {
        tree->buildTree(data);
    }
    int Travel(HashTree* tree, IntList trans) {
        return tree->Travel(trans);
    }
    IntList* Output(HashTree* tree, int idx) {
        return new IntList(tree->Output(idx));
    }
}
