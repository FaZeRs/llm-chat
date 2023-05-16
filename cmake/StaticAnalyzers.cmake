macro(enable_cppcheck target WARNINGS_AS_ERRORS CPPCHECK_OPTIONS)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)

        if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
            set(CPPCHECK_TEMPLATE "vs")
        else()
            set(CPPCHECK_TEMPLATE "gcc")
        endif()

        if("${CPPCHECK_OPTIONS}" STREQUAL "")
            # Enable all warnings that are actionable by the user of this toolset
            # style should enable the other 3, but we'll be explicit just in case
            set(CPPCHECK_COMMAND
                    ${CPPCHECK}
                    --template=${CPPCHECK_TEMPLATE}
                    --enable=style,performance,warning,portability
                    --inline-suppr
                    # We cannot act on a bug/missing feature of cppcheck
                    --suppress=cppcheckError
                    --suppress=internalAstError
                    # if a file does not have an internalAstError, we get an unmatchedSuppression error
                    --suppress=unmatchedSuppression
                    # noisy and incorrect sometimes
                    --suppress=passedByValue
                    # ignores code that cppcheck thinks is invalid C++
                    --suppress=syntaxError
                    --suppress=preprocessorErrorDirective
                    --inconclusive)
        else()
            # if the user provides a CPPCHECK_OPTIONS with a template specified, it will override this template
            set(CPPCHECK_COMMAND ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${CPPCHECK_OPTIONS})
        endif()

        if(NOT
                "${CMAKE_CXX_STANDARD}"
                STREQUAL
                "")
            set(CPPCHECK_COMMAND ${CPPCHECK_COMMAND} --std=c++${CMAKE_CXX_STANDARD})
        endif()
        if(${WARNINGS_AS_ERRORS})
            list(APPEND CPPCHECK_COMMAND --error-exitcode=2)
        endif()

        # set(CMAKE_CXX_CPPCHECK ${CPPCHECK_OPTIONS})
        set_target_properties(${target} PROPERTIES CXX_CPPCHECK "${CPPCHECK_COMMAND}")
    else()
        message(${WARNING_MESSAGE} "cppcheck requested but executable not found")
    endif()
endmacro()

macro(enable_clang_tidy target WARNINGS_AS_ERRORS)

    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)        # construct the clang-tidy command line
        set(CLANG_TIDY_COMMAND
                ${CLANGTIDY}
                -extra-arg=-Wno-unknown-warning-option
                -extra-arg=-Wno-ignored-optimization-argument
                -extra-arg=-Wno-unused-command-line-argument
                -p)
        # set standard
        if(NOT
                "${CMAKE_CXX_STANDARD}"
                STREQUAL
                "")
            if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
                set(CLANG_TIDY_COMMAND ${CLANG_TIDY_COMMAND} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
            else()
                set(CLANG_TIDY_COMMAND ${CLANG_TIDY_COMMAND} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
            endif()
        endif()

        # set warnings as errors
        if(${WARNINGS_AS_ERRORS})
            list(APPEND CLANG_TIDY_COMMAND -warnings-as-errors=*)
        endif()

        # set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    else()
        message(${WARNING_MESSAGE} "clang-tidy requested but executable not found")
    endif()
endmacro()

macro(enable_include_what_you_use target)
    find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
    if(INCLUDE_WHAT_YOU_USE)
        # set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
        set_target_properties(${target} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
    else()
        message(${WARNING_MESSAGE} "include-what-you-use requested but executable not found")
    endif()
endmacro()