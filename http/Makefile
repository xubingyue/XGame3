PROJECT_DIR=../..
OBJ_DIR = $(PROJECT_DIR)/obj/http
TARGET=$(PROJECT_DIR)/lib/libhttp.a
INC_PATH = -I$(PROJECT_DIR)/src -I$(PROJECT_DIR)/src/lib
VPATH = .:detail
SVR_SRC = $(foreach dir,$(subst :, ,$(VPATH)),$(wildcard $(dir)/*.cpp)) 
SVR_OBJ = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o, ${SVR_SRC}))
SVR_PATH = $(addprefix $(OBJ_DIR)/,$(subst :, ,$(VPATH)))
DEPS = $(SVR_OBJ:.o=.d)

CXX = g++
DEBUG=TRUE
ifeq ($(DEBUG),FALSE)
CPPFLAGS = -DNDEBUG -Wall -O2 $(INC_PATH)
else
CPPFLAGS = -g -pg -Wall $(INC_PATH)
endif
MKDIR = @mkdir -p
RM = rm -rf
AR = ar -crus
MAKE_OBJ_PATH := $(shell mkdir -p $(OBJ_DIR) $(SVR_PATH))

all : $(TARGET)

$(TARGET) : $(SVR_OBJ)
	${AR} -o $@ $(SVR_OBJ)

$(OBJ_DIR)/%.o : %.cpp $(OBJ_DIR)/%.d
	$(CXX) -o $@ -c $< $(CPPFLAGS)

$(OBJ_DIR)/%.d:%.cpp
	$(CXX) $< $(CPPFLAGS) -MM -MD -MQ'$(OBJ_DIR)/$*.o' -o $@

clean:
	$(RM) $(SVR_OBJ) $(TARGET)

-include $(DEPS)

.PHONY:all clean FORCE

FORCE:
