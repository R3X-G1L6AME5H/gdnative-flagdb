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

So to start of you will need two things: `scons`, and the `godot` executable. What we will attempt is to compile YOUR godot's bindings into a C library. The YOUR part is important because, as implied before, Godot has multiple versions, and hence multiple [godot-headers](https://github.com/godotengine/godot-headers). If the headers and your godot version do not match, your code will fail to compile. The compiler will most likely say that it cannot find the files you are trying to include e.g. `#include <Godot.hpp>`.

To see your godot version, you open Godot Editor go to `Help/About Godot`.

As you can see, I am running v3.5. Now that you know the version you can hop over to [godot-cpp](https://github.com/godotengine/godot-cpp) github page, and get yourself your C++ bindings.

Now, it may be tempting to simply clone this repository BUT don't be swayed; tis but a ruse. Remember that we are looking for a specific version. Take a look at the branches of godot-cpp and you will see versions like `3.3`, `3.4`, `3.x`. I have Godot 3.5 so I will take the `3.5` branch.

Take note how the README of each branch is different. Keep this in mind for when you try to write in GDNative. The information in these READMEs is something you will not find in (the official documentation)[https://godot-doc.readthedocs.io/en/3.0/tutorials/plugins/gdnative/gdnative-cpp-example.html]. 


## Implementation
