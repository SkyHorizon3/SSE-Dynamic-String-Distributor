if(VCPKG_TARGET_IS_LINUX)
    message("Warning: `glaze` requires Clang15+ or GCC 12+ on Linux")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO stephenberry/glaze
    REF 459946d325c497c274fa59d666bd7cb6e2dd7ad0
    SHA512 172714b1aedb5b7f3ab876a286e00b82a73de64573f4d449d912d21e6834aa789214dd8bfcc5506bc8e6a4793954dba9b7d01e898cb4a651e68752d3b3af6d72
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -Dglaze_DEVELOPER_MODE=OFF
        -Dglaze_BUILD_EXAMPLES=OFF
        -Dglaze_DISABLE_SIMD_WHEN_SUPPORTED=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
