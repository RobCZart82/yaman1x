# YamAN1x source-only build recipe, adapted from the VOP3 snapshot CMakeLists.txt.
# Original recipe: BSD-3-Clause; see third_party/vop3/LICENSE.
# Excludes upstream examples and tests requiring Yamaha program images.
file(GLOB ASMJIT CONFIGURE_DEPENDS
  "${YAMANIX_VOP3_SOURCE}/third_party/asmjit/asmjit/core/*.cpp"
  "${YAMANIX_VOP3_SOURCE}/third_party/asmjit/asmjit/x86/*.cpp"
  "${YAMANIX_VOP3_SOURCE}/third_party/asmjit/asmjit/arm/*.cpp"
  "${YAMANIX_VOP3_SOURCE}/third_party/asmjit/asmjit/axl/*.cpp")
# Upstream test translation units are not part of the runtime library.
list(FILTER ASMJIT EXCLUDE REGEX "_test[.]cpp$")
if(NOT ASMJIT)
  message(FATAL_ERROR "VOP3 source snapshot must include its reviewed AsmJit source")
endif()
add_library(asmjit STATIC ${ASMJIT})
target_include_directories(asmjit PUBLIC "${YAMANIX_VOP3_SOURCE}/third_party/asmjit")
target_compile_definitions(asmjit PUBLIC ASMJIT_STATIC ASMJIT_NO_BUILDER ASMJIT_NO_COMPILER ASMJIT_NO_UJIT ASMJIT_NO_FOREIGN ASMJIT_NO_SHM_OPEN)
find_package(Threads REQUIRED)
target_link_libraries(asmjit PUBLIC Threads::Threads)
add_library(vop3 STATIC "${YAMANIX_VOP3_SOURCE}/src/vop3.cpp" "${YAMANIX_VOP3_SOURCE}/src/yss236_jit.cpp")
target_include_directories(vop3 PUBLIC "${YAMANIX_VOP3_SOURCE}/src")
target_link_libraries(vop3 PRIVATE asmjit)
if(MSVC)
  target_compile_options(vop3 PRIVATE /W3 /fp:precise /permissive-)
else()
  target_compile_options(vop3 PRIVATE -Wall -ffp-contract=off)
endif()
