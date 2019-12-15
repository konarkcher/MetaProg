#pragma once

#include "Utils.h"

template<template<class, class, class, class...> class Functor, class BaseVisitor, class Visitors, class BaseAcceptor, class Acceptors,
        class ResultType, class... Args>
class VisitFactory;

template<template<class, class, class, class...> class Functor, class BaseVisitor, class Visitors,
        class BaseAcceptor, class Acceptors, class ResultType, class... Args>
class VisitFactory {
public:
    using VisitorsHead = typename Visitors::Head;
    using VisitorsTail = typename Visitors::Tail;

    static ResultType Go(BaseVisitor* baseVisitor, BaseAcceptor* baseAcceptor, Args... args) {
        if (auto derivedVisitor = dynamic_cast<VisitorsHead*>(baseVisitor)) {
            return VisitFactory<Functor, VisitorsHead, Visitors, BaseAcceptor, Acceptors, ResultType, Args...>
            ::DispatchAcceptor(derivedVisitor, baseAcceptor, args...);
        } else {
            return VisitFactory<Functor, BaseVisitor, VisitorsTail, BaseAcceptor, Acceptors, ResultType, Args...>
            ::Go(baseVisitor, baseAcceptor, args...);
        }
    }

    using AcceptorsHead = typename Acceptors::Head;
    using AcceptorsTail = typename Acceptors::Tail;

    static ResultType DispatchAcceptor(VisitorsHead* derivedVisitor, BaseAcceptor* baseAcceptor, Args... args) {
        if (auto derivedAcceptor = dynamic_cast<AcceptorsHead*>(baseAcceptor)) {
            derivedAcceptor->Accept(derivedVisitor);
            Functor<ResultType, VisitorsHead, AcceptorsHead, Args...> functor;
            return functor(derivedVisitor, derivedAcceptor, args...);
        } else {
            return VisitFactory<Functor, VisitorsHead, Visitors, BaseAcceptor, AcceptorsTail, ResultType, Args...>
            ::DispatchAcceptor(derivedVisitor, baseAcceptor, args...);
        }
    }
};

template<template<class, class, class, class...> class Functor, class BaseVisitor, class BaseAcceptor, class Acceptors,
        class ResultType, class... Args>
class VisitFactory<Functor, BaseVisitor, TypeList<>, BaseAcceptor, Acceptors, ResultType, Args...> {
public:
    static ResultType Go(BaseVisitor* baseVisitor, BaseAcceptor* baseAcceptor, Args... args) {
        std::cout << "NO SUITABLE CLASS WAS FOUND IN VISITORS TYPELIST\n";
        return ResultType();
    }
};

template<template<class, class, class, class...> class Functor, class BaseVisitor, class Visitors, class BaseAcceptor,
        class ResultType, class... Args>
class VisitFactory<Functor, BaseVisitor, Visitors, BaseAcceptor, TypeList<>, ResultType, Args...> {
public:
    static ResultType Go(BaseVisitor* baseVisitor, BaseAcceptor* baseAcceptor, Args... args) {
        std::cout << "NO SUITABLE CLASS WAS FOUND IN ACCEPTORS TYPELIST\n";
        return ResultType();
    }

    static ResultType DispatchAcceptor(BaseVisitor* derivedVisitor, BaseAcceptor* baseAcceptor, Args... args) {
        std::cout << "NO SUITABLE CLASS WAS FOUND IN ACCEPTORS TYPELIST\n";
        return ResultType();
    }
};
