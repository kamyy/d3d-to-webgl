import os
import sys

def makeshaders(cwd = os.getcwd(), build = 'D'):
	"""Compare time stamps of source HLSL files in the specified directory
	to target .h files and recompile if they are out of date."""

	from os.path import isfile
	from os.path import splitext
	from os.path import getmtime
	from os.path import exists
	from os.path import join
	
	build = build == 'D' and 'D' or 'R' # must be either D or R

	cwd = os.path.normcase(cwd) # replace forward slashes with 2 back slashes
	old = os.getcwd(); # store so it can be restored later
	os.chdir(cwd) # set current working directory
	
	# create a list of vertex and pixel shader HLSL source files
	filelist = [f for f in os.listdir(cwd) if isfile(join(cwd, f))]
	vshfiles = [f for f in filelist if splitext(f)[1].lower() == '.vsh']
	pshfiles = [f for f in filelist if splitext(f)[1].lower() == '.psh']

	# attempt to locate HLSL compiler in default DX9 install location or trust it's in the env path
	fxccmd = exists('C:\\DXSDK\\Bin\\DXUtils\\fxc.exe') and 'C:\\DXSDK\\Bin\\DXUtils\\fxc.exe ' or 'fxc.exe '

	# dbg options are vertex/pixel shader v2.0, disable optimizations, enable debugging info, output header
	# rel options are vertex/pixel shader v2.0, output header
	vshopt = build == 'D' and '/Tvs_2_0 /Od /Zi /nologo /Fh' or '/Tvs_2_0 /nologo /Fh'
	pshopt = build == 'D' and '/Tps_2_0 /Od /Zi /nologo /Fh' or '/Tps_2_0 /nologo /Fh'
	
	for src in vshfiles:
		tgt = splitext(src)[0] + '_VS_' + build + '.h' # form tgt filename using base and _VS_D.h or _VS_R.h
		if not exists(tgt) or getmtime(tgt) < getmtime(src): # if source newer than target
			os.system(fxccmd + vshopt + tgt + ' ' + src)
				
	for src in pshfiles:
		tgt = splitext(src)[0] + '_PS_' + build + '.h' # form tgt filename using base and _PS_D.h or _PS_R.h
		if not exists(tgt) or getmtime(tgt) < getmtime(src): # if source newer than target
			os.system(fxccmd + pshopt + tgt + ' ' + src)

	os.chdir(old) # restore old current working dir
			
if __name__ == '__main__':
	if len(sys.argv) == 1:
		# make both debug and release versions using current working dir
		makeshaders(build = 'D')
		makeshaders(build = 'R')
		
	elif len(sys.argv) == 2:
		# make either debug or release using current working dir
		if sys.argv[1] == "-debug":
			makeshaders(build = 'D')
		elif sys.argv[1] == "-release":
			makeshaders(build = 'R')
			
	elif len(sys.argv) == 3:		
		# make either debug or release using input dir
		if sys.argv[1] == "-debug":
			makeshaders(sys.argv[2], 'D')
		elif sys.argv[1] == "-release":
			makeshaders(sys.argv[2], 'R')
		