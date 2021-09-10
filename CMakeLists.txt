# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (smsemu VERSION 0.1)

# Set variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

# Search needed libraries
find_package(SDL2 REQUIRED)

# Create a sources variable with a link to all cpp files to compile
set(SMSEMU_SOURCES
		src/Bios.cpp                                                             
		src/BitplaneShifter.cpp                                                  
		src/Cartridge.cpp                                                        
		src/CircularBuffer.cpp                                                   
		src/Controller.cpp                                                       
		src/DebugConsole.cpp                                                     
		src/filt.cpp                                                             
		src/FrameBuffer.cpp                                                      
		src/main.cpp                                                             
		src/Mapper.cpp                                                           
		src/MapperCodemaster.cpp                                                 
		src/MapperSega.cpp                                                       
		src/MemoryManager.cpp                                                    
		src/NoiseGen.cpp                                                         
		src/PSG.cpp                                                              
		src/SegaEmu.cpp                                                          
		src/SMS.cpp                                                              
		src/ToneGen.cpp                                                          
		src/VDP.cpp                                                              
		src/Z80A.cpp  
)

# Add an executable with the above sources
add_executable(smsemu ${SMSEMU_SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(smsemu
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
		${SDL2_INCLUDE_DIRS}
)

# Set libraries to be included
target_link_libraries(smsemu ${SDL2_LIBRARIES})
