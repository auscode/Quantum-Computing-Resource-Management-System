#include "bitmap.h"

/*
This function creates a new QuantumCircuit object with a specified number of qubits.
It allocates memory for the circuit structure and initializes the qubitStates and gates arrays.
If any memory allocation fails, the function returns NULL. The qubitStates array is initialized to 0,
and the gates array is initialized with default values.
*/
QuantumCircuit *createQuantumCircuit(int numQubits)
{
    // Allocate memory for QuantumCircuit struct
    QuantumCircuit *circuit = (QuantumCircuit *)malloc(sizeof(QuantumCircuit));
    if (circuit == NULL)
    {
        // Memory allocation failed
        return NULL;
    }
    // Set numQubits and numGates
    circuit->numQubits = numQubits;
    circuit->numGates = 0;
    // Allocate memory for qubitStates array
    circuit->qubitStates = (int *)malloc(numQubits * sizeof(int));
    if (circuit->qubitStates == NULL)
    {
        // Memory allocation failed
        free(circuit);
        return NULL;
    }
    // Initialize qubitStates to 0
    for (int i = 0; i < numQubits; i++)
    {
        circuit->qubitStates[i] = 0;
    }
    // Allocate memory for gates array
    circuit->gates = (Gate *)malloc(numQubits * sizeof(Gate));
    if (circuit->gates == NULL)
    {
        // Memory allocation failed
        free(circuit->qubitStates);
        free(circuit);
        return NULL;
    }
    // Initialize gates to default values
    for (int i = 0; i < numQubits; i++)
    {
        circuit->gates[i].qubitIndex = -1;
        circuit->gates[i].gateType = SINGLE_QUBIT_GATE;
    }
    // Return the initialized QuantumCircuit struct
    return circuit;
}

/*
This function adds a gate to a quantum circuit. It first checks for errors such as a null circuit pointer,
an invalid qubit index, or a gate type that is not supported. If the gate is a single qubit or measurement gate,
it adds the gate to the circuit's list of gates. If it is a two-qubit gate, it checks that the circuit has at
least two qubits and then adds the gate to two consecutive qubits in the circuit's gate list.
*/
void addGateToCircuit(QuantumCircuit *circuit, GateType gateType, int qubitIndex)
{
    if (circuit == NULL)
    {
        // Error: Circuit pointer is NULL
        return;
    }
    if (qubitIndex < 0 || qubitIndex >= circuit->numQubits)
    {
        // Error: Invalid qubit index
        return;
    }
    if (gateType == SINGLE_QUBIT_GATE || gateType == MEASUREMENT_GATE)
    {
        // Add single qubit gate or measurement gate to circuit
        Gate gate = {qubitIndex, gateType};
        circuit->gates[circuit->numGates++] = gate;
    }
    else if (gateType == TWO_QUBIT_GATE)
    {
        // Add two qubit gate to circuit
        if (circuit->numQubits < 2)
        {
            // Error: Circuit has less than 2 qubits
            return;
        }
        int qubitIndex2 = (qubitIndex + 1) % circuit->numQubits;
        Gate gate1 = {qubitIndex, gateType};
        Gate gate2 = {qubitIndex2, gateType};
        circuit->gates[circuit->numGates++] = gate1;
        circuit->gates[circuit->numGates++] = gate2;
    }
    // Error: Invalid gate type
    else
    {
        return;
    }
}

/*
This function deallocates the memory allocated to a QuantumCircuit struct, including any
dynamically allocated memory inside its qubitStates and gates arrays. It first checks
if the circuit pointer is NULL, and returns early if it is. It then frees the qubitStates
and gates arrays if they are not NULL, and frees any dynamically allocated memory inside the Gate struct.
Finally, it frees the QuantumCircuit struct itself and sets its pointer to NULL.
*/
void destroyQuantumCircuit(QuantumCircuit *circuit)
{
    if (circuit == NULL)
    {
        return; // return early if circuit is already NULL
    }
    // free qubitStates array if it's not NULL
    if (circuit->qubitStates != NULL)
    {
        free(circuit->qubitStates);
        circuit->qubitStates = NULL;
    }
    // free gates array if it's not NULL
    if (circuit->gates != NULL)
    {
        for (int i = 0; i < circuit->numGates; i++)
        {
            // free any dynamically allocated memory inside the Gate struct
            switch (circuit->gates[i].gateType)
            {
            case SINGLE_QUBIT_GATE:
                // no dynamically allocated memory inside SingleQubitGate struct
                break;
            case TWO_QUBIT_GATE:
                // dynamically allocated memory inside TwoQubitGate struct: none in this implementation
                break;
            case MEASUREMENT_GATE:
                // no dynamically allocated memory inside MeasurementGate struct
                break;
            default:
                // invalid gateType, ignore
                break;
            }
        }
        free(circuit->gates);
        circuit->gates = NULL;
    }
    circuit->numQubits = 0;
    circuit->numGates = 0;
    free(circuit);
    circuit = NULL;
}

/*
This function applies a single qubit gate operation to a quantum circuit.
It first checks if the circuit is valid and if the qubit state is within the range of the circuit's qubits.
If the gate type is not a single qubit gate, it returns an error code. If the gate is a single qubit gate,
it toggles the qubit state and records the gate operation in the circuit. Finally, it returns the updated qubit state.
*/
int applySingleQubitGate(int qubitState, GateType gateType, QuantumCircuit *circuit)
{
    if (circuit == NULL)
    {
        return -1;
    }
    if (qubitState < 0 || qubitState >= circuit->numQubits)
    {
        return -2;
    }
    if (gateType != SINGLE_QUBIT_GATE)
    {
        return -3;
    }
    switch (gateType)
    {
    case SINGLE_QUBIT_GATE:
        if (circuit->qubitStates[qubitState] == 0)
        {
            circuit->qubitStates[qubitState] = 1;
        }
        else
        {
            circuit->qubitStates[qubitState] = 0;
        }
        break;
    default:
        return -4;
    }
    circuit->gates[circuit->numGates].qubitIndex = qubitState;
    circuit->gates[circuit->numGates].gateType = gateType;
    circuit->numGates++;
    return circuit->qubitStates[qubitState];
}

/*
This function applies a two-qubit gate (either a CNOT or SWAP gate) to two qubits in a quantum circuit.
It checks for errors and returns a status code indicating success or failure.
If successful, it updates the state of the qubits in the quantum circuit according to the specified two-qubit gate.
*/
int applyTwoQubitGate(int qubitState1, int qubitState2, GateType gateType, QuantumCircuit *circuit)
{
    if (circuit == NULL)
    {
        return -1;
    }
    if (qubitState1 < 0 || qubitState1 >= circuit->numQubits || qubitState2 < 0 || qubitState2 >= circuit->numQubits)
    {
        return -2;
    }
    if (gateType != TWO_QUBIT_GATE)
    {
        return -3;
    }
    int temp;
    switch (gateType)
    {
    case CNOT_GATE:
        if (circuit->qubitStates[qubitState1] == 1)
        {
            circuit->qubitStates[qubitState2] = !circuit->qubitStates[qubitState2];
        }
        break;
    case SWAP_GATE:
        temp = circuit->qubitStates[qubitState1];
        circuit->qubitStates[qubitState1] = circuit->qubitStates[qubitState2];
        circuit->qubitStates[qubitState2] = temp;
        break;
    default:
        return -4;
        break;
    }
    return 0;
}

/*
The function measureQubit() measures a quantum bit (qubit) in a given quantum circuit and
returns the measurement result. It first checks if the qubit index is valid, applies a measurement
gate to the qubit, simulates the measurement process, updates the qubit state based on the measurement result,
and then removes the measurement gate from the circuit. The function also returns an error code if the qubit
index is invalid or if the last gate in the circuit is not the expected measurement gate.
*/
// Measure the specified qubit in the given quantum circuit
// Returns the measurement result (0 or 1)
int measureQubit(QuantumCircuit *circuit, int qubitIndex)
{
    // Make sure qubitIndex is valid
    if (qubitIndex < 0 || qubitIndex >= circuit->numQubits)
    {
        return -1; // Invalid qubit index
    }
    // Apply measurement gate to qubit
    addGateToCircuit(circuit, MEASUREMENT_GATE, qubitIndex);
    // Simulate measurement process
    int measurementResult = rand() % 2; // Generate random 0 or 1
    // Update qubit state based on measurement result
    circuit->qubitStates[qubitIndex] = measurementResult;
    // Remove measurement gate from circuit
    circuit->numGates--; // Decrement gate count
    Gate lastGate = circuit->gates[circuit->numGates];
    if (lastGate.gateType != MEASUREMENT_GATE || lastGate.qubitIndex != qubitIndex)
    {
        return -2; // Last gate in circuit is not the expected measurement gate
    }
    // Return measurement result
    return measurementResult;
}
