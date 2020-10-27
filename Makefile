#General
COMPILER = g++
COMPILE_FLAGS = -Wall -Wextra -pedantic -O2 -std=c++11
LINK_FLAGS = 

#File names
OBJ=$(addsuffix .o, $(basename $(wildcard *.cpp)))

#Eigen
EIGEN_DIRECTORY = /home/meta-chan/Project/_lib/eigen/Eigen

#wxWidgets
WX_COMPILE_FLAGS = -I /usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-3.0 -I/usr/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread
WX_LINK_FLAGS = -L/usr/lib/x86_64-linux-gnu -pthread   -lwx_gtk2u_xrc-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_qa-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0

%.o : %.cpp
	$(COMPILER) $^ $(COMPILE_FLAGS) $(WX_COMPILE_FLAGS) -I $(EIGEN_DIRECTORY) -c -o $@

p6.exe : $(OBJ)
	$(COMPILER) $^ $(LINK_FLAGS) $(WX_LINK_FLAGS) -o $@

all : p6.exe

doc :
	cd doc && doxygen

clean :
	rm -f *.exe

.PHONY : all doc clean
