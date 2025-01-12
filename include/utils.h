#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

// Thread-safe functions to generate random integers and floats
int generate_random(int min, int max);
double generate_random_real(double min, double max);
std::string to_string_with_precision(double value, int precision = 2);
std::string get_random_stock_name();
std::string get_random_sector();

#endif /* __UTILS_H__ */