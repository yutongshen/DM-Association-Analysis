SHELL         := bash
CC            := g++
INCLUDE_DIR   := ./include/
LIB_DIR       := ./lib/
CARR_DIR      := ./CArray/
CARR          := CArray
INT_LIST      := IntList
HASH_TREE_DIR := ./HashTree/
HASH_TREE     := HashTree
FP_TREE_DIR   := ./FPTree/
FP_TREE       := FPTree
BUILD_DIR     := ./buile/

# Compile flag
CFLAG         := -I${INCLUDE_DIR}

# Source code path
CARR_SRC      := ${CARR_DIR}${CARR}.cpp
INT_LIST_SRC  := ${CARR_DIR}${INT_LIST}.cpp
HASH_TREE_SRC := ${HASH_TREE_DIR}${HASH_TREE}.cpp
FP_TREE_SRC   := ${FP_TREE_DIR}${FP_TREE}.cpp

# Object file path
CARR_OBJ      := ${BUILD_DIR}${CARR}.o
INT_LIST_OBJ  := ${BUILD_DIR}${INT_LIST}.o
HASH_TREE_OBJ := ${BUILD_DIR}${HASH_TREE}.o
FP_TREE_OBJ   := ${BUILD_DIR}${FP_TREE}.o

# Library path
CARR_LIB      := ${LIB_DIR}lib${CARR}.so 
HASH_TREE_LIB := ${LIB_DIR}lib${HASH_TREE}.so 
FP_TREE_LIB   := ${LIB_DIR}lib${FP_TREE}.so

all: clean ${BUILD_DIR} libcarr libhash libfp

# Compile and run
run: clean ${BUILD_DIR} libcarr libhash libfp
	python main.py

# Build temporary directory
${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

# Compile library
libcarr: ${CARR_OBJ} ${INT_LIST_OBJ}
	${CC} -shared ${CARR_OBJ} ${INT_LIST_OBJ} -o ${CARR_LIB}

${CARR_OBJ}: ${CARR_SRC}
	${CC} -fPIC -c ${CARR_SRC} -o ${CARR_OBJ} ${CFLAG}

${INT_LIST_OBJ}: ${INT_LIST_SRC}
	${CC} -fPIC -c ${INT_LIST_SRC} -o ${INT_LIST_OBJ} ${CFLAG}

libhash: ${HASH_TREE_OBJ}
	${CC} -shared ${HASH_TREE_OBJ} ${INT_LIST_OBJ} -o ${HASH_TREE_LIB}

${HASH_TREE_OBJ}: ${HASH_TREE_SRC}
	${CC} -fPIC -c ${HASH_TREE_SRC} -o ${HASH_TREE_OBJ} ${CFLAG}

libfp: ${FP_TREE_OBJ}
	${CC} -shared ${FP_TREE_OBJ} ${INT_LIST_OBJ} -o ${FP_TREE_LIB}

${FP_TREE_OBJ}: ${FP_TREE_SRC}
	${CC} -fPIC -c ${FP_TREE_SRC} -o ${FP_TREE_OBJ} ${CFLAG}

# Clean all file
clean:
	rm -rf ${BUILD_DIR}*.o ${LIB_DIR}*.so
