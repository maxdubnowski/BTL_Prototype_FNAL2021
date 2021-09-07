All:
	rootcint -f Dict_analysis.cpp include/DRSWAVE.h include/mcp.h include/testbeam.h LinkDef.h 
	g++ analysis.cc src/testbeam.cxx src/mcp.cxx src/DRSWAVE.cxx Dict_analysis.cpp -Iinclude `root-config --cflags --glibs`
