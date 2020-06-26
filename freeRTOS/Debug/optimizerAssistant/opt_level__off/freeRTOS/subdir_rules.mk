################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
freeRTOS/%.obj: ../freeRTOS/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/opt/ti/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/bin/cl430" -vmspx -Ooff --use_hw_mpy=F5 --include_path="C:/opt/ti/ccs/ccs_base/msp430/include" --include_path="D:/workspace/ccs/freeRTOS-demo/freeRTOS/include" --include_path="D:/workspace/ccs/freeRTOS-demo" --include_path="C:/opt/ti/ccs/tools/compiler/ti-cgt-msp430_18.12.4.LTS/include" --advice:power="all" --advice:hw_config=all --define=__MSP430FR2355__ --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="freeRTOS/$(basename $(<F)).d_raw" --obj_directory="freeRTOS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


