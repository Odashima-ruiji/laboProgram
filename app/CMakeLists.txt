add_executable(a.out main.c)
# a.outをコンパイルする際にgreetingsをリンクする
target_link_libraries(a.out carshare m)
