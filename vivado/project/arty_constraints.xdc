## Arty A7-35T 100MHz clock
set_property PACKAGE_PIN E3 [get_ports {sys_clk}]
set_property IOSTANDARD LVCMOS33 [get_ports {sys_clk}]
create_clock -period 10.000 -name sys_clk -waveform {0.000 5.000} [get_ports {sys_clk}]

## Reset (active high) - map to RESET button (usually)
set_property PACKAGE_PIN C2 [get_ports {sys_rst}]
set_property IOSTANDARD LVCMOS33 [get_ports {sys_rst}]

## USB-UART (Arty A7-35)
set_property PACKAGE_PIN D10 [get_ports {usb_uart_rxd}]
set_property IOSTANDARD LVCMOS33 [get_ports {usb_uart_rxd}]

set_property PACKAGE_PIN C10 [get_ports {usb_uart_txd}]
set_property IOSTANDARD LVCMOS33 [get_ports {usb_uart_txd}]