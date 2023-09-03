//
//  main.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/4.
//
#include <iostream>
#include <vector>
int main(){
    std::vector<int> nums = { 1,2,3,4,5,6 };
    for (auto i = nums.size() - 1; i > -1; --i) {
        std::cout << i;
    }
    return 0;
}
