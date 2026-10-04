# Simulácia vo Verilatore
#   make test        preloží a spustí všetky testy v tests/
#   make test V=1    s výpisom po taktoch
#   make clean

SRC   = control-unit/controlunit.sv registers/register_file.sv system_top.sv
TB    = tb_full_system.cpp
BUILD = build
BIN   = $(BUILD)/Vsystem_top
TESTS = $(wildcard tests/*.txt)

VFLAGS = --cc --exe --build --top system_top --Mdir $(BUILD) --public-flat-rw \
         -Wno-EOFNEWLINE -Wno-UNUSEDSIGNAL -Wno-UNDRIVEN -Wno-DECLFILENAME -Wno-PINMISSING

$(BIN): $(SRC) $(TB)
	verilator $(VFLAGS) $(SRC) $(TB)

test: $(BIN)
	./$(BIN) $(if $(V),-v) $(TESTS)

clean:
	rm -rf $(BUILD) obj_dir

.PHONY: test clean
