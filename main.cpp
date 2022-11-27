#include <sqlite3.h>
#include <restbed>
#include <iostream>

int main(void) {
    std::cout << sqlite3_libversion() << '\n';
    return 0;
}