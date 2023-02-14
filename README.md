# Arm SysReg specification to header (asrs2h)

## purpose

Arm has over 1000 system registers that may be needed in various situations.
They are accessed through MRS and MSR instructions.

The specifications of those registers can be found here:
https://developer.arm.com/downloads/-/exploration-tools

The scripts in the project transform the specifications into a C header.

## usage

Assuming custom path covers the generate script and the current directory
is where the specification files are

    generate 'AArch64-*' > sys_regs.h


# Discussion on Arm system registers

They are encoded based on combination of Op0, Op1, Op2, CRn, CRm fields.

Those fields are place in an instruction encoding in a certain way and in 
different way when placed in ESR (Exception Syndrome Register).

The generated "tag" in the C header is modeled after instruction encoding fields placement.

## confidence

There are some unclear elements in the specifications:

### AArch64-dbgbcrn.xml

reg array descriptions states that there can be 64 (0-63) debug registers.
While accessor instructions refer to 16 (0-16) registers.

It may be the case that there can be 64 registers architecturally but the current 
instructions can only access 16 of them. 

I haven't found authoritative answer.

### AArch64-allint.xml

I don't know what to think about the CRm encoding.
Currently this accessor is excluded (bad hack) from generation.

```xml
        <access_mechanism accessor="MSRimmediate ALLINT" type="SystemAccessor">
            <encoding>
            <access_instruction>MSR ALLINT, #&lt;imm&gt;</access_instruction>
                
                <enc n="op0" v="0b00"/>
                
                <enc n="op1" v="0b001"/>
                
                <enc n="CRn" v="0b0100"/>
                
                <enc n="CRm" v="0b000x"/>
                
                <enc n="op2" v="0b000"/>
            </encoding>
        </access_mechanism
```

### Redundant specificaitons

Several specification files lke AArch64-zcr_el2.xml actually describe both EL1 and EL2 access registers.
The scripts are currently ignoring them but don't know why those accessors are described
