#!/usr/bin/python

import sys
import os

M_TYPE = "type"
M_PATH = "path"
M_LIBRARY = "library"
M_OBJECT = "object"
M_NAME = "name"
M_ADDRESS = "address"
M_SIZE = "size"
M_SIZE_BR = "size_before_relaxing"
M_FILL = "fill"



def print_last_data(f):
	global M_TYPE
	global M_PATH
	global M_LIBRARY
	global M_OBJECT
	global M_NAME
	global M_ADDRESS
	global M_SIZE
	global M_SIZE_BR
	global M_FILL

	if (M_PATH == "---"):
		return

	WS_PATH = os.getcwd()
	simple_path = M_PATH.replace(WS_PATH, "esp8266", 1)
	
	path_array = simple_path.split("(")
	if (len(path_array) >= 2):
		M_LIBRARY = path_array[0]
		M_OBJECT = path_array[1].replace(")","",1)
	
	f.write("%s,%s,%s,%s,%s,%s,%s,%s,%s\n"
			% (M_TYPE, M_PATH, M_LIBRARY, M_OBJECT,
				M_NAME, M_ADDRESS, M_SIZE, M_SIZE_BR, M_FILL))
	M_PATH = "---"
	M_NAME = "---"
	M_ADDRESS = "---"
	M_SIZE = "---"
	M_SIZE_BR = "---"
	M_FILL = "0"

def main():
	global M_TYPE
	global M_PATH
	global M_NAME
	global M_ADDRESS
	global M_SIZE
	global M_SIZE_BR
	global M_FILL

	START_FLAG = 0

	readfile = open(sys.argv[1], 'r')
	writefile = open(sys.argv[2], 'w')
	while True:
		line = readfile.readline()
		if not line: break

		if (START_FLAG == 0):
			if "END GROUP" in line :
				print_last_data(writefile)
				START_FLAG = 1
			else:
				continue

		if not line: continue
		if line.startswith(".debug"): break


		line_array = line.split()

		if len(line_array) < 1: continue
		


		if line_array[0][0] == '.':
			print_last_data(writefile)
			M_NAME = line_array[0]

			if (len(line_array) >= 4) and (line_array[3][0] == '/'):
				M_ADDRESS = line_array[1]
				M_SIZE = int(line_array[2], 16)
				M_SIZE_BR = int(line_array[2], 16)
				M_PATH = line_array[3]
			elif (len(line_array) >= 3) and (line_array[2].startswith("0x")):
				M_TYPE=line_array[0]
			continue

		if line_array[0] == "*fill*":
			M_FILL = int(line_array[2], 16)
			continue

		if line_array[0].startswith("0x0000"):
			if not line_array[1].startswith("0x"): continue
			if (len(line_array) >= 3 and line_array[2][0] != '/'): continue
			M_ADDRESS = line_array[0]
			M_SIZE = int(line_array[1], 16)
			M_SIZE_BR = int(line_array[1], 16)

			if (len(line_array) >= 3): M_PATH = line_array[2]
			else: M_PATH = "---"

		if "size before" in line:
			M_SIZE_BR = int(line_array[0], 16)

	readfile.close()
	writefile.close()

main()
