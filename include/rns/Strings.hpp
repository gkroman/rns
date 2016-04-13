// vim: ts=4:sts=4:et
//
#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <cstring>
#include <cstdint>

struct String_ {
    String_(uint16_t l) : m_length(l) {}

    void printx() const;

    char *data() { return (char*)&this[1]; }
    const char *data() const { return (const char*)&this[1];; }

    uint16_t m_length;
};

template<size_t MAX>
struct String : public String_ {

    template<size_t N>
    String(const char (&s)[N]) : String_(((N - 1) < MAX) ? (N - 1) : MAX) {
        memcpy(m_data, s, ((N - 1) < MAX) ? (N - 1) : MAX);
    }
    
    void print() const { String_::printx(); }

    char m_data[MAX];
};

#endif // STRINGS_HPP
