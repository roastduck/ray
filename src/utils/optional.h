#ifndef OPTIONAL_H_
#define OPTIONAL_H_

class None {};

template <class T>
class Optional
{
private:
    bool valid;
    T content;

public:
    Optional() : valid(false) {}
    Optional(None) : valid(false) {}
    Optional(const T &_content) : valid(true), content(_content) {}
    Optional(T &&_content) : valid(true), content(_content) {}

    bool isOk() const { return valid; }
    T &ok() { assert(valid); return content; }
    const T &ok() const { assert(valid); return content; }
};

#endif // OPTIONAL_H_

