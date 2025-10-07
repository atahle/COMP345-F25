cmake_minimum_required(VERSION 3.16)
project(WarzoneRiskGame CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
add_executable(OrdersDriver
    src/OrdersDriver.cpp
    src/Orders.cpp
)
add_executable(GameEngineDriver
    src/GameEngineDriver.cpp 
    src/GameEngine.cpp 
    src/Player.cpp
    src/Map.cpp
    src/Cards.cpp
    src/Orders.cpp)

add_executable(PlayerDriver
    src/PlayerDriver.cpp
    src/Player.cpp
    src/Map.cpp
    src/Orders.cpp
    src/Cards.cpp
)

add_executable(CardsDriver
    src/CardsDriver.cpp
    src/Cards.cpp
)

add_executable(MapDriver
    src/MapDriver.cpp
    src/Map.cpp

)