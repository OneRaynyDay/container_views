#pragma once

// A simple class that acts like a rng::view that can perform deque-like operations
// on the underlying data.
#include <exception>
#include <iterator>

namespace views {

// Just like fixed-sized containers in STL, accessing or inserting an element
// past the specified capacity is undefined behavior. In this case, it will override
// elements in the beginning of the deque.
//
// Another important note is that upon a reallocation of the underlying container,
// the memory this view occupies may be freed and access to deque_view is undefined behavior.
template <typename Container>
class deque_view {
    using value_type = typename Container::value_type;
    using iter_type = typename Container::iterator;
    using const_iter_type = typename Container::const_iterator;
    using reverse_iter_type = typename std::reverse_iterator<iter_type>;
    using reverse_const_iter_type = typename std::reverse_iterator<const_iter_type>;
    using difference_type = typename std::iterator_traits<iter_type>::difference_type;
public:
    deque_view(const iter_type& begin, const iter_type& end) : _begin(begin), _end(end), _front(begin), _back(std::next(begin)), _capacity(end - begin), _size(0) {}
    deque_view(const deque_view& s) = default;
    deque_view& operator=(const deque_view& s) = default;

    /// Element access
    [[nodiscard]] value_type& at(std::size_t pos) const {
        // range check
        if(pos > _capacity)
            throw std::out_of_range("deque at() detected out of range.");
        return (*this)[pos];
    }

    // The following operation requires a random access iterator.
    [[nodiscard]] value_type& operator[](difference_type n) const {
        auto distance_to_end = std::distance(_front, _end);
        if (distance_to_end > n)
            return *(_front + n);
        // wrap backwards
        return *(_begin + (n - distance_to_end + 1));
    }

    [[nodiscard]] value_type& back() const {
        [[likely]] if (_size) {
            return *std::prev(_back);
        }
        throw std::runtime_error("deque_view is empty, so back() is undefined");
    }

    [[nodiscard]] value_type& front() const {
        [[likely]] if (_size) {
            return *_front;
        }
        throw std::runtime_error("deque_view is empty, so front() is undefined");
    }

    /// Iterators
    [[nodiscard]] iter_type begin() {
        return _front;
    }

    [[nodiscard]] const_iter_type begin() const {
        return const_iter_type(_front);
    }

    [[nodiscard]] const_iter_type cbegin() const {
        return const_iter_type(_front);
    }

    [[nodiscard]] iter_type end() {
        return _back;
    }

    [[nodiscard]] const_iter_type end() const {
        return const_iter_type(_back);
    }

    [[nodiscard]] const_iter_type cend() const {
        return const_iter_type(_back);
    }

    [[nodiscard]] reverse_iter_type rbegin() {
        return reverse_iter_type(_back);
    }

    [[nodiscard]] reverse_const_iter_type rbegin() const {
        return reverse_const_iter_type(_back);
    }

    [[nodiscard]] reverse_const_iter_type crbegin() const {
        return reverse_const_iter_type(_back);
    }

    [[nodiscard]] reverse_iter_type rend() {
        return reverse_iter_type(_front);
    }

    [[nodiscard]] reverse_const_iter_type rend() const {
        return reverse_const_iter_type(_front);
    }

    [[nodiscard]] reverse_const_iter_type crend() const {
        return reverse_const_iter_type(_front);
    }

    /// Capacity
    [[nodiscard]] bool empty() const {
        return _size == 0;
    }

    [[nodiscard]] std::size_t size() const {
        return _size;
    }

    [[nodiscard]] difference_type capacity() const {
        return std::distance(_begin, _end);
    }

    /// Modifiers
    void clear() {
        // O(1) clear
        _back = _front;
        _size = 0;
    }

    void push_front(const value_type& value) {
        // [ {x} x x x x ]
        //    f  b
        // Becomes
        //
        // [ x} x x x {x ]
        //      b      f
        // So _front never hits _end by popping.
        if (_front == _begin)
            _front = _end;
        --_front;
        *_front = value;
        _size++;
    }

    void push_back(const value_type& value) {
        // [ x x x x {x} ]
        //            f  b
        // Becomes
        //
        // [ x} x x x {x ]
        //      b      f
        // So _back never hits _begin by popping.
        if (_back == _end)
            _back = _begin;
        *_back = value;
        ++_back;
        _size++;
    }

    template<class... Args>
    void emplace_front(Args&&... args) {
        push_front(value_type(std::forward<Args>(args)...));
    }

    template<class... Args>
    void emplace_back(Args&&... args) {
        push_back(value_type(std::forward<Args>(args)...));
    }

    void pop_front() {
        [[unlikely]] if (!_size) {
            throw std::runtime_error("queue_view is empty, so pop() is undefined");
        }
        // [ x} x x x {x ]
        //      b      f
        //
        // Becomes
        //
        // [ {x} x x x x ]
        //    f  b
        // So _front never hits _end by popping.
        ++_front;
        if (_front == _end)
            _front = _begin;
        _size--;
    }

    void pop_back() {
        [[unlikely]] if (!_size) {
            throw std::runtime_error("deque_view is empty, so pop_back() is undefined");
        }
        --_back;
        // [ x} x x x {x ]
        //      b      f
        //
        // Becomes
        //
        // [ x x x x {x} ]
        //            f  b
        // So _back never hits _begin by popping.
        if (_back == _begin)
            _back = _end;
        _size--;
    }

    void resize(std::size_t count) {
        resize(count, value_type{});
    }

    void resize(std::size_t count, const value_type& value) {
        int diff = count - _size;
        if(diff > 0) {
            // Add the differential
            while(count--) {
                emplace_back();
            }
        }
        else if(diff  < 0) {
            // Subtract the differential
            while(count++) {
                pop_back();
            }
        }
        _size = count;
    }

    iter_type insert(iter_type pos, const value_type&  value) {
        throw std::runtime_error("deque_view does not have ownership over the "
                                 "underlying container, so insert in O(1) is not possible.");
    }

    iter_type erase(iter_type pos) {
        throw std::runtime_error("deque_view does not have ownership over the "
                                 "underlying container, so erase in O(1) is not possible.");
    }

    void swap(deque_view& s) {
        std::swap(*this, s);
    }
private:
    iter_type _begin, _end;
    iter_type _front, _back;
    difference_type _capacity;
    std::size_t _size;
};

} // namespace views
