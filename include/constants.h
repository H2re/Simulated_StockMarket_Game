#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <vector>
#include <string>

// -----------------------------------------------------------------------------
// Global Constants
// -----------------------------------------------------------------------------

// Settings
inline constexpr int NUM_AGENTS            = 20;
inline constexpr int MAX_STOCKS            = 5;
inline constexpr int MAX_STOCK_VALUE       = 2000;
inline constexpr int MIN_STOCK_VALUE       = 0;
inline constexpr int MAX_VOLATILITY        = 100;
inline constexpr int TRANSACTION_LOG_LIMIT = 100;
inline constexpr int INITIAL_AGENT_WEALTH  = 5000;

// EVENTS
inline constexpr int MACRO_EVENT_PROB_PERCENT   = 10;
inline constexpr double MACRO_EVENT_MIN_IMPACT  = -0.05;
inline constexpr double MACRO_EVENT_MAX_IMPACT  =  0.05;

inline const std::vector<std::string> STOCK_NAMES = {
    "AAPL", "TSLA", "AMZN", "MSFT", "GOOGL",
    "FB",   "NVDA", "NFLX", "INTC", "AMD",
    "PYPL", "ADBE", "CRM",  "SHOP", "SQ"
};

inline const std::vector<std::string> SECTORS = { 
    "Technology", 
    "Healthcare", 
    "Finance", 
    "Energy" 
};

#endif /* __CONSTANTS_H__ */