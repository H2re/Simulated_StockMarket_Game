#include <iostream>
#include "../include/constants.h"
#include "../include/transaction_logger.h"

void TransactionLogger::log(const std::string &message) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    if (messages_.size() >= TRANSACTION_LOG_LIMIT) {
        messages_.pop();
    }
    messages_.push(message);
}

void TransactionLogger::printAll() {
    std::lock_guard<std::mutex> lock(log_mutex_);
    while (!messages_.empty()) {
        std::cout << messages_.front() << std::endl;
        messages_.pop();
    }
}