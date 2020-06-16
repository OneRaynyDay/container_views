#pragma once

// A simple class that acts like a rng::view that can perform queue-like operations
// on the underlying data.
#include <exception>
#include <iterator>

namespace views {

// Just like fixed-sized containers in STL, accessing or inserting an element
// past the specified capacity is undefined behavior. In this case, it will override
// elements in the beginning of the queue.
//
// Another important note is that upon a reallocation of the underlying container,
// the memory this view occupies may be freed and access to queue_view is undefined behavior.
template <typename Container>
class queue_view {
    using value_type = typename Container::value_type;
    using iter_type = typename Container::iterator;
    using difference_type = typename std::iterator_traits<iter_type>::difference_type;
public:
    queue_view(const iter_type& begin, const iter_type& end) : _begin(begin), _end(end), _front(begin), _back(), _size(0) {}
    queue_view(const queue_view& s) = default;
    queue_view& operator=(const queue_view& s) = default;

    /// Element access
    [[nodiscard]] value_type& back() const {
        [[likely]] if (_size) {
            return *_back;
        }
        throw std::runtime_error("queue_view is empty, so back() is undefined");
    }

    [[nodiscard]] value_type& front() const {
        [[likely]] if (_size) {
            return *_front;
        }
        throw std::runtime_error("queue_view is empty, so front() is undefined");
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
    void push(const value_type& value) {
        [[unlikely]] if (!_size) {
            // _top is not a valid pointer, we point it to begin.
            _back = _front;
        }
        else {
            // Previously inserted element was at the last index.
            if (_back == _end)
                _back = _begin;
            else
                ++_back;
        }
        *_back = value;
        _size++;
    }

    template<class... Args>
    void emplace(Args&&... args) {
        push(value_type(std::forward<Args>(args)...));
    }

    void pop() {
        [[unlikely]] if (!_size) {
            throw std::runtime_error("queue_view is empty, so pop() is undefined");
        }
        // Element to pop is at the end of the container
        if (_front == _end)
            _front = _begin;
        else
            ++_front;
        _size--;
    }

    void swap(queue_view& s) {
        std::swap(*this, s);
    }
private:
    iter_type _begin, _end;
    iter_type _front, _back;
    std::size_t _size;
};

} // namespace views
