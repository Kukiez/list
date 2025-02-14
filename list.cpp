#include <iostream>
#include <list>
#include <ranges>
#include <chrono>

#define T_Convertible     template <typename _T> \
requires std::is_convertible_v<_T, T>

template <typename T, size_t Capacity>
class NodeAllocator {
    T* memory_pool;
    T** empty_spots;

    size_t offset = 0;
    size_t capacity;

    size_t empty_offset = 0;
public:
    using value_type = T;

    template <typename U>
    struct rebind {
        using other = NodeAllocator<U, Capacity>;
    };

    NodeAllocator() : capacity(Capacity) {
        memory_pool = static_cast<T*>(::operator new(sizeof(T) * capacity));
        empty_spots = static_cast<T**>(::operator new(sizeof(T*) * capacity));
    }

    NodeAllocator(size_t capacity) : capacity(capacity) {
        memory_pool = static_cast<T*>(::operator new(sizeof(T) * capacity));
        empty_spots = static_cast<T**>(::operator new(sizeof(T*) * capacity));    
    }

    NodeAllocator& operator=(NodeAllocator&& other) noexcept {
        if (this != &other) {
            ::operator delete(memory_pool);
            ::operator delete(empty_spots);

            memory_pool  = other.memory_pool;
            empty_spots  = other.empty_spots;
            offset       = other.offset;
            capacity     = other.capacity;
            empty_offset = other.empty_offset;

            other.memory_pool  = nullptr;
            other.empty_spots  = nullptr;
            other.offset       = 0;
            other.capacity     = 0;
            other.empty_offset = 0;
        }
        return *this;
    }

    ~NodeAllocator() {
        ::operator delete(memory_pool);
        ::operator delete(empty_spots);
    }

    T* allocate(size_t) {
        if (empty_offset > 1) {
            return empty_spots[empty_offset--];
        } else {
            T* ptr = memory_pool + offset;
            offset += 1;
            return ptr;
        }
    }

    void deallocate(T* ptr, size_t) {
        empty_spots[empty_offset++] = ptr;
        ptr->~T();
    }

    size_t get_capacity() {
        return capacity;
    }

    T* get_pointer() {
        return memory_pool;
    }

    void clear() {
        for (int i = 0; i < offset; ++i) {
            (memory_pool + i)->~T();
        }
        ::operator delete(empty_spots);
        ::operator delete(memory_pool);
    }
};

template <typename T, size_t Capacity = 24>
class List {
    struct Node {
        T element;
        Node* next = nullptr;

        template <typename _T>
        requires std::is_convertible_v<_T, T>
        Node(_T&& element) : element(std::forward<_T>(element)) {}

        ~Node() = default;
    };

    NodeAllocator<Node, Capacity> allocator;

    Node* head = nullptr;
    Node* tail = nullptr;

    size_t length = 0;
public:
    class Iterator {
    private:
        Node* prev;
        Node* current;
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::input_iterator_tag;

        explicit Iterator(Node* prev, Node* current) : prev(prev), current(current) {}
        explicit Iterator(Node* head) : current(head), prev(nullptr) {}

        Iterator& operator ++ () {
            current = current->next;
            return *this;
        }

        Iterator operator ++ (int) {
            Iterator temp = *this;
            prev = current;
            current = current->next;

            return temp;
        }

        Iterator operator + (int increment) {
            Node* it = current;

            for (int i = 0; i < increment; ++i) {
                prev = it;
                it = it->next;
            }

            return Iterator(prev, it);
        }

        T& operator * () {
            return current->element;
        }

        const T& operator * () const {
            return current->element;
        }

        bool operator != (const Iterator& end) const {
            return current != end.current;
        }

        bool operator == (const Iterator& other) const {
            return current == other.current;
        }

        Node*& getNode() {
            return current;
        }

        Node*& getPrevNode() {
            return prev;
        }
    };

    List(size_t capacity = Capacity) : allocator(capacity) {}

    template <typename InputIterator>
    List(InputIterator first, InputIterator last) : allocator(Capacity) {
        tail = insert_range(begin(), first, last).getPrevNode();
    }

    template <typename... Args>
    List(Args&&... args) : allocator(Capacity) {
        tail = insert_range(begin(), std::forward<Args>(args)...).getPrevNode();
    }

    List(const List& other) : allocator(other.allocator) {
        tail = insert_range(begin(), other.begin(), other.end());
    }

    List& operator = (const List& other) noexcept {
        if (this == &other) return *this;

        List temp(other);
        swap(*this, temp);

        return *this;
    }

    List(List&& other) noexcept {
        head = other.head;
        tail = other.tail;
        length = other.length;
        allocator = std::move(other.allocator);

        other.head = nullptr;   
        other.tail = nullptr;   
        other.length = 0;
    }

    List& operator = (List&& other) noexcept {
        if (this != &other) {
            clear();

            head = other.head;
            tail = other.tail;
            length = other.length;
            allocator = std::move(other.allocator);
    
            other.head = nullptr;   
            other.tail = nullptr;   
            other.length = 0;
        }

        return *this;
    }

    ~List() {
        clear();
    }

    void resize(size_t new_capacity) {
        NodeAllocator<T, Capacity> new_allocator(new_capacity);

        Node* new_list = new (new_allocator.allocate(1)) Node(std::move(head->element));

        Node* current = head->next;
        allocator.deallocate(head, 1);

        Node* new_head = new_list;

        while (current != nullptr) {
            Node* next = current->next;
            new_list->next = new (new_allocator.allocate(1)) Node(std::move(current->element));

            allocator.deallocate(current, 1);
            current = next;
            new_list = new_list->next;
        }

        tail = new_list;
        head = new_head;

        allocator.clear();

        allocator = std::move(new_allocator);
    }

    T_Convertible Iterator insert_front(_T&& element) {
        if (allocator.get_capacity() <= length) resize(allocator.get_capacity() * 2);

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));
        
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head = node;            
        }

        length++;
        return Iterator(head, head->next);
    }

    T_Convertible Iterator insert_back(_T&& element) {
        if (allocator.get_capacity() <= length) resize(allocator.get_capacity() * 2);

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        length++;
        return end();
    }

    T_Convertible Iterator insert(Iterator at, _T&& element) {
        if (at.getNode() == head) _UNLIKELY {
            return insert_front(std::forward<_T>(element));
        }

        if (at.getNode() == nullptr) _UNLIKELY {
            return insert_back(std::forward<_T>(element));
        }

        Node* node = new (allocator.allocate(1)) Node(std::forward<_T>(element));

        at.getPrevNode()->next = node;
        node->next = at.getNode();

        length++;
        return Iterator(node, node->next);
    }

    template <typename Range>
    requires std::is_convertible_v<
        typename std::iterator_traits<Range>::iterator_category, std::input_iterator_tag
    >
    constexpr Iterator insert_range(Iterator from, Range begin, Range end) {
        Node* temp = new (allocator.allocate(1)) Node(*begin);
        Node* tempPtr = temp;

        ++length;
        for (Range it = std::next(begin); it != end; ++it, ++length) {
            Node* node = new (allocator.allocate(1)) Node(*it);
            temp->next = node;
            temp = temp->next;
        }

        if (from.getNode() == head) {
            Node* rest = head;
            head = tempPtr;
            temp->next = rest;
        } else if (from.getPrevNode() == tail) { // dd
            tail->next = tempPtr;
            tail = temp;
        } else {
            from.getPrevNode()->next = tempPtr;
            temp->next = from.getNode();            
        }

        return Iterator(temp, temp->next);
    }

    template <typename Iteratable>
    constexpr Iterator insert_range(Iterator from, Iteratable&& container) 
    requires requires {
        { std::begin(container) };
        { std::end(container) };
    }
    {
        return insert_range(from,
            std::begin(std::forward<Iteratable>(container)),
            std::end(std::forward<Iteratable>(container))
        );
    }

    template <typename... Ts>
    requires std::conjunction_v<std::is_convertible<T, Ts>...>
    constexpr Iterator insert_range(Iterator from, Ts&&... elements) noexcept(
        std::conjunction_v<std::is_nothrow_convertible<T, Ts>...>)
    {
        return insert_range(from, std::array<T, sizeof...(elements)>{std::forward<Ts>(elements)...});
    }

    Iterator pop_front() noexcept {
        Node* temp = head;

        if (!head->next) {
            head = tail = nullptr;
        } else {
            head = head->next;
        }

        temp->~Node();

        allocator.deallocate(temp, 1);

        length--;

        return begin();
    }

    Iterator pop_back() noexcept  {
        if (!head) return Iterator(nullptr);

        if (head == tail) {
            allocator.deallocate(head, 1);

            head = tail = nullptr;
        } else {
            Node* current = head;

            while (current->next != tail) {
                current = current->next;
            }

            allocator.deallocate(tail, 1);
            
            tail = current;
            tail->next = nullptr;  
        }
        length--;
        return Iterator(tail);
    }

    Iterator erase(Iterator at) noexcept {
        if (at.getNode() == head) _UNLIKELY {
            return pop_front();
        }

        Node* temp = at.getNode();

        at.getPrevNode()->next = at.getNode()->next;
        allocator.deallocate(temp, 1);
        length--;

        return Iterator(at.getPrevNode()->next);
    }

    Iterator erase_range(Iterator from, Iterator to = end()) noexcept  {
        Node* current = from.getNode();

        while (current != to.getNode()) {
            Node* next = current->next;
            allocator.deallocate(current, 1);
            current = next;
            length--;
        }

        if (from.getNode() == head) {
            head = current;
        } else {
            from.getPrevNode()->next = current;
        }

        return Iterator(current);
    }

    void clear() noexcept  {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            allocator.deallocate(temp, 1);
        }
    }

    void swap(const List& other) noexcept  {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(length, other.length);
        std::swap(allocator, other.allocator);
    }

    decltype(auto) front(this auto&& self) {
        return std::forward<decltype(self)>(self)->head->element;
    }

    decltype(auto) back(this auto&& self) {
        return std::forward<decltype(self)>(self)->tail->element;
    }

    constexpr Iterator find(const T& to_find, Iterator from, Iterator to) const noexcept {
        for (auto it = from; it != to; ++it) {
            if (it.getNode()->element == to_find) {
                return Iterator(it.getNode());
            }
        }

        return to;
    }

    template <typename Predicate>
    constexpr Iterator find_if(Iterator from, Iterator to, Predicate&& predicate) const noexcept {
        for (auto it = from; it != to; ++it) {
            if (predicate(*it)) {
                return Iterator(it.getNode());
            }
        }

        return to;
    }

    constexpr List sublist(Iterator from, Iterator to) const noexcept {
        return {from, to};
    }

    size_t size() const {
        return length;
    }

    bool empty() const {
        return length;
    }
    
    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(tail, nullptr);
    }

    const auto& get_allocator() {
        return allocator;
    }
};


int main() {
    auto start = std::chrono::high_resolution_clock::now();

    List<int, 10'000'000> list;
    int i = 0;

    for (i = 0; i < 10'000'000; ++i) {
        list.insert_back(i);
    }

    std::cout << "i: " << i << std::endl;


    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
}