#ifndef __STOCK_H__
#define __STOCK_H__
#include "../orders/orderbook.h"
#include "../../util/queue.h"

class Stock {
public:
    inline Stock() : name_(""), price_(0.0), growth_(0.0), availableShares_(0), totShares_(0) {}
    inline Stock(std::string name, unsigned int shares) : 
        name_(name),
        price_(0.0), 
        growth_(0.0),
        availableShares_(shares), 
        totShares_(shares) {}
        
    inline const Queue& getHistory() const { return history_; }
    inline const Queue& getChange() const { return changes_; }
    inline std::string getName() const;
    inline double getPrice() const;
    inline double getGrowth() const;
    inline unsigned int getAvailableShares() const;

    double sell(unsigned int shares, double price);
    void buy(unsigned int shares, double price);
private:
    void updateStock(double value);
    std::string name_;
    Orderbook order_book_;
    Queue history_ = Queue(31);
    Queue changes_ = Queue(30);
    double price_;
    double growth_;
    unsigned int availableShares_;  
    unsigned int totShares_;
};

std::ostream& operator<<(std::ostream &ostr, const Stock& stock);

#endif // __STOCK_H__
