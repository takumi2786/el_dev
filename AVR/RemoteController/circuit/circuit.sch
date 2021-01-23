EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:LED D?
U 1 1 5E8A0F92
P 6200 3250
F 0 "D?" V 6147 3328 50  0000 L CNN
F 1 "LED" V 6238 3328 50  0000 L CNN
F 2 "" H 6200 3250 50  0001 C CNN
F 3 "~" H 6200 3250 50  0001 C CNN
	1    6200 3250
	0    -1   -1   0   
$EndComp
Text Label 6200 2700 0    50   ~ 0
5V_PWM
Wire Wire Line
	6200 3400 6200 3750
$Comp
L power:GND #PWR?
U 1 1 5E8A4D52
P 6200 3750
F 0 "#PWR?" H 6200 3500 50  0001 C CNN
F 1 "GND" H 6205 3577 50  0000 C CNN
F 2 "" H 6200 3750 50  0001 C CNN
F 3 "" H 6200 3750 50  0001 C CNN
	1    6200 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R
U 1 1 5E8A5105
P 6200 2950
F 0 "R" H 6270 2996 50  0000 L CNN
F 1 "330" H 6270 2905 50  0000 L CNN
F 2 "" V 6130 2950 50  0001 C CNN
F 3 "~" H 6200 2950 50  0001 C CNN
F 4 "R" H 6200 2950 50  0001 C CNN "Spice_Primitive"
F 5 "330" H 6200 2950 50  0001 C CNN "Spice_Model"
F 6 "Y" H 6200 2950 50  0001 C CNN "Spice_Netlist_Enabled"
	1    6200 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 2700 6200 2800
$EndSCHEMATC
