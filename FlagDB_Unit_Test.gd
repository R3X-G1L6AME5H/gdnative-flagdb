extends Node

"""
	FlagDB Unit Tests
		by Nemo Czanderlitch/Nino Čandrlić
			@R3X-G1L       (godot assets store)
			R3X-G1L6AME5H  (github)
	A set of tests to confirm that the binary has been compiled correctly.
	In addition, it holds the examples of all methods available.

	size()          - returns array size
	capacity()      - returns the amount of flags the database can currently hold (array size * 32)
	tick(i)         - sets the i-th bit to True
			- when i > capacity() new memory/integer is automatically allocated
	bit_table(i, r) - prints out the integers at index i to index (i + r) in their binary representation 
	read(i)         - returns the boolean value representing the i-th bit
	clear(i)        - sets the i-th bit to False
	flip(i)         - inverts the state of the i-th bit
"""


func _ready():
	print("FlagDB: _init: create 2 ints (128 flags)")
	var fdb = load("res://FlagDB.gdns").new(2)
	print("FlagDB: _init: confirm the size is 2")
	
	print(">> Size is: ", fdb.size())
	# print(">> Capacity is: ", fdb.capacity())

	print("FlagDB: tick: 1st, 2nd, 4th, 8th, 16th, and 32nd bit")
	fdb.tick(0)
	fdb.tick(1)
	fdb.tick(3)
	fdb.tick(7)
	fdb.tick(15)
	fdb.tick(31)
	print("FlagDB: tick: Show the bit table and confirm that the bits are ticked properly")
	
	fdb.bit_table(0,1)
	print("FlagDB: tick: tick a bit out of range(720)")
	fdb.tick(720)
	print("FlagDB: tick: confirm 720th bit is true")
	print(">> 720th bit is: ", fdb.read(720))
	print("FlagDB: tick: confirm new integers were allocated")
	print(">> Size is: ", fdb.size())
	print()

	print("FlagDB: read: Check if 4th bit is ticked (should be 1)")
	print(">> 4th bit is: ", fdb.read(3))
	print("FlagDB: read: Check if 3rd bit is ticked (should be 0)")
	print(">> 3th bit is: ",fdb.read(2))
	print()

	print("FlagDB: clear: 4th bit")
	fdb.clear(3)
	print("FlagDB: clear: confirm 4th bit is cleared (should be 0)")
	print(">> 4th bit is: ",fdb.read(3))
	print()

	print("FlagDB: flip: 4th bit")
	fdb.flip(3)
	print("FlagDB: flip: confirm 4th bit is flipped (should be 1)")
	print(">> 4th bit is: ",fdb.read(3))
	print("FlagDB: flip: confirm flip has not messed up other bits")
	fdb.bit_table(0,1)
