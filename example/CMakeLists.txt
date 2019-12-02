add_executable(rectpack2D-example "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set_target_properties(rectpack2D-example PROPERTIES CXX_EXTENSIONS OFF)
if(CMAKE_MINOR_VERSION GREATER 8)
  target_compile_features(rectpack2D-example PUBLIC cxx_std_11)
elseif(UNIX OR APPLE)
  target_compile_options(rectpack2D-example PRIVATE -std=c++17)
elseif(MSVC)
  target_compile_options(rectpack2D-example PRIVATE /std:c++latest)
endif()
target_link_libraries(rectpack2D-example PRIVATE rectpack2D::rectpack2D stdc++fs)
target_compile_options(rectpack2D-example PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Werror -Wextra -ftemplate-backtrace-limit=0 -Wno-error=missing-braces>
  # $<$<CXX_COMPILER_ID:GNU>:-Weffc++>
  $<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics -stdlib=libc++>
)
