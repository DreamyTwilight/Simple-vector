#pragma once

#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <initializer_list>
#include <iterator>
#include <functional>
#include <utility>

#include "array_ptr.h"

struct ReserveProxyObj {
    size_t capacity_to_reserve_ = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    ReserveProxyObj rezult;
    rezult.capacity_to_reserve_ = capacity_to_reserve;
    return rezult;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size)
        : SimpleVector(size, Type{})
    {
    }
    explicit SimpleVector(ReserveProxyObj struct_with_capacity_to_reserve) {
        if (struct_with_capacity_to_reserve.capacity_to_reserve_ > 0) {
            ResizeCapacity(struct_with_capacity_to_reserve.capacity_to_reserve_);
        }
    }

    SimpleVector(size_t size, const Type& value)
        : data_(size)
        , size_(size)
        , capacity_(size)
    {
        std::generate(begin(), end(), [&value]() { return value; });
    }

    SimpleVector(size_t size, Type&& value)
        : data_(size)
        , size_(size)
        , capacity_(size)
    {
        std::generate(begin(), end(), [&value]() { return std::move(value); });
    }

    SimpleVector(std::initializer_list<Type> init) : SimpleVector(init.size(), Type{}) {
        std::copy(init.begin(), init.end(), this->begin());
    }

    SimpleVector(const SimpleVector& other) : SimpleVector(other.size_, Type{}) {
        std::copy(other.begin(), other.end(), this->begin());
    }

    SimpleVector(SimpleVector&& other) noexcept {
        capacity_ = std::exchange(other.capacity_, 0);
        size_ = std::exchange(other.size_, 0);
        data_.Swap(other.data_);
    }

    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return data_[index];
    }

    Type& At(size_t index) {
        if (index >= capacity_) {
            throw std::out_of_range("Out of range.");
        }
        return data_[index];
    }

    const Type& At(size_t index) const {
        if (index >= capacity_) {
            throw std::out_of_range("Out of range.");
        }
        return data_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
        }
        else if (new_size > size_ && new_size <= capacity_) {
            SimpleVector<Type> tmp(new_size - size_);
            std::move(tmp.begin(), tmp.end(), this->end());
            size_ = new_size;
        }
        else {
            auto new_capacity = capacity_ * 2;
            if (new_size >= new_capacity) new_capacity = new_size * 2;
            ResizeCapacity(new_capacity);
            size_ = new_size;
        }
    }

    Iterator begin() noexcept {
        return data_.Get();
    }

    Iterator end() noexcept {
        return data_.Get() + size_;
    }

    ConstIterator begin() const noexcept {
        return this->cbegin();
    }

    ConstIterator end() const noexcept {
        return this->cend();
    }

    ConstIterator cbegin() const noexcept {
        return data_.Get();
    }

    ConstIterator cend() const noexcept {
        return data_.Get() + size_;
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this == &rhs) return *this;
        if (rhs.IsEmpty()) {
                this->data_.Clear();
            size_ = 0;
            capacity_ = 0;
            return *this;
        }
        SimpleVector copy_rhs(rhs);
        this->swap(copy_rhs);
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this == &rhs) return *this;
        if (rhs.IsEmpty()) {
            this->data_.Clear();
            size_ = 0;
            capacity_ = 0;
            return *this;
        }
        this->swap(rhs);
        return *this;
    }
    
    void PushBack(const Type& item) {
        if (size_ < capacity_) {
            data_[size_++] = item;
        }
        else {
            if (capacity_ == 0) {
                ResizeCapacity(1);
                data_[size_++] = item;
            }
            else {
                ResizeCapacity(capacity_ * 2);
                data_[size_++] = item;
            }
        }
    }

    void PushBack(Type&& item) {
        if (size_ < capacity_) {
            data_[size_++] = std::move(item);
        }
        else {
            if (capacity_ == 0) {
                ResizeCapacity(1);
                data_[size_++] = std::move(item);
            }
            else {
                ResizeCapacity(capacity_ * 2);
                data_[size_++] = std::move(item);
            }
        }
    }

    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        if (capacity_ == 0) {
            ResizeCapacity(1);
            data_[size_++] = value;
            return this->begin();
        }
        if (capacity_ == size_) {
            size_t id_pos = std::distance(&data_[0], const_cast<Type*>(pos));
            ResizeCapacity(capacity_ * 2);
            std::copy_backward(&data_[id_pos], &data_[size_], &data_[size_ + 1]);
            data_[id_pos] = value;
            ++size_;
            return &data_[id_pos];
        }
        else {
            size_t id_pos = std::distance(&data_[0], const_cast<Type*>(pos));
            std::copy_backward(&data_[id_pos], &data_[size_], &data_[size_ + 1]);
            data_[id_pos] = value;
            ++size_;
            return &data_[id_pos];
        }
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        size_t id_pos = std::distance(&data_[0], const_cast<Type*>(pos));
        if (capacity_ == 0) {
            ResizeCapacity(1);
            data_[size_++] = std::move(value);
            return this->begin();
        }
        if (capacity_ == size_) {
            ResizeCapacity(capacity_ * 2);
            std::copy_backward(std::make_move_iterator(&data_[id_pos]), std::make_move_iterator(&data_[size_]), &data_[size_ + 1]);
            data_[id_pos] = std::move(value);
            ++size_;
            return &data_[id_pos];
        }
        else {
            std::copy_backward(std::make_move_iterator(&data_[id_pos]), std::make_move_iterator (&data_[size_]), &data_[size_ + 1]);
            data_[id_pos] = std::move(value);
            ++size_;
            return &data_[id_pos];
        }
    }

    void PopBack() noexcept {
        if (size_ > 0) {
            --size_;
        }
    }

    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos < end());
        size_t distance_begin_to_post = std::distance(&data_[0], const_cast<Type*>(pos));
        std::copy(std::make_move_iterator(&data_[distance_begin_to_post + 1]), std::make_move_iterator(&data_[size_]), &data_[distance_begin_to_post]);
        --size_;
        return &data_[distance_begin_to_post];
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ResizeCapacity(new_capacity);
        }
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(other.size_, size_);
        std::swap(other.capacity_, capacity_);
    }

private:
    ArrayPtr<Type> data_ = {};
    size_t size_ = 0;
    size_t capacity_ = 0;

    void ResizeCapacity(size_t new_capacity) {
        ArrayPtr<Type> data_tmp(new_capacity);
        if (size_ > 0) {
            std::move(&data_[0], &data_[size_], &data_tmp[0]);
        }
        std::generate(&data_tmp[size_], &data_tmp[new_capacity - 1], []() { return Type{};});
        data_.Swap(data_tmp);
        capacity_ = new_capacity;
    }
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return  std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs > lhs);
}
