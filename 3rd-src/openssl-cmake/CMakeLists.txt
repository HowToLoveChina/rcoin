cmake_minimum_required(VERSION 2.8)

project(openssl)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(crypto EXCLUDE_FROM_ALL)
add_subdirectory(ssl EXCLUDE_FROM_ALL)

set(OPENSSL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

set(OPENSSL_LIBRARIES crypto_static eay32_static PARENT_SCOPE)
set(OPENSSL_VERSION "1.0.1j" PARENT_SCOPE)
set(OPENSSL_FOUND TRUE PARENT_SCOPE)

set(LIB_EAY_LIBRARY crypto_static PARENT_SCOPE)
set(SSL_EAY_LIBRARY eay32_static PARENT_SCOPE)

set(OPENSSL_SSL_LIBRARY eay32_static PARENT_SCOPE)
set(OPENSSL_CRYPTO_LIBRARY crypto_static PARENT_SCOPE)
