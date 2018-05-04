################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./AncProc.obj" \
"./ConstVar.obj" \
"./aic3106_init.obj" \
"./codecif.obj" \
"./main.obj" \
"./mcasp_init.obj" \
"./siso_data.obj" \
"./spmod_data.obj" \
"../C6748.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"../../../Library/Platform/Debug/Platform.lib" \
-l"../../../Library/StarterWare/SystemConfig/Debug/system_config.lib" \
-l"../../../Library/StarterWare/utils/Debug/utils.lib" \
-l"../../../Library/StarterWare/Drivers/Debug/drivers.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ANC_MOD3.out \

EXE_OUTPUTS__QUOTED += \
"ANC_MOD3.out" \

BIN_OUTPUTS += \
ANC_MOD3.hex \

BIN_OUTPUTS__QUOTED += \
"ANC_MOD3.hex" \


# All Target
all: ANC_MOD3.out

# Tool invocations
ANC_MOD3.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"ANC_MOD3.map" --heap_size=0x800 --stack_size=0x800 -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/lib" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ANC_MOD3_linkInfo.xml" --rom_model -o "ANC_MOD3.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

ANC_MOD3.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C6000 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/hex6x"  -o "ANC_MOD3.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "AncProc.obj" "ConstVar.obj" "aic3106_init.obj" "codecif.obj" "main.obj" "mcasp_init.obj" "siso_data.obj" "spmod_data.obj" 
	-$(RM) "AncProc.d" "ConstVar.d" "aic3106_init.d" "codecif.d" "main.d" "mcasp_init.d" "siso_data.d" "spmod_data.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

