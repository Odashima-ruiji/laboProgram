add_executable(a.out main_no_gif.c)
# a.outをコンパイルする際にgreetingsをリンクする
target_link_libraries(a.out carshare m)
