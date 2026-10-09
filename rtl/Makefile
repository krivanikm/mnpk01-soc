# Simulácia vo Verilatore
#   make test        preloží a spustí všetky testy v tests/
#   make test V=1    s výpisom po taktoch
#   make alu         samostatný test ALU (hraničné + náhodné hodnoty)
#   make schematics  RTL schémy (Yosys) do docs/assets/schematics/
#   make clean

SRC   = control-unit/controlunit.sv registers/register_file.sv alu/alu.sv system_top.sv program-counter/programcounter.v
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

SCHEM = ../docs/assets/schematics
YOSYS_READ = read_verilog -sv $(SRC)

schematics: $(SRC)
	mkdir -p $(SCHEM)
	for m in system_top alu register_file controlunit; do \
	  yosys -q -p "$(YOSYS_READ); prep -top system_top; opt -full; clean -purge; show -format svg -stretch -width -prefix $(SCHEM)/$$m $$m"; \
	done
	yosys -q -p "$(YOSYS_READ); prep -top programcounter; show -format svg -stretch -width -prefix $(SCHEM)/programcounter programcounter"
	yosys -q -p "$(YOSYS_READ); prep -top system_top -flatten; opt -full; clean -purge; show -format svg -stretch -width -prefix $(SCHEM)/cpu_full system_top"
	rm -f $(SCHEM)/*.dot

clean:
	rm -rf $(BUILD) obj_dir

.PHONY: test alu schematics clean
