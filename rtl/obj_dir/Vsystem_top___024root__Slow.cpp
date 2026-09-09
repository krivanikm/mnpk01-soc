// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsystem_top.h for the primary calling header

#include "Vsystem_top__pch.h"

void Vsystem_top___024root___ctor_var_reset(Vsystem_top___024root* vlSelf);

Vsystem_top___024root::Vsystem_top___024root(Vsystem_top__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vsystem_top___024root___ctor_var_reset(this);
}

void Vsystem_top___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vsystem_top___024root::~Vsystem_top___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
