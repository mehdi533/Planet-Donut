OUT = planetdonut
CXX = g++
CXXFLAGS = -Wall -std=c++11 
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = projet.o gui.o simulation.o gisement.o base.o geomod.o robot.o message.o \
  graphic.o

all: $(OUT)

projet.o: projet.cc gui.h simulation.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

gui.o: gui.cc gui.h constantes.h simulation.h graphic.h geomod.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
graphic.o: graphic.cc graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
geomod.o: geomod.cc geomod.h constantes.h graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o: simulation.cc simulation.h base.h geomod.h message.h \
  constantes.h gisement.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
base.o: base.cc base.h geomod.h robot.h constantes.h message.h gisement.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

robot.o: robot.cc robot.h constantes.h geomod.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

gisement.o: gisement.cc gisement.h geomod.h message.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "Cleaning compilation files"
	@rm *.o $(OUT) *.cc~ *.h~
