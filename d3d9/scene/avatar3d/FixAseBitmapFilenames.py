import sys
import os.path

def fix_ase_bitmap_filenames(filename):
	"""
	Replace absolute bitmap filenames in the ase file with filenames relative to the ase file.
	The bitmap files must be in the same directory as the ase file or in a subdirectory below.
	"""
	ase = filename.lower()
	try:
		f = file(ase, 'r')
	except IOError:
		print 'Unable to open ASE file for reading!\n'
	else:
		lines = f.readlines();
		f.close()

		head, tail = os.path.split(ase) # split up the basepath and name
		i = 0
		n = 0
		for l in lines:
			if l.find('*NODE_TM') > 0:	# read past material section
				break
			if l.find('*BITMAP ') > 0:
				strs = l.split('\"')	# extract the bitmap
				name = strs[1].lower()	# filename in quotes
				if name.find(head) == 0:
					lines[i] = strs[0] + '\"' + strs[1][len(head) + 1:] + '\"' + strs[2]
					n = n + 1
			i = i + 1
		if n >= 1:
			try:
				f = file(filename, 'w')
			except:
				print 'Unable to open ASE file for writing!\n'
			else:
				f.writelines(lines)
				f.close()

if __name__ == '__main__':
	try:
		pyt_filename = sys.argv[0]
		ase_filename = sys.argv[1]
	except IndexError:
		print 'Please enter a valid 3DS-Max ASE file name!'
	else:
		if os.path.isabs(ase_filename): # if already an absolute file name
			fix_ase_bitmap_filenames(ase_filename)
		else:
			curr_working_dir1 = os.getcwd()
			curr_working_dir2 = os.path.split(pyt_filename)[0] # need directory python script is in
			filename1 = os.path.join(curr_working_dir1, ase_filename)
			filename2 = os.path.join(curr_working_dir2, ase_filename)
			if os.path.exists(filename1):
				fix_ase_bitmap_filenames(filename1)
			else:
				fix_ase_bitmap_filenames(filename2)
