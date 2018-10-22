#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include "IntList.hpp"
#include "FPTree.hpp"

PatternNode::PatternNode(int id, PatternNode* parent):
    id(id),
    freq(0),
    parent_node_ptr(parent) {}

PatternNode::~PatternNode() {
    std::map<int, PatternNode*>::iterator it;
    for (it = child_node_ptr.begin(); it != child_node_ptr.end(); ++it)
        delete (it->second);
    if (parent_node_ptr) parent_node_ptr->child_node_ptr.erase(id);
}

PatternNode* PatternNode::Trace(const int& id, int weight) {
    // Will be insert new node, when child_node_ptr is NULL
    if (child_node_ptr.find(id) == child_node_ptr.end())
        child_node_ptr[id] = new PatternNode(id, this);
    child_node_ptr[id]->freq += weight;
    return child_node_ptr[id];
}

int PatternNode::GetID() const {
    return id;
}

int PatternNode::GetFreq() const {
    return freq;
}

std::map<int, PatternNode*>::iterator PatternNode::GetBeginPtr() {
    return child_node_ptr.begin();
}

std::map<int, PatternNode*>::iterator PatternNode::GetEndPtr() {
    return child_node_ptr.end();
}

FPNode::FPNode(int id, FPNode* parent):
    id(id),
    freq(0),
    child_node_ptr(),
    link_ptr(NULL),
    parent_node_ptr(parent) {}

FPNode::~FPNode() {
    std::map<int, FPNode*>::iterator it;
    for (it = child_node_ptr.begin(); it != child_node_ptr.end(); ++it)
        delete (it->second);
}

bool FPNode::TryID(const int& id) { // Will be insert new node, when child_node_ptr is NULL
    bool res = child_node_ptr.find(id) == child_node_ptr.end();
    if (res)
        child_node_ptr[id] = new FPNode(id, this);
    return res;
}

FPNode* FPNode::Trace(const int& id, int weight) {
    child_node_ptr[id]->freq += weight;
    return child_node_ptr[id];
}

FPNode* FPNode::SetLink(FPNode* next) {
    return link_ptr = next;
}

FPNode* FPNode::NextLink() const {
    return link_ptr;
}

FPNode* FPNode::Parent() const {
    return parent_node_ptr;
}

int FPNode::GetID() const {
    return id;
}

int FPNode::GetFreq() const {
    return freq;
}

void FPTree::_GenPattern(PatternNode* node,
                 const int& main_id, std::vector<int> current_pattern,
                 int freq, bool flag) {
    std::map<int, PatternNode*>::iterator it;
    bool is_leaf(1);
    for (it = node->GetBeginPtr(); it != node->GetEndPtr(); ++it) {
        is_leaf = 0;
        _GenPattern(it->second, main_id, current_pattern, freq, flag);
        current_pattern.push_back(it->second->GetID());
        _GenPattern(it->second, main_id, current_pattern, it->second->GetFreq(), 0);
        current_pattern.pop_back();
        flag = 1;
    }
    if (!freq | !is_leaf | flag) return;
    current_pattern.push_back(main_id);
    IntList pat(current_pattern.size());

    for (int i(0); i < pat.length(); ++i)
        pat[i] = current_pattern[i];

    if (buff.find(pat) == buff.end()) buff[pat] = freq;
    else                              buff[pat] += freq;
}

FPTree::FPTree(): root(new FPNode()) {}

FPTree::~FPTree() {
    delete root;
}

void FPTree::AddTrans(const IntList& trans) {
    int i;
    FPNode* trace(root);
    for (i = 0; i < trans.length(); ++i) {
        if (trace->TryID(trans[i])) {
            trace = trace->Trace(trans[i]);
            if (link_tail.find(trans[i]) == link_tail.end()) { // New linked list
                link_head[trans[i]] = link_tail[trans[i]] = trace;
            }
            else {
                link_tail[trans[i]] = link_tail[trans[i]]->SetLink(trace);
            }
        }
        else {
            trace = trace->Trace(trans[i]);
        }
    }
}

void FPTree::TraceID(const int& id) {
    FPNode* trace(link_head[id]);
    while (trace) trace = trace->NextLink();
}

int FPTree::FindPattern() {
    buff.clear();
    FPNode* trace_right;
    FPNode* trace_up;
    PatternNode* pat_root;
    PatternNode* trace_pattern;
    std::stack<int> pattern;
    int current_id;
    int weight;
    int id;
    std::map<int, FPNode*>::iterator it;
    for (it = link_head.begin(); it != link_head.end(); ++it) {
        current_id = it->first;
        trace_right = it->second;
        while (trace_right) {
            trace_up = trace_right->Parent();
            while (trace_up->Parent()) {
                pattern.push(trace_up->GetID());
                trace_up = trace_up->Parent();
            }
            pattern.push(trace_right->GetFreq());
            pattern.push(-1); // Mark the pattern end
            trace_right = trace_right->NextLink();
        }
        pat_root = new PatternNode(); // Build pattern tree
        while (!pattern.empty()) {
            trace_pattern = pat_root;
            pattern.pop(); // Pop -1 at top
            weight = pattern.top();
            pattern.pop();
            while (!pattern.empty() && (id = pattern.top()) != -1) {
                pattern.pop();
                trace_pattern = trace_pattern->Trace(id, weight);
            }
        }
        // Generate pattern
        _GenPattern(pat_root, current_id);
        delete pat_root;
    }
    return buff.size();
}

IntList FPTree::OutputPattern(const int& idx) {
    std::map<IntList, int>::iterator it(buff.begin());
    IntList res(it->first.length() + 1);
    for (int i(0); i < res.length() - 1; ++i) {
        res[i] = it->first[i];
    }
    res[res.length() - 1] = it->second;
    buff.erase(it);
    return res;
}

extern "C" {
    FPTree* new_FPTree() { return new FPTree(); }
    void AddTrans(FPTree fp, IntList trans) {
        fp.AddTrans(trans);
    }
    void TraceID(FPTree fp, int id) {
        fp.TraceID(id);
    }
    int FindPattern(FPTree fp) {
        return fp.FindPattern();
    }
    IntList* OutputPattern(FPTree fp, int idx) {
        return new IntList(fp.OutputPattern(idx));
    }
    void del_FPTree(FPTree* fp) {
        delete fp;
    }
}
