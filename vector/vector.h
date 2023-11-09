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
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::conditional_t<is_const, const T, T> *pointer;
        typedef std::conditional_t<is_const, const T, T> &reference;

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
//        typedef VectorIterator<T, false> iterator;
//        typedef VectorIterator<T, true> const_iterator;
    private:
        std::size_t size_ = 0;
        std::size_t capacity = 0;
        T *array = nullptr;
    public:
        vector() = default;
        explicit vector(std::size_t capacity);
        explicit vector(std::size_t size, T el);
        vector(std::initializer_list<T> list);
        vector(const vector &second);
        vector(vector &&second) noexcept;
        vector &operator=(const vector &second);
//        vector &operator=(const T &element); //?
        vector &operator=(vector &&second) noexcept;
        [[nodiscard]] std::size_t size() const{ return size_; };
        [[nodiscard]] bool empty() const{ return size_ == 0; };
        [[nodiscard]] T &operator[](std::size_t index){ return array[index]; };
        [[nodiscard]] const T &operator[](std::size_t index) const{ return array[index]; };
        void reserve(std::size_t newCap);
        [[nodiscard]] const T &at(std::size_t index) const;
        [[nodiscard]] T &back() const{ return array[size_ - 1]; };
        void push_back(T element);
        template <typename ...Args>
        void emplace_back(Args &&... args);
        ~vector();

//        iterator begin(){ return iterator(array); }
//        iterator end(){ return iterator(array + size_); }
//        const_iterator begin() const{ return const_iterator(array); }
//        const_iterator end() const{ return const_iterator(array + size_); }

//        iterator find(const T &value) const;
        void clear() noexcept;
//        VectorIterator<T, false> erase(iterator it);
//        VectorIterator<T, false> erase(iterator begin, iterator end);
    };
}

namespace MyVec {
    template <typename T>
    vector<T>::vector(std::size_t capacity){
        reserve(capacity);
        size_ = capacity;
        for (int i = 0; i < size_; i++){
            array[i] = T{};
        }
    }

    template <typename T>
    vector<T>::vector(std::size_t size, T el){
        reserve(size);
        for (int i = 0; i < size; i++){
            array[i] = el;
        }
        size_ = size;
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list){
        reserve(list.size());

        int i = 0;
        for (const T &element: list){
            array[i++] = element;
        }
        size_ = list.size();
    }

    template <typename T>
    vector<T>::vector(const vector &second){
        reserve(second.size());

        int i = 0;
        for (i = 0; i < second.size_; i++){
            array[i] = second.array[i];
        }
//        for (const T &element: second){
//            array[i++] = element;
//        }
        size_ = second.size();
    }

    template <typename T>
    vector<T>::vector(vector &&second) noexcept{
        std::swap(array, second.array);
        std::swap(size_, second.size_);
        std::swap(capacity, second.capacity);
    }

    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &second){
        if (this != &second){
            clear();
            reserve(second.capacity);

            for (int i = 0; i < second.size_; i++){
                array[i] = second[i];
            }
            size_ = second.size_;
        }
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&second) noexcept{
        if (this != &second){
            std::swap(array, second.array);
            std::swap(size_, second.size_);
            std::swap(capacity, second.capacity);
        }
        return *this;
    }

    template <typename T>
    void vector<T>::reserve(std::size_t newCap){
        try {
            if (newCap == 0){
                newCap = 1;
            } else if (newCap < capacity){
                return;
            }

            T *buffer = new T[newCap];
            for (int i = 0; i < size_; i++){
                buffer[i] = std::move(array[i]);
            }
            std::swap(array, buffer);
            capacity = newCap;
            delete[] buffer;
        } catch (const std::exception &e){
            cout << e.what() << endl;
        }
    }

    template <typename T>
    const T &vector<T>::at(std::size_t index) const{
        if (index < 0 || index >= size_){
            throw std::out_of_range("Index is out of range");
        }
        return array[index];
    }

    template <typename T>
    void vector<T>::push_back(T element){
        if (size_ == capacity){
            reserve(capacity * 2);
        }
        array[size_++] = element;
    }

    template <typename T>
    template <typename ...Args>
    void vector<T>::emplace_back(Args &&... args){
        if (size_ == capacity){
            reserve(capacity * 2);
        }
        array[size_++] = T{std::forward<Args>(args)...};
    }

    template <typename T>
    vector<T>::~vector(){
        clear();
        delete[] array;
        array = nullptr;
    }

//    template <typename T>
//    VectorIterator<T, false> vector<T>::find(const T &value) const{
//        for (const_iterator it = begin(); it != end(); it++){
//            if (*it == value){
//                return it;
//            }
//        }
//        return end();
//    }

    template <typename T>
    void vector<T>::clear() noexcept{
        for (int i = 0; i < size_; i++){
            array[i].~T();
        }
        size_ = 0;
    }

//    template <typename T>
//    VectorIterator<T, false> vector<T>::erase(iterator it){
//        iterator next = it++;
//
//        if (it < begin()){
//            throw std::out_of_range("Can't be before begin()");
//        } else if (it == end()){
//            return end();
//        }
//
//        it->~T();
//
//        for (iterator i = it; i != end() - 1; i++){
//            *i = *(i + 1);
//        }
//
//        size_--;
//        return next;
//    }
//
//    template <typename T>
//    VectorIterator<T, false> vector<T>::erase(iterator begin, iterator end){
//        while (begin != end){
//            begin = erase(begin);
//            size_--;
//        }
//        return end;
//    }
}

#endif