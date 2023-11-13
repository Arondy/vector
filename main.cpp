#include <iostream>
#include <memory>
#include <vector>
#include "vector/vector.h"
#include "buffer.h"

using namespace MyVec;
using std::cin, std::cout, std::cerr, std::endl, std::string, std::shared_ptr, std::make_shared;
using MyVec::vector, MyVec::VectorIterator;

int main(){
    try {
        using iterator = VectorIterator<Buffer, false>;
        using const_iterator = VectorIterator<Buffer, true>;
        vector<Buffer> v{Buffer{1}, Buffer{2}, Buffer{3}, Buffer{4}, Buffer{5}};
        Buffer b4{4};
        auto it = std::find(v.begin(), v.end(), b4);
        v.erase(it);
        bool t1 = (v[2] == Buffer{3});
        bool t2 = (v[3] == Buffer{4});
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