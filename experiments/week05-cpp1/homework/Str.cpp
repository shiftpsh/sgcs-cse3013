#include "Str.h"
#include <cstring>
#include <cstdlib>

Str::Str(int length) {
    len = length;
    str = new char[len + 1];
    str[0] = '\0';
}

Str::Str(char* contents) {
    len = strlen(contents);

    str = new char[len + 1];
    strcpy(str, contents);
    str[len] = '\0';
}

Str::~Str() {
    delete []str;
}

int Str::length() {
    return len;
}

char *Str::contents() {
    return str;
}

int Str::compare(char *a) {
    return strcmp(str, a);
}

int Str::compare(Str &a) {
    return (*this).compare(a);
}

void Str::operator=(char *a) {
    if (a == 0) { // nullptr = 0
        len = 0;
        delete []str;
		str = new char[1];
        str[0] = '\0';
        return;
    }

    int a_len = strlen(a);

    if (len < a_len) {
		delete []str;
		str = new char[a_len + 1];
	}

    len = a_len;

    strcpy(str, a);
    str[len] = '\0';
}

void Str::operator=(Str &a) {
    (*this) = a.contents();
}