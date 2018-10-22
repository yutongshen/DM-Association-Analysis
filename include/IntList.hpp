class IntList{
    int len;
    int* data;
public:
    IntList(int len=0);
    IntList(const IntList& copy);
    ~IntList();
    int length() const;
    int& operator[](const int& idx);
    int operator[](const int& idx) const;
    IntList& operator=(const IntList& list);
    bool operator<(const IntList& list) const;
};
