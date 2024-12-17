#pragma once

#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size) {
        if (size == 0) {
            ArrayPtr();
        }
        else {
            Type* new_arr_ptr = new Type[size];
            raw_ptr_ = new_arr_ptr;
        }
    }

    explicit ArrayPtr(Type* raw_ptr) noexcept {
        raw_ptr_ = raw_ptr;
    }

    ~ArrayPtr() {
        delete[] this->raw_ptr_;
    }

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(const ArrayPtr&) = delete;
    
    ArrayPtr(ArrayPtr&& rhs) {
        raw_ptr_(std::exchange(rhs.raw_ptr_, nullptr));
    }

    ArrayPtr& operator=(ArrayPtr&& rhs) {
        if (this == &rhs) return *this;
        std::swap(raw_ptr_, rhs.raw_ptr_);
        return *this;
    }
    
    [[nodiscard]] Type* Release() noexcept {
        return std::exchange(raw_ptr_, nullptr);
    }

    void Clear() noexcept {
        raw_ptr_ = nullptr;
    }

    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return raw_ptr_[index];
    }

    explicit operator bool() const {
        return raw_ptr_ != nullptr;
    }

    Type* Get() const noexcept {
        return raw_ptr_;
    }

    void Swap(ArrayPtr& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};

