#ifndef __FPTREE__
#define __FPTREE__

class PatternNode{
    int id;
    int freq;
    std::map<int, PatternNode*> child_node_ptr;
    PatternNode* parent_node_ptr;
public:
    PatternNode(int id=0, PatternNode* parent=NULL);
    ~PatternNode();
    PatternNode* Trace(const int& id, int weight=1);
    int GetID() const;
    int GetFreq() const;
    std::map<int, PatternNode*>::iterator GetBeginPtr();
    std::map<int, PatternNode*>::iterator GetEndPtr();
};

class FPNode{
    int id;
    int freq;
    std::map<int, FPNode*> child_node_ptr;
    FPNode* parent_node_ptr;
    FPNode* link_ptr;
public:
    FPNode(int id=0, FPNode* parent=NULL);
    ~FPNode();
    bool TryID(const int& id);
    FPNode* Trace(const int& id, int weight=1);
    FPNode* SetLink(FPNode* next);
    FPNode* NextLink() const;
    FPNode* Parent() const;
    int GetID() const;
    int GetFreq() const;
};

class FPTree{
    FPNode* root;
    std::map<int, FPNode*> link_head;
    std::map<int, FPNode*> link_tail;
    std::map<IntList, int> buff;
    void _GenPattern(PatternNode* node,
                     const int& main_id,
                     std::vector<int> current_pattern=std::vector<int>(),
                     int freq=0, bool flag=0);
public:
    FPTree();
    ~FPTree();
    void AddTrans(const IntList& trans);
    void TraceID(const int& id);
    int FindPattern();
    IntList OutputPattern(const int& idx);
};

#endif
