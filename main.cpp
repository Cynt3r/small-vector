#define BOOST_TEST_MODULE epc::small_vector test
#include <typeinfo>
#include <cstddef>
#include <cstdint>
#include <boost/test/included/unit_test.hpp>
#include "small_vector.h"
using small_int_vector = epc::small_vector<int, 2>;

BOOST_AUTO_TEST_SUITE(small_vector_test)
    //== EPC TESTS =================================
    BOOST_AUTO_TEST_CASE(def_ctor_size_cap)
    {
        epc::small_vector<int, 16> v;
        epc::small_vector<int, 8> x;
        BOOST_CHECK_EQUAL(v.size() , 0);
        BOOST_CHECK_EQUAL(v.capacity(), 16);
    }
    BOOST_AUTO_TEST_CASE(sbo_buffer_align)
    {
        const size_t max_align = alignof(std::max_align_t);
        struct X
        {
            alignas(max_align) char _[max_align];
        };
        epc::small_vector<X, 4> v;

        uintptr_t addr = reinterpret_cast<uintptr_t>(v.data());
        BOOST_CHECK_EQUAL(addr % max_align, 0);
    }

    //== MY OWN TESTS ==============================
    BOOST_AUTO_TEST_CASE(def_member_types)
    {
        BOOST_CHECK_EQUAL(typeid(small_int_vector::value_type).name(), typeid(int).name());
        BOOST_CHECK_EQUAL(typeid(small_int_vector::reference).name(), typeid(int&).name());
        BOOST_CHECK_EQUAL(typeid(small_int_vector::const_reference).name(), typeid(const int&).name());
        BOOST_CHECK_EQUAL(typeid(small_int_vector::pointer).name(), typeid(int*).name());
        BOOST_CHECK_EQUAL(typeid(small_int_vector::const_pointer).name(), typeid(const int*).name());
    }
    BOOST_AUTO_TEST_CASE(push_back)
    {
        small_int_vector v;
        v.push_back(5);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v.size(), 1);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        v.push_back(10);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        v.push_back(15);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v.size(), 3);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        v.push_back(20);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v.size(), 4);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        v.push_back(25);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v[4], 25);
        BOOST_CHECK_EQUAL(v.size(), 5);
        BOOST_CHECK_EQUAL(v.capacity(), 8);
    }
    BOOST_AUTO_TEST_CASE(emplace_back)
    {
        small_int_vector v;
        v.emplace_back(5);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v.size(), 1);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        v.emplace_back(10);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        v.emplace_back(15);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v.size(), 3);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        v.emplace_back(20);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v.size(), 4);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        v.emplace_back(25);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v[4], 25);
        BOOST_CHECK_EQUAL(v.size(), 5);
        BOOST_CHECK_EQUAL(v.capacity(), 8);
    }
    BOOST_AUTO_TEST_CASE(resize_down_buffer)
    {
        small_int_vector v;
        v.push_back(5);
        v.push_back(10);
        v.resize(1);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v.size(), 1);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
    }
    BOOST_AUTO_TEST_CASE(resize_down_nonbuffer)
    {
        small_int_vector v;
        v.push_back(5);
        v.push_back(10);
        v.push_back(15);
        v.push_back(20);
        v.push_back(25);
        v.push_back(30);
        v.resize(2);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 8);
    }
    BOOST_AUTO_TEST_CASE(resize_up_buffer)
    {
        small_int_vector v;
        v.resize(2, 10);
        BOOST_CHECK_EQUAL(v[0], 10);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        v.resize(4, 20);
        BOOST_CHECK_EQUAL(v[0], 10);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 20);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v.size(), 4);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
    }
    BOOST_AUTO_TEST_CASE(resize_up_nonbuffer)
    {
        small_int_vector v;
        v.push_back(5);
        v.push_back(10);
        v.push_back(15);
        v.push_back(20);
        v.push_back(25);
        v.resize(7, 30);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v[4], 25);
        BOOST_CHECK_EQUAL(v[5], 30);
        BOOST_CHECK_EQUAL(v[6], 30);
        BOOST_CHECK_EQUAL(v.size(), 7);
        BOOST_CHECK_EQUAL(v.capacity(), 8);
        v.resize(9, 35);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v[3], 20);
        BOOST_CHECK_EQUAL(v[4], 25);
        BOOST_CHECK_EQUAL(v[5], 30);
        BOOST_CHECK_EQUAL(v[6], 30);
        BOOST_CHECK_EQUAL(v[7], 35);
        BOOST_CHECK_EQUAL(v[8], 35);
        BOOST_CHECK_EQUAL(v.size(), 9);
        BOOST_CHECK_EQUAL(v.capacity(), 16);
    }
    BOOST_AUTO_TEST_CASE(cc_buffer)
    {
        small_int_vector v;
        v.push_back(5);
        v.push_back(10);
        small_int_vector w(v);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        BOOST_CHECK_EQUAL(w[0], 5);
        BOOST_CHECK_EQUAL(w[1], 10);
        BOOST_CHECK_EQUAL(w.size(), 2);
        BOOST_CHECK_EQUAL(w.capacity(), 2);
    }
    BOOST_AUTO_TEST_CASE(cc_nonbuffer)
    {
        small_int_vector v;
        v.push_back(5);
        v.push_back(10);
        v.push_back(15);
        small_int_vector w(v);
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v.size(), 3);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        BOOST_CHECK_EQUAL(w[0], 5);
        BOOST_CHECK_EQUAL(w[1], 10);
        BOOST_CHECK_EQUAL(w[2], 15);
        BOOST_CHECK_EQUAL(w.size(), 3);
        BOOST_CHECK_EQUAL(w.capacity(), 3);
    }
    BOOST_AUTO_TEST_CASE(ao_buffer_to_buffer)
    {
        small_int_vector v, w;
        v = w;
        BOOST_CHECK_EQUAL(v.size(), 0);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        w.push_back(5);
        v = w;
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v.size(), 1);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        BOOST_CHECK_EQUAL(w[0], 5);
        BOOST_CHECK_EQUAL(w.size(), 1);
        BOOST_CHECK_EQUAL(w.capacity(), 2);
        w.push_back(10);
        v = w;
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
        BOOST_CHECK_EQUAL(w[0], 5);
        BOOST_CHECK_EQUAL(w[1], 10);
        BOOST_CHECK_EQUAL(w.size(), 2);
        BOOST_CHECK_EQUAL(w.capacity(), 2);
    }
    BOOST_AUTO_TEST_CASE(ao_buffer_to_nonbuffer)
    {
        small_int_vector v, w;
        v.push_back(5);
        v.push_back(10);
        v.push_back(15);
        w.push_back(20);
        v = w;
        BOOST_CHECK_EQUAL(v[0], 20);
        BOOST_CHECK_EQUAL(w[0], 20);
        BOOST_CHECK_EQUAL(v.size(), 1);
        BOOST_CHECK_EQUAL(v.capacity(), 4);
        BOOST_CHECK_EQUAL(w.size(), 1);
        BOOST_CHECK_EQUAL(w.capacity(), 2);
    }
    BOOST_AUTO_TEST_CASE(ao_nonbuffer_to_buffer)
    {
        small_int_vector v, w;
        v.push_back(20);
        w.push_back(5);
        w.push_back(10);
        w.push_back(15);
        v = w;
        BOOST_CHECK_EQUAL(v[0], 5);
        BOOST_CHECK_EQUAL(v[1], 10);
        BOOST_CHECK_EQUAL(v[2], 15);
        BOOST_CHECK_EQUAL(v.size(), 3);
        BOOST_CHECK_EQUAL(v.capacity(), 3);
        BOOST_CHECK_EQUAL(w[0], 5);
        BOOST_CHECK_EQUAL(w[1], 10);
        BOOST_CHECK_EQUAL(w[2], 15);
        BOOST_CHECK_EQUAL(w.size(), 3);
        BOOST_CHECK_EQUAL(w.capacity(), 4);
    }
    BOOST_AUTO_TEST_CASE(ao_nonbuffer_to_nonbuffer)
    {
        small_int_vector v, v1, w, w1;
        for (int i = 1; i <= 3; i++) {
            v.push_back(i);
            v1.push_back(i);
        }
        for (int i = 10; i <= 15; i++) {
            w.push_back(i);
            w1.push_back(i);
        }

        v = w;
        for (int i = 0; i < 5; i++) {
            BOOST_CHECK_EQUAL(v[i], i+10);
            BOOST_CHECK_EQUAL(w[i], i+10);
        }
        BOOST_CHECK_EQUAL(v.size(), 6);
        BOOST_CHECK_EQUAL(v.capacity(), 6);
        BOOST_CHECK_EQUAL(w.size(), 6);
        BOOST_CHECK_EQUAL(w.capacity(), 8);

        w1 = v1;
        for (int i = 0; i < 3; i++) {
            BOOST_CHECK_EQUAL(v1[i], i+1);
            BOOST_CHECK_EQUAL(w1[i], i+1);
        }
        BOOST_CHECK_EQUAL(v1.size(), 3);
        BOOST_CHECK_EQUAL(v1.capacity(), 4);
        BOOST_CHECK_EQUAL(w1.size(), 3);
        BOOST_CHECK_EQUAL(w1.capacity(), 8);
    }
    BOOST_AUTO_TEST_CASE(init_list_cons_buffer)
    {
        small_int_vector v({0,1});
        for (size_t i = 0; i < 2; i++) BOOST_CHECK_EQUAL(v[i], i);
        BOOST_CHECK_EQUAL(v.size(), 2);
        BOOST_CHECK_EQUAL(v.capacity(), 2);
    }
    BOOST_AUTO_TEST_CASE(init_list_cons_nonbuffer)
    {
        small_int_vector v({0,1,2,3,4,5,6,7,8});
        for (size_t i = 0; i < 9; i++) BOOST_CHECK_EQUAL(v[i], i);
        BOOST_CHECK_EQUAL(v.size(), 9);
        BOOST_CHECK_EQUAL(v.capacity(), 16);
    }
    BOOST_AUTO_TEST_CASE(iterator_basic_for)
    {
        small_int_vector v({0,1});
        size_t i = 0;
        for (small_int_vector::iterator it = v.begin(); it != v.end(); it++) BOOST_CHECK_EQUAL(*it, i++);
        for (; i < 9; i++) v.push_back(i);
        i = 0;
        for (small_int_vector::iterator it = v.begin(); it != v.end(); it++) BOOST_CHECK_EQUAL(*it, i++);
    }
    BOOST_AUTO_TEST_CASE(iterator_range_based_for)
    {
        small_int_vector v({0,1});
        size_t i = 0;
        for (const auto item : v) BOOST_CHECK_EQUAL(item, i++);
        for (; i < 9; i++) v.push_back(i);
        i = 0;
        for  (const auto item : v) BOOST_CHECK_EQUAL(item, i++);
    }
BOOST_AUTO_TEST_SUITE_END()
