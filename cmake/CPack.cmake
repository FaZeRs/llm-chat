include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR IFW TGZ)
set(CPACK_PACKAGE_NAME "RoomSketcher")
set(CPACK_PACKAGE_VENDOR "Giraffe360")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Room Sketcher - Floor Plan Editor")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://giraffe360.com")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "RoomSketcher")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nauris Linde <naurislinde@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libgcc-s1")

set(CPACK_IFW_PACKAGE_NAME "RoomSketcher")
set(CPACK_IFW_PACKAGE_TITLE "RoomSketcher Installer")
set(CPACK_IFW_PACKAGE_PUBLISHER "Giraffe360")
set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
set(CPACK_IFW_PACKAGE_WIZARD_SHOW_PAGE_LIST OFF)

if(WIN32)
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-win64")
else()
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-linux")
endif()

include(CPack)
include(CPackIFW)