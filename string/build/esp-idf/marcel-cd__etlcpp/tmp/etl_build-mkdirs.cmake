# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/warren/Exjobb/Tests/string/managed_components/marcel-cd__etlcpp/etl"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/etl"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/tmp"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/src"
  "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/warren/Exjobb/Tests/string/build/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp${cfgdir}") # cfgdir has leading slash
endif()
