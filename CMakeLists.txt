cmake_minimum_required(VERSION 3.26)
project(DeipiSTL)

set(CMAKE_CXX_STANDARD 14)

add_executable(DeipiSTL
        DeipiSTL/achievement/alloc.cpp
        DeipiSTL/achievement/String.cpp
        DeipiSTL/achievement/Vector.cpp
        DeipiSTL/Algorithms.h
        DeipiSTL/alloc.h
        DeipiSTL/Allocator.h
        DeipiSTL/Construct.h
        DeipiSTL/Iterator.h
        DeipiSTL/main.cpp
        DeipiSTL/Memory.h
        DeipiSTL/Move.h
        DeipiSTL/String.h
        DeipiSTL/TypeTraits.h
        DeipiSTL/Uninitalized.h
        DeipiSTL/Vector.h
        DeipiSTL/List.h
        DeipiSTL/ReverseIterator.h
        DeipiSTL/Deque.h
        DeipiSTL/Matrix.h
        DeipiSTL/Matrix.h
        DeipiSTL/TransformMat.h
)
