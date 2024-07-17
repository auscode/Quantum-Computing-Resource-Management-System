#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testCreateQuantumCircuit()
    {
        QuantumCircuit *circuit = createQuantumCircuit(3);
        TS_ASSERT(circuit != NULL);
        TS_ASSERT_EQUALS(circuit->numQubits, 3);
        TS_ASSERT_EQUALS(circuit->numGates, 0);
        TS_ASSERT(circuit->qubitStates != NULL);
        TS_ASSERT(circuit->gates != NULL);
        destroyQuantumCircuit(circuit);
    }
    void testCreateQuantumCircuit2()
    {
        QuantumCircuit *circuit = createQuantumCircuit(5);
        TS_ASSERT(circuit != NULL);
        TS_ASSERT_EQUALS(circuit->numQubits, 5);
        TS_ASSERT_EQUALS(circuit->numGates, 0);
        for (int i = 0; i < 5; i++)
        {
            TS_ASSERT_EQUALS(circuit->qubitStates[i], 0);
            TS_ASSERT_EQUALS(circuit->gates[i].qubitIndex, -1);
            TS_ASSERT_EQUALS(circuit->gates[i].gateType, SINGLE_QUBIT_GATE);
        }
        destroyQuantumCircuit(circuit);
    }

    void testApplySingleQubitGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        int qubitState = 0;
        qubitState = applySingleQubitGate(qubitState, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, 1);
        circuit->qubitStates[0] = 1;
        qubitState = applySingleQubitGate(qubitState, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, -2);
        qubitState = applySingleQubitGate(qubitState, TWO_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(qubitState, -2);
        destroyQuantumCircuit(circuit);
    }
    ///////////////////////////////////////////////////////////////////////////

    void testAddGateToCircuit()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        addGateToCircuit(circuit, CNOT_GATE, 0);
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 1);
        TS_ASSERT_EQUALS(circuit->numGates, 2);
        TS_ASSERT(circuit->gates[0].gateType != CNOT_GATE);
        TS_ASSERT_EQUALS(circuit->gates[0].qubitIndex, 1);
        TS_ASSERT_EQUALS(circuit->gates[1].gateType, TWO_QUBIT_GATE);
        TS_ASSERT_EQUALS(circuit->gates[1].qubitIndex, 0);
        destroyQuantumCircuit(circuit);
    }
    void testAddSingleQubitGate()
    {
        QuantumCircuit *qc = createQuantumCircuit(1);
        addGateToCircuit(qc, SINGLE_QUBIT_GATE, 0);
        TS_ASSERT_EQUALS(qc->numGates, 1);
        TS_ASSERT_EQUALS(qc->gates[0].gateType, SINGLE_QUBIT_GATE);
        TS_ASSERT_EQUALS(qc->gates[0].qubitIndex, 0);
        destroyQuantumCircuit(qc);
    }

    void testAddTwoQubitGate()
    {
        QuantumCircuit *qc = createQuantumCircuit(2);
        addGateToCircuit(qc, TWO_QUBIT_GATE, 0);
        TS_ASSERT_EQUALS(qc->numGates, 2);
        TS_ASSERT_EQUALS(qc->gates[0].gateType, TWO_QUBIT_GATE);
        TS_ASSERT_EQUALS(qc->gates[0].qubitIndex, 0);
        TS_ASSERT_EQUALS(qc->gates[1].gateType, TWO_QUBIT_GATE);
        TS_ASSERT_EQUALS(qc->gates[1].qubitIndex, 1);
        destroyQuantumCircuit(qc);
    }

//////////////////////////////////////////////////////////////////////////////////////
    void testDestroySingleQubitGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        circuit->qubitStates[0] = 1;
        int result = applySingleQubitGate(1, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -2);
        destroyQuantumCircuit(circuit);
    }
    void testDestroyCircuitWithTwoQubitGates()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 0);
        addGateToCircuit(circuit, TWO_QUBIT_GATE, 1);
        destroyQuantumCircuit(circuit);
        TS_ASSERT(circuit != NULL);
    }

    void testDestroyCircuitWithMeasurementGates()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        addGateToCircuit(circuit, MEASUREMENT_GATE, 0);
        addGateToCircuit(circuit, MEASUREMENT_GATE, 1);
        destroyQuantumCircuit(circuit);
        TS_ASSERT(circuit != NULL);
    }
    ////////////////////////////////////////////////////////////////////////
    void testApplySingleQubitGate_ValidInputs_Success()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 0;
        circuit->qubitStates[1] = 1;
        int result = applySingleQubitGate(0, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[1], 1);
        TS_ASSERT_EQUALS(circuit->numGates, 1);
        TS_ASSERT_EQUALS(circuit->gates[0].qubitIndex, 0);
        TS_ASSERT_EQUALS(circuit->gates[0].gateType, SINGLE_QUBIT_GATE);
        destroyQuantumCircuit(circuit);
    }

    void testApplySingleQubitGate_InvalidCircuit_ReturnsError()
    {
        QuantumCircuit *circuit = NULL;
        int result = applySingleQubitGate(0, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -1);
    }

    void testApplySingleQubitGate_InvalidQubitIndex_ReturnsError()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int result = applySingleQubitGate(2, SINGLE_QUBIT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -2);
        destroyQuantumCircuit(circuit);
    }


    /////////////////////////////////////////////////////////////////

    void testApplyTwoQubitGate()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        circuit->qubitStates[0] = 1;
        circuit->qubitStates[1] = 1;
        int result = applyTwoQubitGate(1, 1, CNOT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -3);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[1], 1);
        destroyQuantumCircuit(circuit);
    }

    void testInvalidCircuit()
    {
        QuantumCircuit *circuit = NULL;
        int result = applyTwoQubitGate(0, 1, CNOT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -1);
    }

    void testInvalidQubitIndex()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int result = applyTwoQubitGate(-1, 1, CNOT_GATE, circuit);
        TS_ASSERT_EQUALS(result, -2);
        destroyQuantumCircuit(circuit);
    }

    /////////////////////////////////////////////////

    void testMeasureQubit()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        circuit->qubitStates[0] = 1;
        int result = measureQubit(circuit, 0);
        TS_ASSERT(result == 0 || result == 1);
        destroyQuantumCircuit(circuit);
    }
    void testMeasureQubitInvalidIndex()
    {
        QuantumCircuit *circuit = createQuantumCircuit(2);
        int result = measureQubit(circuit, -1);
        TS_ASSERT_EQUALS(result, -1);
        result = measureQubit(circuit, 2);
        TS_ASSERT_EQUALS(result, -1);
        destroyQuantumCircuit(circuit);
    }

    void testMeasureQubitSuccess()
    {
        QuantumCircuit *circuit = createQuantumCircuit(1);
        circuit->qubitStates[0] = 0;
        addGateToCircuit(circuit, SINGLE_QUBIT_GATE, 0);
        int result = measureQubit(circuit, 0);
        TS_ASSERT(result == 0 || result == 1);
        TS_ASSERT_EQUALS(circuit->qubitStates[0], result);
        TS_ASSERT_EQUALS(circuit->numGates, 1);
        destroyQuantumCircuit(circuit);
    }

};