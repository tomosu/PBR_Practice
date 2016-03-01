#######################################################################
#                                                                     #
#                        Makefile ( GNU make )                        #
#                                                                     #
#######################################################################
CPPSRC += main.cpp
#CPPSRC += PhotonMap.cpp
CPPSRC += PhotonMapThread.cpp
CPPSRC += Shape.cpp
CPPSRC += Scene.cpp
CPPSRC += Material.cpp
CPPSRC += CornellBox.cpp
CPPSRC += BunnyBox.cpp
CPPSRC += Bvh.cpp
CPPSRC += BBox.cpp
CPPSRC += Light.cpp
CPPSRC += TriangleMesh.cpp


CCOPT  = -O2 -g -lpthread -fopenmp -std=c++11 -lstdc++
LDOPT  = -pthread -lgomp
TARGET = test

INC_DIR += -I./
INC_DIR += -I../
INC_DIR += -I../Geometric
INC_DIR += -I../Material
INC_DIR += -I../Shape
INC_DIR += -I../include
INC_DIR += -I../Types
INC_DIR += -I../RandomNumber
INC_DIR += -I../Quaternion
INC_DIR += -I../Camera
INC_DIR += -I../Image
INC_DIR += -I../Bvh
INC_DIR += -I../PhotonInteraction
INC_DIR += -I../KDTree
INC_DIR += -I../Light
INC_DIR += -I../Scene
INC_DIR += -I../TriangleMesh
INC_DIR += -I../Utils


vpath %.cpp ./
vpath %.cpp ../
vpath %.cpp ../Shape
vpath %.cpp ../Material
vpath %.cpp ../Bvh
vpath %.cpp ../PhotonInteraction
vpath %.cpp ../Light
vpath %.cpp ../Scene
vpath %.cpp ../TriangleMesh



#######################################################################
#                                                                     #
#                        Makefile ( GNU make )                        #
#                                                                     #
#######################################################################
TOP	?=

CPPSRC  ?=
CSRC    ?=
ASRC    ?=

#######################################################################
#                                                                     #
#         TOOL                                                        #
#                                                                     #
#######################################################################
CROSS	?=

AS       =$(CROSS)as
ASOPT   ?=

CC       =$(CROSS)g++
CCOPT   += -D__LINUX__

LD       =$(CROSS)g++
LDOPT	?=


#######################################################################
#                                                                     #
#         PATH                                                        #
#                                                                     #
#######################################################################
INC_DIR += -I$(TOP)/.


OBJDIR ?= obj

#######################################################################
#                           Standard processing                       #
#######################################################################
CPPOBJS= ${patsubst %.cpp, ${OBJDIR}/%.o, ${notdir ${CPPSRC}}}
COBJS  = ${patsubst %.c, ${OBJDIR}/%.o, ${notdir ${CSRC}}}
AOBJS  = ${patsubst %.S, ${OBJDIR}/%.o, ${notdir ${ASRC}}}

OBJS   = ${CPPOBJS} ${COBJS} ${AOBJS}

all: make_kernel ${OBJDIR} ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LIB_DIR} ${LIBS} ${LDOPT}

${OBJDIR}:
	@if [ ! -d ${OBJDIR} ]; then \
		echo ";; mkdir ${OBJDIR}"; mkdir ${OBJDIR}; \
	fi

$(CPPOBJS) : ${OBJDIR}/%.o : %.cpp
	${CC} ${CCOPT} ${INC_DIR} ${ADD_INC_DIR} -c $< -o $@

$(COBJS) : ${OBJDIR}/%.o : %.c
	${CC} ${CCOPT} ${INC_DIR} ${ADD_INC_DIR} -c $< -o $@

$(AOBJS) : ${OBJDIR}/%.o : %.S
	${CC} ${CCOPT} ${INC_DIR} ${ADD_INC_DIR} -c $< -o $@

.cpp.o: 
	${CC} ${CCOPT} ${INC_DIR} -c $<	-o ${OBJDIR}/${notdir $@}

.c.o: 
	${CC} ${CCOPT} ${INC_DIR} -c $<	-o ${OBJDIR}/${notdir $@}

.S.o: 
	${CC} ${CCOPT} ${INC_DIR} -c $<	-o ${OBJDIR}/${notdir $@}

clean:	make_kernel_clean
	rm -f .Depend ${OBJS} ${TARGET}
	rm -rf ${OBJDIR}


#######################################################################
#                            Depend processing                        #
#######################################################################

make_kernel: $(KERNEL_DIRS)
	@for d in $(KERNEL_DIRS); \
	do \
	(cd $$d && \
	echo "" && \
	echo "*******************************************************" && \
	echo "* making in $(CURRENT_DIR)/$$d" && \
	echo "*******************************************************" && \
	$(MAKE) $(MFLAGS) "CURRENT_DIR=$(CURRENT_DIR)/$$d" \
	"DEBUG=$(DEBUG)" all); \
	if [ $$? -ne 0 ]; then \
		echo "!!!!!! ERROR !!!!!!" ; exit 1; \
	fi \
	done

make_kernel_clean: $(KERNEL_DIRS)
	@for d in $(KERNEL_DIRS); \
	do \
	(cd $$d && \
	echo "" && \
	echo "*******************************************************" && \
	echo "* making in $(CURRENT_DIR)/$$d" && \
	echo "*******************************************************" && \
	$(MAKE) $(MFLAGS) "CURRENT_DIR=$(CURRENT_DIR)/$$d" \
	"DEBUG=$(DEBUG)" clean); \
	done

#######################################################################
#                            Depend processing                        #
#######################################################################
.Depend: # ${SRCS}
	@if [ ! -d ${OBJDIR} ]; then \
		echo ";; mkdir ${OBJDIR}"; mkdir ${OBJDIR}; \
	fi
	${CC} ${CCOPT} ${INC_DIR} -MM ${CSRC} ${ASRC} > .Depend

-include .Depend
