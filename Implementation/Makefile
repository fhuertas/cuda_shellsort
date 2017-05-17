# COMPILADORES
CC = nvcc
CCC = nvcc
CXX = nvcc

# SOURCES
#manual
#SOURCES=openClUtilities main
#automatic
SOURCES=$(patsubst %.c,%,$(wildcard *.c)) $(patsubst %.cpp,%,$(wildcard *.cpp))

# OPCIONES-DESARROLLO
# COMPILACION
OPT-COMP-DEV = $(OPTS) -g $(OPT-COMP)
# CONSTRUCCIÓN
OPT-BUILD-DEV = $(OPTS) $(OPT-BUILD) -g -lOpenCL
OPT-BUILD-DEV-POST = -o 




# OPCIONES GENERALES
BUILD-PATH = build
# COMPILACION
OPT-COMP = --compiler-options "-std=gnu99" -c 
# CONSTRUCCION
OPT-BUILD = -lOpenCL
OUTPUTFILE = main
OPTS = 


# AUTOVARS
OBJS=$(patsubst %,%.o,$(SOURCES))
SOURCES_C_=$(patsubst %,%.c %.cpp,$(SOURCES))
SOURCES_C=$(filter $(wildcard *.cpp), $(SOURCES_C_)) $(filter $(wildcard *.c), $(SOURCES_C_)) 

#build-dir

#VARIABLES DE ENTORNO
export PATH:=			/usr/local/cuda/bin:${PATH}
export LD_LIBRARY_PATH:=	/usr/local/cuda/lib:/usr/local/cuda/lib64

build:  compile .pre-build .build .post-build
# build: .build
	

compile: .pre-compile .compile .post-compile
#compile: .compile 

clean: 
	rm $(wildcard *.o)
	rm $(OUTPUTFILE)


%.o: %.c
#	cd $(BUILD-PATH);$(CC) $(OPT-COMP-DEV) ../$@
	$(CC) $(OPT-COMP-DEV) $< 
%.o: %.cpp
#	cd $(BUILD-PATH);$(CCC) $(OPT-COMP-DEV) ../$@
	$(CCC) $(OPT-COMP-DEV) $<

.pre-compile:
	@echo "pre compilación"
	@echo "-----------------"
.compile: $(OBJS)
#$(ALL-SOURCE)

.post-compile: 
	@echo "post compilación"
	@echo "-----------------"

.pre-build: 
	@echo "pre construcción"
	@echo "-----------------"

.build: $(OUTPUTFILE)

	
$(OUTPUTFILE): $(SOURCES_C)
	$(CC) $(OPT-BUILD-DEV) $(OBJS) -o $(OUTPUTFILE)
.post-build: 
	@echo "post construcción"
	@echo "-----------------"
	

.clean:
.pre-clean:
.post-clean:
	
exec:
	./$(OUTPUTFILE)

test: 
	@echo $(SOURCES)
	@echo $(SOURCES_C)
	@echo $${PATH}
	@echo $${LD_LIBRARY_PATH}
	
