# AGH Technika Mikroprocesorowa

Repozytorium z kodem do wykorzystania na laboratoriach.

## Użytkowanie

Najlepiej skopiować potrzebny kod, popatrzeć na swój setup w CubeMX i poprawić to co trzeba. Jak ktoś chce kopiować całe projekty, to trzeba odtworzyć tą konfigurację.

## Struktura

```
/nazwa_projektu/
	Core/
		Inc/
		Src/
		Startup/
```

Repozytorium nie zawiera bibliotek, driverów i innych include'ów - jest tutaj tylko trochę generowanego kodu + kod z laboratorium.

## Wersje

- STM32CubeIDE - 1.3.0 (takie, jak na komputerach w laboratorium) (są też 1.7.0, ale ja używałem 1.3.0).
- Nucleo-64 STM32F411RE - takie procki mamy na płytkach (są też F410, ale podczas labów ich nie używamy - ja taką dostałem do domu).
- Shield - nie pamiętam (jest na UPEL-u).
