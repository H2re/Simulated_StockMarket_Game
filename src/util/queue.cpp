#include "queue.h"

Queue::Queue(unsigned int s) : array_(s) {
    rear_ = -1;
    front_ = -1;
}

bool Queue::isEmpty() const {
    return rear_ == -1;
}

bool Queue::isFull() const {
    return (front_ + 1) % array_.size() == rear_;
}

void Queue::enqueue(double item) {
    if (isFull()) {
        rear_ = (rear_ + 1) % array_.size();
    }
    if (isEmpty()) {
        rear_ = 0;
    }
    front_ = (front_ + 1) % array_.size();
    array_[front_] = item;
}

unsigned int Queue::size() const {
    if (isEmpty()) {
        return 0;
    }
    if (front_ >= rear_) {
        return front_ - rear_ + 1;
    }
    return array_.size() - rear_ + front_ + 1;
}

double& Queue::operator[](int index) {
    if (isEmpty() || (!isFull() && index > size())) {
        throw std::out_of_range("Index out of range");
    }
    return array_[(rear_ + index) % array_.size()];
}

const double& Queue::operator[](int index) const {
    if (isEmpty() || (!isFull() && index > size())) {
        throw std::out_of_range("Index out of range");
    }
    return array_[(rear_ + index) % array_.size()];
}
