cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(MagicalForestFights)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Werror -std=c++23")

#if(NOT TARGET spdlog)
#    # Stand-alone build
#    find_package(spdlog REQUIRED)
#endif()
#find_package(spdlog REQUIRED)

find_package(ryml REQUIRED)

include_directories("lib/spdlog/include")
include_directories("lib/rapidvsc")

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MagicalForestFights src/main.cpp
        src/creatures/MagicalCreature.cpp includes/creatures/MagicalCreature.h
        src/creatures/stats/CreatureStat.cpp includes/creatures/stats/CreatureStat.h
        includes/creatures/stats/Health.h includes/creatures/stats/Strength.h
        includes/creatures/stats/Defence.h includes/creatures/stats/Speed.h
        includes/creatures/stats/Luck.h
        src/creatures/skills/CreatureSkill.cpp includes/creatures/skills/CreatureSkill.h
        src/creatures/FightManager.cpp includes/creatures/FightManager.h
        includes/creatures/MagicalCreatureState.h
        includes/game/GameState.h src/game/GameEngine.cpp includes/game/GameEngine.h src/game/GameState.cpp
        src/game/IntroState.cpp includes/game/IntroState.h
        src/game/PlayState.cpp includes/game/PlayState.h
        includes/constants.h includes/creatures/skills/RapidStrike.h includes/creatures/skills/MagicalShield.h includes/utils/CharContainer.h)
target_link_libraries(MagicalForestFights ryml::ryml)

add_custom_target(run MagicalForestFights
        COMMAND $<TARGET_FILE:MagicalForestFights>
        DEPENDS MagicalForestFights
        COMMENT "running: $<TARGET_FILE:MagicalForestFights>")

enable_testing()

add_executable(
        MagicalForestFights_test
        src/creatures/MagicalCreature.cpp includes/creatures/MagicalCreature.h
        src/creatures/stats/CreatureStat.cpp includes/creatures/stats/CreatureStat.h
        includes/creatures/stats/Health.h
        includes/creatures/stats/Strength.h
        includes/creatures/stats/Defence.h
        includes/creatures/stats/Speed.h
        includes/creatures/stats/Luck.h
        src/creatures/skills/CreatureSkill.cpp includes/creatures/skills/CreatureSkill.h
        includes/creatures/MagicalCreatureState.h
        src/creatures/FightManager.cpp includes/creatures/FightManager.h
        test/CreatureSkill_test.cc
        test/CreatureStat_test.cc test/MagicalCreature_test.cc src/creatures/FightManager.cpp includes/creatures/FightManager.h
        includes/creatures/skills/RapidStrike.h includes/creatures/skills/MagicalShield.h)
target_link_libraries(
        MagicalForestFights_test
        GTest::gtest_main
        ryml::ryml
)

include(GoogleTest)
gtest_discover_tests(MagicalForestFights_test)
