#include <string.h>

#include <iostream>
#include <string>

#include "cstack.h"

template<typename T>
value_t CreateValue(const T& pValue);

template<typename T>
T ExtractValue(const value_t& value);

template<typename T>
class Stack {
public:
    // constructor
    Stack(int pMaxSize) {
        mStack = cstack_new();
        cstack_ctor(mStack, pMaxSize);
    }

    // destructor
    ~Stack() {
        cstack_dtor(mStack, free_value);
        cstack_delete(mStack);
    }

    size_t Size() {
        return cstack_size(mStack);
    }

    void Push(const T& pItem) {
        value_t value = CreateValue(pItem);

        if (!cstack_push(mStack, value))
            throw "Stack is full!";
    }

    const T Pop() {
        value_t value;

        if (!cstack_pop(mStack, &value))
            throw "Stack is empty!";

        return ExtractValue<T>(value);
    }

    void Clear() {
        cstack_clear(mStack, free_value);
    }

private:
    cstack_t* mStack;
};

template<>
value_t CreateValue(const std::string& pValue) {
    value_t value;
    value.len = pValue.size() + 1;
    value.data = new char[value.len];

    strcpy(value.data, pValue.c_str());

    return value;
}

template<>
std::string ExtractValue(const value_t& value) {
    return std::string(value.data, value.len);
}

int main(int argc, char** argv) {
    Stack<std::string> stringStack(100);
    stringStack.Push("Hello");
    stringStack.Push("World");
    stringStack.Push("!");

    std::cout << "Stack size: " << stringStack.Size() << std::endl;

    while (stringStack.Size() > 0) {
        std::cout << "Popped > " << stringStack.Pop() << std::endl;
    }

    std::cout << "Stack size after pop: " << stringStack.Size() << std::endl;

    stringStack.Push("Bye");
    stringStack.Push("Bye");

    std::cout << "Stack size before clear: " << stringStack.Size() << std::endl;

    stringStack.Clear();

    std::cout << "Stack size after clear: " << stringStack.Size() << std::endl;

    return 0;
}