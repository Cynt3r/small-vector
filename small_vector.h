#ifndef EPC_SMALL_VECTOR
#define EPC_SMALL_VECTOR

#include <cstddef>
#include <memory>

namespace epc
{
template<typename T, size_t N>
class small_vector
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;

    small_vector() : size_(0), capacity_(N), data_(nullptr) {}
    small_vector(const small_vector& other) : small_vector() {
        if (other.size_ == 0) return;
        else if (other.size_ <= N) std::uninitialized_copy((T*)other.buffer_, (T*)other.buffer_ + other.size_, (T*)buffer_);
        else {
            data_ = (T*)::operator new(other.size_ * sizeof(T));
            capacity_ = other.size_;
            try { std::uninitialized_copy(other.data_, other.data_ + other.size_, data_); }
            catch(...) {
                ::operator delete(data_); throw;
            }
        }
        size_ = other.size_;
    }
    small_vector(small_vector&& other) noexcept : small_vector()  {
        swap(other);
        other.clear();
    }
    small_vector(std::initializer_list<value_type> init) : small_vector() {
        prepare_storage(init.size());
        try { std::uninitialized_copy(init.begin(), init.end(), data()); }
        catch(...) {
            if (capacity_ != N) ::operator delete(data_);
            throw;
        }
        size_ = init.size();
    }
    ~small_vector() {
        clear();
        if (capacity_ != N) ::operator delete(data_);
    }
    small_vector& operator=(const small_vector& other) {
        if (capacity_ < other.size_) {
            small_vector temp(other);
            swap(temp);
        }
        else if (this != &other) {
            clear();
            std::uninitialized_copy(other.data(), other.data() + other.size_, data());
            size_ = other.size_;
        }
        return *this;
    }
    small_vector& operator=(small_vector&& other) noexcept {
        if (this == &other) return *this;
        //clearing vector before swapping it is more efficient
        clear();
        if (capacity_ != N) {
            //if vector is not using the buffer, we have to take care of deleting data_
            ::operator delete(data_);
            data_ = nullptr;
            //setting capacity back to N is necessary, otherwise swap would not work properly
            capacity_ = N;
        }
        swap(other);
        return *this;
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    pointer data() { return capacity_ == N ? (T*)buffer_ : data_; }
    const_pointer data() const { return capacity_ == N ? (T*)buffer_ : data_; }
    void clear() noexcept {
        try { for (; size_ > 0; size_--) (data() + size_ - 1)->~T(); }
        catch (...) {}
    }
    void reserve(size_t capacity) {
        if (capacity <= capacity_) return;
        T* new_data = (T*)::operator new(capacity * sizeof(T));
        size_t i = 0;
        try { for (; i < size_; i++) new (new_data + i) T(std::move_if_noexcept(*(data() + i))); }
        catch(...) {
            for (; i > 0; i--) (new_data + i - 1)->~T();
            ::operator delete(new_data);
            throw;
        }
        clear();
        if (capacity_ != N) ::operator delete(data_);
        data_ = new_data;
        capacity_ = capacity;
        size_ = i;
    }
    void push_back(const value_type& value) { emplace_back(value); }
    void push_back(value_type&& value) { emplace_back(std::move(value)); }
    template <typename... Ts>
    void emplace_back(Ts&&... vs) {
        prepare_storage(size_ + 1);
        new (data() + size_) T(std::forward<Ts>(vs)...);
        size_++;
    }
    void resize(size_t size, const value_type& value = value_type()) {
        prepare_storage(size);
        //if required size is bigger than current size, insert new elements
        while (size_ < size) push_back(value);
        //if current size is bigger than required size, delete elements
        try { for (; size_ > size; size_--) (data() + size_ - 1)->~T(); }
        catch (...) {}
    }
    reference operator[](size_t index) { return (*(data() + index)); }
    const_reference operator[](size_t index) const { return (*(data() + index)); }
    iterator begin() { return data(); }
    const_iterator begin() const { return data(); }
    iterator end() { return (data() + size_); }
    const_iterator end() const { return (data() + size_); }
    void swap(small_vector& other) noexcept {
        using std::swap;

        small_vector<T, N> * smaller;
        small_vector<T, N> * bigger;
        if (size_ < other.size_) {  smaller = this; bigger = &other; }
        else { smaller = &other; bigger = this; }

        if (smaller->capacity_ == N && bigger->capacity_ == N) {
            //swapping smaller->size_ elements
            for (size_t i = 0; i < smaller->size_; i++) {
                T temp(std::move_if_noexcept(*((T*)smaller->buffer_ + i)));
                new ((T*)smaller->buffer_ + i) T(std::move_if_noexcept(*((T*)bigger->buffer_ + i)));
                new ((T*)bigger->buffer_ + i) T(std::move_if_noexcept(temp));
            }
            //moving the rest
            for (size_t i = smaller->size_; i < bigger->size_; i++) new ((T*)smaller->buffer_ + i) T(std::move_if_noexcept(*((T*)bigger->buffer_ + i)));
        }
        else {
            if (smaller->capacity_ == N) {
                for (size_t i = 0; i < smaller->size_; i++) new ((T*)bigger->buffer_ + i) T(std::move_if_noexcept(*((T*)smaller->buffer_ + i)));
            }
            swap(smaller->data_, bigger->data_);
            swap(smaller->capacity_, bigger->capacity_);
        }
        swap(smaller->size_, bigger->size_);
    }

private:
    size_t size_;
    size_t capacity_;
    T* data_;
    alignas(alignof(T))char buffer_[sizeof(T) * N];

    //prepares storage for n elements and calls reserve if necessary
    void prepare_storage(size_t n) {
        size_t new_capacity = capacity_;
        while (new_capacity < n) new_capacity *= 2;
        reserve(new_capacity);
    }
};

template <typename T, size_t N>
void swap(small_vector<T,N>& a, small_vector<T,N>& b) noexcept { a.swap(); }
}

#endif // EPC_SMALL_VECTOR