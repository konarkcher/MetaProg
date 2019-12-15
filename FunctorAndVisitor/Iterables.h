#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "TypeList.h"
#include "Functor.h"
#include "VisitFactory.h"

namespace iter {

template<typename T>
class Mapping;

template<typename T>
class Iterable {
public:
    virtual void Accept(Mapping<T>*) = 0;
};

// Iterables

template<typename T>
class Array : public Iterable<T> {
public:
    explicit Array(const std::vector<T>& vec) : buffer(vec) {}

    explicit Array(std::vector<T>&& vec) : buffer(std::move(vec)) {}

    void Accept(Mapping<T>* map) override {
        map->Visit(this);
    }

    void Print() {
        for (size_t i = 0; i < buffer.size(); ++i) {
            std::cout << buffer[i];
            if (i < buffer.size() - 1) {
                std::cout << ", ";
            }
        }
    }

    std::vector<T> buffer;
};

// Set

template<typename T>
class Set : public Iterable<T> {
public:
    explicit Set(std::vector<T> vec) : buffer(vec.begin(), vec.end()) {
    }

    void Accept(Mapping<T>* map) override {
        map->Visit(this);
    }

    void Print() {
        bool first = true;
        for (auto elem: buffer) {
            if (first) {
                first = false;
            } else {
                std::cout << ", ";
            }
            std::cout << elem;
        }
    }

    std::set<T> buffer;
};

// Mappings

template<typename T>
class Mapping {
public:
    virtual T GetResult() = 0;

    virtual void Visit(Array<T>*) = 0;

    virtual void Visit(Set<T>*) = 0;
};

template<typename T>
class Sum : public Mapping<T> {
public:
    void Visit(Array<T>* array) override {
        result_ = 0;
        for (auto elem: array->buffer) {
            result_ += elem;
        }
    }

    void Visit(Set<T>* array) override {
        result_ = 0;
        for (auto elem: array->buffer) {
            result_ += elem;
        }
    }

    T GetResult() override {
        return result_;
    }

private:
    T result_{0};
};

template<typename T>
class Prod : public Mapping<T> {
public:
    void Visit(Array<T>* array) override {
        result_ = 1;
        for (auto elem: array->buffer) {
            result_ *= elem;
        }
    }

    void Visit(Set<T>* array) override {
        result_ = 1;
        for (auto elem: array->buffer) {
            result_ *= elem;
        }
    }

    T GetResult() override {
        return result_;
    }

private:
    T result_{1};
};

// Functor

template<class ReturnType, class Visitor, class Acceptor, class... Args>
class PrintFunctor : public DoubleDispatchFunctor<ReturnType, Visitor, Acceptor, Args...> {
    std::string getExpectedArgs() override {
        return "<int>";
    }
};

template<>
class PrintFunctor<void, Prod<int>, Array<int>, size_t> {
public:
    void operator()(Prod<int>* prod, Array<int>* array, size_t separatorSize) {
        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';

        std::cout << '[';
        array->Print();
        std::cout << "]\n";

        std::cout << "Product = " << prod->GetResult() << '\n';

        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
};

template<>
class PrintFunctor<void, Sum<int>, Array<int>, size_t> {
public:
    void operator()(Sum<int>* sum, Array<int>* array, size_t separatorSize) {
        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';

        std::cout << '[';
        array->Print();
        std::cout << "]\n";

        std::cout << "Sum = " << sum->GetResult() << '\n';

        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
};

template<>
class PrintFunctor<void, Prod<int>, Set<int>, size_t> {
public:
    void operator()(Prod<int>* prod, Set<int>* array, size_t separatorSize) {
        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';

        std::cout << '{';
        array->Print();
        std::cout << "}\n";

        std::cout << "Product = " << prod->GetResult() << '\n';

        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
};

template<>
class PrintFunctor<void, Sum<int>, Set<int>, size_t> {
public:
    void operator()(Sum<int>* sum, Set<int>* array, size_t separatorSize) {
        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';

        std::cout << '{';
        array->Print();
        std::cout << "}\n";

        std::cout << "Sum = " << sum->GetResult() << '\n';

        for (size_t i = 0; i < separatorSize; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
};

}
