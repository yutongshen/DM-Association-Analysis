#ifndef __HASHTREE__
#define __HASHTREE__

class Node{
    friend class HashTree;
    Node** node_ptr;
    std::set<IntList> data_set;
public:
    Node(const int& num);
    ~Node();
};

class HashTree{
    int hash_num;
    int patten_len;
    Node* root;
    std::vector<IntList> buff;
    void _travel(IntList patten, int patten_pos, const IntList& trans, int trans_pos);
public:
    HashTree(const int& hash_num);
    ~HashTree();
    void buildTree(const IntList& data);
    int Travel(const IntList& trans);
    IntList Output(const int& idx);
};

#endif
