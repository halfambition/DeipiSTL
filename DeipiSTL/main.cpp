//
//  main.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/4.
//
#include <iostream>
#include "TypeTraits.h"
int main(){
    int&& a = 5;
    std::cout << DeipiSTL::is_pointer<int*>::value << std::endl;
    std::cout << DeipiSTL::is_pointer<int>::value << std::endl;
    std::cout << DeipiSTL::is_pointer<int&&>::value << std::endl;
    std::cout << DeipiSTL::is_pointer<const int*>::value << std::endl;
    std::cout << DeipiSTL::is_pointer<volatile int*>::value << std::endl;

    return 0;
}
