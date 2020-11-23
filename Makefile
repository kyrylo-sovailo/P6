#General
COMPILER = g++
COMPILE_FLAGS = -Wall -Wextra -O2 -std=c++11

#Eigen
EIGEN_DIRECTORY = /mnt/E/Project/_lib/eigen/Eigen

#Google test
GOOGLE_LINK_FLAGS = -lgtest -lpthread

#wxWidgets
WX_COMPILE_FLAGS = -D P6_FILE_WXWIDGETS $(shell wx-config --cxxflags)
WX_LINK_FLAGS = $(shell wx-config --libs)

#Search path
VPATH = tmp:source

%.o : %.cpp
	if [ ! -d tmp ]; then mkdir tmp; fi
	$(COMPILER) source/$(^F) $(COMPILE_FLAGS) $(WX_COMPILE_FLAGS) -I $(EIGEN_DIRECTORY) -c -o tmp/$(@F)

P6.exe : p6_app.o p6_common.o p6_construction.o p6_file.o p6_force_bar.o p6_frame.o p6_linear_material.o p6_main_panel.o p6_material.o p6_material_bar.o p6_menubar.o p6_mouse.o p6_move_bar.o p6_node_bar.o p6_nonlinear_material.o p6_side_panel.o p6_stick_bar.o p6_toolbar.o
	$(COMPILER) $(addprefix tmp/,$(^F)) $(WX_LINK_FLAGS) -o $(@F)

P6_test.exe : p6_common.o p6_construction.o p6_file.o p6_linear_material.o p6_material.o p6_nonlinear_material.o p6_test.o
	$(COMPILER) $(addprefix tmp/,$(^F)) $(WX_LINK_FLAGS) $(GOOGLE_LINK_FLAGS) -o $(@F)

all : p6.exe

doc :
	doxygen

clean :
	rm -rf documentation
	rm -rf tmp
	rm -f *.exe

run : P6.exe
	./P6.exe

test : P6_test.exe
	./P6_test.exe

.PHONY : all doc clean run test
