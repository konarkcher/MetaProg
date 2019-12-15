#pragma once

template<class... T>
struct TypeList;

template<class H, class... T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

template<class H>
struct TypeList<H> {
    using Head = H;
    using Tail = TypeList<>;
};

template<>
struct TypeList<> {
};
