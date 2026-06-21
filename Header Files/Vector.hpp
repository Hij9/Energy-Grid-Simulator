// Custom-built Vector template to store CSV data, created by hand before main project start, which is why there is no implementation file

#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <initializer_list>

template <typename T>
class Vector
{
private:
    std::allocator<T> alloc;
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    void reallocate(size_t newCapacity) // Private helper function for dynamically resizing memory
    {
        T *newMemory = alloc.allocate(newCapacity);

        for (size_t i = 0; i < m_size; ++i)
        {
            new (&newMemory[i]) T(std::move(m_data[i]));
        }

        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T();
        }
        alloc.deallocate(m_data, m_capacity);

        m_data = newMemory;
        m_capacity = newCapacity;
    }

public:
    Vector() // Default constructor
    {
        m_size = 0;
        m_capacity = 2;
        m_data = alloc.allocate(m_capacity);
    }

    Vector(std::initializer_list<T> list) // For when {1,2,3} is used
    {
        m_size = 0;
        m_capacity = list.size();
        m_data = alloc.allocate(m_capacity);

        for (const T &item : list)
        {
            new (&m_data[m_size]) T(item);
            m_size++;
        }
    }

    Vector(const Vector &other) // Ro5: Copy constructor
    {
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        m_data = alloc.allocate(m_capacity);

        for (size_t i = 0; i < m_size; i++)
        {
            new (&m_data[i]) T(other.m_data[i]);
        }
    }

    Vector(Vector &&other) noexcept // Ro5: Move constructor
    {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = other.m_data;

        other.m_size = 0;
        other.m_capacity = 0;
        other.m_data = nullptr;
    }

    ~Vector() // Ro5: Destructor
    {
        for (size_t i = 0; i < m_size; i++)
        {
            std::allocator_traits<decltype(alloc)>::destroy(alloc, &m_data[i]);
        }
        alloc.deallocate(m_data, m_capacity);
    }

    void push_back(const T &value) // Add an element to the end of the array
    {
        if (m_size == m_capacity)
        {
            reallocate(m_capacity * 2);
        }
        new (&m_data[m_size]) T(value);
        m_size++;
    }

    void push_front(const T &value) // Own little challenge, DO NOT USE for actual purposes because it's VERY inefficient for when vectors get huge
    {
        if (m_size == m_capacity)
        {
            reallocate(m_capacity * 2);
        }
        else if (m_size == 0)
        {
            new (&m_data) T(value);
            m_size++;
            return;
        }
        new (&m_data[m_size]) T(std::move(m_data[m_size - 1]));

        for (size_t i = m_size - 1; i > 0; i--)
        {
            m_data[i] = std::move(m_data[i - 1]);
        }

        m_data[0] = value;
        m_size++;
    }

    void pop_back() // Delete the last element in array
    {
        if (m_size == 0)
        {
            std::cout << "Size too small to execute pop_back()\n";
            return;
        }
        m_data[m_size - 1].~T();
        m_size--;
    }

    T &operator[](size_t index) // Operator overload to make array syntax available
    {
        return m_data[index];
    }

    size_t size() const // Getters for size and capacity
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    void printData() const // Print whatever's inside of the array
    {
        for (size_t i = 0; i < m_size; i++)
        {
            if (i == (m_size - 1))
            {
                std::cout << m_data[i] << std::endl;
                return;
            }
            std::cout << m_data[i] << ", ";
        }
    }

    Vector &operator=(const Vector &other) // Ro5: Copy assignment operator overload
    {
        if (this == &other)
        {
            return *this;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T();
        }
        alloc.deallocate(m_data, m_capacity);

        m_capacity = other.m_capacity;
        m_size = other.m_size;

        m_data = alloc.allocate(m_capacity);

        for (size_t i = 0; i < m_size; i++)
        {
            new (&m_data[i]) T(other.m_data[i]);
        }

        return *this;
    }

    Vector &operator=(Vector &&other) noexcept // Ro5: Move assignment operator overload
    {
        if (this == &other)
        {
            return *this;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            m_data[i].~T();
        }

        alloc.deallocate(m_data, m_capacity);

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = other.m_data;

        other.m_capacity = 0;
        other.m_size = 0;
        other.m_data = nullptr;

        return *this;
    }

    T *begin() // Iterators
    {
        return m_data;
    }

    T *end()
    {
        return m_data + m_size;
    }
};