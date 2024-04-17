###
# This file is responsible for providing the full toolchain of creating result graphs.
# Each target is responsible for one scenario, and should
# - start the simulation(s)
# - parse the resulting .sca and .vec files
# - and plot the parsed results into .pdfs
# 
# Before each target, some brief comments should describe what the scenario is about.
# Also, variables can be created that specify parameters of the respective target. These can be shared among several targets (where e.g. each target runs some sub-scenario).
# Naming these variables should follow a SCENARIO_VARNAME syntax.
###
build-debug:
	echo -e "\nLDACSAbstractRadio"; \
	cd ../src; opp_makemake -f --deep -O out -KINET4_PROJ=../../inet4 -DINET_IMPORT -I../../inet4 -I. -I../../inet4/src -L../../inet4/src -lINET_dbg; make -j8 MODE=debug; cd ../simulations; 

build-release:	
	echo -e "\nLDACSAbstractRadio"; \
	cd ../src; opp_makemake -f --deep -KINET_PROJ=../../inet4 -DINET_IMPORT -I../../inet4/src -L../../inet4/src -lINET; make -j8 MODE=release; cd ../simulations; 

# GLOBAL VARIABLES  can be shared among all targets
# default binary in RELEASE mode
SIM_BINARY = ../out/gcc-release/src/ldacs-abstract-radio
SIM_BINARY_DBG = ../out/gcc-debug/src/ldacs-abstract-radio_dbg
# include .ned files
INCL = -n ../src:./:../../inet4/src/