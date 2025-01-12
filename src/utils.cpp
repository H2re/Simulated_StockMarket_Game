#include <random>
#include <sstream>
#include <iomanip>

#include "../include/constants.h"
#include "../include/utils.h"

int generate_random(int min, int max) {
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

double generate_random_real(double min, double max) {
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

std::string to_string_with_precision(double value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

std::string get_random_stock_name() {
    if (STOCK_NAMES.empty()) {
        return "Unnamed";  // fallback if the list is empty
    }
    int index = generate_random(0, static_cast<int>(STOCK_NAMES.size() - 1));
    return STOCK_NAMES[index];
}

std::string get_random_sector() {
    if (SECTORS.empty()) {
        return "Misc";
    }
    int index = generate_random(0, static_cast<int>(SECTORS.size() - 1));
    return SECTORS[index];
}