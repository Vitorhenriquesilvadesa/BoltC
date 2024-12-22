section .data
	string_literal_0000021795ECFEC0 db "Hello World", 0
section .text
	mov eax, string_literal_0000021795ECFEC0
	push eax
