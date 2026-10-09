# Simulácia vo Verilatore
#   make test        preloží a spustí všetky testy v tests/
#   make test V=1    s výpisom po taktoch
#   make alu         samostatný test ALU (hraničné + náhodné hodnoty)
#   make clean

SRC   = control-unit/controlunit.sv registers/register_file.sv alu/alu.sv system_top.sv
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

ALU_BIN = $(BUILD)/alu/Valu

$(ALU_BIN): alu/alu.sv alu/sim_main.cpp
	verilator --cc --exe --build --top alu --Mdir $(BUILD)/alu -Wno-fatal alu/alu.sv alu/sim_main.cpp

alu: $(ALU_BIN)
	./$(ALU_BIN)

clean:
	rm -rf $(BUILD) obj_dir

.PHONY: test alu clean
