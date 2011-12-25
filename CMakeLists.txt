cmake_minimum_required(VERSION 2.8)

project(nV CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

include (Utils)

# Configurable options

set(CMAKE_PREFIX_PATH ""
    CACHE STRING "We will try to find all external libraries here")
set(EXTRA_INCLUDE ""
    CACHE STRING "extra directories to find *.h files")

set(USE_JAVA      "YES" CACHE STRING "nVLink - Java")
set(USE_NTL_FLINT "YES" CACHE STRING "nVLink - ntl & flint")
set(USE_YAFU      "NO"  CACHE STRING "nVLink - yafu")
set(USE_PYTHON    "NO"  CACHE STRING "nVLink - Python")
set(USE_LUA       "YES" CACHE STRING "nVLink - Lua")
set(USE_ECL       "NO"  CACHE STRING "nVLink - Ecl")
set(USE_MU0       "NO"  CACHE STRING "nVLink - mU0")

# Overall -I s

include_directories (BEFORE ${PROJECT_SOURCE_DIR}/Include/external)
include_directories (${PROJECT_SOURCE_DIR}/Include)
include_directories (${EXTRA_INCLUDE})

# Definitions
add_definitions(-DUNICODE)

# Depenences

find_package (Boost REQUIRED thread system date_time regex)
include_directories(${Boost_INCLUDE_DIRS})
find_library (gmp_LIBRARIES gmp REQUIRED)
find_library (mpfr_LIBRARIES mpfr REQUIRED)
find_library (mpfi_LIBRARIES mpfi REQUIRED)

if (USE_JAVA)
    find_package (JNI REQUIRED)
endif ()

if (USE_NTL_FLINT)
    find_library (ntl_LIBRARIES ntl REQUIRED)
    find_library (flint_LIBRARIES flint REQUIRED)
endif ()

if (USE_YAFU)
    find_library (yafu_LIBRARIES yafu REQUIRED)
endif ()

if (USE_PYTHON)
    find_package (PythonLibs REQUIRED)
endif ()

if (USE_LUA)
    find_package (Lua51 REQUIRED)
    find_library (luabind_LIBRARIES luabind REQUIRED)
endif ()

if (USE_ECL)
    find_library (ecl_LIBRARIES ecl REQUIRED)
endif ()

if (USE_MU0)
    find_library (mU0_LIBRARIES mU0 REQUIRED)
    find_library (mU0_kernel_LIBRARIES kernel REQUIRED)
    set(mU0_LIBRARIES ${mU0_LIBRARIES} ${mU0_kernel_LIBRARIES})
endif ()

# subdirectory

add_subdirectory(mU)
add_subdirectory(mUKernel)
add_subdirectory(mUSystem)
add_subdirectory(mUScript)
add_subdirectory(mULink)

add_subdirectory(nV)
add_subdirectory(nVKernel)
add_subdirectory(nVNumber)
add_subdirectory(nVParser)
add_subdirectory(nVPolynomial)
add_subdirectory(nVMatrix)
add_subdirectory(nVSystem)
add_subdirectory(nVCalculus)
add_subdirectory(nVGraphics)
add_subdirectory(nVString)
add_subdirectory(nVTest)
add_subdirectory(nVLink)

# from orignal CMake files

#set(USE_GOOGLE_PERFTOOLS 1)
#if(USE_GOOGLE_PERFTOOLS)
#  set(CMAKE_EXE_LINKER_FLAGS_PROF "-lprofiler -ltcmalloc")
#  set(CMAKE_SHARED_LINKER_FLAGS_PROF="-lprofiler -ltcmalloc")
#  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-ltcmalloc")
#  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE="-ltcmalloc")
#endif()