// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsystem_top.h for the primary calling header

#include "Vsystem_top__pch.h"

void Vsystem_top___024root___eval_sample(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_sample\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vsystem_top___024root___eval_ico(Vsystem_top___024root* vlSelf, CData/*0:0*/ firstIteration) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_ico\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[1U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[1U]) 
                                     | (IData)((IData)(firstIteration)));
    {
        // Inlined CFunc: _eval_triggers_vec__ico
        vlSelfRef.__VicoTriggered[0U] = (QData)((IData)(
                                                        ((((IData)(vlSelfRef.rom_data) 
                                                           != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rom_data__0)) 
                                                          << 2U) 
                                                         | ((((IData)(vlSelfRef.rst) 
                                                              != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst__0)) 
                                                             << 1U) 
                                                            | ((IData)(vlSelfRef.clk) 
                                                               != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))))));
        vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
        vlSelfRef.__Vtrigprevexpr___TOP__rst__0 = vlSelfRef.rst;
        vlSelfRef.__Vtrigprevexpr___TOP__rom_data__0 
            = vlSelfRef.rom_data;
        if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VicoDidInit)))))) {
            vlSelfRef.__VicoDidInit = 1U;
            vlSelfRef.__VicoTriggered[0U] = (1ULL | vlSelfRef.__VicoTriggered[0U]);
            vlSelfRef.__VicoTriggered[0U] = (2ULL | vlSelfRef.__VicoTriggered[0U]);
            vlSelfRef.__VicoTriggered[0U] = (4ULL | vlSelfRef.__VicoTriggered[0U]);
        }
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsystem_top___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
    return (0U);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
void Vsystem_top___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in);

bool Vsystem_top___024root___eval_act(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_act\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    {
        // Inlined CFunc: _eval_triggers_vec__act
        vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                        ((((IData)(vlSelfRef.rst) 
                                                           & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst__1))) 
                                                          << 1U) 
                                                         | ((IData)(vlSelfRef.clk) 
                                                            & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__1))))));
        vlSelfRef.__Vtrigprevexpr___TOP__clk__1 = vlSelfRef.clk;
        vlSelfRef.__Vtrigprevexpr___TOP__rst__1 = vlSelfRef.rst;
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsystem_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vsystem_top___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

bool Vsystem_top___024root___eval_inact(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_inact\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

bool Vsystem_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);
void Vsystem_top___024root___nba_sequent__TOP__0(Vsystem_top___024root* vlSelf);
void Vsystem_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out);

bool Vsystem_top___024root___eval_nba(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_nba\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vsystem_top___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        {
            // Inlined CFunc: _eval_body__nba
            if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
                Vsystem_top___024root___nba_sequent__TOP__0(vlSelf);
            }
        }
        Vsystem_top___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

bool Vsystem_top___024root___eval_obs(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_obs\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

bool Vsystem_top___024root___eval_react(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_react\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    return (0U);
}

void Vsystem_top___024root___eval_postponed(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_postponed\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

bool Vsystem_top___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((2U > n));
    return (0U);
}

bool Vsystem_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vsystem_top___024root___nba_sequent__TOP__0(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___nba_sequent__TOP__0\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ __Vdly__system_top__DOT__cu_inst__DOT__state;
    __Vdly__system_top__DOT__cu_inst__DOT__state = 0;
    CData/*7:0*/ __Vdly__system_top__DOT__cu_inst__DOT__ir;
    __Vdly__system_top__DOT__cu_inst__DOT__ir = 0;
    CData/*0:0*/ __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v0;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v0 = 0;
    CData/*7:0*/ __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v16;
    __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v16 = 0;
    CData/*3:0*/ __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v16;
    __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v16 = 0;
    CData/*0:0*/ __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v16;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v16 = 0;
    CData/*7:0*/ __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v17;
    __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v17 = 0;
    CData/*3:0*/ __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v17;
    __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v17 = 0;
    CData/*0:0*/ __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v17;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v17 = 0;
    // Body
    __Vdly__system_top__DOT__cu_inst__DOT__ir = vlSelfRef.system_top__DOT__cu_inst__DOT__ir;
    __Vdly__system_top__DOT__cu_inst__DOT__state = vlSelfRef.system_top__DOT__cu_inst__DOT__state;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v0 = 0U;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v16 = 0U;
    __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v17 = 0U;
    if (vlSelfRef.rst) {
        __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v0 = 1U;
        __Vdly__system_top__DOT__cu_inst__DOT__state = 0U;
        __Vdly__system_top__DOT__cu_inst__DOT__ir = 0U;
        vlSelfRef.system_top__DOT__w_pc_inc = 0U;
        vlSelfRef.system_top__DOT__w_write_reg_en = 0U;
        vlSelfRef.system_top__DOT__w_reg_addr = 0U;
        vlSelfRef.system_top__DOT__w_reg_data = 0U;
        vlSelfRef.system_top__DOT__w_high_b = 0U;
    } else {
        if (vlSelfRef.system_top__DOT__w_write_reg_en) {
            if (vlSelfRef.system_top__DOT__w_high_b) {
                __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v16 
                    = vlSelfRef.system_top__DOT__w_reg_data;
                __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v16 
                    = vlSelfRef.system_top__DOT__w_reg_addr;
                __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v16 = 1U;
            } else {
                __VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v17 
                    = vlSelfRef.system_top__DOT__w_reg_data;
                __VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v17 
                    = vlSelfRef.system_top__DOT__w_reg_addr;
                __VdlySet__system_top__DOT__rf_inst__DOT__regfile__v17 = 1U;
            }
        }
        vlSelfRef.system_top__DOT__w_pc_inc = 0U;
        vlSelfRef.system_top__DOT__w_write_reg_en = 0U;
        vlSelfRef.system_top__DOT__w_high_b = 0U;
        if ((0U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__state))) {
            __Vdly__system_top__DOT__cu_inst__DOT__ir 
                = (0x000000ffU & ((IData)(vlSelfRef.rom_data) 
                                  >> 8U));
            vlSelfRef.system_top__DOT__w_pc_inc = 1U;
            __Vdly__system_top__DOT__cu_inst__DOT__state = 1U;
        } else if ((1U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__state))) {
            if (((1U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__ir)) 
                 || (3U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__ir)))) {
                __Vdly__system_top__DOT__cu_inst__DOT__state = 2U;
            } else {
                if ((2U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__ir))) {
                    vlSelfRef.system_top__DOT__w_write_reg_en = 1U;
                    vlSelfRef.system_top__DOT__w_reg_addr 
                        = (0x0000000fU & ((IData)(vlSelfRef.rom_data) 
                                          >> 4U));
                }
                __Vdly__system_top__DOT__cu_inst__DOT__state = 0U;
            }
        } else if ((2U == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__state))) {
            vlSelfRef.system_top__DOT__w_write_reg_en = 1U;
            vlSelfRef.system_top__DOT__w_reg_addr = 
                (0x0000000fU & ((IData)(vlSelfRef.rom_data) 
                                >> 0x0cU));
            vlSelfRef.system_top__DOT__w_reg_data = 
                (0x000000ffU & ((IData)(vlSelfRef.rom_data) 
                                >> 4U));
            vlSelfRef.system_top__DOT__w_pc_inc = 1U;
            __Vdly__system_top__DOT__cu_inst__DOT__state = 0U;
            vlSelfRef.system_top__DOT__w_high_b = (3U 
                                                   == (IData)(vlSelfRef.system_top__DOT__cu_inst__DOT__ir));
        } else {
            __Vdly__system_top__DOT__cu_inst__DOT__state = 0U;
        }
    }
    if (__VdlySet__system_top__DOT__rf_inst__DOT__regfile__v0) {
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[0U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[1U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[2U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[3U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[4U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[5U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[6U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[7U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[8U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[9U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[10U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[11U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[12U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[13U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[14U] = 0U;
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[15U] = 0U;
    }
    if (__VdlySet__system_top__DOT__rf_inst__DOT__regfile__v16) {
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[__VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v16] 
            = ((0x00ffU & vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                [__VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v16]) 
               | ((SData)((IData)(__VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v16)) 
                  << 8U));
    }
    if (__VdlySet__system_top__DOT__rf_inst__DOT__regfile__v17) {
        vlSelfRef.system_top__DOT__rf_inst__DOT__regfile[__VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v17] 
            = ((0xff00U & vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                [__VdlyDim0__system_top__DOT__rf_inst__DOT__regfile__v17]) 
               | (SData)(__VdlyVal__system_top__DOT__rf_inst__DOT__regfile__v17));
    }
    vlSelfRef.system_top__DOT__cu_inst__DOT__ir = __Vdly__system_top__DOT__cu_inst__DOT__ir;
    vlSelfRef.system_top__DOT__cu_inst__DOT__state 
        = __Vdly__system_top__DOT__cu_inst__DOT__state;
    vlSelfRef.state_out = vlSelfRef.system_top__DOT__cu_inst__DOT__state;
    vlSelfRef.pc_inc = vlSelfRef.system_top__DOT__w_pc_inc;
    vlSelfRef.write_reg_en = vlSelfRef.system_top__DOT__w_write_reg_en;
    vlSelfRef.reg_data = vlSelfRef.system_top__DOT__w_reg_data;
    vlSelfRef.reg_addr = vlSelfRef.system_top__DOT__w_reg_addr;
    if (vlSelfRef.system_top__DOT__w_high_b) {
        vlSelfRef.high_b = 1U;
        vlSelfRef.reg_q_out = (0x000000ffU & (vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                                              [vlSelfRef.system_top__DOT__w_reg_addr] 
                                              >> 8U));
    } else {
        vlSelfRef.high_b = 0U;
        vlSelfRef.reg_q_out = (0x000000ffU & vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                               [vlSelfRef.system_top__DOT__w_reg_addr]);
    }
}

void Vsystem_top___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

void Vsystem_top___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

#ifdef VL_DEBUG
void Vsystem_top___024root___eval_debug_assertions(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_debug_assertions\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst & 0xfeU)))) {
        Verilated::overWidthError("rst");
    }
}
#endif  // VL_DEBUG
