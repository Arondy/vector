#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <algorithm>
#include <iterator>

using std::cout, std::endl;
namespace MyVec {
    template <typename T, bool is_const>
    class VectorIterator {
    private:
        T *ptr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        VectorIterator() noexcept: ptr{nullptr}{};
        explicit VectorIterator(T *p) : ptr(p){}

        template <bool other_const>
        explicit
        VectorIterator(const VectorIterator<T, other_const> &other) noexcept requires (is_const <= other_const): ptr{
                other.ptr}{}

        template <bool other_const>
        explicit
        VectorIterator(VectorIterator<T, other_const> &&other) noexcept requires (is_const <= other_const): ptr{
                other.ptr}{
            other.ptr = nullptr;
        }

        template <bool other_const>
        VectorIterator &
        operator=(const VectorIterator<T, other_const> &other) noexcept requires (is_const <= other_const){
            if (this != &other){
                ptr = other.ptr;
            }
            return *this;
        }

        template <bool other_const>
        VectorIterator &operator=(VectorIterator<T, other_const> &&other) noexcept requires (is_const <= other_const){
            if (this != &other){
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }

        VectorIterator &operator++() noexcept{
            ++ptr;
            return *this;
        }
        VectorIterator operator++(int){
            VectorIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        VectorIterator &operator--() noexcept{
            --ptr;
            return *this;
        }
        VectorIterator operator--(int){
            VectorIterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const noexcept{ return *ptr; }
        pointer operator->(){ return ptr; }
        reference operator[](difference_type n){ return *(ptr + n); }
        reference operator[](difference_type n) const{ return *(ptr + n); }

        VectorIterator &operator+=(difference_type n){
            ptr += n;
            return *this;
        }
        VectorIterator &operator-=(difference_type n){
            ptr -= n;
            return *this;
        }
        VectorIterator operator+(difference_type n) const{ return VectorIterator(ptr + n); }
        VectorIterator operator-(difference_type n) const{ return VectorIterator(ptr - n); }
        difference_type operator+(const VectorIterator &other) const{ return ptr + other.ptr; }
        difference_type operator-(const VectorIterator &other) const{ return ptr - other.ptr; }
        friend VectorIterator operator+(difference_type n, const VectorIterator &it){
            return VectorIterator(n + it.ptr);
        }
        friend VectorIterator operator-(difference_type n, const VectorIterator &it){
            return VectorIterator(n - it.ptr);
        }

        bool operator<(const VectorIterator &other) const{ return ptr < other.ptr; }
        bool operator>(const VectorIterator &other) const{ return ptr > other.ptr; }
        bool operator<=(const VectorIterator &other) const{ return ptr <= other.ptr; }
        bool operator>=(const VectorIterator &other) const{ return ptr >= other.ptr; }
        bool operator==(const VectorIterator &other) const{ return ptr == other.ptr; }
        bool operator!=(const VectorIterator &other) const{ return !(*this == other); }
    };

    static_assert(std::random_access_iterator<VectorIterator<std::size_t, false>>);
    static_assert(std::random_access_iterator<VectorIterator<std::size_t, true>>);

    template <typename T>
    class vector {
        using iterator = VectorIterator<T, false>;
        using const_iterator = VectorIterator<T, true>;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = std::size_t;
    private:
        size_type _size = 0;
        size_type _capacity = 0;
        T *array = nullptr;
        void erase_till_end(iterator pos) noexcept;
        void swap(vector &second) noexcept;
    public:
        vector() noexcept(std::is_nothrow_default_constructible_v<T>) = default;
        explicit vector(size_type capacity) requires std::copy_constructible<T>;
        explicit vector(size_type size, const T &el) requires std::copy_constructible<T>;
        vector(std::initializer_list<T> list) requires std::move_constructible<T>;
        vector(const vector &second) requires std::copy_constructible<T>;
        vector(vector &&second) noexcept;

        vector &operator=(const vector &second) requires std::copy_constructible<T>;
        vector &operator=(vector &&second) noexcept;
        vector &operator=(std::initializer_list<T> list) requires std::move_constructible<T>;
        [[nodiscard]] T &operator[](size_type index){ return array[index]; };
        [[nodiscard]] const T &operator[](size_type index) const{ return array[index]; };

        [[nodiscard]] size_type size() const noexcept{ return _size; };
        [[nodiscard]] size_type capacity() const noexcept{ return _capacity; };
        [[nodiscard]] bool empty() const noexcept{ return _size == 0; };
        void reserve(size_type newCap);
        [[nodiscard]] const T &at(size_type index) const;
        [[nodiscard]] T &at(size_type index);
        [[nodiscard]] const T &front() const{ return array[0]; };
        [[nodiscard]] T &front(){ return array[0]; };
        [[nodiscard]] const T &back() const{ return array[_size - 1]; };
        [[nodiscard]] T &back(){ return array[_size - 1]; };
        [[nodiscard]] const T *data() const{ return array; };
        [[nodiscard]] T *data(){ return array; };
        void push_back(const T &element) requires std::copy_constructible<T>;
        void push_back(T &&element) requires std::move_constructible<T>;
        template <typename ...Args>
        void emplace_back(Args &&... args) requires std::constructible_from<T, Args...>;
        void clear() noexcept;
        ~vector();

        iterator begin() noexcept{ return iterator(array); }
        iterator end() noexcept{ return iterator(array + _size); }
        const_iterator begin() const noexcept{ return const_iterator(array); }
        const_iterator end() const noexcept{ return const_iterator(array + _size); }
        const_iterator cbegin() const noexcept{ return const_iterator(array); }
        const_iterator cend() const noexcept{ return const_iterator(array + _size); }

        iterator erase(iterator it) noexcept;
        iterator erase(iterator first, iterator last) noexcept;
    };
}

namespace MyVec {
    template <typename T>
    using iterator = VectorIterator<T, false>;
    template <typename T>
    using const_iterator = VectorIterator<T, true>;

    template <typename T>
    vector<T>::vector(size_type capacity) requires std::copy_constructible<T>{
        reserve(capacity);
        for (size_type i = 0; i < capacity; i++){
            new(&array[i]) T();
        }
        _size = capacity;
    }

    template <typename T>
    vector<T>::vector(size_type size, const T &el) requires std::copy_constructible<T>{
        reserve(size);
        for (size_type i = 0; i < size; i++){
            new(&array[i]) T(el);
        }
        _size = size;
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list) requires std::move_constructible<T>{
        reserve(list.size());

        size_type i = 0;
        for (const T &element: list){
            new(&array[i++]) T(element);
        }
        _size = list.size();
    }

    template <typename T>
    vector<T>::vector(const vector &second) requires std::copy_constructible<T>{
        reserve(second.size());

        for (size_type i = 0; i < second._size; i++){
            new(&array[i]) T(second.array[i]);
        }
        _size = second.size();
    }

    template <typename T>
    vector<T>::vector(vector &&second) noexcept{
        swap(second);
    }

    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &second) requires std::copy_constructible<T>{
        if (this != &second){
            clear();
            reserve(second._capacity);

            for (size_type i = 0; i < second._size; i++){
                new(&array[i]) T(second.array[i]);
            }
            _size = second._size;
        }
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&second) noexcept{
        if (this != &second){
            swap(second);
        }
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(std::initializer_list<T> list) requires std::move_constructible<T>{
        reserve(list.size());

        size_type i = 0;
        for (const T &element: list){
            new(&array[i++]) T(element);
        }
        _size = list.size();
        return *this;
    }

    template <typename T>
    void vector<T>::reserve(size_type newCap){
        if (newCap == 0){
            newCap = 1;
        } else if (newCap < _capacity){
            return;
        }
        size_type temp = _size;

        T *buffer = static_cast<T *>(::operator new(newCap * sizeof(T)));
        for (size_type i = 0; i < _size; i++){
            new(&buffer[i]) T(std::move(array[i]));
        }
        clear();
        ::operator delete(array, _capacity * sizeof(T));
        array = std::move(buffer);
        _capacity = newCap;
        _size = temp;
    }

    template <typename T>
    const T &vector<T>::at(size_type index) const{
        if (index >= _size){
            throw std::out_of_range("Index is out of range");
        }
        return array[index];
    }

    template <typename T>
    T &vector<T>::at(size_type index){
        if (index >= _size){
            throw std::out_of_range("Index is out of range");
        }
        return array[index];
    }

    template <typename T>
    void vector<T>::push_back(const T &element) requires std::copy_constructible<T>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size++]) T(std::move(element));
    }

    template <typename T>
    void vector<T>::push_back(T &&element) requires std::move_constructible<T>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size++]) T(std::move(element));
    }

    template <typename T>
    template <typename ...Args>
    void vector<T>::emplace_back(Args &&... args) requires std::constructible_from<T, Args...>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size]) T{std::forward<Args>(args)...};
        _size++;
    }

    template <typename T>
    void vector<T>::clear() noexcept{
        for (int i = 0; i < _size; ++i){
            array[i].~T();
        }
        _size = 0;
    }

    template <typename T>
    vector<T>::~vector(){
        clear();
        ::operator delete(array, _capacity * sizeof(T));
    }

    template <typename T>
    iterator<T> vector<T>::erase(iterator it) noexcept{
        if (it >= end()){
            return end();
        }

        if (it + 1 != end()){
            std::move(it + 1, end(), it);
        }

        array[--_size].~T();
        return it;
    }

    template <typename T>
    iterator<T> vector<T>::erase(iterator first, iterator last) noexcept{
        if (first != last){
            if (last != end()){
                std::move(last, end(), first);
            }
            erase_till_end(first + (end() - last));
        }
        return first;
    }

    template <typename T>
    void vector<T>::erase_till_end(iterator pos) noexcept{
        size_type num = end() - pos;
        if (num){
            for (iterator it = pos; it != end(); ++it){
                it->~T();
            }
            _size -= num;
        }
    }

    template <typename T>
    void vector<T>::swap(vector &second) noexcept{
        std::swap(array, second.array);
        std::swap(_size, second._size);
        std::swap(_capacity, second._capacity);
    }
}

#endif