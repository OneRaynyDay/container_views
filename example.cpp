#include "stack_view.h"
#include "queue_view.h"
#include <iostream>
#include <vector>
#include <array>
#include <deque>

using std::vector;
using std::array;
using std::deque;
using views::stack_view;
using views::queue_view;

int main() {
    // Each one below should print:
    //
    // Size of stack_view: 2 and capacity: 50
    // Top element of stack: 2
    // Size of queue_view: 2 and capacity: 50
    // Front element of the queue: 1 and back element of the queue : 2
    {
        // Using vector as the underlying buffer
        vector<int> v;
        v.resize(50);

        stack_view<vector<int>> sv(v.begin(), v.end());
        sv.push(1);
        sv.push(2);
        // Prints:
        std::cout << "Size of stack_view: " << sv.size() << " and capacity: " << sv.capacity() << std::endl;
        // Prints:
        std::cout << "Top element of stack: " << sv.top() << std::endl;
        sv.pop();

        queue_view<vector<int>> qv(v.begin(), v.begin() + 50);
        qv.push(1);
        qv.push(2);
        // Prints:
        std::cout << "Size of queue_view: " << qv.size() << " and capacity: " << qv.capacity() << std::endl;
        // Prints:
        std::cout << "Front element of the queue: " << qv.front() << " and back element of the queue : " << qv.back() << std::endl;
        sv.pop();
    }

    // Using array as the underlying buffer
    {
        array<int, 50> arr;

        stack_view<array<int, 50>> sv(arr.begin(), arr.end());
        sv.push(1);
        sv.push(2);
        // Prints:
        std::cout << "Size of stack_view: " << sv.size() << " and capacity: " << sv.capacity() << std::endl;
        // Prints:
        std::cout << "Top element of stack: " << sv.top() << std::endl;
        sv.pop();

        queue_view<array<int, 50>> qv(arr.begin(), arr.begin() + 50);
        qv.push(1);
        qv.push(2);
        // Prints:
        std::cout << "Size of queue_view: " << qv.size() << " and capacity: " << qv.capacity() << std::endl;
        // Prints:
        std::cout << "Front element of the queue: " << qv.front() << " and back element of the queue : " << qv.back() << std::endl;
        sv.pop();
    }

    // Using deque as the underlying buffer
    {
        deque<int> arr(50);

        stack_view<deque<int>> sv(arr.begin(), arr.end());
        sv.push(1);
        sv.push(2);
        // Prints:
        std::cout << "Size of stack_view: " << sv.size() << " and capacity: " << sv.capacity() << std::endl;
        // Prints:
        std::cout << "Top element of stack: " << sv.top() << std::endl;
        sv.pop();

        queue_view<deque<int>> qv(arr.begin(), arr.begin() + 50);
        qv.push(1);
        qv.push(2);
        // Prints:
        std::cout << "Size of queue_view: " << qv.size() << " and capacity: " << qv.capacity() << std::endl;
        // Prints:
        std::cout << "Front element of the queue: " << qv.front() << " and back element of the queue : " << qv.back() << std::endl;
        sv.pop();
    }
}
