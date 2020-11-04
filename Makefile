CXX=g++
CXXFLAGS=-O3 -g -Wall -fPIC -std=c++1y `root-config --cflags` -I${PYTHIA8_INCLUDE_DIR} -I${FASTJET_INCLUDE_DIR} -I${ATLAS_PATH}/include -I${ROOT_UTILS_PATH}/include
LDFLAGS=-Wl,-rpath `root-config --libdir --glibs` -L${ATLAS_PATH}/lib -L${ROOT_UTILS_PATH}/lib -L${PYTHIA8_LIBRARY} -L${FASTJET_LIBRARY} -lGlobalParams -lUtilities -lfastjet -lAtlasUtils -lAtlasStyle -ldl

reqdirs= bin outputs logs errors Plots

directories:
	mkdir -p ${reqdirs}

all: directories z_gen photon_gen z_analyze photon_analyze generate_photon_weights plotter

z_gen: src/z_gen.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) $(PYTHIA8_DIR)/lib/libpythia8.a -o bin/z_gen

photon_gen: src/photon_gen.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) $(PYTHIA8_DIR)/lib/libpythia8.a -o bin/photon_gen

z_analyze: src/z_analyze.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o bin/z_analyze

photon_analyze: src/photon_analyze.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o bin/photon_analyze

generate_photon_weights: src/generate_photon_weights.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o bin/generate_photon_weights

plotter: src/plotter.cxx
	$(CXX) $< $(CXXFLAGS) $(LDFLAGS) -o bin/plotter

clean:
	rm -rf bin logs errors
