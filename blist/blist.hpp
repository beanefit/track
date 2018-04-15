#ifndef __BLIST_HPP__
#define __BLIST_HPP__

#include <cstddef>
#include "../log/b_log.hpp"

extern b_log* _log;

template <typename data_T>
class blist
{
public:
    blist ();
    ~blist();

    class blist_elem
    {
    public:
        void         fill (data_T val, blist_elem* nxt, blist_elem* prv);

        data_T       get_elem ()
        {
            return elem;
        }

        blist_elem*  get_next()
        {
            return next;
        }

        blist_elem*  get_prev()
        {
            return prev;
        }

    private:
        data_T      elem;
        blist_elem* next;
        blist_elem* prev;
    };

    bool        insert (blist_elem* pos, data_T value);
    bool        insert (size_t index, data_T value);
    bool        push_back (data_T value);
    bool        push_front (data_T value);

    data_T      back ();
    data_T      front();
    data_T      get_elem (blist_elem* pos);
    data_T      get_elem (size_t index);

    blist_elem* get_head();
    blist_elem* get_tail();

    size_t      size();

    void        erase();

private:
    size_t      _size;
    blist_elem* _head;
    blist_elem* _tail;
};

template <typename data_T>
blist <data_T> :: blist ():
        _size (0),
        _head (nullptr),
        _tail (nullptr)
{
    printlog ("list constr\n");
}

template <typename data_T>
blist <data_T> :: ~blist ()
{
    //printlog ("list destruct\n");

    erase();
}

template <typename data_T>
void blist <data_T> :: blist_elem :: fill (data_T val, blist_elem* nxt, blist_elem* prv)
{
    elem = val;
    next = nxt;
    prev = prv;
}

template <typename data_T>
bool blist <data_T> :: insert (blist_elem* pos, data_T value)
{
    if (_size == 0)
    {
        auto tmp = new (std::nothrow) blist_elem;

        if (tmp == nullptr)
        {
            printlog ("ERROR:\t\tcan't create an element\n\n\n");

            return 1;
        }

        tmp -> fill (value, nullptr, nullptr);
        _head = tmp;
        _tail = tmp;

        _size++;

        return 0;
    }
    if (pos == nullptr)
    {
        printlog ("ERROR:\t\tpos == nullptr\n\n\n");

        return 1;
    }

    if (_size == 1)
    {
        auto tmp = new (std::nothrow) blist_elem;

        if (tmp == nullptr)
        {
            printlog ("ERROR:\t\tcan't create an element\n\n\n");

            return 1;
        }

        tmp -> fill (value, _head, nullptr);

        _head = tmp;
        _tail -> fill (_tail -> get_elem(), nullptr, tmp);

        _size++;

        return 0;
    }

    if (pos == _head)
    {
        auto tmp = new (std::nothrow) blist_elem;

        if (tmp == nullptr)
        {
            printlog ("ERROR:\t\tcan't create an element\n\n\n");

            return 1;
        }

        tmp -> fill (value, _head, nullptr);

        _head -> fill (_head -> get_elem(), _head -> get_next(), tmp);

        _head = tmp;

        _size++;

        return 0;

    }

    auto tmp = new (std::nothrow) blist_elem;

    if (tmp == nullptr)
    {
        printlog ("ERROR:\t\tcan't create an element\n\n\n");

        return 1;
    }

    tmp -> fill (value, pos, pos -> get_prev());

    pos -> get_prev() -> fill (pos -> get_prev() -> get_elem(), tmp, pos -> get_prev() -> get_prev());

    pos -> fill (pos -> get_elem(), pos -> get_next(), tmp);

    _size++;

    return 0;
}

template <typename data_T>
bool blist <data_T> :: insert (size_t index, data_T value)
{
    if (_size == 0)
    {
        insert (nullptr, value);
    }

    if (index >= _size)
    {
        printlog ("ERROR:\t\tindex >= _size\n\n\n");

        return 1;
    }

    auto tmp = _head;

    for (int i = 0; i < index; i++)
        tmp = tmp -> get_next();

    if (tmp == nullptr)
    {
        printlog ("ERROR:\t\ttmp == nullptr\n\n\n");

        return 1;
    }

    insert (tmp, value);
}

template <typename data_T>
bool blist <data_T> :: push_back(data_T value)
{
    if (_tail == nullptr)
    {
        printlog ("WARNING:\t\tpushing 1st elem\n");

        auto tmp = new (std::nothrow) blist_elem;

        if (tmp == nullptr)
        {
            printlog ("ERROR:\t\tcan't create an element\n\n\n");

            return 1;
        }

        tmp -> fill (value, nullptr, nullptr);
        _tail = tmp;
        _head = tmp;
        _size++;

        return 0;
    }

    auto tmp = new (std::nothrow) blist_elem;

    if (tmp == nullptr)
    {
        printlog ("ERROR:\t\tcan't create an element\n\n\n");

        return 1;
    }

    tmp -> fill (value, nullptr, _tail);
    _tail -> fill (_tail -> get_elem(), tmp, _tail -> get_prev());
    _tail = tmp;
    _size++;

    return 0;
}

template <typename data_T>
bool blist <data_T> :: push_front (data_T value)
{
    if (_head == nullptr)
    {
        printlog ("WARNING:\t\tpushing 1st element\n\n");

        auto tmp = new (std::nothrow) blist_elem;

        if (tmp == nullptr)
        {
            printlog ("ERROR:\t\tcan't create an element\n\n\n");

            return 1;
        }

        tmp -> fill (value, nullptr, nullptr);
        _head = tmp;
        _tail = tmp;

        return 0;
    }

    auto tmp = new (std::nothrow) blist_elem;

    if (tmp == nullptr)
    {
        printlog ("ERROR:\t\tcan't create an element\n\n\n");

        return 1;
    }

    tmp -> fill (value, _head, nullptr);
    _head -> fill (_head -> get_elem(), _head -> get_next(), tmp);
    _head = tmp;
    _size++;

    return 0;
}

template <typename data_T>
data_T blist <data_T> :: back()
{
    if (_size > 0)
    {
        auto tmp_elem = _tail -> get_elem();
        auto tmp_tail = _tail -> get_prev();

        delete _tail;
        _size--;
        _tail = tmp_tail;

        return tmp_elem;
    }

    printlog ("ERROR:\t\tcan't back() for _size = 0, returned poison value\n\n\n");
}

template <typename data_T>
data_T blist <data_T> :: front()
{
    if (_size > 0)
    {
        auto tmp_elem = _head -> get_elem();
        auto tmp_head = _head -> get_next();

        delete _head;
        _size--;
        _head = tmp_head;

        return tmp_elem;
    }

    printlog ("ERROR:\t\tcan't front() for _size = 0, returned poison value\n\n\n");
}

template <typename data_T>
data_T blist <data_T> :: get_elem (blist_elem* pos)
{
    return pos -> get_elem();
}

template <typename data_T>
data_T blist <data_T> :: get_elem (size_t index)
{
    if (index < _size)
    {
        auto tmp = _head;

        for (int i = 0; i < index; i++)
            tmp = tmp -> get_next();

        return tmp -> get_elem();
    }

    printlog ("ERROR:\t\tcan't get_elem for pos >= _size\n\n\n");
}

template <typename data_T>
typename blist<data_T>::blist_elem* blist <data_T> :: get_head()
{
    return _head;
}

template <typename data_T>
typename blist<data_T>::blist_elem* blist <data_T> :: get_tail()
{
    return _tail;
}

template <typename data_T>
size_t blist <data_T> :: size()
{
    return _size;
}

template <typename data_T>
void blist <data_T> :: erase()
{
    auto tmp = _head;
    for (int i = 0; i < _size; i++)
    {
        auto a = tmp -> get_next();

        delete tmp;

        tmp = a;
    }
}

#endif //__BLIST_HPP__
