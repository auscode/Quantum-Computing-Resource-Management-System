#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    SINGLE_QUBIT_GATE,
    TWO_QUBIT_GATE,
    MEASUREMENT_GATE,
    CNOT_GATE,
    SWAP_GATE
} GateType;

typedef struct
{
    int qubitIndex;
    GateType gateType;
} Gate;

typedef struct
{
    int numQubits;
    int *qubitStates;
    Gate *gates;
    int numGates;
} QuantumCircuit;

QuantumCircuit *createQuantumCircuit(int numQubits);

void addGateToCircuit(QuantumCircuit *circuit, GateType gateType, int qubitIndex);

void destroyQuantumCircuit(QuantumCircuit *circuit);

int applySingleQubitGate(int qubitState, GateType gateType, QuantumCircuit *circuit);

int applyTwoQubitGate(int qubitState1, int qubitState2, GateType gateType, QuantumCircuit *circuit);

int measureQubit(QuantumCircuit *circuit, int qubitIndex);
