################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/opt/ccstudio/ccs/tools/compiler/msp430-gcc-8.3.0.16_linux64/bin/msp430-elf-gcc-8.3.0" -c -mmcu=msp430fr5994 -mhwmult=f5series -I"/opt/ccstudio/ccs/ccs_base/msp430/include_gcc" -I"/home/user/Projetos/ccstudio/rtos" -I"/opt/ccstudio/ccs/tools/compiler/msp430-gcc-8.3.0.16_linux64/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -mlarge -mcode-region=none -mdata-region=none -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


