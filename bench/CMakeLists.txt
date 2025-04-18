add_subdirectory(ref)

add_definitions(-DCONSOLE)

brlcad_addexec(pixcmp pixcmp.c libbu)
set(BARK_SOURCES benchmark.c compute.c run.c clean.c)
brlcad_addexec(bark "${BARK_SOURCES}" libbu NO_STRICT NO_INSTALL TEST_USESDATA)

if(BUILD_TESTING)
  configure_file(run.sh "${CMAKE_CURRENT_BINARY_DIR}/benchmark" COPYONLY)
  install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/benchmark" DESTINATION ${BIN_DIR})
endif(BUILD_TESTING)
distclean("${CMAKE_CURRENT_BINARY_DIR}/benchmark")

if(BUILD_TESTING)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/benchmark-done
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/run.sh $<TARGET_FILE_DIR:bark>/benchmark
    COMMAND "${CMAKE_COMMAND}" -E touch ${CMAKE_CURRENT_BINARY_DIR}/benchmark-done
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
  )
  add_custom_target(benchmark_cp ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/benchmark-done)
  set_target_properties(benchmark_cp PROPERTIES FOLDER "Benchmark")
endif(BUILD_TESTING)
distclean(${CMAKE_BINARY_DIR}/${BIN_DIR}/benchmark)

file(WRITE "${CMAKE_BINARY_DIR}/CMakeTmp/benchmsg.cmake" "message(\"---\")\n")
file(
  APPEND
  "${CMAKE_BINARY_DIR}/CMakeTmp/benchmsg.cmake"
  "message(\"Run 'make benchmark-clean' in the bench directory to delete the logic\")\n"
)
file(
  APPEND
  "${CMAKE_BINARY_DIR}/CMakeTmp/benchmsg.cmake"
  "message(\"and image files generated during the benchmark analysis.\")\n"
)

cmakefiles(
  CMakeLists.txt
  run.sh
  try.sh
  viewdiff.sh
)

if(SH_EXEC AND TARGET m35.g AND BUILD_TESTING)
  # CTest enabled short run of the benchmark suite
  brlcad_add_test(NAME benchmark COMMAND ${SH_EXEC} "$<TARGET_FILE_DIR:bark>/benchmark" run TIMEFRAME=1)
  set_tests_properties(benchmark PROPERTIES LABELS "Benchmark")
  add_custom_target(
    benchmark-check
    COMMAND ${CMAKE_CTEST_COMMAND} -L Benchmark --output-on-failure
    DEPENDS rt pixcmp BENCHMARK_PIX_cp PIX_LOG_cp benchmark_cp
  )
  add_dependencies(
    benchmark-check
    bldg391.g
    m35.g
    moss.g
    sphflake.g
    star.g
    world.g
  )
  set_target_properties(benchmark-check PROPERTIES FOLDER "Benchmark")
  if(TARGET check)
    add_dependencies(check benchmark-check)
  endif(TARGET check)

  # Full run of the benchmark suite
  add_custom_target(
    benchmark
    COMMAND ${SH_EXEC} $<TARGET_FILE_DIR:bark>/benchmark run
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/CMakeTmp/benchmsg.cmake"
    DEPENDS rt pixcmp BENCHMARK_PIX_cp PIX_LOG_cp benchmark_cp
  )
  add_dependencies(
    benchmark
    bldg391.g
    m35.g
    moss.g
    sphflake.g
    star.g
    world.g
  )
  set_target_properties(benchmark PROPERTIES FOLDER "Benchmark")

  # benchmark cleanup targets
  add_custom_target(benchmark-clean ${SH_EXEC} ${CMAKE_BINARY_DIR}/bin/benchmark clean)
  set_target_properties(benchmark-clean PROPERTIES FOLDER "Benchmark")
  add_custom_target(benchmark-clobber ${SH_EXEC} ${CMAKE_BINARY_DIR}/bin/benchmark clobber)
  set_target_properties(benchmark-clobber PROPERTIES FOLDER "Benchmark")
endif(SH_EXEC AND TARGET m35.g AND BUILD_TESTING)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
