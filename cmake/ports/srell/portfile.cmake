vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO data-man/SRELL
    REF 21abff2412c52c2ea6402e3e8798f4607d445728
    SHA512 7040145ccc383d6058799ea464a2c8f3067e817ad5c8ea6a6bf44c3b122fdcbbbb465a7f38c6b7fa2cb1596e4e2a83f8c80af4001075b19937c415629cee6ad8
    HEAD_REF trunk
)

file(INSTALL
    "${SOURCE_PATH}/srell.hpp"
    "${SOURCE_PATH}/srell_ucfdata2.h"
    "${SOURCE_PATH}/srell_updata3.h"
    DESTINATION "${CURRENT_PACKAGES_DIR}/include"
)

file(INSTALL "${SOURCE_PATH}/license.txt" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
