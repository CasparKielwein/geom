CMAKE_MINIMUM_REQUIRED(VERSION 3.0.)

ADD_EXECUTABLE(geom_benchmark
    	src/benchmark.cpp
)
set_property(TARGET geom_benchmark PROPERTY CXX_STANDARD 14)

#TARGET_COMPILE_OPTIONS( geom_benchmark
#	PUBLIC "-march=native" )

TARGET_LINK_LIBRARIES( geom_benchmark PUBLIC  
	benchmark
	pthread
	)