#pragma once

#include <string>

#include "TypeList.h"
#include "Utils.h"

template<class ResultType, class ArgsTL>
class IFunctor;

template<class RT, class... A>
class IFunctor<RT, TypeList<A...>> {
public:
    using ResultType = RT;
    using Args = TypeList<A...>;

    virtual RT operator()(A...) = 0;
};

// Fully functional functor

template<class Func, class FunctorToHold>
class FuncHolder {
};

template<class Func, class ResultType, class... Args>
class FuncHolder<Func, IFunctor<ResultType, TypeList<Args...>>> : public IFunctor<ResultType, TypeList<Args...>> {
public:
    explicit FuncHolder(Func* func) : funcPtr(func) {}

    ResultType operator()(Args... args) override {
        return (*funcPtr)(args...);
    }

private:
    Func* funcPtr;
};

template<class ResultType, class... Args>
class Functor : public IFunctor<ResultType, TypeList<Args...>> {
public:
    using FunctorToHold = IFunctor<ResultType, TypeList<Args...>>;

    template<class Func>
    explicit Functor(Func* func) : funcPtr(new FuncHolder<Func, FunctorToHold>(func)) {}

    ResultType operator()(Args... args) override {
        return (*funcPtr)(args...);
    }

private:
    IFunctor<ResultType, TypeList<Args...>>* funcPtr;
};

// Double dispatch functor

template<class ReturnType, class Left, class Right, class... Args>
class DoubleDispatchFunctor : IFunctor<ReturnType, TypeList<Left*, Right*, Args...>> {
public:
    ReturnType operator()(Left*, Right*, Args... args) override {
        std::cout << "Expected DoubleDispatchFunctor() call with " << getExpectedArgs() << " argument, got (";
        PrintArgs(args...);
        std::cout << ") instead\n";
    }

protected:
    virtual std::string getExpectedArgs() {
        return "";
    }
};
