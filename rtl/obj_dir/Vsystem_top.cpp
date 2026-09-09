// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vsystem_top__pch.h"

//============================================================
// Constructors

Vsystem_top::Vsystem_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vsystem_top__Syms(contextp(), _vcname__, this)}
    , m_evalLoop{*this, /*convergeLimit:*/ 10000}
    , clk{vlSymsp->TOP.clk}
    , rst{vlSymsp->TOP.rst}
    , state_out{vlSymsp->TOP.state_out}
    , pc_inc{vlSymsp->TOP.pc_inc}
    , write_reg_en{vlSymsp->TOP.write_reg_en}
    , reg_addr{vlSymsp->TOP.reg_addr}
    , high_b{vlSymsp->TOP.high_b}
    , reg_data{vlSymsp->TOP.reg_data}
    , reg_q_out{vlSymsp->TOP.reg_q_out}
    , rom_data{vlSymsp->TOP.rom_data}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vsystem_top::Vsystem_top(const char* _vcname__)
    : Vsystem_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vsystem_top::~Vsystem_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vsystem_top___024root___eval_debug_assertions(Vsystem_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD void Vsystem_top___024root___eval_static(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_initial(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD bool Vsystem_top___024root___eval_stl(Vsystem_top___024root* vlSelf, CData/*0:0*/ firstIteration);
void Vsystem_top___024root___eval_sample(Vsystem_top___024root* vlSelf);
bool Vsystem_top___024root___eval_ico(Vsystem_top___024root* vlSelf, CData/*0:0*/ firstIteration);
bool Vsystem_top___024root___eval_act(Vsystem_top___024root* vlSelf);
bool Vsystem_top___024root___eval_inact(Vsystem_top___024root* vlSelf);
bool Vsystem_top___024root___eval_nba(Vsystem_top___024root* vlSelf);
bool Vsystem_top___024root___eval_obs(Vsystem_top___024root* vlSelf);
bool Vsystem_top___024root___eval_react(Vsystem_top___024root* vlSelf);
void Vsystem_top___024root___eval_postponed(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_final(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__stl(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__ico(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__act(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__nba(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__obs(Vsystem_top___024root* vlSelf);
VL_ATTR_COLD void Vsystem_top___024root___eval_dump_triggers__react(Vsystem_top___024root* vlSelf);

void Vsystem_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsystem_top::eval_step\n"); );
    m_evalLoop.eval();
}

void Vsystem_top::evalBegin() {
#ifdef VL_DEBUG
    // Debug assertions
    Vsystem_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
}

void Vsystem_top::evalEnd() {
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

void Vsystem_top::evalStatic() {
    Vsystem_top___024root___eval_static(&(vlSymsp->TOP));
}

void Vsystem_top::evalInitial() {
    Vsystem_top___024root___eval_initial(&(vlSymsp->TOP));
}

bool Vsystem_top::evalStl(bool firstIteration) {
    return Vsystem_top___024root___eval_stl(&(vlSymsp->TOP), firstIteration);
}

void Vsystem_top::evalSample() {
    Vsystem_top___024root___eval_sample(&(vlSymsp->TOP));
}

bool Vsystem_top::evalIco(bool firstIteration) {
    return Vsystem_top___024root___eval_ico(&(vlSymsp->TOP), firstIteration);
}

bool Vsystem_top::evalAct() {
    return Vsystem_top___024root___eval_act(&(vlSymsp->TOP));
}

bool Vsystem_top::evalInact() {
    return Vsystem_top___024root___eval_inact(&(vlSymsp->TOP));
}

bool Vsystem_top::evalNba() {
    return Vsystem_top___024root___eval_nba(&(vlSymsp->TOP));
}

bool Vsystem_top::evalObs() {
    return Vsystem_top___024root___eval_obs(&(vlSymsp->TOP));
}

bool Vsystem_top::evalReact() {
    return Vsystem_top___024root___eval_react(&(vlSymsp->TOP));
}

void Vsystem_top::evalPostponed() {
    Vsystem_top___024root___eval_postponed(&(vlSymsp->TOP));
}

void Vsystem_top::evalFinal() {
    Vsystem_top___024root___eval_final(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersStl() {
    Vsystem_top___024root___eval_dump_triggers__stl(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersIco() {
    Vsystem_top___024root___eval_dump_triggers__ico(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersAct() {
    Vsystem_top___024root___eval_dump_triggers__act(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersNba() {
    Vsystem_top___024root___eval_dump_triggers__nba(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersObs() {
    Vsystem_top___024root___eval_dump_triggers__obs(&(vlSymsp->TOP));
}

VL_ATTR_COLD void Vsystem_top::dumpTriggersReact() {
    Vsystem_top___024root___eval_dump_triggers__react(&(vlSymsp->TOP));
}

//============================================================
// Events and timing
bool Vsystem_top::eventsPending() { return false; }

uint64_t Vsystem_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vsystem_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

VL_ATTR_COLD void Vsystem_top::final() {
    contextp()->executingFinal(true);
    evalFinal();
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vsystem_top::hierName() const { return vlSymsp->name(); }
const char* Vsystem_top::modelName() const { return "Vsystem_top"; }
unsigned Vsystem_top::threads() const { return 1; }
void Vsystem_top::prepareClone() const { contextp()->prepareClone(); }
void Vsystem_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
