# Microcontrollers_project
I created this project back in college for a discipline called "Microcontrollers". The requirements for this project were:
- create the PCB for a project that contains a microcontroller;
- use at least a digital input, an analogic input, a digital output, a digital PWM output, a communication protocol;
- simulation in Proteus, code creation in MPLABX IDE.

The structure of the project is the following:
- the folder "Documente" contains a file in romanian with the project's requirements and 3 datasheets in english, regarding the LCD;
- the folder "MPLAB" contains the code for the chosen microcontroller (dsPIC33FJ32MC202);
- the folder "PCB" contains the schematic of the project and the printed circuit board;
- the folder "Proteus" contains the project schematic created in Isis module of Proteus. Here you can easily upload the .hex file from the "MPLAB" folder and simulate the microcontroller's functionality.

The project's functionality:

The microcontroller measures an analogic value from a temperature sensor (LM35), then displays the temperature on an Liquid Crystal Display. There are also 3 buttons: one to show the max value of the temperature, one to show the min value of the temperature and one to reset these values. The mentioned values are recorded since the system is turned on. Also, if the temperature is >23 degrees Celsius, a fan starts (DC motor)

Used components:
- LM temperature sensor;
- dsPIC33FJ32MC202 microcontroller from Microchip;
- LM016 Liquid Crystal Display;
- push buttons;
- DC motor;
- 2N2222 transistor.

The file "Proiect la disciplina Microcontrollere.docx" explains the project's functionality and contains images of the schematic, PCB and the used code. The disadvantage is that the documentation is in Romanian. 
