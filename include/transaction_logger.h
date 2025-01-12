#ifndef __TRANSACTION_LOGGER_H__
#define __TRANSACTION_LOGGER_H__

#include <string>
#include <mutex>
#include <queue>

class TransactionLogger {
public:
    void log(const std::string &message);
    void printAll();

private:
    std::mutex log_mutex_;
    std::queue<std::string> messages_;
};

#endif /* __TRANSACTION_LOGGER_H__ */