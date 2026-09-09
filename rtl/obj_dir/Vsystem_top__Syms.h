// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VSYSTEM_TOP__SYMS_H_
#define VERILATED_VSYSTEM_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vsystem_top.h"

// INCLUDE MODULE CLASSES
#include "Vsystem_top___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vsystem_top__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vsystem_top* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool& __Vm_didInit;

    // MODULE INSTANCE STATE
    Vsystem_top___024root          TOP;

    // CONSTRUCTORS
    Vsystem_top__Syms(VerilatedContext* contextp, const char* namep, Vsystem_top* modelp);
    ~Vsystem_top__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
