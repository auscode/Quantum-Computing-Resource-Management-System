#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testCreateQuantumCircuit3()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        TS_ASSERT(circuit != NULL);
        TS_ASSERT_EQUALS(circuit->numQubits, 2);
        TS_ASSERT_EQUALS(circuit->numGates, 0);
        TS_ASSERT(circuit->qubitStates != NULL);
        TS_ASSERT(circuit->gates != NULL);
        destroyQuantumCircuit(circuit);
    }
    void testCreateQuantumCircuit4()
    {
        QuantumCircuit *circuit = createQuantumCircuit(4);
        TS_ASSERT(circuit != NULL);
        TS_ASSERT_EQUALS(circuit->numQubits, 4);
        TS_ASSERT_EQUALS(circuit->numGates, 0);
        for (int i = 0; i < 4; i++)
        {
            TS_ASSERT_EQUALS(circuit->qubitStates[i], 0);
            TS_ASSERT_EQUALS(circuit->gates[i].qubitIndex, -1);
            TS_ASSERT_EQUALS(circuit->gates[i].gateType, SINGLE_QUBIT_GATE);
        }
        destroyQuantumCircuit(circuit);
    }
    void testApplySingleQubitGate2()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int qubitState = 0;
        qubitState = applySingleQubitGate(qubitState, TWO_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, -3);
        circuit->qubitStates[1] = 1;
        qubitState = applySingleQubitGate(qubitState, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, -2);
        qubitState = applySingleQubitGate(qubitState, TWO_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, -2);
        destroyQuantumCircuit(circuit);
    }

    /////////////////////////////////////////////////////
    void testAddSingleQubitGateWithInvalidIndex()
    {
        QuantumCircuit *qc = createQuantumCircuit(2);
        addGateToCircuit(qc, SINGLE_QUBIT_GATE, 2);
        TS_ASSERT_EQUALS(qc->numGates, 0);
        destroyQuantumCircuit(qc);
    }

    void testAddTwoQubitGateWithInvalidIndex()
    {
        QuantumCircuit *qc = createQuantumCircuit(1);
        addGateToCircuit(qc, TWO_QUBIT_GATE, 0);
        TS_ASSERT_EQUALS(qc->numGates, 0);
        destroyQuantumCircuit(qc);
    }

    void testAddInvalidGateType()
    {
        QuantumCircuit *qc = createQuantumCircuit(1);
        addGateToCircuit(qc, SWAP_GATE, 0);
        TS_ASSERT_EQUALS(qc->numGates, 0);
        destroyQuantumCircuit(qc);
    }
    /////////////////////////////////////////////////////
    void testDestroyNullCircuit()
    {
        QuantumCircuit *circuit = NULL;
        destroyQuantumCircuit(circuit);
        TS_ASSERT(circuit == NULL);
    }
    void testApplySingleQubitGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        circuit->qubitStates[0] = 0;
        addGateToCircuit(circuit, SINGLE_QUBIT_GATE, 0);
        int result = applySingleQubitGate(0, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        destroyQuantumCircuit(circuit);
    }
    void testAddGateToCircuit()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 0);
        addGateToCircuit(circuit, SINGLE_QUBIT_GATE, 1);
        TS_ASSERT(circuit->numGates != 2);
        destroyQuantumCircuit(circuit);
    }
    ////////////////////////////////////////////////////////
    void testApplySingleQubitGate_InvalidGateType_ReturnsError()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int result = applySingleQubitGate(0, CNOT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -3);
        destroyQuantumCircuit(circuit);
    }
    void testApplySingleQubitGate_InvalidCircuit_ReturnsError_Case1()
    {
        QuantumCircuit *circuit = NULL;
        int result = applySingleQubitGate(1, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -1);
    }
    void testApplySingleQubitGateInvalidCircuit()
    {
        QuantumCircuit *circuit = NULL;
        GateType gateType = SINGLE_QUBIT_GATE;
        int qubitIndex = 0;
        int qubitState = 0;
        int result = applySingleQubitGate(qubitState, gateType, circuit);
        TS_ASSERT_EQUALS(result, -1);
    }

    /////////////////////////////////////////////////////
    void testApplyCNOTGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 1;
        circuit->qubitStates[1] = 0;
        int result = applyTwoQubitGate(0, 1, CNOT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -3);
        TS_ASSERT_EQUALS(circuit->qubitStates[1], 0);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        destroyQuantumCircuit(circuit);
    }

    void testApplySwapGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 1;
        circuit->qubitStates[1] = 0;
        int result = applyTwoQubitGate(0, 1, SWAP_GATE, circuit);
        TS_ASSERT_EQUALS(result, -3);
        TS_ASSERT_EQUALS(circuit->qubitStates[1], 0);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        destroyQuantumCircuit(circuit);
    }
    void testInvalidGateType()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int result = applyTwoQubitGate(0, 1, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -3);
        destroyQuantumCircuit(circuit);
    }
    /////////////////////////////////////////////////////////////////////
    void testMeasureQubitNoMeasurementGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        circuit->qubitStates[0] = 0;
        int result = measureQubit(circuit, 0);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        TS_ASSERT_EQUALS(circuit->numGates, 0);
        destroyQuantumCircuit(circuit);
    }

    void testMeasureQubitMultipleGates()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 0;
        circuit->qubitStates[1] = 0;
        addGateToCircuit(circuit, SINGLE_QUBIT_GATE, 0);
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 0);
        addGateToCircuit(circuit, MEASUREMENT_GATE, 0);
        addGateToCircuit(circuit, SINGLE_QUBIT_GATE, 1);
        int result = measureQubit(circuit, 0);
        TS_ASSERT(result == 0 || result == 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], result);
        TS_ASSERT_EQUALS(circuit->numGates, 5);
        destroyQuantumCircuit(circuit);
    }
    void testMeasureQubitOnTwoQubitSystem()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 1;
        circuit->qubitStates[1] = 0;
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 0);
        int result = measureQubit(circuit, 0);
        TS_ASSERT(result == 0 || result == 1);
        TS_ASSERT_EQUALS(circuit->numGates, 2);
        destroyQuantumCircuit(circuit);
    }
};