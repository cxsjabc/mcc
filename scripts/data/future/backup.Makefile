
# Checking a folder existing on Windows or not, is sometimes strange, please forgive me for
# the following checking mechanism!
ifeq ($(OS), Windows_NT)
BUILD_OUT_EXIST = $(shell dir > /dev/nul 2>&1 $(BUILD_OBJ_DIR) && echo "exist" || echo "noexist")
else
BUILD_OUT_EXIST = $(shell if [ -d $(BUILD_OBJ_DIR) ]; then echo "exist"; else echo "noexist"; fi)
endif

ifeq ("$(BUILD_OUT_EXIST)", "noexist")
$(shell mkdir $(BUILD_OBJ_DIR) $(BUILD_OBJ_DIR)/$(SRC_DIR))
endif

# 32bit built
C_FLAGS += -m32