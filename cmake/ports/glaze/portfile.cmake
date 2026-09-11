if(VCPKG_TARGET_IS_LINUX)
    message("Warning: `glaze` requires Clang15+ or GCC 12+ on Linux")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO stephenberry/glaze
    REF dce5e0f7ec572725ec369a0bc59a00eeb2270a9a
    SHA512 e28965d4fedb47b460787f8b2b78bcc23f1af33ffa94a00d564bdc7ea168a013a166ed182f8a1a68095f335eab4e507cb11b203fb9154090e431e89d80fb243b
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
