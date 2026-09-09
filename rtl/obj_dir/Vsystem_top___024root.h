// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsystem_top.h for the primary calling header

#ifndef VERILATED_VSYSTEM_TOP___024ROOT_H_
#define VERILATED_VSYSTEM_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vsystem_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vsystem_top___024root final {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_OUT8(state_out,1,0);
    VL_OUT8(pc_inc,0,0);
    VL_OUT8(write_reg_en,0,0);
    VL_OUT8(reg_addr,3,0);
    VL_OUT8(high_b,0,0);
    VL_OUT8(reg_data,7,0);
    VL_OUT8(reg_q_out,7,0);
    CData/*0:0*/ system_top__DOT__w_pc_inc;
    CData/*0:0*/ system_top__DOT__w_write_reg_en;
    CData/*3:0*/ system_top__DOT__w_reg_addr;
    CData/*7:0*/ system_top__DOT__w_reg_data;
    CData/*0:0*/ system_top__DOT__w_high_b;
    CData/*1:0*/ system_top__DOT__cu_inst__DOT__state;
    CData/*7:0*/ system_top__DOT__cu_inst__DOT__ir;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst__0;
    CData/*0:0*/ __VicoDidInit;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__1;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst__1;
    VL_IN16(rom_data,15,0);
    SData/*15:0*/ __Vtrigprevexpr___TOP__rom_data__0;
    VlUnpacked<SData/*15:0*/, 16> system_top__DOT__rf_inst__DOT__regfile;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 2> __VicoTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vsystem_top__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vsystem_top___024root(Vsystem_top__Syms* symsp, const char* namep);
    ~Vsystem_top___024root();
    VL_UNCOPYABLE(Vsystem_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
