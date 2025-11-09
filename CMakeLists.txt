cmake_minimum_required(VERSION 3.16)
project(WarzoneRiskGame CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
add_executable(MainDriver
    src/MainDriver.cpp

    src/GameEngineDriver.cpp 
    src/PlayerDriver.cpp
    src/CardsDriver.cpp
    src/MapDriver.cpp
    src/OrdersDriver.cpp

    src/GameEngine.cpp 
    src/Player.cpp
    src/Map.cpp
    src/Cards.cpp
    src/Orders.cpp
    src/CommandProcessing.cpp 
    src/CommandProcessing.h
    src/CommandProcessingDriver.cpp
)
