# The LDACS Abstract Radio models the radio communications for accurate LDACS air-to-air simulation.
# Copyright (C) 2024  Musab Ahmed, Konrad Fuger, Koojana Kuladinithi, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
