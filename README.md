# Arm SysReg specification to header (asrs2h)

## Purpose

Arm has over 1000 system registers that may be needed in various situations.
They are accessed through MRS and MSR instructions.

The specifications of those registers can be found here:
https://developer.arm.com/downloads/-/exploration-tools

The scripts in the project transform the specifications into a C header.

## Usage

### Scripts
Assuming custom path covers the generate script and the current directory
is where the specification files are

    generate 'AArch64-*' > sys_regs.h
    info AArch64-vbar-el1.xml

### headers

The values of "__AARCH64_SYS_REG(a, b, c, d, e)" can be used to compare 
to instruction and ESR encodings (use the proper positioning of all fields).

Should you wan to use them to communicate with KVM, you would need to complement
the value with KVM "tags": 

    #define KVM_SYSREG_FROM_AARCH64(x)	((x) | KVM_REG_ARM64 | KVM_REG_ARM64_SYSREG | (KVM_REG_SIZE_U64))

# Capbilities

The info script extracts information from a single specification:
short name, long name and accessors.

The specifications describe either a single register (VBAR_EL1) or a register template (DBGBCR<m>_EL1).
    
For selected accessors (limited to MRS MRRS instructions and trying to avoid dupplicates - read below),
a register or a set of registers are defined.

For single register spec Op0, Op1, Op2, CRn, CRm fields are combined into a 16bits value that can be
compared to instruction encoding.
#define AARCH64_VBAR_EL1                __AARCH64_SYS_REG(0b11, 0b000, 0b1100, 0b0000, 0b000)

For register templates, such as DBGBCR<m>_EL1, the specification can define bit fields:

```xml
                <encoding>
                
                <acc_array var="m">
                    <acc_array_range>0-15</acc_array_range>
                </acc_array>
                <access_instruction>MSR DBGBCR&lt;m&gt;_EL1, &lt;Xt&gt;</access_instruction>
                
                <enc n="op0" v="0b10"/>
                
                <enc n="op1" v="0b000"/>
                
                <enc n="CRn" v="0b0000"/>
                
                <enc n="CRm" v="m[3:0]"/>
                
                <enc n="op2" v="0b101"/>
            </encoding>
.```
    
the "info" script interprets the bit field definition with the range to generate:
    #define AARCH64_DBGBCR_0_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0000, 0b101)
    #define AARCH64_DBGBCR_1_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0001, 0b101)
    #define AARCH64_DBGBCR_2_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0010, 0b101)
    #define AARCH64_DBGBCR_3_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0011, 0b101)
    #define AARCH64_DBGBCR_4_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0100, 0b101)
    #define AARCH64_DBGBCR_5_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0101, 0b101)                                                                                                             
    #define AARCH64_DBGBCR_6_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0110, 0b101)                                                                                                             
    #define AARCH64_DBGBCR_7_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0111, 0b101)                                                                                                             
    #define AARCH64_DBGBCR_8_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1000, 0b101)                                                                                                             
    #define AARCH64_DBGBCR_9_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1001, 0b101)                                                                                                             
    #define AARCH64_DBGBVR_0_EL1            __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b0000, 0b100)                                                                                                             
    #define AARCH64_DBGBVR_10_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1010, 0b100)                                                                                                             
    #define AARCH64_DBGBVR_11_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1011, 0b100)                                                                                                             
    #define AARCH64_DBGBVR_12_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1100, 0b100)                                                                                                             
    #define AARCH64_DBGBVR_13_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1101, 0b100)                                                                                                             
    #define AARCH64_DBGBVR_14_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1110, 0b100)                                                                                                             
    #define AARCH64_DBGBCR_10_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1010, 0b101)
    #define AARCH64_DBGBCR_11_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1011, 0b101)
    #define AARCH64_DBGBCR_12_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1100, 0b101)
    #define AARCH64_DBGBCR_13_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1101, 0b101)
    #define AARCH64_DBGBCR_14_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1110, 0b101)
    #define AARCH64_DBGBCR_15_EL1           __AARCH64_SYS_REG(0b10, 0b000, 0b0000, 0b1111, 0b101)

There are several supported encoding values but it is not guaranteed that any new encoding will be supported:
    
- v="m[x-y]"
- v="0b00:m[x-y]"
- v="m[x-y]:0b00"

the index can be encoded acrros multiple elements for instance:
```xml
                <encoding>
                
                <acc_array var="m">
                    <acc_array_range>0-15</acc_array_range>
                </acc_array>
                 <access_instruction>MRS &lt;Xt&gt;, AMEVCNTR1&lt;m&gt;_EL0</access_instruction>
                
                <enc n="op0" v="0b11"/>
                
                <enc n="op1" v="0b011"/>
                
                <enc n="CRn" v="0b1101"/>
                
                <enc n="CRm" v="0b110:m[3]"/>
                
                <enc n="op2" v="m[2:0]"/>
            </encoding>
.```
And the info script generates:
    #define AARCH64_AMEVTYPER1_0_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b000)
    #define AARCH64_AMEVTYPER1_1_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b001)
    #define AARCH64_AMEVTYPER1_2_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b010)
    #define AARCH64_AMEVTYPER1_3_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b011)
    #define AARCH64_AMEVTYPER1_4_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b100)
    #define AARCH64_AMEVTYPER1_5_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b101)
    #define AARCH64_AMEVTYPER1_6_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b110)
    #define AARCH64_AMEVTYPER1_7_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1110, 0b111)
    #define AARCH64_AMEVTYPER1_8_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b000)
    #define AARCH64_AMEVTYPER1_9_EL0        __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b001)
    #define AARCH64_AMEVTYPER1_10_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b010)
    #define AARCH64_AMEVTYPER1_11_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b011)
    #define AARCH64_AMEVTYPER1_12_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b100)
    #define AARCH64_AMEVTYPER1_13_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b101)
    #define AARCH64_AMEVTYPER1_14_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b110)
    #define AARCH64_AMEVTYPER1_15_EL0       __AARCH64_SYS_REG(0b11, 0b011, 0b1101, 0b1111, 0b111)


    
# Discussion on Arm system registers

They are encoded based on combination of Op0, Op1, Op2, CRn, CRm fields.

Those fields are place in an instruction encoding in a certain way and in 
different way when placed in ESR (Exception Syndrome Register).

The generated "tag" in the C header is modeled after instruction encoding fields placement.

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
.```

### AArch64-amevtyper0n_el0.xml

Define a range of 4 registers but define a 16 bit field to encode the index:
```xml
                <enc n="CRm" v="0b011:m[3]"/>
                <enc n="op2" v="m[2:0]"/>
 .```
            
### Redundant specificaitons

Several specification files lke AArch64-zcr_el2.xml actually describe both EL1 and EL2 access registers.
The scripts are currently ignoring them but don't know why those accessors are described
