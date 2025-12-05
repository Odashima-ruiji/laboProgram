add_library(carshare
  SHARED
    base_func.c
    Common_func.c
    move.c
    my_func.c
    placement.c
  )
# greetingライブラリのインクルードディレクトリを教えてあげる
# PROJECT_SOURCE_DIRはこのプロジェクトのルートディレクトリの絶対パス
target_include_directories(carshare
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

target_link_libraries(carshare PRIVATE m)
