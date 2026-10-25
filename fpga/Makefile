export PATH := $(HOME)/tools/oss-cad-suite/bin:$(PATH)
R = ../rtl
SRC = top.sv $(R)/system_top.sv $(R)/control-unit/controlunit.sv $(R)/registers/register_file.sv $(R)/alu/alu.sv $(R)/program-counter/programcounter.v $(R)/memory/data_ram.sv $(R)/uart/uart_tx.sv
DEVICE = GW1NR-LV9QN88PC6/I5

all: top.fs

top.json: $(SRC) prog.hex
	yosys -q -p "read_verilog -sv $(SRC); synth_gowin -top top -json top.json"

top.pnr.json: top.json tang9k.cst
	nextpnr-himbaechel --json top.json --write top.pnr.json --device $(DEVICE) --vopt family=GW1N-9C --vopt cst=tang9k.cst

top.fs: top.pnr.json
	gowin_pack -d GW1N-9C -o top.fs top.pnr.json

flash: top.fs
	openFPGALoader -b tangnano9k top.fs

clean:
	rm -f top.json top.pnr.json top.fs
