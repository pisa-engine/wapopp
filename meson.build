project(
  'wapopp',
  'cpp',
  version : '1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17', 'default_library=static'],
  subproject_dir: 'external',
)

cmake = import('cmake')
json = cmake.subproject('json').dependency('nlohmann_json')

wapopp = library(
  'wapopp',
  'src/wapopp.cpp',
  include_directories: 'include',
  dependencies: json,
  install: true,
)
wapopp_dep = declare_dependency(
  include_directories : 'include',
  link_with : wapopp
)

build_tests = get_option('build_tests')
if build_tests
  catch2 = subproject('Catch2').get_variable('catch2_with_main_dep')

  tests = executable(
    'test_wapopp',
    # 'test/test_detail.cpp',
    'test/test_wapopp.cpp',
    include_directories: 'include',
    dependencies: [catch2, json],
    link_with: wapopp,
  )
  test('unit tests', tests)
endif
