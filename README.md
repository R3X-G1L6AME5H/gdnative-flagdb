# FlagDB; commpressing for the fun of it

## The point?
`Flagpole.gd` was a script I whiped up out of boredom one day; it was of little importance. However, recently I have been trying to get another script working using GDNative. I was struck by just how little help there is online. Therefore I decided to translate this one off script to GDNative in the name of the Godot Comunity. If you are here strictly for a GDNative guide, proceed here.

## Prologue
I was building a game. This game needed to have a story manager. So I built one in a fashion simmilar to Ink. However, I noticed how Ink would use strings as flags. Inspired by witnessing a man cram an 8K, 60fps video onto a Nintendo 64 cartrige ([Inigo Quilez](https://www.youtube.com/c/InigoQuilez), if I remember correctly) I thought to myself: "this is unacceptable".

A flag is a boolean value; single bool should be sufficient. These bools would then be placed in an array, and would be directly accessed through their index. It was a genius plan; until a shocking revelation.

Booleans are not actually stored in memory as a single bit. Some sources say they are 1 byte large, while others claim that booleans are actually an integer, 4 bytes large... Unacceptable; that is too many bits wasted.

There had to be a way out of this conundrum. Wait a minute, bytes are just a bunch of bits. What if we stored each individual flag in a bit of an integer. A 32 bit integer could then hold 32 flags, in a single register. Sometimes my genius frigntens me.

## The Logic

## Setting GDNative
There is no going around it. You will have to compile your own godot libraries, and then compile this script to get to use this script. This is simply due to the fact that Godot is constantly evolving. Although it might seem like a drag, this way is for the better, because this way you can most likely compile a binary for any version of Godot, which is incredibly useful. So roll up your sleeves and get through it. 

Setting up the GDNative environment is a fun ride. There is very little help online for someone who is not already experienced in C development. Hence why I'm throwing my own hat into the ring.

### Versioning

So to start of you will need two things: `scons`, and the `godot` executable. What we will attempt is to compile YOUR godot's bindings into a C library. The YOUR part is important because, as implied before, Godot has multiple versions, and hence multiple [godot-headers](https://github.com/godotengine/godot-headers). If the headers and your godot version do not match, your code will fail to compile. The compiler will most likely say that it cannot find the files you are trying to include e.g. `#include <Godot.hpp>`.

To see your godot version, you open Godot Editor go to `Help/About Godot`.

As you can see, I am running v3.5. Now that you know the version you can hop over to [godot-cpp](https://github.com/godotengine/godot-cpp) github page, and get yourself your C++ bindings.

Now, it may be tempting to simply clone this repository BUT don't be swayed; tis but a ruse. Remember that we are looking for a specific version. Take a look at the branches of godot-cpp and you will see versions like `3.3`, `3.4`, `3.x`. I have Godot 3.5 so I will take the `3.5` branch. Follow the instructions on how to clone that is provided in the `README.md`. Just keep in mind `godot-cpp` has a `godot-headers` submodule that must also be installed for the build to work. 

Take note how the README of each branch is different. Keep this in mind for when you try to write in GDNative. The information in these READMEs is something you will not find in (the official documentation)[https://godot-doc.readthedocs.io/en/3.0/tutorials/plugins/gdnative/gdnative-cpp-example.html]. 

### Generating bindings

Now that everything is cloned i.e. you have a `./godot-cpp` directory, and a non-empty `./godot-cpp/godot-headers` directory. You will want to get the godot api data. This is to absolutely sync your godot version with the bindings you just cloned. To this just run:

```
godot --gdnative-generate-json-api api.json
```

Double check this with the repository's documentation since, depending on the version, `api.json` used to be `nativescript-api.json`. Now, copy the `api.json` to `./godot-cpp/godot-headers/` and overwrite the existing file.


### Compilation

Now its time to compile the bindings. This is where `scons` comes in. C++ bindings library:

```
cd godot-cpp
scons platform=<your platform> generate_bindings=yes
```

> "Replace <your platform> with either `windows`, `linux`, `osx` or `android`. If you leave out platform, the target platform will automatically be detected from the host platform."  -  godot-cpp, branch 3.5, README.md 

This process will take some time, so grab yourself some tea while you wait.

Once the compilation is done, you will be left with a C++ library inside the `./godot-cpp/bin`. This and the files inside the `./godot-cpp/include` will be the things you will need to compile your code.

### The problems

Speaking of compilation; welcome to the thunder-dome. Your first challenge will be figuring out why the linker cannot find `Godot.hpp`, at least it was in my case. The docs will tell you to, for example, simply run:

```
clang++ -fPIC -o src/init.o -c src/init.cpp -g -O3 -std=c++14 -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
clang++ -o bin/libtest.so -shared src/init.o -Lgodot-cpp/bin -l<name of the godot-cpp>
```

But the linker will (refering to the first line) for some reason still fail to find your files even though they truly do exist; well, at least you think they exist. In such a case, double check that the file you are trying to include is, in fact, inside the directories you are trying to include e.g. `#include <Godot.hpp>` is trying to include a `Godot.hpp` file, which should be inside `./godot-cpp/include/core`. If its not, you messed up in the cloning process. Chances are you cloned the wrong branch. But if it is, and it still cannot be found check the paths you are including via `-I` parameter. Is `-Igodot-cpp/include/core` present? 

Pro tip: triple check that all the `-` and the `_` in your paths match the actual directory names. This will save you hours of trouble.  

Regarding the second line, watch out for the `-l` flag. Make sure that the library name matches. And if that doesn't fix the problem, try `-l:` instead.
I do not know what difference it makes, but changing `-llibgodot-cpp.linux.debug.64.a` to `-l:libgodot-cpp.linux.debug.64.a` compiled on my system.

Finally, watch out for the ordering of the parameters. `clang++ -o bin/libtest.so -shared src/init.o -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a` compiles. `clang++ -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a -o bin/libtest.so -shared src/init.o` does not.

### Scaling up

Now that you know what to look out for, how to scale up the compilation process? Overall there are 2 steps to the compilation: compiling your scripts, and compiling your executable, respectively.

Say your project looked something like this:

```
src/car.hpp
src/car.cpp
src/wheels.hpp
src/wheels.cpp
src/gdlibrary.cpp
```

First step would be compiling your scripts. So:
```
clang++ -fPIC -o src/car.o    -c src/car.cpp    -g -O3 -std=c++14 -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
clang++ -fPIC -o src/wheels.o -c src/wheels.cpp -g -O3 -std=c++14 -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
```

Then, you would compile the executable using the newly aquired files:
```
clang++ -o bin/libveicle.so -shared src/car.o -shared src/wheels.o -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a
```

That is the gist of it. But as you may notice, it is a rather tedious process. Hence, for the optimal GDNative workflow I advise you to familiarize yourself with `make` and `Makefile` to automate the hassle.


## Implementation
