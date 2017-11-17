cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(SOMpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

# Config stuff
set(USE_TAGGING FALSE CACHE BOOL "")
set(GC_TYPE generational CACHE STRING "")
set(CACHE_INTEGER FALSE CACHE BOOL "")
set(INT_CACHE_MIN_VALUE -5 CACHE STRING "")
set(INT_CACHE_MAX_VALUE 100 CACHE STRING "")
set(GENERATE_INTEGER_HISTOGRAM FALSE CACHE BOOL "")
set(GENERATE_ALLOCATION_STATISTICS FALSE CACHE BOOL "")
set(LOG_RECIEVER_TYPES FALSE CACHE BOOL "")
set(UNSAFE_FRAME_OPTIMIZATION FALSE CACHE BOOL "")
set(ADDITIONAL_ALLOCATION FALSE CACHE BOOL "")

#TODO error if cache_integer && USE_TAGGING
add_library(som_feature_flags INTERFACE)
if(USE_TAGGING)
    target_compile_definitions(som_feature_flags INTERFACE USE_TAGGING)
endif()
#TODO CACHE INTEGER

#TODO set this properly
target_compile_definitions(som_feature_flags INTERFACE GC_TYPE=GENERATIONAL)

#add_subdirectory(src)
add_executable(SOM++
    src/compiler/BytecodeGenerator.cpp
    src/compiler/ClassGenerationContext.cpp
    src/compiler/Disassembler.cpp
    src/compiler/Lexer.cpp
    src/compiler/MethodGenerationContext.cpp
    src/compiler/Parser.cpp
    src/compiler/SourcecodeCompiler.cpp

    src/interpreter/bytecodes.cpp
    src/interpreter/Interpreter.cpp
    
    src/memory/CopyingCollector.cpp
    src/memory/CopyingHeap.cpp
    src/memory/GenerationalCollector.cpp
    src/memory/GenerationalHeap.cpp
    src/memory/Heap.cpp
    src/memory/MarkSweepCollector.cpp
    src/memory/MarkSweepHeap.cpp

    src/misc/Timer.cpp

    src/primitives/Array.cpp
    src/primitives/Block.cpp
    src/primitives/Class.cpp
    src/primitives/Double.cpp
    src/primitives/Integer.cpp
    src/primitives/Method.cpp
    src/primitives/Object.cpp
    src/primitives/Primitive.cpp
    src/primitives/String.cpp
    src/primitives/Symbol.cpp
    src/primitives/System.cpp

    src/primitivesCore/PrimitiveContainer.cpp
    src/primitivesCore/PrimitiveLoader.cpp

    #src/unitTests/CloneObjectsTest.cpp
    #src/unitTests/main.cpp
    #src/unitTests/WalkObjectsTest.cpp
    #src/unitTests/WriteBarrierTest.cpp

    src/vmobjects/AbstractObject.cpp
    src/vmobjects/IntegerBox.cpp
    src/vmobjects/Signature.cpp
    src/vmobjects/VMArray.cpp
    src/vmobjects/VMBlock.cpp
    src/vmobjects/VMClass.cpp
    src/vmobjects/VMDouble.cpp
    src/vmobjects/VMEvaluationPrimitive.cpp
    src/vmobjects/VMFrame.cpp
    src/vmobjects/VMInteger.cpp
    src/vmobjects/VMInvokable.cpp
    src/vmobjects/VMMethod.cpp
    src/vmobjects/VMObject.cpp
    src/vmobjects/VMPrimitive.cpp
    src/vmobjects/VMString.cpp
    src/vmobjects/VMSymbol.cpp

    src/vm/Shell.cpp
    src/vm/Universe.cpp

    src/Main.cpp
)

target_include_directories(SOM++ PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(SOM++ PRIVATE som_feature_flags)
