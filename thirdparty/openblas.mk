CXX = g++
CC = gcc         # used for sph2pipe
# CXX = clang++  # Uncomment these lines
# CC = clang     # to build with Clang.

all:
	@echo "use make -f Makefile.blas x64|arm|download"

fortran_opt = "BINARY=64"

x64:
	cd OpenBLAS; sed 's:# FCOMMON_OPT = -frecursive:FCOMMON_OPT = -frecursive:' < Makefile.rule >tmp && mv tmp Makefile.rule
	# $(MAKE) PREFIX=`pwd`/OpenBLAS/install FC=gfortran $(fortran_opt) DEBUG=1 USE_THREAD=1 NUM_THREADS=64 -C OpenBLAS all install
	cd OpenBLAS && make clean && rm -rf install_x64_threads
	$(MAKE) PREFIX=`pwd`/OpenBLAS/install_x64_threads FC=gfortran $(fortran_opt) DEBUG=1 USE_THREAD=1 -C OpenBLAS all install

download:
	# -git clone https://github.com/xianyi/OpenBLAS.git
	#-cd OpenBLAS; git pull
	-cd OpenBLAS;git checkout v0.3.5
