#pragma once
// A simple class that acts like a rng::view that can perform stack-like operations
// on the underlying data.
#include <exception>
#include <iterator>

namespace views {

// Just like fixed-sized containers in STL, accessing or inserting an element
// past the specified capacity is undefined behavior.
//
// Another important note is that upon a reallocation of the underlying container,
// the memory this view occupies may be freed and access to stack_view is undefined behavior.
template <typename Container>
class stack_view {
    using value_type = typename Container::value_type;
    using iter_type = typename Container::iterator;
    using difference_type = typename std::iterator_traits<iter_type>::difference_type;
public:
    stack_view(const iter_type& begin, const iter_type& end) : _begin(begin), _end(end), _top(), _size(0) {}
    stack_view(const stack_view& s) = default;
    stack_view& operator=(const stack_view& s) = default;

    /// Element access
    [[nodiscard]] value_type& top() const {
        [[likely]] if (_size) {
            return *_top;
        }
        throw std::runtime_error("stack_view is empty, so top() is undefined");
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
            _top = _begin;
        }
        else {
            ++_top;
        }
        *_top = value;
        _size++;
    }

    template<class... Args>
    void emplace(Args&&... args) {
        push(value_type(std::forward<Args>(args)...));
    }

    void pop() {
        [[unlikely]] if (!_size) {
            throw std::runtime_error("stack_view is empty, so pop() is undefined");
        }
        // Move back 1
        _top = std::prev(_top);
        _size--;
    }

    void swap(stack_view& s) {
        std::swap(*this, s);
    }

private:
    iter_type _begin, _end;
    iter_type _top;
    std::size_t _size;
};

} // namespace views
