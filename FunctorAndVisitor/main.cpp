#include <iostream>

#include "Functor.h"
#include "Iterables.h"

int square(int x) {
    return x * x;
}


int main() {

    // Functor

    Functor<int, int> squareFunctor(square);
    std::cout << "squareFunctor(2) = " << squareFunctor(2) << '\n';

    // Iterables

    iter::Iterable<int>* arr = new iter::Set<int>({1, 2, 3, 4, 5});

    iter::Mapping<int>* prod = new iter::Prod<int>;
    arr->Accept(prod);

    // VisitFactory

    auto visitFactory = VisitFactory<iter::PrintFunctor,
            iter::Mapping<int>, TypeList<iter::Sum<int>, iter::Prod<int>>,
            iter::Iterable<int>, TypeList<iter::Array<int>, iter::Set<int>>, void, size_t>();

    visitFactory.Go(prod, arr, 20);

    return 0;
}