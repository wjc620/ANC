################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AncProc.obj: ../AncProc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="AncProc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ConstVar.obj: ../ConstVar.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ConstVar.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

aic3106_init.obj: ../aic3106_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="aic3106_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

codecif.obj: ../codecif.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="codecif.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mcasp_init.obj: ../mcasp_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="mcasp_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

siso_data.obj: ../siso_data.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="siso_data.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

spmod_data.obj: ../spmod_data.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.2.1/include" --include_path="../../../Include" --include_path="../../../Include/StarterWare/Drivers" --include_path="../../../Include/StarterWare/Drivers/hw" --include_path="../../../Include/StarterWare/Drivers/c674x" --include_path="../../../Include/StarterWare/Drivers/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="spmod_data.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


