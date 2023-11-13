# Vector implementation in C++ (with iterator)
Raw memory allocation and deallocation, placement new are used for correct operations with complex classes working with memory.
## Following methods are implemented:
- size()
- capacity()
- empty()
- reserve()
- at()
- front()
- back()
- data()
- push_back()
- emplace_back()
- clear()
- erase()
- begin()
- end()
- cbegin()
- cend()
- swap()

Iterator realization meets std::random_access_iterator requirements.\
Tests are made with CATCH2; buffer.h contains class with memory use (new[], delete[]) for additional tests.\
Integration by CmakeLists may require "set_target_properties(vector PROPERTIES LINKER_LANGUAGE CXX)".
