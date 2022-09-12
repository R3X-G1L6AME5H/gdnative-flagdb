# FlagDB; commpressing for the fun of it

## The point?
`Flagpole.gd` was a script I whiped up out of boredom one day; it was of little importance. However, recently I have been trying to get another script working using GDNative. I was struck by just how little help there is online. Therefore I decided to translate this one off script to GDNative in the name of the Godot Comunity. If you are here strictly for a GDNative guide, proceed here.

## Prologue
I was building a game. This game needed to have a story manager. So I built one in a fashion simmilar to Ink. However, I noticed how Ink would use strings as flags. Inspired by witnessing a man cram an 8K, 60fps video onto a Nintendo 64 cartrige ([Inigo Quilez](https://www.youtube.com/c/InigoQuilez), if I remember correctly) I thought to myself: "this is unacceptable".

A flag is a boolean value; single bool should be sufficient. These bools would then be placed in an array, and would be directly accessed through their index. It was a genius plan; until a shocking revelation.

Booleans are not actually stored in memory as a single bit. Some sources say they are 1 byte large, while others claim that booleans are actually an integer, 4 bytes large... Unacceptable; that is too many bits wasted.

There had to be a way out of this conundrum. Wait a minute, bytes are just a bunch of bits. What if we stored each individual flag in a bit of an integer. A 32 bit integer could then hold 32 flags, in a single register. Sometimes my genius frigntens me.

## The Logic

## Setting up the environment

## Implementation
