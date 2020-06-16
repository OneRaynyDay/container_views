# `container_views`
C++11 lightweight mutating views that act like STL containers. All of the STL container API is accessible through these mutating view objects.

So far, we have:

- `stack_view`
- `queue_view`

... and more to come.

## Works with many underlying container types

**`std::vector` as underlying container:**
```
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
```

**`std::array` as underlying container:**
```
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
```

**`std::deque` as underlying container:**
```
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
```
