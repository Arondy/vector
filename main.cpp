#include <iostream>
#include <memory>
#include <vector>
#include "vector/vector.h"
#include "history/history.h"
#include "square.h"

using std::cin, std::cout, std::cerr, std::endl, std::string, std::shared_ptr, std::make_shared, MyVec::vector;

int main(){
    try {
        vector<shared_ptr<Square>> s{};
//        s.reserve(20);
        s.push_back(make_shared<Square>(Square{1, 0}));
        s.~vector();
    }
    catch (const std::bad_alloc &ba){
        cerr << "Not enough memory" << endl;
        return 1;
    }
    catch (const std::exception &e){
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}