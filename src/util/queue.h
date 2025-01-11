#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <vector>

class Queue {
private:
    int front_;
    int rear_;
    std::vector<double> array_;
public:
    Queue(unsigned int s);
    bool isEmpty() const;
    bool isFull() const;
    void enqueue(double item);
    unsigned int size() const;
    double& operator[](int index);
    const double& operator[](int index) const;
};

#endif // __QUEUE_H__
