#pragma once

#include <exception>
#include <iostream>

namespace stppp {

template<typename T>
class dynamicArrayIterator
{
  public:
    using iterator_t = dynamicArrayIterator<T>;
    using value_t = T;
    using ptr_t = T*;

  public:
    dynamicArrayIterator() = delete;

    explicit dynamicArrayIterator(ptr_t ptr) noexcept
      : m_current(ptr)
    {
    }

    virtual ~dynamicArrayIterator()
    {
        //포인터 내가 할당 한게 아니라 지우면 안된다.
    }

  public:
    value_t& operator*()
    {
        return *m_current;
    }

    bool operator==(iterator_t rhs)
    {
        return (m_current == rhs.m_current ? true : false);
    }

    bool operator!=(iterator_t rhs)
    {
        return !(*this == rhs);
    }

    //pre
    iterator_t operator++()
    {
        return iterator_t(++m_current);
    }

    iterator_t operator++(int)
    {
        iterator_t temp = m_current;
        ++m_current;
        return temp;
    }

    std::size_t operator-(iterator_t rhs)
    {
        return (m_current - rhs.m_current);
    }

  private:
    ptr_t m_current;
};  //dynamicArrayIterator

template<typename T>
class dynamicArray
{
  public:
    using value_t = T;
    using ptr_t = T*;
    using iterator = dynamicArrayIterator<value_t>;

  public:
    dynamicArray() noexcept
      : DATABYTE(sizeof(value_t))
      , m_begin(nullptr)
      , m_end(nullptr)
      , m_capacity(nullptr)
    {
        alloc(INITSIZE);
    }

    virtual ~dynamicArray() noexcept
    {
        if (nullptr != m_begin)
        {
            free(m_begin);
            m_begin = nullptr;
        }
    }

  public:
    iterator begin()
    {
        return iterator(m_begin);
    }

    iterator end()
    {
        return iterator(m_end);
    }

    value_t& operator[](std::size_t index)
    {
        return *(m_begin + index);
    }

    void push_back(const value_t& value)
    {
        if (capacity() == size())
            alloc(capacity() + INITSIZE);

        *m_end++ = value;
    }

    void pop_back()
    {
        if (empty())
            return;

        *m_end = value_t();
    }

    bool empty() const
    {
        return size() == 0 ? true : false;
    }

    std::size_t size() const
    {
        return (m_end - m_begin);
    }

    std::size_t capacity() const
    {
        return (m_capacity - m_begin);
    }

  private:
    void alloc(const std::size_t count) noexcept
    {
        ptr_t newBegin = nullptr;
        try
        {
            newBegin = static_cast<ptr_t>(malloc(count * DATABYTE));
            if (newBegin == nullptr)
                throw std::bad_array_new_length();

            if (false == empty())
            {
                memcpy(newBegin, m_begin, size() * DATABYTE);
                free(m_begin);
            }
        }
        catch (const std::bad_array_new_length& e)
        {
            std::cout << e.what();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what();
        }

        //기존 size만큼 더해준다
        m_end = newBegin + size();
        m_capacity = newBegin + count;
        m_begin = newBegin;
    }

  private:
    ptr_t m_begin;
    ptr_t m_end;
    ptr_t m_capacity;

    const std::size_t DATABYTE;
    static constexpr std::size_t INITSIZE = 10;
};  //dynamicArray
}  //namespace stppp
