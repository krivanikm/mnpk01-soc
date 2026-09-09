// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsystem_top.h for the primary calling header

#include "Vsystem_top__pch.h"

VL_ATTR_COLD void Vsystem_top___024root___eval_static(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_static\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst__0 = vlSelfRef.rst;
    vlSelfRef.__Vtrigprevexpr___TOP__rom_data__0 = vlSelfRef.rom_data;
    vlSelfRef.__Vtrigprevexpr___TOP__clk__1 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst__1 = vlSelfRef.rst;
}

VL_ATTR_COLD void Vsystem_top___024root___eval_initial(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_initial\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsystem_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

VL_ATTR_COLD bool Vsystem_top___024root___eval_stl(Vsystem_top___024root* vlSelf, CData/*0:0*/ firstIteration) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_stl\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(firstIteration)));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsystem_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vsystem_top___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        {
            // Inlined CFunc: _eval_body__stl
            if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
                {
                    // Inlined CFunc: _stl_sequent__TOP__0
                    vlSelfRef.state_out = vlSelfRef.system_top__DOT__cu_inst__DOT__state;
                    vlSelfRef.pc_inc = vlSelfRef.system_top__DOT__w_pc_inc;
                    vlSelfRef.write_reg_en = vlSelfRef.system_top__DOT__w_write_reg_en;
                    vlSelfRef.reg_addr = vlSelfRef.system_top__DOT__w_reg_addr;
                    if (vlSelfRef.system_top__DOT__w_high_b) {
                        vlSelfRef.high_b = 1U;
                        vlSelfRef.reg_data = vlSelfRef.system_top__DOT__w_reg_data;
                        vlSelfRef.reg_q_out = (0x000000ffU 
                                               & (vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                                                  [vlSelfRef.system_top__DOT__w_reg_addr] 
                                                  >> 8U));
                    } else {
                        vlSelfRef.high_b = 0U;
                        vlSelfRef.reg_data = vlSelfRef.system_top__DOT__w_reg_data;
                        vlSelfRef.reg_q_out = (0x000000ffU 
                                               & vlSelfRef.system_top__DOT__rf_inst__DOT__regfile
                                               [vlSelfRef.system_top__DOT__w_reg_addr]);
                    }
                }
            }
        }
    }
    return (__VstlExecute);
}

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__stl(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__stl\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    Vsystem_top___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__ico(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__ico\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    Vsystem_top___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__act(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__act\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    Vsystem_top___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
}

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__nba(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__nba\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
#ifdef VL_DEBUG
    Vsystem_top___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
}

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__obs(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__obs\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__react(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_dump_triggers__react\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vsystem_top___024root___eval_final(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___eval_final\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vsystem_top___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vsystem_top___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___trigger_anySet__stl\n"); );
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

bool Vsystem_top___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 2> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vsystem_top___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @( clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @( rst)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @( rom_data)\n");
    }
    if ((1U & (IData)(triggers[1U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 64 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vsystem_top___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vsystem_top___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(posedge rst)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsystem_top___024root___ctor_var_reset(Vsystem_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsystem_top___024root___ctor_var_reset\n"); );
    Vsystem_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->rom_data = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13914442448858418783ull);
    vlSelf->state_out = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16606792248447486632ull);
    vlSelf->pc_inc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8787069882628309898ull);
    vlSelf->write_reg_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11499200764953958383ull);
    vlSelf->reg_addr = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16295127466182514263ull);
    vlSelf->high_b = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16231671804743948332ull);
    vlSelf->reg_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2113356192903233237ull);
    vlSelf->reg_q_out = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5173923369622336639ull);
    vlSelf->system_top__DOT__w_pc_inc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17735426411231933341ull);
    vlSelf->system_top__DOT__w_write_reg_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3484101337411237540ull);
    vlSelf->system_top__DOT__w_reg_addr = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15003317620717101861ull);
    vlSelf->system_top__DOT__w_reg_data = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16708867325203186274ull);
    vlSelf->system_top__DOT__w_high_b = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2841037508159788966ull);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->system_top__DOT__rf_inst__DOT__regfile[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11896981863141623253ull);
    }
    vlSelf->system_top__DOT__cu_inst__DOT__state = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9163183380396394072ull);
    vlSelf->system_top__DOT__cu_inst__DOT__ir = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6278755198022964963ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rom_data__0 = 0;
    vlSelf->__VicoDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__1 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst__1 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
