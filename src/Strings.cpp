// vim: ts=4:sts=4:et
//
#include <stdio.h>
#include <rns/Strings.hpp>

void String_::printx() const {
    printf("{%.*s}\n", m_length, data());
}

