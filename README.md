# FlagDB; commpressing for the fun of it

## The point?
`Flagpole.gd` was a script I whiped up out of boredom one day; it was of little importance. However, recently I have been trying to get another script working using GDNative. I was struck by just how little help there is online. Therefore I decided to translate this one off script to GDNative in the name of the Godot Comunity. If you are here strictly for a GDNative guide, proceed here.


## Index
- [**How it all started**](#prologue)
- [**How it works**](#the-logic)
  - [**Ticking a bit**](#ticking)
  - [**Clearing a bit**](#clearing)
  - [**Fliping a bit**](#fliping)
  - [**Reading a bit**](#reading)
  - [**Managing the array**](#managing)
  
- [**Setting up GDNative**](#gdnative-setup)
  - [**Versioning**](#versioning)
  - [**Generating Bindings**](#generating-bindings)
  - [**Compiling**](#compiling)
  - [**Troubleshooting**](#troubleshooting)
  - [**Practical Workflow**](#scaling-up)
- [**Usage**](#usage)



## Prologue
I was building a game. This game needed to have a story manager. So I built one in a fashion simmilar to Ink. However, I noticed how Ink would use strings as flags. Inspired by witnessing a man cram an 8K, 60fps video onto a Nintendo 64 cartrige ([Inigo Quilez](https://www.youtube.com/c/InigoQuilez), if I remember correctly) I thought to myself: "this is unacceptable".

A flag is a boolean value; single bool should be sufficient. These bools would then be placed in an array, and would be directly accessed through their index. It was a genius plan; until a shocking revelation.

Booleans are not actually stored in memory as a single bit. Some sources say they are 1 byte large, while others claim that booleans are actually an integer, 4 bytes large... Unacceptable; that is too many bits wasted.

There had to be a way out of this conundrum. Wait a minute, bytes are just a bunch of bits. What if we stored each individual flag in a bit of an integer. A 32 bit integer could then hold 32 flags, in a single register. Sometimes my genius frigntens me.

## The Logic

### Ticking

Ticking a bit means, setting the n-th bit to 1. We can do this with bitwise operator OR(|). But what to compare? We compare out storage integer(S), with our mask integer(M). This way we will be adding data to our database.
```
Storage           -> 0011(3)        0111(7)
Mask        |     -> 0100(4) |      0010(2) |
-------------        ---------      ---------
New Storage          0111(7)        0111(7)

                    # 3rd bit #       # Remains   #    
                    # ticked  #       # unchanged #

```


The mask M serves to select which bit to tick. In the binary representation of M we need only a single 1, and it should be the n-th bit. To achieve this we use the left bitshift operator(<<).
```
0001(1) << 0   =   0001(1)
0001(1) << 1   =   0010(2)
0001(1) << 2   =   0100(4)
              ...
      1 << n   =   M
```

When put together you get:
```C
unsigned int tick( unsigned int* storage, unsigned int n ){
    unsigned int mask = 1 << n;     // Generate the mask
    *storage = *storage & mask;     // Tick the bit in storage
}
```

### Clearing

Clearing a bit means seting the n-th bit to 0, and is fittingly just the inverse of the ticking function. The only thing we need to change is the mask.
We need to invert it; flip its bits. For this we use the COMPLEMENT operator(~).

```
~Mask                -> ~0100(4)       ~0010(2) 
---------------         ---------      ---------
Mask Complement      ->  1011(~4)       1101(~2)



Storage               -> 0111(7)        0101(5)
Mask Complement &     -> 1011(~4) &     1101(~2) &
-----------------        ----------     ----------
New Storage              0011(3)        0101(5)

                       # 3rd bit #       # Remains   #    
                       # cleared  #      # unchanged #
```

And thus, our clearing method should look something like this:


```C
unsigned int clear( unsigned int* storage, unsigned int n ){
    unsigned int mask = 1 << n;     // Generate the mask
    *storage = *storage & ~mask;    // Clear the bit in storage
}
```

### Fliping
To flip is to invert the state of the n-th bit; 1 to 0, and vice versa. Fliping can be easly done with the XOR(^) operator.

```
Storage           -> 0011(3)           0111(7)
Mask        ^     -> 0100(4) ^         0100(4) ^
-------------        ---------         ---------
New Storage          0111(7)           0011(3)

                   # 3rd bit #       # 3rd bit #    
                   # is now  #       # is now  #
                   # a one   #       # a zero  #

```

With little alterations, our method ends up like this:

```C
unsigned int flip( unsigned int* storage, unsigned int n ){
    unsigned int mask = 1 << n;     // Generate the mask
    *storage = *storage ^ mask;     // Flip the bit in storage
}
```

### Reading
Reading is the way we access our data. We use the AND(&) operator, again. But, notice what happens when the ask matches, as opposed to when it does not.

```
Storage           -> 0111(7)           0011(3)
Mask        ^     -> 0100(4) &         0100(4) ^
-------------        ---------         ---------
New Storage          0100(4)           0000(0)
```

When the mask detects a bit, the resault is greater than 0, whilst in the opposite situation, it is just 0. This is how we check the data in our database. Our C code is therefore:


```C
bool read( unsigned int* storage, unsigned int n ){
    unsigned int mask = 1 << n;     // Generate the mask
    return (*storage & mask) > 0;   // Read the bit in storage
}
```

### Managing
Now, all of our operations work on a presuposition that we have an infinite register. Alas, modern machines work on a 64 bit architecture, some still run on 32 bit architecture. Hence, we have to get creative in how we manage our numbers.

True, the bigest register a machine can load is 64/32 bits, but if we were to use two such registers independently, we would have acces to 128/64 bits. Therefore, if we manage our numbers correctly, we can get a virtually ininite register.

We use arrays of integers then. If each integer took up 4 bits, to get to the 6th bit B we would have to access the 2th bit in the 2nd integer. The way we get this 1(note that the 2nd bit has the index of 1) for the bit, and 1 for the index is by INTEGER DIVISION(/), and MODULO(%).

```
Index    |    1    0
Integer  |   1000 0001
               ^


INDEX = B / WORD_LENGTH = 6 / 4 = 1
BIT   = B % WORD LENGTH = 6 % 4 = 1
```

Thus, if we wanted to read the nth bit from a integer array `INT_ARRAY` where an integer takes up `WORD_LENGTH` bits, we would have to wrap our `read()` method with the following:

```C
bool read_wrapper(unsigned int n){
    int n_in_intger     = n % WORD_LENGTH;
    int n_integer_index = n / WORD_LENGTH;
    
    int* storage_integer_address = &( INT_ARRAY[n_integer_index] )
    return read( storage_integer_address, n_in_integer );
}
```

Of course, each method mentioned thus far would need to be wrapped as well.


## GDNative Setup
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


### Compiling

Now its time to compile the bindings. This is where `scons` comes in. C++ bindings library:

```
cd godot-cpp
scons platform=<your platform> generate_bindings=yes
```

> "Replace <your platform> with either `windows`, `linux`, `osx` or `android`. If you leave out platform, the target platform will automatically be detected from the host platform."  -  godot-cpp, branch 3.5, README.md 

This process will take some time, so grab yourself some tea while you wait.

Once the compilation is done, you will be left with a C++ library inside the `./godot-cpp/bin`. This and the files inside the `./godot-cpp/include` will be the things you will need to compile your code.

### Troubleshooting

Speaking of compilation; welcome to the thunder-dome. Your first challenge will be figuring out why the linker cannot find `Godot.hpp`, at least it was in my case. The docs will tell you to, for example, simply run:

```
clang++ -fPIC -o src/init.o -c src/init.cpp -g -O3 -std=c++14 -Igodot-cpp/include -Igodot-cpp/include/core -Igodot-cpp/include/gen -Igodot-cpp/godot-headers
clang++ -o bin/libtest.so -shared src/init.o -Lgodot-cpp/bin -l<name of the godot-cpp>
```

But the linker will (refering to the first line) for some reason still fail to find your files even though they truly do exist; well, at least you think they exist. In such a case, double check that the file you are trying to include is, in fact, inside the directories you are trying to include e.g. `#include <Godot.hpp>` is trying to include a `Godot.hpp` file, which should be inside `./godot-cpp/include/core`. If its not, you messed up in the cloning process. Chances are you cloned the wrong branch. But if it is, and it still cannot be found check the paths you are including via `-I` parameter. Is `-Igodot-cpp/include/core` present? 

> Pro tip: triple check that all the `-` and the `_` in your paths match the actual directory names. This will save you hours of trouble.  

Regarding the second line, watch out for the `-l` flag. Make sure that the library name matches. And if that doesn't fix the problem, try `-l:` instead.
I do not know what difference it makes, but changing `-llibgodot-cpp.linux.debug.64.a` to `-l:libgodot-cpp.linux.debug.64.a` compiled on my system.

Finally, watch out for the ordering of the parameters. 

`clang++ -o bin/libtest.so -shared src/init.o -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a` compiles.

`clang++ -Lgodot-cpp/bin -l:libgodot-cpp.linux.debug.64.a -o bin/libtest.so -shared src/init.o` does not.

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

## Usage

Once you have compiled a binary. You can test it by placing it inside an empty project, placing a `FlagDB_Unit_Test.gd` on an empty node, and running the scene. (You can follow the godot documentation on how to make the binary usable within godot.)

Read through `FlagDB_Unit_Test.gd`, and you will see all the available functions and what they do.
