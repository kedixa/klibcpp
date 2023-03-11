CMAKE := $(shell if which cmake3>/dev/null ; then echo cmake3; else echo cmake; fi;)
CPACK := $(shell if which cpack3>/dev/null ; then echo cpack3; else echo cpack; fi;)
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
KEDIXA_SRC_DIR := $(ROOT_DIR)
TEST_SRC_DIR := $(ROOT_DIR)/test
KEDIXA_BUILD_DIR := build.kedixa.cmake
TEST_BUILD_DIR := build.test.cmake
RM := rm

.PHONY: build tests check clean rpm

build:
	mkdir -p $(KEDIXA_BUILD_DIR)
	cd $(KEDIXA_BUILD_DIR) && $(CMAKE) $(KEDIXA_SRC_DIR)
	make -C $(KEDIXA_BUILD_DIR) -f Makefile

tests: build
	mkdir -p $(TEST_BUILD_DIR)
	cd $(TEST_BUILD_DIR) && $(CMAKE) -D kedixa_DIR=$(ROOT_DIR)/$(KEDIXA_BUILD_DIR) $(TEST_SRC_DIR)
	make -C $(TEST_BUILD_DIR) -f Makefile

check: tests
	$(TEST_BUILD_DIR)/test_bigint
	$(TEST_BUILD_DIR)/test_multiarray
	$(TEST_BUILD_DIR)/test_rational
	$(TEST_BUILD_DIR)/test_unsigned_bigint
	$(TEST_BUILD_DIR)/test_timer

rpm: build
	cd $(KEDIXA_BUILD_DIR) && $(CPACK)

clean:
	$(RM) -rf $(KEDIXA_BUILD_DIR)
	$(RM) -rf $(TEST_BUILD_DIR)
