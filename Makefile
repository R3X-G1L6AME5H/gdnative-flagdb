##
# FlagDB
#
# @file
# @version 0.1


CFLAGS  = -fPIC
MACROS  = -g -O3 -std=c++14
INCLUDE = -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
LIBS    = -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a
CC      = clang++

libflagdb: src/flagdb.o src/gdlibrary.o
	$(CC) $(LIBS) --shared src/flagdb.o --shared src/gdlibrary.o -o bin/libflagdb.so

src/flagdb.o: src/flagdb.cpp
	$(CC) $(CFLAGS) -o $@ -c $? $(MACROS) $(INCLUDE)

src/gdlibrary.o: src/gdlibrary.cpp
	$(CC) $(CFLAGS) -o $@ -c $? $(MACROS) $(INCLUDE)


# end
